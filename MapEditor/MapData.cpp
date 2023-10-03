//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "MapData.h"
#include "MapSources.h"
#include "AddCubeUnit.h"
#include "AddConeUnit.h"
#include "AddCylinderUnit.h"
#include "AddSphereUnit.h"
#include "LightDlgUnit.h"
#include "ObjectDlgUnit.h"
#include "SoundDlgUnit.h"
#include "BuildDlgUnit.h"
#include "Quake3Loader.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//////////////////////////////////////////////////////////////////////
ENMap          *ENMapData::map=NULL;
ENBooleanArray *ENMapData::VertSelected=NULL;
ENBooleanArray *ENMapData::LightSelected=NULL;
ENBooleanArray *ENMapData::SoundSelected=NULL;
ENBooleanArray *ENMapData::ObjectSelected=NULL;
ENbool          ENMapData::CameraSelected;
char            ENMapData::CurrentFile[MAX_PATH];
ENbool          ENMapData::MustSave=false;
ENMaterialLoaded **ENMapData::materials=NULL;

void ENMapData::Init()
{
 CurrentFile[0]=0;
 MustSave=false;
 map=new ENMap();
 VertSelected=new ENBooleanArray();
 LightSelected=new ENBooleanArray();
 SoundSelected=new ENBooleanArray();
 ObjectSelected=new ENBooleanArray();
 CameraSelected=false;
}

void ENMapData::Free()
{
 DeleteAllTextures();
 delete map;
 delete VertSelected;
 delete LightSelected;
 delete SoundSelected;
 delete ObjectSelected;
}

void ENMapData::New()
{
 map->Clear();
 MustSave=false;
 CurrentFile[0]=0;
 DeleteAllTextures();
 VertSelected->Clear();
 LightSelected->Clear();
 SoundSelected->Clear();
 ObjectSelected->Clear();
 ENMapSources::Clear();
 CameraSelected=false;
}

void ENMapData::AddVertex(ENVector2 mpos,ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec)
{
 MustSave=true;
 //1. Convert mouse position to 3D space
 ENVector vertex=From2DTo3D(mpos,0.8,mspec);
 //2. Convert to window specification
 switch(type)
 {
  case ENDrawClass::ENDRAWTOP:
    vertex.v[1]=0.0f;
  break;
  case ENDrawClass::ENDRAWFRONT:
    vertex.v[2]=0.0f;
  break;
  case ENDrawClass::ENDRAWSIDE:
    vertex.v[0]=0.0f;
  break;
 }
 //Add vertex to model data
 AddVertexRaw(vertex);
}

void ENMapData::AddLight(ENVector2 mpos,ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec)
{
 MustSave=true;
 //1. Convert mouse position to 3D space
 ENVector pos=From2DTo3D(mpos,0.8,mspec);
 //2. Convert to window specification
 switch(type)
 {
  case ENDrawClass::ENDRAWTOP:
    pos.v[1]=0.0f;
  break;
  case ENDrawClass::ENDRAWFRONT:
    pos.v[2]=0.0f;
  break;
  case ENDrawClass::ENDRAWSIDE:
    pos.v[0]=0.0f;
  break;
 }
 //Call light dialog
 LightDlg->NewLightDlg(pos);
 if(LightDlg->isDataValid())
 {
  map->AddLight(LightDlg->GetLight());
  ENMapBase::ENMapHeader header=GetHeader();
  LightSelected->AddElement(1);
 }
}

void ENMapData::EditLight()
{
 ENMapBase::ENMapHeader header=GetHeader();
 for(ENuint a=0;a<header.NumLights;a++)
   if(IsLightSelected(a))
   {
    LightDlg->EditLightDlg(map->GetLight(a));
    if(LightDlg->isDataValid())
      map->SetLight(a,LightDlg->GetLight());

    return;
   }
}

void ENMapData::AddSound(ENVector2 mpos,ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec)
{
 MustSave=true;
 //1. Convert mouse position to 3D space
 ENVector pos=From2DTo3D(mpos,0.8,mspec);
 //2. Convert to window specification
 switch(type)
 {
  case ENDrawClass::ENDRAWTOP:
    pos.v[1]=0.0f;
  break;
  case ENDrawClass::ENDRAWFRONT:
    pos.v[2]=0.0f;
  break;
  case ENDrawClass::ENDRAWSIDE:
    pos.v[0]=0.0f;
  break;
 }
 //Call sound dialog
 SoundDlg->NewSoundDlg(pos);
 if(SoundDlg->isDataValid())
 {
  map->AddSound(SoundDlg->GetSound());
  ENMapBase::ENMapHeader header=GetHeader();
  SoundSelected->AddElement(1);
 }
}

void ENMapData::EditSound()
{
 ENMapBase::ENMapHeader header=GetHeader();
 for(ENuint a=0;a<header.NumSounds;a++)
   if(IsSoundSelected(a))
   {
    SoundDlg->EditSoundDlg(map->GetSound(a));
    if(SoundDlg->isDataValid())
      map->SetSound(a,SoundDlg->GetSound());

    return;
   }
}

void ENMapData::AddObject(ENVector2 mpos,ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec)
{
 MustSave=true;
 //1. Convert mouse position to 3D space
 ENVector pos=From2DTo3D(mpos,0.8,mspec);
 //2. Convert to window specification
 switch(type)
 {
  case ENDrawClass::ENDRAWTOP:
    pos.v[1]=0.0f;
  break;
  case ENDrawClass::ENDRAWFRONT:
    pos.v[2]=0.0f;
  break;
  case ENDrawClass::ENDRAWSIDE:
    pos.v[0]=0.0f;
  break;
 }
 //Call object dialog
 ObjectDlg->NewObjectDlg(pos);
 if(ObjectDlg->isDataValid())
 {
  map->AddObject(ObjectDlg->GetObject());
  ENMapBase::ENMapHeader header=GetHeader();
  ObjectSelected->AddElement(1);
 }  
}

void ENMapData::EditObject()
{
 ENMapBase::ENMapHeader header=GetHeader();
 for(ENuint a=0;a<header.NumObjects;a++)
   if(IsObjectSelected(a))
   {
    ObjectDlg->EditObjectDlg(map->GetObject(a));
    if(ObjectDlg->isDataValid())
      map->SetObject(a,ObjectDlg->GetObject());

    return;      
   }
}

void ENMapData::SetPackage(char *FileName)
{
 MustSave=true;
 map->SetPackage(FileName);
 ENMapSources::Load(ExtractFileDir(CurrentFile).c_str(),FileName);
}

