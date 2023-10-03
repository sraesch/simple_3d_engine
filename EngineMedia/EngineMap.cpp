//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "EngineMedia.h"

///////////////////////////////////////////////////////////////////////
///Engine Map Base class
///////////////////////////////////////////////////////////////////////
void ENMapBase::InitDefaultCamera(ENCamera &cam)
{
 cam.Pos=ENVector(0.0f,0.0f,0.0f);
 cam.Angle=ENVector(0.0f,0.0f,0.0f);
 cam.Arc=60.0f;
 cam.Range=10000.0f;
}

void ENMapBase::InitMapFace(ENMapFace &face)
{
 face.texcoord[0]=ENVector2(0.0f,0.0f);
 face.texcoord[1]=ENVector2(0.0f,0.0f);
 face.texcoord[2]=ENVector2(0.0f,0.0f);

 face.props.Passable=false;
 face.props.Invisible=false;
 face.props.Lighting=true;
 face.props.Shadow=true;
 face.props.Animated=false;
 face.props.Mode=EN_SHOW_NORMAL;

 face.texind=-1;

 face.numframes=1;
 face.framelen=1000;
}

void ENMapBase::InitHeader(ENMapHeader &header)
{
 strcpy(header.ID,"EMAP01");
 *header.PackageFile=0;
 header.NumFaces=0;
 header.NumTextures=0;
 header.NumLights=0;
 header.NumGroups=0;
 header.NumObjects=0;
 header.NumSounds=0;
}

ENbool ENMapBase::CheckHeader(ENMapHeader *header)
{
 return (strcmp(header->ID,"EMAP01")==0);
}
///////////////////////////////////////////////////////////////////////
///Engine Map class
///////////////////////////////////////////////////////////////////////
ENMap::ENMap()
{
 Vertexes=new ENVertexArray();
 lights=new ENArray(sizeof(ENLight));
 Objects=new ENArray(sizeof(ENMapObject));
 Faces=new ENArray(sizeof(ENMapFace));
 Sounds=new ENArray(sizeof(ENSoundSrc));
 Groups=new ENArray(sizeof(ENMapGroup));
 Textures=new ENArray(sizeof(ENMapTexture));
 //Init sky box
 for(ENuint a=0;a<6;a++)
   SkyBox[a]=new ENPicture();
 //Init default camera
 InitDefaultCamera(CameraObj);
 //Init sun
 Sun.Enable=false;
 Sun.dir=ENVector2(0.0f,0.0f);
 memset(Sun.SunColor,0,sizeof(Sun.SunColor));   
 //Init Package file
 *PackageFile=0;
 //Init Error log
 *ErrorLog=0;
}

ENMap::~ENMap()
{
 Clear();
 delete Vertexes;
 delete lights;
 delete Objects;
 delete Faces;
 delete Sounds;
 delete Groups;
 delete Textures;
 //Delete sky box
 for(ENuint a=0;a<6;a++)
   delete SkyBox[a];
}

void ENMap::Clear()
{
 //Delete groups
 for(ENuint a=0;a<Groups->GetNum();a++)
 {
  ENMapGroup group;
  Groups->GetElement(a,(ENubyte*)&group);
  delete group.Indices;
 }
 //Delete materials
 for(ENuint b=0;b<Textures->GetNum();b++)
 {
  ENMapTexture tex;
  Textures->GetElement(b,(ENubyte*)&tex);
  delete tex.mat;
 }
 //Clear sky box
 for(ENuint a=0;a<6;a++)
   SkyBox[a]->Clear();
 //Init default camera
 InitDefaultCamera(CameraObj);
 //Init sun
 Sun.Enable=false;
 Sun.dir=ENVector2(0.0f,0.0f);
 memset(Sun.SunColor,0,sizeof(Sun.SunColor));
 //Clear others
 Vertexes->Clear();
 lights->Clear();
 Objects->Clear();
 Faces->Clear();
 Sounds->Clear();
 Groups->Clear();
 Textures->Clear();
 //Init Package file
 *PackageFile=0;
 //Init Error log
 *ErrorLog=0;
}

void   ENMap::GetErrorLog(char *str)
{
 strcpy(str,ErrorLog);
}

