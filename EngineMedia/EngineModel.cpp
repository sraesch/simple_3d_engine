#include <math.h>
#include "EngineMedia.h"

///////////////////////////////////////////////////////////////////////
///Engine Model
///////////////////////////////////////////////////////////////////////
ENModel::ENModel()
{
 Header.NumFrames=1;
 Header.NumGroups=0;
 Header.NumSkins=0;
 Header.NumFaces=0;
 Header.NumVertexes=0;
 strcpy(Header.ID,"EO01");
 Skins=NULL;
 Groups=NULL;
 Faces=NULL;
 Frames=(ENModelFrame*)ENAllocMemory(sizeof(ENModelFrame));
 Frames->Vertexes=NULL;
}

ENModel::~ENModel()
{
 Clear();
 ENFreeMemory(Frames);
}

void ENModel::Clear()
{
 //Delete frames vertexes
 if(Header.NumVertexes)
   for(ENuint a=0;a<Header.NumFrames;a++)
     ENFreeMemory(Frames[a].Vertexes);

 ENFreeMemory(Frames);
 //Delete skins
 for(ENuint b=0;b<Header.NumSkins;b++)
   delete Skins[b].mat;

 if(Header.NumSkins)
   ENFreeMemory(Skins);
 //Delete groups
 for(ENuint c=0;c<Header.NumGroups;c++)
   if(Groups[c].NumIndices)
     ENFreeMemory(Groups[c].Indices);

 if(Header.NumGroups)
   ENFreeMemory(Groups);
 //Free indices and texcoord data
 if(Header.NumFaces)
   ENFreeMemory(Faces);
 //Init
 Header.NumFrames=1;
 Header.NumGroups=0;
 Header.NumSkins=0;
 Header.NumFaces=0;
 Header.NumVertexes=0;
 strcpy(Header.ID,"EO01");
 Skins=NULL;
 Groups=NULL;
 Faces=NULL;
 Frames=(ENModelFrame*)ENAllocMemory(sizeof(ENModelFrame));
 Frames->Vertexes=NULL;
}

ENbool ENModel::Write(char *FileName)
{
 //Vars
 ENFile handle;
 //Open
 handle=ENOpenFile(FileName,"wb");
 if(handle==NULL) return false;
 //Write header
 ENWriteFile(&Header,sizeof(ENModelHeader),1,handle);
 //Write frames
 for(ENuint a=0;a<Header.NumFrames;a++)
   ENWriteFile(Frames[a].Vertexes,sizeof(ENVector),Header.NumVertexes,handle);
 //Write skins
 for(ENuint b=0;b<Header.NumSkins;b++)
 {
  ENWriteFile(Skins[b].SkinName,33,1,handle);
  Skins[b].mat->SaveToHandle(handle);
 }
 //Write Groups
 for(ENuint c=0;c<Header.NumGroups;c++)
 {   
  ENWriteFile(Groups[c].Name,sizeof(char),33,handle);
  ENWriteFile(&Groups[c].NumIndices,sizeof(ENuint),1,handle),
  ENWriteFile(Groups[c].Indices,sizeof(ENuint),Groups[c].NumIndices,handle);
 }
 //Write faces
 ENWriteFile(Faces,sizeof(ENModelFace),Header.NumFaces,handle);
 //Close
 ENCloseFile(handle);
 return true;
}

ENbool ENModel::Load(char *FileName)
{
 //Vars
 ENFile handle;
 //Open
 handle=ENOpenFile(FileName,"rb");
 if(handle==NULL) return false;
 //Read
 ENbool res=LoadFromHandle(handle);
 //Close
 ENCloseFile(handle);
 return res;
}