void ENMapData::Build(char *FileName)
{
 ENMapBuilder *builder=new ENMapBuilder(map);
 builder->SetReportFunc(TBuildDlg::ReportFunc);
 if(!builder->Build(FileName))
   ShowMessage("Cannot build map as "+AnsiString(FileName)+"!!!");
}

void ENMapData::SetSky(ENuint ind,ENPicture *pic)
{
 MustSave=true;
 map->SetSkyBox(ind,pic);
}

void ENMapData::GetSky(ENuint ind,ENPicture *pic)
{
 map->GetSkyBox(ind,pic);
}

void ENMapData::SetSun(ENMapBase::ENMapSun *sun)
{
 MustSave=true;
 map->SetSun(sun);
}

void ENMapData::GetSun(ENMapBase::ENMapSun *sun)
{
 map->GetSun(sun);
}

ENCamera ENMapData::GetCamera()
{
 return map->GetCamera();
}

void ENMapData::SetCamera(ENCamera cam)
{
 map->SetCamera(cam);
}

void ENMapData::AddPrefab(ENMapData::ENModelPrefab prefab,ENVector2 mpos,
                            ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec)
{
 MustSave=true;
 //1. Convert mouse position to 3D space
 ENVector pos=From2DTo3D(mpos,0.8,mspec);
 //2. Convert to window specification
 switch(type)
 {
  case ENDrawClass::ENDRAWTOP:
    pos.v[1]=0.0f;
  break;
  case ENDrawClass::ENDRAWFRONT:
    pos.v[2]=0.0f;
  break;
  case ENDrawClass::ENDRAWSIDE:
    pos.v[0]=0.0f;
  break;
 }
 //Call dialog
 switch(prefab)
 {
  case ENPREFABCUBE:
    AddCubeDlg->SetPosValues(pos);
    AddCubeDlg->ShowModal();
  break;
  case ENPREFABCONE:
    AddConeDlg->SetPosValues(pos);
    AddConeDlg->ShowModal();
  break;
  case ENPREFABCYLINDER:
    AddCylinderDlg->SetPosValues(pos);
    AddCylinderDlg->ShowModal();
  break;
  case ENPREFABSPHERE:
    AddSphereDlg->SetPosValues(pos);
    AddSphereDlg->ShowModal();
  break;
 }
}

void ENMapData::Select(ENVector2 a,ENVector2 b,
                         ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec)
{
 ENuint i;
 ENVector2 rc[2];
 ENVector vert2D;
 //X
 if(a.v[0]>=b.v[0])
 {
  rc[0].v[0]=b.v[0];
  rc[1].v[0]=a.v[0];
 }
 else
 {
  rc[1].v[0]=b.v[0];
  rc[0].v[0]=a.v[0];
 }
 //Y
 if(a.v[1]>=b.v[1])
 {
  rc[0].v[1]=b.v[1];
  rc[1].v[1]=a.v[1];
 }
 else
 {
  rc[1].v[1]=b.v[1];
  rc[0].v[1]=a.v[1];
 }
 ENMapBase::ENMapHeader header=map->GetHeader();
 //Check all vertexes
 for(i=0;i<header.NumVertexes;i++)
 {
  vert2D=From3DTo2D(map->GetVertex(i),mspec);
  if(vert2D.v[0]>=rc[0].v[0]&&vert2D.v[0]<=rc[1].v[0]&&
     vert2D.v[1]>=rc[0].v[1]&&vert2D.v[1]<=rc[1].v[1]&&
     (type!=ENDrawClass::ENDRAW3D||
     (vert2D.v[2]>=0&&vert2D.v[2]<=1)))
      VertSelected->SetElement(1,i);
 }
 //Check all lights
 for(i=0;i<header.NumLights;i++)
 {
  vert2D=From3DTo2D(map->GetLight(i).pos,mspec);
  if(vert2D.v[0]>=rc[0].v[0]&&vert2D.v[0]<=rc[1].v[0]&&
     vert2D.v[1]>=rc[0].v[1]&&vert2D.v[1]<=rc[1].v[1]&&
     (type!=ENDrawClass::ENDRAW3D||
     (vert2D.v[2]>=0&&vert2D.v[2]<=1)))
       LightSelected->SetElement(1,i);
 }
 //Check all sounds
 for(i=0;i<header.NumSounds;i++)
 {
  vert2D=From3DTo2D(map->GetSound(i).pos,mspec);
  if(vert2D.v[0]>=rc[0].v[0]&&vert2D.v[0]<=rc[1].v[0]&&
     vert2D.v[1]>=rc[0].v[1]&&vert2D.v[1]<=rc[1].v[1]&&
     (type!=ENDrawClass::ENDRAW3D||
     (vert2D.v[2]>=0&&vert2D.v[2]<=1)))
       SoundSelected->SetElement(1,i);
 }
 //Check all objects
 for(i=0;i<header.NumObjects;i++)
 {
  vert2D=From3DTo2D(map->GetObject(i).Pos,mspec);
  if(vert2D.v[0]>=rc[0].v[0]&&vert2D.v[0]<=rc[1].v[0]&&
     vert2D.v[1]>=rc[0].v[1]&&vert2D.v[1]<=rc[1].v[1]&&
     (type!=ENDrawClass::ENDRAW3D||
     (vert2D.v[2]>=0&&vert2D.v[2]<=1)))
       ObjectSelected->SetElement(1,i);
 }
 //Check camera
 vert2D=From3DTo2D(GetCamera().Pos,mspec);
 if(vert2D.v[0]>=rc[0].v[0]&&vert2D.v[0]<=rc[1].v[0]&&
    vert2D.v[1]>=rc[0].v[1]&&vert2D.v[1]<=rc[1].v[1]&&
    (type!=ENDrawClass::ENDRAW3D||
    (vert2D.v[2]>=0&&vert2D.v[2]<=1)))
      CameraSelected=true;
}

ENint ENMapData::SelectOneVert(ENVector2 p,ENDrawClass::ENDrawClassTypes type,
                                 ENMatrixSpec mspec)
{
 //Variables
 ENint res=-1;
 ENfloat depth=0.0f;
 ENMapBase::ENMapHeader header=GetHeader();
 //Check all vertexes
 for(ENuint a=0;a<header.NumVertexes;a++)
 {
  ENVector vert2D=From3DTo2D(map->GetVertex(a),mspec);
  if(vert2D.v[0]>=p.v[0]-2&&vert2D.v[0]<=p.v[0]+2&&
     vert2D.v[1]>=p.v[1]-2&&vert2D.v[1]<=p.v[1]+2&&
     (type!=ENDrawClass::ENDRAW3D||
     (vert2D.v[2]>=0&&vert2D.v[2]<=1)))
      if(res==-1||depth<vert2D.v[2])
      {
       res=a;
       depth=vert2D.v[2];
      }
 }
 return res;
}