ENbool ENMap::Write(char *FileName)
{
 //Variables
 ENMapHeader header;
 ENFile handle;
 ENuint a;
 //Open file
 handle=ENOpenFile(FileName,"wb");
 if(!handle)
 {
  sprintf(ErrorLog,"Cannot write to file %s!!!",FileName);
  return false;
 }
 //Write header
 header=GetHeader();
 ENWriteFile(&header,sizeof(ENMapHeader),1,handle);
 //Write sun
 ENWriteFile(&Sun,sizeof(ENMapSun),1,handle);
 //Write skybox
 for(a=0;a<6;a++)
   SkyBox[a]->SaveToHandle(handle);
 //Write camera
 ENWriteFile(&CameraObj,sizeof(ENCamera),1,handle);   
 //Write common body data
 Vertexes->SaveToHandle(handle);
 Faces->SaveToHandle(handle);
 lights->SaveToHandle(handle);
 Sounds->SaveToHandle(handle);
 Objects->SaveToHandle(handle);
 //Write groups
 for(a=0;a<header.NumGroups;a++)
 {
  ENMapGroup group;
  Groups->GetElement(a,(ENubyte*)&group);
  ENWriteFile(group.Name,1,33,handle);
  group.Indices->SaveToHandle(handle);
 }
 //Write textures
 for(a=0;a<header.NumTextures;a++)
 {
  ENMapTexture tex;
  Textures->GetElement(a,(ENubyte*)&tex);
  ENWriteFile(tex.Name,33,1,handle);
  tex.mat->SaveToHandle(handle);
 }
 //Finished
 ENCloseFile(handle);
 return true;
}

ENbool ENMap::Load(char *FileName)
{
 //Variables
 ENMapHeader header;
 ENFile handle;
 ENuint a;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle)
 {
  sprintf(ErrorLog,"Cannot read file %s!!!",FileName);
  return false;
 }
 //Read header
 ENReadFile(&header,sizeof(ENMapHeader),1,handle);
 if(!CheckHeader(&header))
 {
  ENCloseFile(handle);
  sprintf(ErrorLog,"Invalid header of file %s!!!",FileName);
  return false;
 }
 else
 {
  Clear();
  strcpy(PackageFile,header.PackageFile);  
 }
 //Read sun
 ENReadFile(&Sun,sizeof(ENMapSun),1,handle);
 //Read skybox
 for(a=0;a<6;a++)
   SkyBox[a]->LoadFromHandle(handle);
 //Read camera
 ENReadFile(&CameraObj,sizeof(ENCamera),1,handle);
 //Read common body data
 Vertexes->LoadFromHandle(handle);
 Faces->LoadFromHandle(handle);
 lights->LoadFromHandle(handle);
 Sounds->LoadFromHandle(handle);
 Objects->LoadFromHandle(handle);
 //Read groups
 for(a=0;a<header.NumGroups;a++)
 {
  ENMapGroup group;
  group.Indices=new ENArray(sizeof(ENuint));
  ENReadFile(group.Name,1,33,handle);
  group.Indices->LoadFromHandle(handle);
  Groups->AddElement((ENubyte*)&group);
 }
 //Read textures
 for(a=0;a<header.NumTextures;a++)
 {
  ENMapTexture tex;
  ENReadFile(tex.Name,33,1,handle);
  tex.mat=new ENMaterial();
  tex.mat->LoadFromHandle(handle);
  Textures->AddElement((ENubyte*)&tex);
 }
 //Finished
 ENCloseFile(handle);
 return true;
}

void ENMap::AddVertex(ENVector vert)
{
 Vertexes->AddElement((ENubyte*)&vert);
}

void ENMap::AddVertexes(ENVector *verts,ENuint num)
{
 Vertexes->AddElements((ENubyte*)verts,num);
}

void ENMap::AddFace(ENMap::ENMapFace face)
{
 Faces->AddElement((ENubyte*)&face);
}

void ENMap::AddFaces(ENMap::ENMapFace *faces,ENuint num)
{
 Faces->AddElements((ENubyte*)faces,num);
}

void ENMap::DeleteFace(ENuint ind)
{
 Faces->DelElement(ind);
}