ENbool ENModel::LoadFromHandle(ENFile handle)
{
 //Vars
 ENModelHeader h;
 //Read header
 ENReadFile(&h,sizeof(ENModelHeader),1,handle);
 if(!CheckModelHeader(h))
   return false;
 else
 {
  //Clear model data
  Clear();
  ENFreeMemory(Frames);
  //Set new header
  Header=h;
 }
 //Alloc space
 Frames=(ENModelFrame*)ENAllocMemory(sizeof(ENModelFrame)*Header.NumFrames);
 Faces=(ENModelFace*)ENAllocMemory(sizeof(ENModelFace)*Header.NumFaces);
 Skins=(ENModelSkin*)ENAllocMemory(sizeof(ENModelSkin)*Header.NumSkins);
 Groups=(ENModelGroup*)ENAllocMemory(sizeof(ENModelGroup)*Header.NumGroups);
 //Read frames
 for(ENuint a=0;a<Header.NumFrames;a++)
 {
  Frames[a].Vertexes=(ENVector*)ENAllocMemory(sizeof(ENVector)*Header.NumVertexes);
  ENReadFile(Frames[a].Vertexes,sizeof(ENVector),Header.NumVertexes,handle);
 }
 //Read skins
 for(ENuint b=0;b<Header.NumSkins;b++)
 {
  ENReadFile(Skins[b].SkinName,33,1,handle);
  Skins[b].mat=new ENMaterial();
  Skins[b].mat->LoadFromHandle(handle);
 }
 //Read Groups
 for(ENuint c=0;c<Header.NumGroups;c++)
 {
  ENReadFile(Groups[c].Name,sizeof(char),33,handle);
  ENReadFile(&Groups[c].NumIndices,sizeof(ENuint),1,handle),
  Groups[c].Indices=(ENuint*)ENAllocMemory(sizeof(ENuint)*Groups[c].NumIndices);
  ENReadFile(Groups[c].Indices,sizeof(ENuint),Groups[c].NumIndices,handle);
 }
 //Read faces
 ENReadFile(Faces,sizeof(ENModelFace),Header.NumFaces,handle);
 //Finished
 return true;
}

void ENModel::AddVertex(ENVector vert)
{
 for(ENuint f=0;f<Header.NumFrames;f++)
 {
  ENModelFrame *frame=&Frames[f];
  ENuint numVerts=Header.NumVertexes;
  frame->Vertexes=(ENVector*)ENAddElement((ENubyte*)frame->Vertexes,
                                          sizeof(ENVector),numVerts,
                                          (ENubyte*)&vert);
 }
 Header.NumVertexes++;
}

void ENModel::AddVertexes(ENVector *verts,ENuint num)
{
 for(ENuint f=0;f<Header.NumFrames;f++)
 {
  ENModelFrame *frame=&Frames[f];
  ENuint numVerts=Header.NumVertexes;
  frame->Vertexes=(ENVector*)ENAddElements((ENubyte*)frame->Vertexes,
                                           sizeof(ENVector),numVerts,
                                          (ENubyte*)verts,num);
 }
 Header.NumVertexes+=num;
}

void ENModel::AddFace(ENModel::ENModelFace face)
{
 Faces=(ENModelFace*)ENAddElement((ENubyte*)Faces,
                                  sizeof(ENModelFace),Header.NumFaces,
                                  (ENubyte*)&face);
}

void ENModel::AddFaces(ENModel::ENModelFace *faces,ENuint num)
{
 Faces=(ENModelFace*)ENAddElements((ENubyte*)Faces,
                                  sizeof(ENModelFace),Header.NumFaces,
                                  (ENubyte*)faces,num);
}

void ENModel::AddFrame(ENuint srcframe)
{
 ENModelFrame newframe;
 //Create new frame
 newframe.Vertexes=(ENVector*)ENAllocMemory(sizeof(ENVector)*Header.NumVertexes);
 memcpy(newframe.Vertexes,Frames[srcframe].Vertexes,
        sizeof(ENVector)*Header.NumVertexes);
 //Add new frame
 Frames=(ENModelFrame*)ENAddElement((ENubyte*)Frames,
                                  sizeof(ENModelFrame),Header.NumFrames,
                                  (ENubyte*)&newframe);
}

void ENModel::DeleteFrame(ENuint ind)
{
 if(Header.NumFrames>1)
 {
  ind%=Header.NumFrames;
  ENModelFrame *frame=&Frames[ind];
  ENFreeMemory(frame->Vertexes);
  Frames=(ENModelFrame*)ENDelElement((ENubyte*)Frames,
                        sizeof(ENModelFrame),Header.NumFrames,ind);
 }
}

void ENModel::SetGLVertexPointer(ENuint frame)
{
 ENuint curframe=0;
 if(Header.NumFrames)
 {
  curframe=frame%Header.NumFrames;
  glVertexPointer(3,GL_FLOAT,0,Frames[curframe].Vertexes);
 }
}

ENModel::ENModelHeader ENModel::GetHeader()
{
 return Header;
}

ENfloat ENModel::GetMinX(ENuint frame)
{
 ENuint curframe=0;
 if(Header.NumFrames&&Header.NumVertexes)
 {
  curframe=frame%Header.NumFrames;
  ENVector *verts=Frames[curframe].Vertexes;
  ENfloat res=verts[0].v[0];
  for(ENuint a=1;a<Header.NumVertexes;a++)
    if(verts[a].v[0]<res)
      res=verts[a].v[0];

  return res;
 }
 else
   return 0.0f;
}