void ENMapData::AddVertexRaw(ENVector vec)
{
 ENMapBase::ENMapHeader header=map->GetHeader();
 MustSave=true;
 map->AddVertex(vec);
 VertSelected->AddElement(1);
}

void ENMapData::AddVertexes(ENVector *verts,ENuint num)
{
 MustSave=true;
 ENMapBase::ENMapHeader header=map->GetHeader();
 VertSelected->AddElements(1,num);
 map->AddVertexes(verts,num);
}

void ENMapData::AddFace(ENuint Ind[])
{
 MustSave=true;
 ENMapBase::ENMapFace face;
 ENMapBase::InitMapFace(face);
 memcpy(face.indices,Ind,sizeof(ENuint)*3);
 CubeCoord(&face,ENVector2(0.0f,0.0f),ENVector2(64.0f,64.0f),0.0f);
 map->AddFace(face);
}

void ENMapData::AddFace2(ENMapBase::ENMapFace *face)
{
 MustSave=true;
 map->AddFace(*face);
}

void ENMapData::AddFaces(ENMapBase::ENMapFace *faces,ENuint num)
{
 MustSave=true;
 map->AddFaces(faces,num);
}

ENVector ENMapData::From2DTo3D(ENVector2 mpos,ENfloat depth,ENMatrixSpec mspec)
{
 //Variables
 ENdouble     res[3];
 //Unproject
 gluUnProject(mpos.v[0],mpos.v[1],depth,
              mspec.modelMatrix,mspec.projMatrix,mspec.viewport,
              &res[0],&res[1],&res[2]);
 //Return result
 return ENVector(res[0],res[1],res[2]);
}

ENVector ENMapData::From3DTo2D(ENVector vec,ENMatrixSpec mspec)
{
 //Variables
 ENdouble     res[3];
 //Unproject
 gluProject(vec.v[0],vec.v[1],vec.v[2],
            mspec.modelMatrix,mspec.projMatrix,mspec.viewport,
            &res[0],&res[1],&res[2]);
 //Return result
 return ENVector(res[0],res[1],res[2]);
}

void ENMapData::SetGLVertexes()
{
 map->SetGLVertexPointer();
}

ENVector ENMapData::GetVertex(ENuint ind)
{
 return map->GetVertex(ind);
}

ENLight ENMapData::GetLight(ENuint ind)
{
 return map->GetLight(ind);
}

ENSoundSrc ENMapData::GetSound(ENuint ind)
{
 return map->GetSound(ind);
}

ENMapBase::ENMapObject ENMapData::GetObject(ENuint ind)
{
 return map->GetObject(ind);
}

ENMapBase::ENMapFace ENMapData::GetFace(ENuint ind)
{
 return map->GetFace(ind);
}

void ENMapData::SetFace(ENuint ind,ENMapBase::ENMapFace face)
{
 MustSave=true;
 map->SetFace(ind,face);
}

ENfloat ENMapData::GetMinVal(char coord)
{
 switch(coord)
 {
  case 'x':
    return map->GetMinX();
  case 'y':
    return map->GetMinY();
  case 'z':
    return map->GetMinZ();
 }

 return 0.0f;
}

ENfloat ENMapData::GetMaxVal(char coord)
{
 switch(coord)
 {
  case 'x':
    return map->GetMaxX();
  case 'y':
    return map->GetMaxY();
  case 'z':
    return map->GetMaxZ();
 }

 return 0.0f;
}

ENMap::ENMapHeader ENMapData::GetHeader()
{
 return map->GetHeader();
}

void ENMapData::SelectAll()
{
 ENMapBase::ENMapHeader header=GetHeader();
 VertSelected->SetElements(1,0,header.NumVertexes-1);
 LightSelected->SetElements(1,0,header.NumLights-1);
 SoundSelected->SetElements(1,0,header.NumSounds-1);
 ObjectSelected->SetElements(1,0,header.NumObjects-1);
 CameraSelected=true;
}

void ENMapData::SelectNone()
{
 ENMapBase::ENMapHeader header=GetHeader();
 VertSelected->SetElements(0,0,header.NumVertexes-1);
 LightSelected->SetElements(0,0,header.NumLights-1);
 SoundSelected->SetElements(0,0,header.NumSounds-1);
 ObjectSelected->SetElements(0,0,header.NumObjects-1);
 CameraSelected=false;
}

void ENMapData::InvertSelected()
{
 ENuint a;
 ENMapBase::ENMapHeader header=GetHeader();
 //Vertexes
 for(a=0;a<header.NumVertexes;a++)
   if(VertSelected->GetElement(a))
     VertSelected->SetElement(0,a);
   else
     VertSelected->SetElement(1,a);
 //Lights
 for(a=0;a<header.NumLights;a++)
   if(LightSelected->GetElement(a))
     LightSelected->SetElement(0,a);
   else
     LightSelected->SetElement(1,a);
 //Sounds
 for(a=0;a<header.NumSounds;a++)
   if(SoundSelected->GetElement(a))
     SoundSelected->SetElement(0,a);
   else
     SoundSelected->SetElement(1,a);
 //Objects
 for(a=0;a<header.NumObjects;a++)
   if(ObjectSelected->GetElement(a))
     ObjectSelected->SetElement(0,a);
   else
     ObjectSelected->SetElement(1,a);
 //Camera
 CameraSelected=!CameraSelected;     
}

ENbool ENMapData::IsVertSelected(ENuint ind)
{
 return VertSelected->GetElement(ind);
}

ENbool ENMapData::IsLightSelected(ENuint ind)
{
 return LightSelected->GetElement(ind);
}

ENbool ENMapData::IsSoundSelected(ENuint ind)
{
 return SoundSelected->GetElement(ind);
}

ENbool ENMapData::IsObjectSelected(ENuint ind)
{
 return ObjectSelected->GetElement(ind);
}

ENbool ENMapData::IsCameraSelected()
{
 return CameraSelected;
}

ENbool ENMapData::IsFaceSelected(ENuint ind)
{
 ENMapBase::ENMapFace face=map->GetFace(ind);
 if(IsVertSelected(face.indices[0])&&
    IsVertSelected(face.indices[1])&&
    IsVertSelected(face.indices[2]))
    return true;
 else
    return false;
}

ENbool ENMapData::Save()
{
 if(CurrentFile[0]==0)
   return false;
 else
   Save(CurrentFile);

 return true;
}