void ENMap::DeleteVertex(ENuint ind)
{
 //Delete Vertex
 Vertexes->DelElement(ind);
 //Check faces
 ENuint i=0;
 ENMapFace face;
 while(i<Faces->GetNum())
 {
  Faces->GetElement(i,(ENubyte*)&face);
  if(face.indices[0]==ind||face.indices[1]==ind||
     face.indices[2]==ind)
     DeleteFace(i);
  else
  {
   if(face.indices[0]>ind) face.indices[0]--;
   if(face.indices[1]>ind) face.indices[1]--;
   if(face.indices[2]>ind) face.indices[2]--;
   Faces->SetElement(i,(ENubyte*)&face);
   i++;
  }
 }
 //Check groups
 for(ENuint g=0;g<Groups->GetNum();g++)
 {
  DeleteVertexFromGroup(g,ind);
  //Correct indices
  ENMapGroup group;
  Groups->GetElement(g,(ENubyte*)&group);
  for(ENuint a=0;a<group.Indices->GetNum();a++)
  {
   ENuint in;
   group.Indices->GetElement(a,(ENubyte*)&in);
   if(in>ind) in--;
   group.Indices->SetElement(a,(ENubyte*)&in);   
  }
 }
}

void ENMap::SetGLVertexPointer()
{
 Vertexes->SetGLVertexPointer();
}

ENMap::ENMapHeader ENMap::GetHeader()
{
 //Variables
 ENMapHeader header;
 //Standard header
 InitHeader(header);
 //Copy values
 strcpy(header.PackageFile,PackageFile);
 header.NumVertexes=Vertexes->GetNum();
 header.NumFaces=Faces->GetNum();
 header.NumTextures=Textures->GetNum();
 header.NumLights=lights->GetNum();
 header.NumGroups=Groups->GetNum();
 header.NumObjects=Objects->GetNum();
 header.NumSounds=Sounds->GetNum();
 //Finished
 return header;
}

void ENMap::SetPackage(char *FileName)
{
 strcpy(PackageFile,FileName);
}

ENfloat ENMap::GetMinX()
{
 //Variables
 ENVector vert;
 //Check number of vertexes
 if(Vertexes->GetNum())
 {
  Vertexes->GetElement(0,(ENubyte*)&vert);
  ENfloat res=vert.v[0];
  for(ENuint a=1;a<Vertexes->GetNum();a++)
  {
   Vertexes->GetElement(a,(ENubyte*)&vert);
   if(vert.v[0]<res)
     res=vert.v[0];
  }

  return res;
 }
 else
   return 0.0f;
}

ENfloat ENMap::GetMinY()
{
 //Variables
 ENVector vert;
 //Check number of vertexes
 if(Vertexes->GetNum())
 {
  Vertexes->GetElement(0,(ENubyte*)&vert);
  ENfloat res=vert.v[1];
  for(ENuint a=1;a<Vertexes->GetNum();a++)
  {
   Vertexes->GetElement(a,(ENubyte*)&vert);
   if(vert.v[1]<res)
     res=vert.v[1];
  }

  return res;
 }
 else
   return 0.0f;
}

ENfloat ENMap::GetMinZ()
{
 //Variables
 ENVector vert;
 //Check number of vertexes
 if(Vertexes->GetNum())
 {
  Vertexes->GetElement(0,(ENubyte*)&vert);
  ENfloat res=vert.v[2];
  for(ENuint a=1;a<Vertexes->GetNum();a++)
  {
   Vertexes->GetElement(a,(ENubyte*)&vert);
   if(vert.v[2]<res)
     res=vert.v[2];
  }

  return res;
 }
 else
   return 0.0f;
}

ENfloat ENMap::GetMaxX()
{
 //Variables
 ENVector vert;
 //Check number of vertexes
 if(Vertexes->GetNum())
 {
  Vertexes->GetElement(0,(ENubyte*)&vert);
  ENfloat res=vert.v[0];
  for(ENuint a=1;a<Vertexes->GetNum();a++)
  {
   Vertexes->GetElement(a,(ENubyte*)&vert);
   if(vert.v[0]>res)
     res=vert.v[0];
  }

  return res;
 }
 else
   return 0.0f;
}