ENfloat ENModel::GetMinY(ENuint frame)
{
 ENuint curframe=0;
 if(Header.NumFrames&&Header.NumVertexes)
 {
  curframe=frame%Header.NumFrames;
  ENVector *verts=Frames[curframe].Vertexes;
  ENfloat res=verts[0].v[1];
  for(ENuint a=1;a<Header.NumVertexes;a++)
    if(verts[a].v[1]<res)
      res=verts[a].v[1];

  return res;
 }
 else
   return 0.0f;
}

ENfloat ENModel::GetMinZ(ENuint frame)
{
 ENuint curframe=0;
 if(Header.NumFrames&&Header.NumVertexes)
 {
  curframe=frame%Header.NumFrames;
  ENVector *verts=Frames[curframe].Vertexes;
  ENfloat res=verts[0].v[2];
  for(ENuint a=1;a<Header.NumVertexes;a++)
    if(verts[a].v[2]<res)
      res=verts[a].v[2];

  return res;
 }
 else
   return 0.0f;
}

ENfloat ENModel::GetMaxX(ENuint frame)
{
 ENuint curframe=0;
 if(Header.NumFrames&&Header.NumVertexes)
 {
  curframe=frame%Header.NumFrames;
  ENVector *verts=Frames[curframe].Vertexes;
  ENfloat res=verts[0].v[0];
  for(ENuint a=1;a<Header.NumVertexes;a++)
    if(verts[a].v[0]>res)
      res=verts[a].v[0];

  return res;
 }
 else
   return 0.0f;
}

ENfloat ENModel::GetMaxY(ENuint frame)
{
 ENuint curframe=0;
 if(Header.NumFrames&&Header.NumVertexes)
 {
  curframe=frame%Header.NumFrames;
  ENVector *verts=Frames[curframe].Vertexes;
  ENfloat res=verts[0].v[1];
  for(ENuint a=1;a<Header.NumVertexes;a++)
    if(verts[a].v[1]>res)
      res=verts[a].v[1];

  return res;
 }
 else
   return 0.0f;
}

ENfloat ENModel::GetMaxZ(ENuint frame)
{
 ENuint curframe=0;
 if(Header.NumFrames&&Header.NumVertexes)
 {
  curframe=frame%Header.NumFrames;
  ENVector *verts=Frames[curframe].Vertexes;
  ENfloat res=verts[0].v[2];
  for(ENuint a=1;a<Header.NumVertexes;a++)
    if(verts[a].v[2]>res)
      res=verts[a].v[2];

  return res;
 }
 else
   return 0.0f;
}

ENVector ENModel::GetVertex(ENuint indV,ENuint indF)
{
 ENuint curframe=0;
 if(Header.NumFrames&&Header.NumVertexes)
 {
  curframe=indF%Header.NumFrames;
  ENVector *verts=Frames[curframe].Vertexes;
  return verts[indV%Header.NumVertexes];
 }
 else
   return ENVector(0.0f,0.0f,0.0f);
}

void ENModel::SetVertex(ENuint indV,ENuint indF,ENVector vec)
{
 ENuint curframe=0;
 if(Header.NumFrames&&Header.NumVertexes)
 {
  curframe=indF%Header.NumFrames;
  ENVector *verts=Frames[curframe].Vertexes;
  verts[indV%Header.NumVertexes]=vec;
 }
}

ENModel::ENModelFace ENModel::GetFace(ENuint ind)
{
 if(Header.NumFaces)
   return Faces[ind%Header.NumFaces];
 else
 {
  ENModelFace defaultf;
  defaultf.Indices[0]=0;
  defaultf.Indices[1]=0;
  defaultf.Indices[2]=0;
  defaultf.Coord[0]=ENVector2(0.0f,0.0f);
  defaultf.Coord[1]=ENVector2(0.0f,0.0f);
  defaultf.Coord[2]=ENVector2(0.0f,0.0f);
  return defaultf;
 }
}

void ENModel::SetFace(ENuint ind,ENModel::ENModelFace face)
{
 if(Header.NumFaces)
   Faces[ind%Header.NumFaces]=face;
}