void ENMapData::Save(char *FileName)
{
 if(!map->Write(FileName))
 {
  char errmsg[1024];
  map->GetErrorLog(errmsg);
  ShowMessage(errmsg);
 }
 else
 {
  strcpy(CurrentFile,FileName);
  MustSave=false;
 }
}

void ENMapData::Open(char *FileName)
{
 DeleteAllTextures();
 if(!map->Load(FileName))
 {
  char errmsg[1024];
  map->GetErrorLog(errmsg);
  ShowMessage(errmsg);
 }
 else
 {
  MustSave=false;
  strcpy(CurrentFile,FileName);
  ENMapBase::ENMapHeader header=GetHeader();
  VertSelected->Clear();
  LightSelected->Clear();
  SoundSelected->Clear();
  ObjectSelected->Clear();
  VertSelected->AddElements(0,header.NumVertexes);
  LightSelected->AddElements(0,header.NumLights);
  SoundSelected->AddElements(0,header.NumSounds);
  ObjectSelected->AddElements(0,header.NumObjects);
  CameraSelected=false;
  InitAllTextures();
  //Load map package
  if(*header.PackageFile)
    ENMapSources::Load(ExtractFileDir(FileName).c_str(),header.PackageFile);
 }
}

void ENMapData::TranslateTex(ENVector2 Pos)
{
 ENuint i;
 ENMapBase::ENMapHeader header;
 header=GetHeader();
 MustSave=true;
 //Translate tex coord
 for(i=0;i<header.NumFaces;i++)
   if(IsFaceSelected(i))
 {
  ENMapBase::ENMapFace face=map->GetFace(i);
  face.texcoord[0]=face.texcoord[0]+Pos/10.0f;
  face.texcoord[1]=face.texcoord[1]+Pos/10.0f;
  face.texcoord[2]=face.texcoord[2]+Pos/10.0f;
  map->SetFace(i,face);
 }
}

void ENMapData::Translate(ENVector Pos,ENbool selected)
{
 ENuint i;
 ENMapBase::ENMapHeader header;
 header=GetHeader();
 MustSave=true;
 //Translate vertexes
 for(i=0;i<header.NumVertexes;i++)
   if(!selected||(selected&&IsVertSelected(i)))
 {
  ENVector vec=map->GetVertex(i);
  map->SetVertex(i,vec+Pos);
 }
 //Translate lights
 for(i=0;i<header.NumLights;i++)
   if(!selected||(selected&&IsLightSelected(i)))
 {
  ENLight light=map->GetLight(i);
  light.pos=light.pos+Pos;
  map->SetLight(i,light);
 }
 //Translate sounds
 for(i=0;i<header.NumSounds;i++)
   if(!selected||(selected&&IsSoundSelected(i)))
 {
  ENSoundSrc sound=map->GetSound(i);
  sound.pos=sound.pos+Pos;
  map->SetSound(i,sound);
 }
 //Translate objects
 for(i=0;i<header.NumObjects;i++)
   if(!selected||(selected&&IsObjectSelected(i)))
 {
  ENMapBase::ENMapObject obj=map->GetObject(i);
  obj.Pos=obj.Pos+Pos;
  map->SetObject(i,obj);
 }
 //Translate camera
 if(!selected||(selected&&CameraSelected))
 {
  ENCamera cam=GetCamera();
  cam.Pos=cam.Pos+Pos;
  SetCamera(cam);
 }
}

void ENMapData::ScaleTex(ENVector2 svec)
{
 ENuint i;
 ENMapBase::ENMapHeader header;
 header=GetHeader();
 MustSave=true;
 //Scale texcoord
 for(i=0;i<header.NumFaces;i++)
   if(IsFaceSelected(i))
 {
  ENMapBase::ENMapFace face=map->GetFace(i);
  face.texcoord[0]=face.texcoord[0]*(ENVector2(1,1)+svec/10.0f);
  face.texcoord[1]=face.texcoord[1]*(ENVector2(1,1)+svec/10.0f);
  face.texcoord[2]=face.texcoord[2]*(ENVector2(1,1)+svec/10.0f);
  map->SetFace(i,face);
 }
}

void ENMapData::Scale(ENVector center,ENVector svec,ENbool selected)
{
 ENuint i;
 ENMapBase::ENMapHeader header;
 header=GetHeader();
 MustSave=true;
 //Scale vertexes
 for(i=0;i<header.NumVertexes;i++)
   if(!selected||(selected&&IsVertSelected(i)))
 {
  ENVector vec=map->GetVertex(i);
  vec=(vec-center)*(ENVector(1,1,1)+svec/100.0f)+center;
  map->SetVertex(i,vec);
 }
 //Scale lights
 for(i=0;i<header.NumLights;i++)
   if(!selected||(selected&&IsLightSelected(i)))
 {
  ENLight light=map->GetLight(i);
  light.range=light.range*(1.0f+(svec.v[0]+svec.v[1]+svec.v[2])/100.0f);
  map->SetLight(i,light);
 }
 //Scale sounds
 for(i=0;i<header.NumSounds;i++)
   if(!selected||(selected&&IsSoundSelected(i)))
 {
  ENSoundSrc sound=map->GetSound(i);
  sound.range=sound.range*(1.0f+(svec.v[0]+svec.v[1]+svec.v[2])/100.0f);
  map->SetSound(i,sound);
 }
 //Scale objects
 for(i=0;i<header.NumObjects;i++)
   if(!selected||(selected&&IsObjectSelected(i)))
 {
  ENMapBase::ENMapObject obj=map->GetObject(i);
  obj.Scale=obj.Scale+(svec/100.0f)+center;
  map->SetObject(i,obj);
 }
}

void ENMapData::RotateTex(ENfloat angle)
{
 ENuint i;
 ENMapBase::ENMapHeader header;
 header=GetHeader();
 MustSave=true;
 //Rotate texcoord
 for(i=0;i<header.NumFaces;i++)
   if(IsFaceSelected(i))
 {
  ENMapBase::ENMapFace face=map->GetFace(i);
  RotateCoord(face.texcoord[0],angle*10);
  RotateCoord(face.texcoord[1],angle*10);
  RotateCoord(face.texcoord[2],angle*10);
  map->SetFace(i,face);
 }
}

void ENMapData::RotateCoord(ENVector2 &coord,ENfloat angle)
{
 ENVector c;
 c=ENRotMatrixZ(ENRadians(angle))*ENVector(coord.v[0],coord.v[1],0.0f);
 coord.v[0]=c.v[0];
 coord.v[1]=c.v[1];
}