ENfloat ENMap::GetMaxY()
{
 //Variables
 ENVector vert;
 //Check number of vertexes
 if(Vertexes->GetNum())
 {
  Vertexes->GetElement(0,(ENubyte*)&vert);
  ENfloat res=vert.v[1];
  for(ENuint a=1;a<Vertexes->GetNum();a++)
  {
   Vertexes->GetElement(a,(ENubyte*)&vert);
   if(vert.v[1]>res)
     res=vert.v[1];
  }

  return res;
 }
 else
   return 0.0f;
}

ENfloat ENMap::GetMaxZ()
{
 //Variables
 ENVector vert;
 //Check number of vertexes
 if(Vertexes->GetNum())
 {
  Vertexes->GetElement(0,(ENubyte*)&vert);
  ENfloat res=vert.v[2];
  for(ENuint a=1;a<Vertexes->GetNum();a++)
  {
   Vertexes->GetElement(a,(ENubyte*)&vert);
   if(vert.v[2]>res)
     res=vert.v[2];
  }

  return res;
 }
 else
   return 0.0f;
}

ENVector ENMap::GetVertex(ENuint ind)
{
 if(Vertexes->GetNum())
 {
  ENVector res;
  Vertexes->GetElement(ind,(ENubyte*)&res);
  return res;
 }
 else
   return ENVector(0.0f,0.0f,0.0f);
}

void ENMap::SetVertex(ENuint ind,ENVector vec)
{
 Vertexes->SetElement(ind,(ENubyte*)&vec);
}

ENMap::ENMapFace ENMap::GetFace(ENuint ind)
{
 if(Faces->GetNum())
 {
  ENMapFace res;
  Faces->GetElement(ind,(ENubyte*)&res);
  return res;
 }
 else
 {
  ENMapFace defaultf;
  defaultf.indices[0]=0;
  defaultf.indices[1]=0;
  defaultf.indices[2]=0;
  defaultf.texcoord[0]=ENVector2(0.0f,0.0f);
  defaultf.texcoord[1]=ENVector2(0.0f,0.0f);
  defaultf.texcoord[2]=ENVector2(0.0f,0.0f);
  defaultf.texind=-1;
  defaultf.numframes=1;
  defaultf.framelen=1000;
  memset(&defaultf.props,0,sizeof(ENFaceProp));
  return defaultf;
 }
}

void ENMap::SetFace(ENuint ind,ENMap::ENMapFace face)
{
 Faces->SetElement(ind,(ENubyte*)&face);
}

void    ENMap::AddLight(ENLight light)
{
 lights->AddElement((ENubyte*)&light);
}

void    ENMap::SetLight(ENuint ind,ENLight light)
{
 lights->SetElement(ind,(ENubyte*)&light);
}

ENLight ENMap::GetLight(ENuint ind)
{
 if(lights->GetNum())
 {
  ENLight res;
  lights->GetElement(ind,(ENubyte*)&res);
  return res;
 }
 else
   return ENLight(ENColor(255,255,255,255),ENVector(0.0f,0.0f,0.f),100.0f,
                  ENVector2(0.0f,0.0f),180.0f);
}

void    ENMap::DeleteLight(ENuint ind)
{
 lights->DelElement(ind);
}

void    ENMap::AddSound(ENSoundSrc snd)
{
 Sounds->AddElement((ENubyte*)&snd);
}

void    ENMap::SetSound(ENuint ind,ENSoundSrc snd)
{
 Sounds->SetElement(ind,(ENubyte*)&snd);
}

ENSoundSrc ENMap::GetSound(ENuint ind)
{
 ENSoundSrc res;
 if(Sounds->GetNum())
 {
  Sounds->GetElement(ind,(ENubyte*)&res);
  return res;
 }
 else
 {
  res.pos=ENVector(0.0f,0.0f,0.0f);
  res.dir=ENVector2(0.0f,0.0f);
  res.range=100.0f;
  res.speed=1.0f;
  res.volume=1.0f;
  res.angle=360.0f;
  res.loop=false;
  *res.src=0;
  return res;
 }
}

void ENMap::DeleteSound(ENuint ind)
{
 Sounds->DelElement(ind);
}

void ENMap::AddObject(ENMapObject object)
{
 Objects->AddElement((ENubyte*)&object);
}

void    ENMap::SetObject(ENuint ind,ENMapObject object)
{
 Objects->SetElement(ind,(ENubyte*)&object);
}