ENint ENModel::GetIndexOfGroup(char *Name)
{
 for(ENuint a=0;a<Header.NumGroups;a++)
   if(strcmpi(Groups[a].Name,Name)==0)
     return a;

 return -1;
}

ENbool ENModel::CreateGroup(char *Name)
{
 //Check if Group already exists
 ENint ind=GetIndexOfGroup(Name);
 if(ind>=0) return false;
 //Create group
 ENModelGroup group;
 strcpy(group.Name,Name);
 group.NumIndices=0;
 group.Indices=NULL;
 //Add group
 Groups=(ENModelGroup*)ENAddElement((ENubyte*)Groups,
                                          sizeof(ENModelGroup),Header.NumGroups,
                                          (ENubyte*)&group);
 //Finished
 return true;                                          
}

void ENModel::GetGroupName(ENuint ind,char *Name)
{
 if(Header.NumGroups)
   strcpy(Name,Groups[ind%Header.NumGroups].Name);
 else
   Name[0]=0;
}

ENbool ENModel::RenameGroup(ENuint ind,char *Name)
{
 //Check if Group name already exists
 ENint res=GetIndexOfGroup(Name);
 if(res>=0) return false;
 //Overwrite old group name
 if(Header.NumGroups)
   strcpy(Groups[ind%Header.NumGroups].Name,Name);

 return true;
}

void ENModel::DeleteGroup(ENuint ind)
{
 if(Header.NumGroups)
 {
  ind%=Header.NumGroups;
  ENModelGroup *group=&Groups[ind];
  if(group->NumIndices)
    ENFreeMemory(group->Indices);
    
  Groups=(ENModelGroup*)ENDelElement((ENubyte*)Groups,sizeof(ENModelGroup),
                                Header.NumGroups,ind);

 }
}

void ENModel::AddGeometry2Group(ENuint ind,ENuint *Indices,ENuint num)
{
 if(Header.NumGroups)
 {
  ind%=Header.NumGroups;
  ENModelGroup *group=&Groups[ind];
  ReleaseGeoemtryFromGroups(Indices,num);
  group->Indices=(ENuint*)ENAddElements((ENubyte*)group->Indices,sizeof(ENuint),
                                        group->NumIndices,(ENubyte*)Indices,
                                        num);
 }
}

void ENModel::ReleaseGeoemtryFromGroups(ENuint *Indices,ENuint num)
{
 for(ENuint a=0;a<Header.NumGroups;a++)
 {
  ENModelGroup *group=&Groups[a];
  for(ENuint b=0;b<num;b++)
    DeleteVertexFromGroup(group,Indices[b]);
 }
}

ENuint ENModel::GetNumGroupIndices(ENuint ind)
{
 if(Header.NumGroups)
   return Groups[ind%Header.NumGroups].NumIndices;
 else
   return 0;
}

void ENModel::GetGroupIndices(ENuint ind,ENuint *Indices)
{
 if(Header.NumGroups)
 {
  ENModelGroup *group=&Groups[ind%Header.NumGroups];
  memcpy(Indices,group->Indices,group->NumIndices*sizeof(ENuint));
 }
}

void ENModel::DeleteVertexFromGroup(ENModelGroup *group,ENuint indV)
{
 for(ENuint a=0;a<group->NumIndices;a++)
   if(group->Indices[a]==indV)
   {
    group->Indices=(ENuint*)ENDelElement((ENubyte*)group->Indices,
                                          sizeof(ENuint),group->NumIndices,a);
    return;
   }
}

void ENModel::DeleteFace(ENuint ind)
{
 if(Header.NumFaces)
 {
  ind%=Header.NumFaces;
  Faces=(ENModelFace*)ENDelElement((ENubyte*)Faces,sizeof(ENModelFace),
                                   Header.NumFaces,ind);
 }
}

void ENModel::DeleteVertex(ENuint ind)
{
 //Delete Vertex
 for(ENuint f=0;f<Header.NumFrames;f++)
 {
  ENModelFrame *frame=&Frames[f];
  ENuint numVerts=Header.NumVertexes;
  frame->Vertexes=(ENVector*)ENDelElement((ENubyte*)frame->Vertexes,
                                sizeof(ENVector),numVerts,ind);
 }
 Header.NumVertexes--;
 //Check faces
 ENuint i=0;
 while(i<Header.NumFaces)
 {
  if(Faces[i].Indices[0]==ind||Faces[i].Indices[1]==ind||
     Faces[i].Indices[2]==ind)
     DeleteFace(i);
  else
  {
   if(Faces[i].Indices[0]>ind) Faces[i].Indices[0]--;
   if(Faces[i].Indices[1]>ind) Faces[i].Indices[1]--;
   if(Faces[i].Indices[2]>ind) Faces[i].Indices[2]--;
   i++;
  }
 }
 //Check groups
 for(ENuint g=0;g<Header.NumGroups;g++)
 {
  ENModelGroup *group=&Groups[g];
  DeleteVertexFromGroup(group,ind);
 }
}