void ENMapData::Rotate(ENVector center,ENfloat angle,char Axis,ENbool selected)
{
 //Get number of vertexes
 ENuint i;
 ENMapBase::ENMapHeader header;
 header=GetHeader();
 //set must save flag
 MustSave=true;
 //Get rotation matrix
 ENMatrix3X3 rmat;
 switch(Axis)
 {
  case 'x':
  case 'X':
    rmat=ENRotMatrixX(angle);
  break;
  case 'y':
  case 'Y':
    rmat=ENRotMatrixY(angle);
  break;
  case 'z':
  case 'Z':
    rmat=ENRotMatrixZ(angle);
  break;
 };
 //Rotate vertexes
 for(i=0;i<header.NumVertexes;i++)
   if(!selected||(selected&&IsVertSelected(i)))
   {
    ENVector vec=map->GetVertex(i);
    vec=rmat*(vec-center)+center;
    map->SetVertex(i,vec);
   }
 //Rotate lights
 for(i=0;i<header.NumLights;i++)
   if(!selected||(selected&&IsLightSelected(i)))
   {
    ENLight light=map->GetLight(i);
    switch(Axis)
    {
     case 'x':
     case 'X':
     case 'z':
     case 'Z':
       light.dir.v[0]+=angle;
     break;
     case 'y':
     case 'Y':
       light.dir.v[1]+=angle;
     break;
    }
    map->SetLight(i,light);
   }
 //Rotate sounds
 for(i=0;i<header.NumSounds;i++)
   if(!selected||(selected&&IsSoundSelected(i)))
   {
    ENSoundSrc sound=map->GetSound(i);
    switch(Axis)
    {
     case 'x':
     case 'X':
     case 'z':
     case 'Z':
       sound.dir.v[0]+=angle;
     break;
     case 'y':
     case 'Y':
       sound.dir.v[1]+=angle;
     break;
    }
    map->SetSound(i,sound);
   }
 //Rotate objects
 for(i=0;i<header.NumObjects;i++)
   if(!selected||(selected&&IsObjectSelected(i)))
   {
    ENMapBase::ENMapObject obj=map->GetObject(i);
    switch(Axis)
    {
     case 'x':
     case 'X':
       obj.Angle.v[0]+=angle;
     break;
     case 'z':
     case 'Z':
       obj.Angle.v[2]+=angle;
     break;
     case 'y':
     case 'Y':
       obj.Angle.v[1]+=angle;
     break;
    }
    map->SetObject(i,obj);
   }
}

ENbool ENMapData::HasToSave()
{
 return MustSave;
}

ENuint ENMapData::GetNumSelected()
{
 ENuint c=0;

 ENMapBase::ENMapHeader header;
 header=map->GetHeader();
 for(ENuint a=0;a<header.NumVertexes;a++)
   if(IsVertSelected(a))
     c++;

 return c;
}

void ENMapData::GetSelectedVerts(ENVector *Vertexes)
{
 ENuint c=0;

 ENMapBase::ENMapHeader header;
 header=map->GetHeader();
 for(ENuint a=0;a<header.NumVertexes;a++)
   if(IsVertSelected(a))
   {
    Vertexes[c]=GetVertex(a);
    c++;
   }
}

void ENMapData::BoxSelected(ENVector &min,ENVector &max)
{
 //Variables
 ENuint num,i;
 ENVector *verts=NULL;
 //Get number of selected vertexes
 num=GetNumSelected();
 if(num)
 {
  //Alloc space and get selected vertexes
  verts=new ENVector[num];
  GetSelectedVerts(verts);
  //Init min and max
  min=verts[0];
  max=verts[0];
  //Get min and max vector
  for(i=1;i<num;i++)
  {
   ENVector vec=verts[i];
   //Min x
   if(vec.v[0]<min.v[0]) min.v[0]=vec.v[0];
   //Min y
   if(vec.v[1]<min.v[1]) min.v[1]=vec.v[1];
   //Min z
   if(vec.v[2]<min.v[2]) min.v[2]=vec.v[2];
   //Max x
   if(vec.v[0]>max.v[0]) max.v[0]=vec.v[0];
   //Max y
   if(vec.v[1]>max.v[1]) max.v[1]=vec.v[1];
   //Max z
   if(vec.v[2]>max.v[2]) max.v[2]=vec.v[2];
  }
  //Delete selected vertexes
  delete[] verts;
 }
 else
 {//No selected vertexes
  min=ENVector(0,0,0);
  max=ENVector(0,0,0);
 }
}

void ENMapData::Box(ENVector &min,ENVector &max)
{
 //Variables
 ENuint i;
 ENMapBase::ENMapHeader header;
 //Get header
 header=map->GetHeader();
 //Get number of selected vertexes
 if(header.NumVertexes)
 {
  //Init min and max
  min=GetVertex(0);
  max=GetVertex(0);
  //Get min and max vector
  for(i=1;i<header.NumVertexes;i++)
  {
   ENVector vec=GetVertex(i);
   //Min x
   if(vec.v[0]<min.v[0]) min.v[0]=vec.v[0];
   //Min y
   if(vec.v[1]<min.v[1]) min.v[1]=vec.v[1];
   //Min z
   if(vec.v[2]<min.v[2]) min.v[2]=vec.v[2];
   //Max x
   if(vec.v[0]>max.v[0]) max.v[0]=vec.v[0];
   //Max y
   if(vec.v[1]>max.v[1]) max.v[1]=vec.v[1];
   //Max z
   if(vec.v[2]>max.v[2]) max.v[2]=vec.v[2];
  }
 }
 else
 {//No selected vertexes
  min=ENVector(0,0,0);
  max=ENVector(0,0,0);
 }
}

ENVector ENMapData::Center(ENbool selected)
{
 ENVector min,max;
 if(selected)
   BoxSelected(min,max);
 else
   Box(min,max);

 return (min+max)/2;
}

AnsiString ENMapData::GetGroupName(ENuint ind)
{
 char GroupName[33];
 map->GetGroupName(ind,GroupName);
 return AnsiString(GroupName);
}

void ENMapData::CreateGroup(char *GroupName)
{
 if(!map->CreateGroup(GroupName))
   ShowMessage("Group name already exists!!!");
 else
   MustSave=true;
}

void ENMapData::RenameGroup(ENuint ind,char *GroupName)
{
 if(!map->RenameGroup(ind,GroupName))
   ShowMessage("Group name already exists!!!");
 else
   MustSave=true;
}

void ENMapData::DeleteGroup(ENuint ind)
{
 MustSave=true;
 map->DeleteGroup(ind);
}