ENMap::ENMapObject ENMap::GetObject(ENuint ind)
{
 ENMapObject res;
 if(Objects->GetNum())
 {
  Objects->GetElement(ind,(ENubyte*)&res);
  return res;
 }
 else
 {
  *res.Func=0;
  *res.Name=0;
  res.Pos=ENVector(0.0f,0.0f,0.0f);
  res.Angle=ENVector(0.0f,0.0f,0.0f);
  res.Scale=ENVector(1.0f,1.0f,1.0f);
  return res;
 }
}

void ENMap::DeleteObject(ENuint ind)
{
 Objects->DelElement(ind);
}


ENint ENMap::GetIndexOfGroup(char *Name)
{
 for(ENuint a=0;a<Groups->GetNum();a++)
 {
  ENMapGroup group;
  Groups->GetElement(a,(ENubyte*)&group);
  if(strcmpi(group.Name,Name)==0)
    return a;
 }

 return -1;
}

ENbool ENMap::CreateGroup(char *Name)
{
 //Check if Group already exists
 ENint ind=GetIndexOfGroup(Name);
 if(ind>=0) return false;
 //Create group
 ENMapGroup group;
 strcpy(group.Name,Name);
 group.Indices=new ENArray(sizeof(ENuint));
 //Add group
 Groups->AddElement((ENubyte*)&group);
 //Finished
 return true;                                          
}

void ENMap::GetGroupName(ENuint ind,char *Name)
{
 if(Groups->GetNum())
 {
  ENMapGroup group;
  Groups->GetElement(ind,(ENubyte*)&group);
  strcpy(Name,group.Name);
 }
 else
   Name[0]=0;
}

ENbool ENMap::RenameGroup(ENuint ind,char *Name)
{
 //Check if Group name already exists
 ENint res=GetIndexOfGroup(Name);
 if(res>=0) return false;
 //Overwrite old group name
 if(Groups->GetNum())
 {
  ENMapGroup group;
  Groups->GetElement(ind,(ENubyte*)&group);
  strcpy(group.Name,Name);
  Groups->SetElement(ind,(ENubyte*)&group);
 }

 return true;
}

void ENMap::DeleteGroup(ENuint ind)
{
 if(Groups->GetNum())
 {
  ENMapGroup group;
  Groups->GetElement(ind,(ENubyte*)&group);
  delete group.Indices;
  Groups->DelElement(ind);
 }
}

void ENMap::AddGeometry2Group(ENuint ind,ENuint *Indices,ENuint num)
{
 if(Groups->GetNum())
 {
  ENMapGroup group;
  Groups->GetElement(ind,(ENubyte*)&group);
  ReleaseGeoemtryFromGroups(Indices,num);
  group.Indices->AddElements((ENubyte*)Indices,num);
 }
}

void ENMap::ReleaseGeoemtryFromGroups(ENuint *Indices,ENuint num)
{
 for(ENuint a=0;a<Groups->GetNum();a++)
  for(ENuint b=0;b<num;b++)
    DeleteVertexFromGroup(a,Indices[b]);
}

ENuint ENMap::GetNumGroupIndices(ENuint ind)
{
 if(Groups->GetNum())
 {
  ENMapGroup group;
  Groups->GetElement(ind,(ENubyte*)&group);
  return group.Indices->GetNum();
 }
 else
   return 0;
}

void ENMap::GetGroupIndices(ENuint ind,ENuint *Indices)
{
 if(Groups->GetNum())
 {
  ENMapGroup group;
  Groups->GetElement(ind,(ENubyte*)&group);
  group.Indices->GetElements((ENubyte*)Indices);
 }
}

void ENMap::DeleteVertexFromGroup(ENuint indG,ENuint indV)
{
 ENMapGroup group;
 Groups->GetElement(indG,(ENubyte*)&group);
 for(ENuint a=0;a<group.Indices->GetNum();a++)
 {
  ENuint index;
  group.Indices->GetElement(a,(ENubyte*)&index);
  if(index==indV)
  {
   group.Indices->DelElement(a);
   return;
  }
 }  
}

void  ENMap::SetSkyBox(ENuint ind,ENPicture *pic)
{
 SkyBox[ind%6]->Set(pic);
}