ENint ENModel::GetIndexOfSkin(char *Name)
{
 for(ENuint a=0;a<Header.NumSkins;a++)
   if(strcmpi(Skins[a].SkinName,Name)==0)
     return a;

 return -1;
}

ENbool ENModel::CreateSkin(char *Name,char *FileName)
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
 if(!CreateSkin(Name,mat))
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

ENbool ENModel::CreateSkin(char *Name,ENMaterial *mat)
{
 //Check if Skin already exists
 ENint ind=GetIndexOfSkin(Name);
 if(ind>=0) return false;
 //Create skin
 ENModelSkin skin;
 strcpy(skin.SkinName,Name);
 skin.mat=new ENMaterial(mat);
 //Add skin
 Skins=(ENModelSkin*)ENAddElement((ENubyte*)Skins,
                                          sizeof(ENModelSkin),Header.NumSkins,
                                          (ENubyte*)&skin);
 //Finished
 return true;
}

void ENModel::GetSkinName(ENuint ind,char *Name)
{
 if(Header.NumSkins)
   strcpy(Name,Skins[ind%Header.NumSkins].SkinName);
 else
   Name[0]=0;
}

ENbool ENModel::RenameSkin(ENuint ind,char *Name)
{
 //Check if Skin name already exists
 ENint res=GetIndexOfSkin(Name);
 if(res>=0) return false;
 //Overwrite old skin name
 if(Header.NumSkins)
   strcpy(Skins[ind%Header.NumSkins].SkinName,Name);

 return true;
}

void ENModel::DeleteSkin(ENuint ind)
{
 if(Header.NumSkins)
 {
  ind%=Header.NumSkins;
  ENModelSkin *skin=&Skins[ind];
  delete skin->mat;

  Skins=(ENModelSkin*)ENDelElement((ENubyte*)Skins,sizeof(ENModelSkin),
                                Header.NumSkins,ind);

 }
}

void ENModel::GetSkinData(ENMaterial *mat,ENuint ind)
{
 if(Header.NumSkins)
 {
  ind%=Header.NumSkins;
  ENModelSkin *skin=&Skins[ind];
  mat->Set(skin->mat);
 }
}

ENbool ENModel::CheckModelHeader(ENModel::ENModelHeader header)
{
 //Check ID
 if(strcmp(header.ID,"EO01")!=0) return false;
 //Check values
 if(!header.NumFrames) return false;
 //Finished
 return true;
}

ENbool ENModel::IsModel(char *FileName)
{
 //Open
 ENFile handle=ENOpenFile(FileName,"rb");
 if(handle==NULL) return false;
 //Read
 ENModelHeader h;
 ENReadFile(&h,sizeof(ENModelHeader),1,handle);
 ENCloseFile(handle);
 //Check
 return CheckModelHeader(h);
}
///////////////////////////////////////////////////////////////////////
///Engine Model loaded
///////////////////////////////////////////////////////////////////////
ENModelLoaded::ENModelLoaded(ENModel *data)
{
 //Init values
 curskin=0;
 curframe=0;
 nxtframe=0;
 framefact=0.0f;
 //Set header
 SetModelHeader(data->GetHeader());
 //Set faces
 SetModelFaces(data);
 //Set skins
 SetModelSkins(data);
 //Set frames
 SetModelFrames(data);
}

ENModelLoaded::~ENModelLoaded()
{
 //Delete faces
 ENFreeMemory(faces);
 //Delete skins
 for(ENuint a=0;a<header.NumSkins;a++)
   delete skins[a];

 delete[] skins;
 //Delete frames
 for(ENuint b=0;b<header.NumFrames;b++)
 {
  ENFreeMemory(frames[b].Vertexes);
  ENFreeMemory(frames[b].Normals);
 }
 ENFreeMemory(frames);
}

