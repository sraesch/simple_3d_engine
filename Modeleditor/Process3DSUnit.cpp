//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Process3DSUnit.h"                                               
#include "ModelData.h"
#include "Lib3DS.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
t3DModel vModel;
TLabel          *ENProcess3DS::stat=NULL;
TProgressBar    *ENProcess3DS::statbar=NULL;
ENPicture      **ENProcess3DS::mats=NULL;
ENuint           ENProcess3DS::NumMaterials=0;
ENuint           ENProcess3DS::SkinWidth=0;
ENuint           ENProcess3DS::SkinHeight=0;
ENProcess3DS::ENSkinDim *ENProcess3DS::SkinDims=NULL;

ENbool ENProcess3DS::Import(AnsiString FileName,TLabel *stat,TProgressBar *statbar,
                            ENbool materials)
{
 //Variables
 t3DObject *pObject;
 ENVector  *vertexes;
 ENModel::ENModelFace *faces;
 //Copy objects
 ENProcess3DS::stat=stat;
 ENProcess3DS::statbar=statbar;
 //Start Controls
 StartControls();
 //Check if file exists
 SetMsg("Check if files exists...");
 if(!FileExists(FileName))
 {
  ShowMessage("Cannot find "+FileName+"!!!");
  return false;
 }
 //Load file
 SetMsg("Load file...");
 CLoad3DS vModelLoader;
 vModel.numOfObjects=0;
 vModel.numOfMaterials=0;
 if(!vModelLoader.Import3DS(&vModel,FileName.c_str()))
 {
  ShowMessage("Cannot load "+FileName+"!!!");
  return false;
 }
 //Clear old data
 SetMsg("Clear Old data...");
 ENModelData::New();
 //Load materials
 NumMaterials=vModel.numOfMaterials;
 if(materials&&NumMaterials)
 {
  statbar->Max=NumMaterials;
  SetMsg("Load materials...");
  mats=new ENPicture*[NumMaterials];
  for(ENuint m=0;m<NumMaterials;m++)
  {
   statbar->Position=m+1;
   Application->ProcessMessages();
   AnsiString MatFile;
   MatFile=vModel.pMaterials[m].strFile;
   mats[m]=new ENPicture();
   if(!LoadMaterial(mats[m],MatFile,FileName))
     return false;
  }
  //Create skin out of materials
  CreateSkin();
 }
 //Load objects
 statbar->Max=vModel.numOfObjects;
 SetMsg("Load objects...");
 for(ENuint a=0;a<(ENuint)vModel.numOfObjects;a++)
 {
  //Get model data
  ENModel::ENModelHeader header=ENModelData::GetHeader();
  //Update position of progressbar
  statbar->Position=a+1;
  Application->ProcessMessages();
  //Get data
  pObject =&vModel.pObject[a];
  vertexes=new ENVector[pObject->numOfVerts];
  faces=new ENModel::ENModelFace[pObject->numOfFaces];
  //Add vertexes
  for(ENuint a=0;a<(ENuint)pObject->numOfVerts;a++)
  {
   vertexes[a].v[0]=pObject->pVerts[a].x;
   vertexes[a].v[1]=pObject->pVerts[a].y;
   vertexes[a].v[2]=pObject->pVerts[a].z;
  }
  ENModelData::AddVertexes(vertexes,pObject->numOfVerts);
  if(pObject->numOfVerts) delete[] vertexes;
  //Add faces
  for(ENuint c=0;c<(ENuint)pObject->numOfFaces;c++)
  {
   faces[c].Indices[0]  =pObject->pFaces[c].vertIndex[0]+header.NumVertexes;
   faces[c].Indices[1]  =pObject->pFaces[c].vertIndex[1]+header.NumVertexes;
   faces[c].Indices[2]  =pObject->pFaces[c].vertIndex[2]+header.NumVertexes;
   if(pObject->bHasTexture&&materials&&NumMaterials)
   {
    faces[c].Coord[0]=ConvertCoord(pObject->pTexVerts[pObject->pFaces[c].vertIndex[0]].x,
                 pObject->pTexVerts[pObject->pFaces[c].vertIndex[0]].y,pObject->materialID);
    faces[c].Coord[1]=ConvertCoord(pObject->pTexVerts[pObject->pFaces[c].vertIndex[1]].x,
                 pObject->pTexVerts[pObject->pFaces[c].vertIndex[1]].y,pObject->materialID);
    faces[c].Coord[2]=ConvertCoord(pObject->pTexVerts[pObject->pFaces[c].vertIndex[2]].x,
                 pObject->pTexVerts[pObject->pFaces[c].vertIndex[2]].y,pObject->materialID);
   }
  }
  ENModelData::AddFaces(faces,pObject->numOfFaces);
  if(pObject->numOfFaces) delete[] faces;
 }
 //---------------Finished---------------//
 if(materials&&NumMaterials)
 {
  //Delete temporary loaded materials
  for(ENuint n=0;n<NumMaterials;n++)
    delete mats[n];

  if(NumMaterials)
  {
   delete[] mats;
   //Delete skin dimension data
   delete[] SkinDims;
  }   
 }

 EndControls();
 return true;
}