void  ENMap::GetSkyBox(ENuint ind,ENPicture *pic)
{
 pic->Set(SkyBox[ind%6]);
}

void   ENMap::SetSun(ENMapSun *sun)
{
 Sun=*sun;
}

void   ENMap::GetSun(ENMapSun *sun)
{
 *sun=Sun;
}

ENCamera ENMap::GetCamera()
{
 return CameraObj;
}

void ENMap::SetCamera(ENCamera cam)
{
 CameraObj=cam;
}

ENint ENMap::GetIndexOfTexture(char *Name)
{
 for(ENuint a=0;a<Textures->GetNum();a++)
 {
  ENMapTexture tex;
  Textures->GetElement(a,(ENubyte*)&tex);
  if(strcmpi(tex.Name,Name)==0)
    return a;
 }

 return -1;
}

ENbool ENMap::CreateTexture(char *Name,char *FileName)
{
 //Create material
 ENMaterial *mat=new ENMaterial();
 if(!mat->Load(FileName))
 {
  //If file is a picture
  ENPicture *pic=new ENPicture();
  if(!pic->Load(FileName))
  {
   delete pic;
   delete mat;
   return false;
  }
  //Set base data
  mat->SetBaseData(pic,0,0,0);
  delete pic;
 }
 if(!CreateTexture(Name,mat))
 {
  delete mat;
  return false;
 }
 else
 {
  delete mat;
  return true;
 }
}

ENbool ENMap::CreateTexture(char *Name,ENMaterial *mat)
{
 //Check if Texture already exists
 ENint ind=GetIndexOfTexture(Name);
 if(ind>=0) return false;
 //Create Texture
 ENMapTexture Texture;
 strcpy(Texture.Name,Name);
 Texture.mat=new ENMaterial(mat);
 //Add Texture
 Textures->AddElement((ENubyte*)&Texture);
 //Finished
 return true;
}

void ENMap::GetTextureName(ENuint ind,char *Name)
{
 if(Textures->GetNum())
 {
  ENMapTexture tex;
  Textures->GetElement(ind,(ENubyte*)&tex);
  strcpy(Name,tex.Name);
 }
 else
   Name[0]=0;
}

ENbool ENMap::RenameTexture(ENuint ind,char *Name)
{
 //Check if Texture name already exists
 ENint res=GetIndexOfTexture(Name);
 if(res>=0) return false;
 //Overwrite old Texture name
 if(Textures->GetNum())
 {
  ENMapTexture tex;
  Textures->GetElement(ind,(ENubyte*)&tex);
  strcpy(tex.Name,Name);
  Textures->SetElement(ind,(ENubyte*)&tex);
 }
 return true;
}

void ENMap::DeleteTexture(ENuint ind)
{
 if(Textures->GetNum())
 {
  ENMapTexture tex;
  Textures->GetElement(ind,(ENubyte*)&tex);
  delete tex.mat;
  Textures->DelElement(ind);
  //Correct textures
  for(ENuint a=0;a<Faces->GetNum();a++)
  {
   ENMapFace face;
   Faces->GetElement(a,(ENubyte*)&face);
   if(face.texind==(ENint)ind) face.texind=-1;
   if(face.texind>(ENint)ind)  face.texind--;
   Faces->SetElement(a,(ENubyte*)&face);
  }
 }
}

void ENMap::GetMaterialHeader(ENuint ind,ENMaterial::ENMaterialHeader &header)
{
 if(Textures->GetNum())
 {
  ENMapTexture tex;
  Textures->GetElement(ind,(ENubyte*)&tex);
  tex.mat->GetHeader(header);
 }
}

void ENMap::GetTextureData(ENMaterial *mat,ENuint ind)
{
 if(Textures->GetNum())
 {
  ENMapTexture tex;
  Textures->GetElement(ind,(ENubyte*)&tex);
  mat->Set(tex.mat);
 }
}

ENbool ENMap::IsMap(char *FileName)
{
 //Open
 ENFile handle=ENOpenFile(FileName,"rb");
 if(handle==NULL) return false;
 //Read
 ENMapHeader h;
 ENReadFile(&h,sizeof(ENMapHeader),1,handle);
 ENCloseFile(handle);
 //Check
 return CheckHeader(&h);
}