void ENModelLoaded::DrawRaw(ENVector *verts)
{
 //Init data
 if(verts)
   glVertexPointer(3,GL_FLOAT,0,verts);
 else
   glVertexPointer(3,GL_FLOAT,0,frames[curframe].Vertexes);
 //Draw data
 glBegin(GL_TRIANGLES);
 for(ENuint a=0;a<header.NumFaces;a++)
 {
  glArrayElement(faces[a].Indices[0]);
  glArrayElement(faces[a].Indices[1]);
  glArrayElement(faces[a].Indices[2]);
 }
 glEnd();
}

void ENModelLoaded::DrawGround(ENVector *verts)
{
 if(skins[curskin]->isLuminanceSupported())
 {
  glColor3f(1,1,1);
  glEnable(GL_TEXTURE_2D);
  skins[curskin]->SetLuminance();
  //Set vertex data
  if(verts)
    glVertexPointer(3,GL_FLOAT,0,verts);
  else
    glVertexPointer(3,GL_FLOAT,0,frames[curframe].Vertexes);
  //Draw mesh data
  glBegin(GL_TRIANGLES);
  for(ENuint a=0;a<header.NumFaces;a++)
  {
   glTexCoord2fv((ENfloat*)&faces[a].Coord[0]);
   glArrayElement(faces[a].Indices[0]);
   glTexCoord2fv((ENfloat*)&faces[a].Coord[1]);
   glArrayElement(faces[a].Indices[1]);
   glTexCoord2fv((ENfloat*)&faces[a].Coord[2]);
   glArrayElement(faces[a].Indices[2]);
  }
  glEnd();
  glDisable(GL_TEXTURE_2D);
 }
 else
   DrawRaw(verts);
}

void ENModelLoaded::Draw(ENVector *verts,ENVector *norms)
{
 //Init data
 glEnableClientState(GL_VERTEX_ARRAY);
 glEnableClientState(GL_NORMAL_ARRAY);
 if(verts)
   glVertexPointer(3,GL_FLOAT,0,verts);
 else
   glVertexPointer(3,GL_FLOAT,0,frames[curframe].Vertexes);
 if(norms)
   glNormalPointer(GL_FLOAT,0,norms);
 else
   glNormalPointer(GL_FLOAT,0,frames[curframe].Normals);
 //Set skin
 skins[curskin]->SetBase();
 //Draw data
 glBegin(GL_TRIANGLES);
 for(ENuint a=0;a<header.NumFaces;a++)
 {
  glTexCoord2fv((ENfloat*)&faces[a].Coord[0]);
  glArrayElement(faces[a].Indices[0]);
  glTexCoord2fv((ENfloat*)&faces[a].Coord[1]);
  glArrayElement(faces[a].Indices[1]);
  glTexCoord2fv((ENfloat*)&faces[a].Coord[2]);
  glArrayElement(faces[a].Indices[2]);
 }
 glEnd();
 //Reset everything
 glDisableClientState(GL_VERTEX_ARRAY);
 glDisableClientState(GL_NORMAL_ARRAY);
}

void ENModelLoaded::DrawTex(ENVector *verts)
{
 //Init data
 if(verts)
   glVertexPointer(3,GL_FLOAT,0,verts);
 else
   glVertexPointer(3,GL_FLOAT,0,frames[curframe].Vertexes);
 //Set skin
 skins[curskin]->SetBase();
 //Draw data
 glBegin(GL_TRIANGLES);
 for(ENuint a=0;a<header.NumFaces;a++)
 {
  glTexCoord2fv((ENfloat*)&faces[a].Coord[0]);
  glArrayElement(faces[a].Indices[0]);
  glTexCoord2fv((ENfloat*)&faces[a].Coord[1]);
  glArrayElement(faces[a].Indices[1]);
  glTexCoord2fv((ENfloat*)&faces[a].Coord[2]);
  glArrayElement(faces[a].Indices[2]);
 }
 glEnd();
}

void ENModelLoaded::DrawLightData(ENVector *verts,ENVector *norms,ENuint tex)
{
 //Set vertexes
 if(verts)
   glVertexPointer(3,GL_FLOAT,0,verts);
 else
   glVertexPointer(3,GL_FLOAT,0,frames[curframe].Vertexes);
 //Set normals
 if(norms)
   glNormalPointer(GL_FLOAT,0,norms);
 else
   glNormalPointer(GL_FLOAT,0,frames[curframe].Normals);
 //Draw data
 if(isCurSkinBump())
 {
  ENGLExtension::ActiveTexture(tex);
  skins[curskin]->SetBump();
  ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);  
 }

 glBegin(GL_TRIANGLES);
 for(ENuint a=0;a<header.NumFaces;a++)
 {
  glTexCoord2fv((ENfloat*)&faces[a].Coord[0]);
  glArrayElement(faces[a].Indices[0]);
  glTexCoord2fv((ENfloat*)&faces[a].Coord[1]);
  glArrayElement(faces[a].Indices[1]);
  glTexCoord2fv((ENfloat*)&faces[a].Coord[2]);
  glArrayElement(faces[a].Indices[2]);
 }
 glEnd();
}