void ENMapData::SelectGroup(ENuint ind)
{
 ENuint num=map->GetNumGroupIndices(ind);
 ENuint *Indices=new ENuint[num];
 map->GetGroupIndices(ind,Indices);
 for(ENuint a=0;a<num;a++)
   VertSelected->SetElement(1,Indices[a]);

 delete[] Indices;
}

void ENMapData::AddGeometryToGroup(ENuint ind)
{
 MustSave=true;
 ENuint num=GetNumSelected();
 ENMapBase::ENMapHeader header=GetHeader();
 if(num)
 {
  ENuint *Indices=new ENuint[num];
  ENuint c=0;
  for(ENuint b=0;b<header.NumVertexes;b++)
    if(IsVertSelected(b))
    {
     Indices[c]=b;
     c++;
    }

  map->AddGeometry2Group(ind,Indices,num);
  delete[] Indices;
 }
}

void ENMapData::DeleteSelectedFaces()
{
 MustSave=true;
 ENMapBase::ENMapHeader header=GetHeader();
 ENuint numFaces=header.NumFaces;
 ENuint ind=0;
 while(ind<numFaces)
 {
  if(IsFaceSelected(ind))
  {
   map->DeleteFace(ind);
   header=GetHeader();
   numFaces=header.NumFaces;
  }
  else
    ind++;
 }
}

void ENMapData::DeleteSelected()
{
 MustSave=true;
 ENMapBase::ENMapHeader header=GetHeader();
 ENuint numVerts=header.NumVertexes;
 ENuint numLights=header.NumLights;
 ENuint numSounds=header.NumSounds;
 ENuint numObjs=header.NumObjects;
 ENuint ind=0;
 //Delete all selected vertexes
 while(ind<numVerts)
 {
  if(IsVertSelected(ind))
  {
   map->DeleteVertex(ind);
   VertSelected->DelElement(ind);
   header=GetHeader();
   numVerts=header.NumVertexes;
  }
  else
    ind++;
 }
 //Delete all selected lights
 ind=0;
 while(ind<numLights)
 {
  if(IsLightSelected(ind))
  {
   map->DeleteLight(ind);
   LightSelected->DelElement(ind);
   header=GetHeader();
   numLights=header.NumLights;
  }
  else
    ind++;
 }
 //Delete all selected sounds
 ind=0;
 while(ind<numSounds)
 {
  if(IsSoundSelected(ind))
  {
   map->DeleteSound(ind);
   SoundSelected->DelElement(ind);
   header=GetHeader();
   numSounds=header.NumSounds;
  }
  else
    ind++;
 }
 //Delete all selected objects
 ind=0;
 while(ind<numObjs)
 {
  if(IsObjectSelected(ind))
  {
   map->DeleteObject(ind);
   ObjectSelected->DelElement(ind);
   header=GetHeader();
   numObjs=header.NumObjects;
  }
  else
    ind++;
 }
}

void ENMapData::SelectConnected()
{
 ENMapBase::ENMapHeader header=GetHeader();
 for(ENuint a=0;a<header.NumVertexes;a++)
   if(IsVertSelected(a))
     SelectConnectedRe(a);
}

void ENMapData::SelectConnectedRe(ENuint index)
{
 ENMapBase::ENMapFace   face;
 ENMapBase::ENMapHeader header=GetHeader();
 VertSelected->SetElement(1,index);
  for(ENuint a=0;a<header.NumFaces;a++)
  {
   face=map->GetFace(a);
   if(face.indices[0]==index||face.indices[1]==index||face.indices[2]==index)
    {
      if(!IsVertSelected(face.indices[0]))
        SelectConnectedRe(face.indices[0]);
      if(!IsVertSelected(face.indices[1]))
        SelectConnectedRe(face.indices[1]);
      if(!IsVertSelected(face.indices[2]))
        SelectConnectedRe(face.indices[2]);
    }
  }
}

void ENMapData::FlipNormals()
{
 MustSave=true;
 ENMapBase::ENMapHeader header=GetHeader();
 for(ENuint a=0;a<header.NumFaces;a++)
   if(IsFaceSelected(a))
   {
    //Get face
    ENMapBase::ENMapFace face=map->GetFace(a);
    //Flip indices
    face.indices[0]^=face.indices[2];
    face.indices[2]^=face.indices[0];
    face.indices[0]^=face.indices[2];
    //Set face
    map->SetFace(a,face);
   }
}

void ENMapData::ConnectAll(ENfloat tolerance)
{
 MustSave=true;
 ENMapBase::ENMapHeader header;
 ENuint  numVerts;

 header=GetHeader();
 numVerts=header.NumVertexes;

 for(ENuint a=0;a<numVerts;a++)
   for(ENuint b=a+1;b<numVerts;b++)
     if(ENDistance(GetVertex(a),GetVertex(b))<=tolerance)
     {
      ConnectVertexes(a,b);
      map->DeleteVertex(b);
      header=GetHeader();
      numVerts=header.NumVertexes;
     }
}

void ENMapData::ConnectVertexes(ENuint a,ENuint b)
{
 ENMapBase::ENMapHeader header=GetHeader();
 for(ENuint i=0;i<header.NumFaces;i++)
 {
  ENMapBase::ENMapFace face=map->GetFace(i);
  if(face.indices[0]==b) face.indices[0]=a;
  if(face.indices[1]==b) face.indices[1]=a;
  if(face.indices[2]==b) face.indices[2]=a;
  map->SetFace(i,face);
 }
}

void ENMapData::Optimize(ENbool UnusedVerts,ENbool Verts,ENfloat toleranz)
{
 //Variables
 ENMapBase::ENMapHeader header;
 ENuint  numVerts;
 ENuint  c=0;
 //Delete all unused vertexes
 MustSave=true;
 if(UnusedVerts)
 {
  header=GetHeader();
  numVerts=header.NumVertexes;
  while(c<numVerts)
  {
   if(IsVertexUnused(c))
   {
    map->DeleteVertex(c);
    header=GetHeader();
    numVerts=header.NumVertexes;
   }
   else
     c++;
  }
 }
 //Optimize vertexes
 if(Verts) ConnectAll(toleranz);
}

ENbool ENMapData::IsVertexUnused(ENuint ind)
{
 ENMapBase::ENMapHeader header=GetHeader();
 for(ENuint a=0;a<header.NumFaces;a++)
 {
  ENMapBase::ENMapFace face=map->GetFace(a);
  if(face.indices[0]==ind||face.indices[1]==ind||face.indices[2]==ind)
    return false;
 }

 return true;
}

AnsiString ENMapData::GetTextureName(ENuint ind)
{
 char TexName[33];
 map->GetTextureName(ind,TexName);
 return AnsiString(TexName);
}