void ENProcess3DS::StartControls()
{
 stat->Visible=true;
 stat->Caption="";

 statbar->Enabled=true;
 statbar->Position=0;

 Application->ProcessMessages();
}

void ENProcess3DS::EndControls()
{
 stat->Visible=false;
 stat->Caption="";

 statbar->Enabled=false;
 statbar->Position=0;

 Application->ProcessMessages();
}

void ENProcess3DS::SetMsg(AnsiString msg)
{
 stat->Caption=msg;
 Application->ProcessMessages();
}

ENbool ENProcess3DS::LoadMaterial(ENPicture *mat,AnsiString FName,AnsiString ModelFile)
{
 //Absolute Path
 if(FileExists(FName))
 {
  if(mat->Load(FName.c_str()))
    return true;
  else
  {
   ShowMessage("Cannot load "+FName+"!!!");
   return false;
  }
 }
 //Relative Path
 FName=ExtractFileDir(ModelFile)+FName;
 if(FileExists(FName))
 {
  if(mat->Load(FName.c_str()))
    return true;
  else
  {
   ShowMessage("Cannot load "+FName+"!!!");
   return false;
  }
 }
 //Nothing found
 ShowMessage("Cannot find "+FName+"!!!");
 return false;
}

void ENProcess3DS::CreateSkin()
{
 //Variables
 ENubyte *SkinData=NULL;
 //Get skin width and height
 SkinWidth=0;
 SkinHeight=0;
 for(ENuint a=0;a<NumMaterials;a++)
 {
  SkinWidth+=mats[a]->GetWidth();
  if(mats[a]->GetHeight()>SkinHeight) SkinHeight=mats[a]->GetHeight();
 }
 //Alloc Space
 SkinData=new ENubyte[SkinWidth*SkinHeight*3];
 SkinDims=new ENSkinDim[NumMaterials];
 //Calculate data for each material
 for(ENuint b=0;b<NumMaterials;b++)
 {
  //Set material position
  if(!b)
    SkinDims[b].Pos=ENVector2(0,0);
  else
  {
   SkinDims[b].Pos.v[1]=0;
   SkinDims[b].Pos.v[0]=SkinDims[b-1].Pos.v[0]+SkinDims[b-1].Size.v[0];
  }
  //Set material size
  SkinDims[b].Size.v[0]=(ENfloat)mats[b]->GetWidth()/SkinWidth;
  SkinDims[b].Size.v[1]=(ENfloat)mats[b]->GetHeight()/SkinHeight;
  //Copy skin data
  for(ENuint x=0;x<mats[b]->GetWidth();x++)
    for(ENuint y=0;y<mats[b]->GetHeight();y++)
    {
     ENuint pixel=mats[b]->GetPixel(x,y);
     memcpy(SkinData+(ENuint)(y*SkinWidth+SkinDims[b].Pos.v[0]*SkinWidth+x)*3,
            &pixel,3);
    }
 }
 //Create skin
 ENPicture  *pic=new ENPicture(3,SkinWidth,SkinHeight,SkinData);
 delete[]    SkinData;
 ENMaterial *mat=new ENMaterial();
 mat->SetBaseData(pic,0,0,0);
 delete pic;
 ENModelData::CreateSkin("Skin",mat);
 delete mat;
}

ENVector2 ENProcess3DS::ConvertCoord(ENfloat x,ENfloat y,ENuint matID)
{
 y=1-y;
 //1. Transformation of coord
 if(x<0.0f)
   x=fmod(1.0f+x,1.0f);
 else
   x=fmod(x,1.0f);

 if(y<0.0f)
   y=fmod(1.0f+y,1.0f);
 else
   y=fmod(y,1.0f);

 //Finished
 return ENVector2(SkinDims[matID].Pos.v[0]+SkinDims[matID].Size.v[0]*x,
                  SkinDims[matID].Pos.v[1]+SkinDims[matID].Size.v[1]*y);
}