void ENModelLoaded::SetCurrentFrame(ENfloat frame)
{
 curframe=((ENuint)floor(frame))%header.NumFrames;
 nxtframe=(curframe+1)%header.NumFrames;
 framefact=(ENfloat)fmod(frame,1.0);
}

void ENModelLoaded::SetCurrentSkin(ENuint skin)
{
 curskin=skin%header.NumSkins;
}

void ENModelLoaded::GetBox(ENVector &min,ENVector &max)
{
 //min
 min=ENInterpLinear(frames[curframe].Min,frames[nxtframe].Min,framefact);
 //max
 max=ENInterpLinear(frames[curframe].Max,frames[nxtframe].Max,framefact);
}

void ENModelLoaded::GetBoxStatic(ENVector &min,ENVector &max)
{
 //min
 min=frames->Min;
 //max
 max=frames->Max;
}

void ENModelLoaded::GetSphere(ENVector &pos,ENfloat &range,ENbool Interp)
{
 if(Interp)
 {
  pos=ENInterpLinear(frames[curframe].Center,frames[nxtframe].Center,framefact);
  range=ENInterpLinear(frames[curframe].Range,frames[nxtframe].Range,framefact);
 }
 else
 {
  pos=frames->Center;
  range=frames->Range;
 }
}

void ENModelLoaded::GetHeader(ENLoadedHeader &h)
{
 h=header;
}

void ENModelLoaded::InterpVertexes(ENVector *vertexes)
{
 ENVector *verts1=frames[curframe].Vertexes;
 ENVector *verts2=frames[nxtframe].Vertexes;
 for(ENuint a=0;a<header.NumVertexes;a++)
   vertexes[a]=ENInterpLinear(verts1[a],verts2[a],framefact);
}

void ENModelLoaded::InterpVertex(ENuint ind,ENVector &vertex)
{
 ENVector *verts1=frames[curframe].Vertexes;
 ENVector *verts2=frames[nxtframe].Vertexes;
 vertex=ENInterpLinear(verts1[ind],verts2[ind],framefact);
}

void ENModelLoaded::InterpNormals (ENVector *normals)
{
 ENVector *norms1=frames[curframe].Normals;
 ENVector *norms2=frames[nxtframe].Normals;
 for(ENuint a=0;a<header.NumVertexes;a++)
   normals[a]=ENInterpLinear(norms1[a],norms2[a],framefact);
}

void ENModelLoaded::InterpData(ENVector *vertexes,ENVector *normals)
{
 ENVector *verts1=frames[curframe].Vertexes;
 ENVector *verts2=frames[nxtframe].Vertexes;
 ENVector *norms1=frames[curframe].Normals;
 ENVector *norms2=frames[nxtframe].Normals;
 for(ENuint a=0;a<header.NumVertexes;a++)
 {
  vertexes[a]=ENInterpLinear(verts1[a],verts2[a],framefact);
  normals[a]=ENInterpLinear(norms1[a],norms2[a],framefact);
 }
}

ENbool ENModelLoaded::isCurSkinBump()
{
 if(header.NumSkins)
   return skins[curskin]->isBumpSupported();
 else
   return false;
}

void ENModelLoaded::SetModelHeader(ENModel::ENModelHeader h)
{
 header.NumFrames=h.NumFrames;
 header.NumSkins=h.NumSkins;
 header.NumVertexes=h.NumVertexes;
 header.NumFaces=h.NumFaces;
}

void ENModelLoaded::SetModelFaces(ENModel *m)
{
 ENModel::ENModelHeader h=m->GetHeader();
 faces=(ENModel::ENModelFace*)ENAllocMemory(sizeof(ENModel::ENModelFace)*h.NumFaces);
 for(ENuint a=0;a<header.NumFaces;a++)
   faces[a]=m->GetFace(a);
}