void ENMapData::CreateTexture(char *Name,char *FileName)
{
 if(!map->CreateTexture(Name,FileName))
   ShowMessage("Cannot create texture "+AnsiString(FileName)+"!!!");
 else
 {
  MustSave=true;
  //Create new material
  ENMaterialLoaded *lmat;
  ENMapBase::ENMapHeader header=GetHeader();
  ENMaterial *mat=new ENMaterial();
  map->GetTextureData(mat,header.NumTextures-1);
  lmat=new ENMaterialLoaded(mat,true);
  delete mat;
  //Add new material
  header.NumTextures--;
  materials=(ENMaterialLoaded**)ENAddElement((ENubyte*)materials,
            sizeof(ENMaterialLoaded*),header.NumTextures,(ENubyte*)&lmat);
 }
}

void ENMapData::CreateTexture(char *Name,ENMaterial *mat)
{
 if(!map->CreateTexture(Name,mat))
   ShowMessage("Cannot add texture!!!");
 else
 {
  MustSave=true;
  //Create new material
  ENMaterialLoaded *lmat;
  ENMapBase::ENMapHeader header=GetHeader();
  lmat=new ENMaterialLoaded(mat,true);
  //Add new material
  header.NumTextures--;
  materials=(ENMaterialLoaded**)ENAddElement((ENubyte*)materials,
            sizeof(ENMaterialLoaded*),header.NumTextures,(ENubyte*)&lmat);
 }
}

void ENMapData::RenameTexture(ENuint ind,char *TextureName)
{
 if(!map->RenameTexture(ind,TextureName))
   ShowMessage("Texture name already exists!!!");
 else
   MustSave=true;
}

void ENMapData::DeleteTexture(ENuint ind)
{
 MustSave=true;
 //Delete material
 delete materials[ind];
 ENMapBase::ENMapHeader header=GetHeader();
 ENuint num=header.NumTextures;
 materials=(ENMaterialLoaded**)ENDelElement((ENubyte*)materials,
           sizeof(ENMaterialLoaded*),num,ind);
 //Delete skin
 map->DeleteTexture(ind);
}

void ENMapData::AssignTexture(ENuint ind)
{
 MustSave=true;
 ENMapBase::ENMapHeader header=GetHeader();
 for(ENuint a=0;a<header.NumFaces;a++)
   if(IsFaceSelected(a))
 {
  ENMapBase::ENMapFace face=map->GetFace(a);
  face.texind=ind;
  map->SetFace(a,face);
 }
}

void ENMapData::GetTexturePreview(Graphics::TBitmap *bmp,ENuint ind)
{
 //Get material data
 ENMaterial *mat=new ENMaterial;
 map->GetTextureData(mat,ind);
 //Get base data
 ENPicture *pic=new ENPicture();
 mat->GetBase(pic);
 delete mat;
 //Make preview
 GetPreview(bmp,pic,64.0f,64.0f);
 //Delete picture
 delete pic;
}

void ENMapData::GetTextureInfo(ENuint ind,char *info)
{
 //Variables
 char Enable[2][10];
 ENMaterial::ENMaterialHeader header;
 //Get material header
 map->GetMaterialHeader(ind,header);
 //Write material info
 if(header.EnableAlpha)
   strcpy(Enable[0],"YES");
 else
   strcpy(Enable[0],"NO");

 if(header.EnableBump)
   strcpy(Enable[1],"YES");
 else
   strcpy(Enable[1],"NO");

 sprintf(info,"Alpha %s\nBump %s\nWidth %i\nHeight %i",Enable[0],Enable[1],
         header.Basewidth,header.Baseheight);
}

void ENMapData::DeleteAllTextures()
{
 ENMapBase::ENMapHeader header=GetHeader();

 for(ENuint a=0;a<header.NumTextures;a++)
   delete materials[a];

 ENFreeMemory(materials);
 materials=NULL;
}

void ENMapData::InitAllTextures()
{
 ENMapBase::ENMapHeader header=GetHeader();
 materials=(ENMaterialLoaded**)ENAllocMemory(sizeof(ENMaterialLoaded*)*header.NumTextures);
 for(ENuint a=0;a<header.NumTextures;a++)
 {
  ENMaterial *mat=new ENMaterial();
  map->GetTextureData(mat,a);
  materials[a]=new ENMaterialLoaded(mat,true);
  delete mat;
 }
}

void ENMapData::ActivateTexture(ENint ind)
{
 ENMapBase::ENMapHeader header=GetHeader();
 if(ind>=0&&header.NumTextures)
   materials[ind%header.NumTextures]->SetBase();
 else
   glBindTexture(GL_TEXTURE_2D,0);
}

void ENMapData::MakeCubeCoord(ENVector vertex,ENVector normal,ENVector2 &coord)
{
 //X
 if(normal.v[0]>=normal.v[1]&&normal.v[0]>=normal.v[2])
 {
  coord.v[0]=vertex.v[2];
  coord.v[1]=-vertex.v[1];
 }
 //Y
 if(normal.v[1]>=normal.v[0]&&normal.v[1]>=normal.v[2])
 {
  coord.v[0]=vertex.v[0];
  coord.v[1]=vertex.v[2];
 }
 //Z
 if(normal.v[2]>=normal.v[0]&&normal.v[2]>=normal.v[1])
 {
  coord.v[0]=vertex.v[0];
  coord.v[1]=-vertex.v[1];
 }
}

void ENMapData::CubeCoord(ENMapBase::ENMapFace *face,
                          ENVector2 pos,ENVector2 scale,ENfloat angle)
{
 //Variables
 ENVector2 coord;
 ENVector vec;
 //Calculate face normal
 ENVector vNormal=FaceNormal(*face);
 vNormal=ENAbsolute(vNormal);
 //Transform
 for(ENuint a=0;a<3;a++)
 {
  vec=GetVertex(face->indices[a]);
  MakeCubeCoord(vec,vNormal,coord);
  coord.v[0]=coord.v[0]/scale.v[0];
  coord.v[1]=coord.v[1]/scale.v[1];
  RotateCoord(coord,angle);
  coord=coord+pos;
  face->texcoord[a]=coord;
 }
}

ENVector ENMapData::FaceNormal(ENMapBase::ENMapFace face)
{
 ENVector F[3];
 F[0]=GetVertex(face.indices[0]);
 F[1]=GetVertex(face.indices[1]);
 F[2]=GetVertex(face.indices[2]);
 ENVector cr=ENCross(F[1]-F[0],F[2]-F[0]);
 ENfloat len=ENLength(cr);
 if(len>0.0f) cr=cr/len;
 return cr;
}