void ENModelLoaded::SetModelSkins(ENModel *m)
{
 ENModel::ENModelHeader h=m->GetHeader();
 skins=(ENMaterialLoaded**)ENAllocMemory(sizeof(ENMaterialLoaded*)*h.NumSkins);
 for(ENuint a=0;a<h.NumSkins;a++)
 {
  ENMaterial *mat=new ENMaterial();
  m->GetSkinData(mat,a);
  skins[a]=new ENMaterialLoaded(mat,true);
  delete mat;
 }
}

void ENModelLoaded::SetModelFrames(ENModel *m)
{
 ENModel::ENModelHeader h=m->GetHeader();
 //Alloc space
 frames=(ENLoadedFrame*)ENAllocMemory(h.NumFrames*sizeof(ENLoadedFrame));
 //Calculate frames
 for(ENuint f=0;f<h.NumFrames;f++)
 {
  //Copy vertexes
  frames[f].Vertexes=(ENVector*)ENAllocMemory(sizeof(ENVector)*h.NumVertexes);
  for(ENuint v=0;v<h.NumVertexes;v++)
    frames[f].Vertexes[v]=m->GetVertex(v,f);
  //Calculate min and max vector
  CalculateFrameBox(frames+f);
  //Calculate range
  CalculateFrameRange(frames+f);
  //Calcualte vertex normals
  CalculateVertexNormals(frames+f);
 }
}

void ENModelLoaded::CalculateFrameRange(ENLoadedFrame *frame)
{
 frame->Range=0.0f;
 for(ENuint v=0;v<header.NumVertexes;v++)
 {
  //Calculate range
  ENfloat r=ENDistance(frame->Vertexes[v],frame->Center);
  //Set range 
  if(r>frame->Range) frame->Range=r;
 }
}

void ENModelLoaded::CalculateFrameBox(ENLoadedFrame *frame)
{
 frame->Min=frame->Vertexes[0];
 frame->Max=frame->Vertexes[0];
 for(ENuint v=0;v<header.NumVertexes;v++)
 {
  //Minimum vector
  if(frame->Min.v[0]>frame->Vertexes[v].v[0])
    frame->Min.v[0]=frame->Vertexes[v].v[0];
  if(frame->Min.v[1]>frame->Vertexes[v].v[1])
    frame->Min.v[1]=frame->Vertexes[v].v[1];
  if(frame->Min.v[2]>frame->Vertexes[v].v[2])
    frame->Min.v[2]=frame->Vertexes[v].v[2];
  //Maximum vector
  if(frame->Max.v[0]<frame->Vertexes[v].v[0])
    frame->Max.v[0]=frame->Vertexes[v].v[0];
  if(frame->Max.v[1]<frame->Vertexes[v].v[1])
    frame->Max.v[1]=frame->Vertexes[v].v[1];
  if(frame->Max.v[2]<frame->Vertexes[v].v[2])
    frame->Max.v[2]=frame->Vertexes[v].v[2];
 }
 frame->Center=(frame->Min+frame->Max)/2.0f;
}

void ENModelLoaded::CalculateFaceNormals(ENLoadedFrame *frame,ENVector *Normals)
{
 for(ENuint a=0;a<header.NumFaces;a++)
 {
  ENVector verts[3];
  verts[0]=frame->Vertexes[faces[a].Indices[0]];
  verts[1]=frame->Vertexes[faces[a].Indices[1]];
  verts[2]=frame->Vertexes[faces[a].Indices[2]];
  Normals[a]=ENNormal(verts)*(-1.0f);
 }
}

void ENModelLoaded::CalculateVertexNormals(ENLoadedFrame *frame)
{
 //Variables
 ENuint a,b;
 ENuint NumConnections;
 ENVector *TempNormals;
 //Init TempNormals
 TempNormals=new ENVector[header.NumFaces];
 //Calcualte face normals
 CalculateFaceNormals(frame,TempNormals);
 //Alloc space for vertex-normals
 frame->Normals=(ENVector*)ENAllocMemory(header.NumVertexes*sizeof(ENVector));
 //Calculate vertex-normals
 for(a=0;a<header.NumVertexes;a++)
 {
  NumConnections=0;
  frame->Normals[a]=ENVector(0,0,0);
  for(b=0;b<header.NumFaces;b++)
    if(faces[b].Indices[0]==a||faces[b].Indices[1]==a||faces[b].Indices[2]==a)
    {
     frame->Normals[a]=frame->Normals[a]+TempNormals[b];
     NumConnections++;
    }
  //Build vertex-normal
  if(NumConnections)
    frame->Normals[a]=frame->Normals[a]/NumConnections;
 }
 //Finished
 delete[] TempNormals;
}