void ENMapData::ExportToASCII(char *FileName)
{
 ENASCIIMesh *eam=new ENASCIIMesh();
 //Get header data and alloc space
 ENMapBase::ENMapHeader header=GetHeader();
 ENASCIIMesh::ENMeshFace *faces=new ENASCIIMesh::ENMeshFace[header.NumFaces];
 ENVector *vertexes=new ENVector[header.NumVertexes];
 //Copy vertexes
 for(ENuint a=0;a<header.NumVertexes;a++)
   vertexes[a]=GetVertex(a);
 //Copy faces
 for(ENuint b=0;b<header.NumFaces;b++)
 {
  ENMapBase::ENMapFace face=GetFace(b);
  memcpy(faces[b].Indices,face.indices,sizeof(ENuint)*3);
 }
 //Add data
 eam->AddVertexes(vertexes,header.NumVertexes);
 eam->AddFaces(faces,header.NumFaces);
 //Save file
 if(!eam->Write(FileName))
   ShowMessage("Cannot save "+AnsiString(FileName)+"!!!");
 //Free data
 if(header.NumVertexes) delete[] vertexes;
 if(header.NumFaces) delete[] faces;
 delete eam;
}

void ENMapData::ImportFromASCII(char *FileName)
{
 ENASCIIMesh *eam=new ENASCIIMesh();
 //Load eam
 if(!eam->Load(FileName))
 {
  ShowMessage("Cannot load "+AnsiString(FileName)+"!!!");
  return;
 }
 //Get data and alloc space
 ENuint NumFaces=eam->GetNumFaces(),NumVertexes=eam->GetNumVertexes();
 ENASCIIMesh::ENMeshFace *faces=new ENASCIIMesh::ENMeshFace[NumFaces];
 ENVector *vertexes=new ENVector[NumVertexes];
 eam->GetVertexes(vertexes);
 eam->GetFaces(faces);
 delete eam;
 //Add vertexes
 AddVertexes(vertexes,NumVertexes);
 //Add faces
 for(ENuint a=0;a<NumFaces;a++)
   AddFace(faces[a].Indices);
 //Free data
 if(NumVertexes) delete[] vertexes;
 if(NumFaces) delete[] faces;
}

void ENMapData::ImportFromQuake3(char *FileName,ENImportQ3Flags flags)
{
 //Variables
 ENVector               *verts;
 ENint                  b;
 ENuint                 NumVerts,NumFaces,a;
 Quake3Loader           *qloader=new Quake3Loader();
 //Load quake 3 bsp
 if(qloader->Load(FileName))
 {
  //Add vertexes
  NumVerts=qloader->GetNumVerts();
  verts=new ENVector[NumVerts];
  for(a=0;a<NumVerts;a++)
    verts[a]=qloader->GetVertex(a).vPosition;

  AddVertexes(verts,NumVerts);
  delete[] verts;
  //Add triangles
  NumFaces=qloader->GetNumFaces();
  for(a=0;a<NumFaces;a++)
  {
   Quake3Face qface=qloader->GetFace(a);
   if((qface.type==1&&flags.polygones)||
      (qface.type==2&&flags.patches)||
      (qface.type==3&&flags.mesh)||
      (qface.type==4&&flags.billb))
   for(b=0;b<qface.numOfVerts-2;b++)
   {
    ENMapBase::ENMapFace mapface;
    ENMapBase::InitMapFace(mapface);
    mapface.indices[2]=qface.startVertIndex;
    mapface.indices[1]=qface.startVertIndex+b+1;
    mapface.indices[0]=qface.startVertIndex+b+2;
    //set texcoord
    mapface.texcoord[0]=qloader->GetVertex(mapface.indices[0]).vTextureCoord;
    mapface.texcoord[1]=qloader->GetVertex(mapface.indices[1]).vTextureCoord;
    mapface.texcoord[2]=qloader->GetVertex(mapface.indices[2]).vTextureCoord;
    //Set texture
    if(flags.textures) mapface.texind=qface.textureID;
    //Add face
    AddFace2(&mapface);
   }
  }
  //Add textures
  if(flags.textures)
  for(a=0;a<qloader->GetNumTextures();a++)
  {
   Quake3Texture tex=qloader->GetTexture(a);
   AnsiString texstr=AnsiString(flags.TexturePath)+"\\"+tex.strName;
   ENMaterial *mat=new ENMaterial();
   ENPicture  *pic=new ENPicture();
   //Find extension
   char FStrName[MAX_PATH];
   FindExt(texstr.c_str(),FStrName);
   //Load and create texture
   if(!pic->Load(FStrName))
     ShowMessage("Cannot find "+AnsiString(FStrName)+"!!!");

   mat->SetBaseData(pic,0,0,0);
   CreateTexture(ExtractFileName(tex.strName).c_str(),mat);
   delete mat,pic;
  }
 }
 //Finished
 delete qloader;
}

void ENMapData::GetPreview(Graphics::TBitmap *bmp,
                           ENPicture *pic,ENfloat w,ENfloat h)
{
 //Config bitmap
 bmp->Width=(ENint)w;
 bmp->Height=(ENint)h;
 //Calculate factors
 ENVector2 f=ENVector2(pic->GetWidth()/w,pic->GetHeight()/h);
 //Copy data
 for(ENuint x=0;x<(ENuint)w;x++)
   for(ENuint y=0;y<(ENuint)h;y++)
   {
    ENuint pixel=pic->GetPixel(x*f.v[0],y*f.v[1]);
    bmp->Canvas->Pixels[x][y]=(TColor)pixel;
   }

}

void ENMapData::FindExt(char *srcstr,char *dststr)
{
 //Check if file is okay
 strcpy(dststr,srcstr);
 if(FileExists(dststr)) return;
 //Check for tga
 sprintf(dststr,"%s.tga",srcstr);
 if(FileExists(dststr)) return;
 //Check for bmp
 sprintf(dststr,"%s.bmp",srcstr);
 if(FileExists(dststr)) return;
 //Check for jpg
 sprintf(dststr,"%s.jpg",srcstr);
 if(FileExists(dststr)) return;
 sprintf(dststr,"%s.jif",srcstr);
 if(FileExists(dststr)) return;
 sprintf(dststr,"%s.jpeg",srcstr);
 if(FileExists(dststr)) return;
 //Check for png
 sprintf(dststr,"%s.png",srcstr);
 if(FileExists(dststr)) return;
 //Check for pcx
 sprintf(dststr,"%s.pcx",srcstr);
 if(FileExists(dststr)) return;
 //Default
 strcpy(dststr,srcstr);
}
