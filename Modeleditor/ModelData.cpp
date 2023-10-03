//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "ModelData.h"
#include "AddCubeUnit.h"
#include "AddConeUnit.h"
#include "AddCylinderUnit.h"
#include "AddSphereUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
ENuint   ENModelData::CurrentFrame=0;
ENModel *ENModelData::model=NULL;
ENubyte *ENModelData::VertSelected=NULL;
ENubyte *ENModelData::CoordSelected=NULL;
char     ENModelData::CurrentFile[MAX_PATH];
ENbool   ENModelData::MustSave=false;
ENMaterialLoaded **ENModelData::materials=NULL;
ENint    ENModelData::CurrentSkin=-1;

void ENModelData::Init()
{
 CurrentFile[0]=0;
 MustSave=false;
 model=new ENModel();
}

void ENModelData::Free()
{
 DeleteAllSkins();
 delete model;
 ENFreeMemory(VertSelected);
 ENFreeMemory(CoordSelected);
}
void ENModelData::New()
{
 model->Clear();
 MustSave=false;
 CurrentFile[0]=0;
 CurrentFrame=0;
 CurrentSkin=-1;
 ENFreeMemory(VertSelected);
 ENFreeMemory(CoordSelected);
 DeleteAllSkins();
 VertSelected=NULL;
 CoordSelected=NULL;
}

void ENModelData::AddVertex(ENVector2 mpos,ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec)
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

void ENModelData::AddPrefab(ENModelData::ENModelPrefab prefab,ENVector2 mpos,
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

void ENModelData::Select(ENVector2 a,ENVector2 b,
                         ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec)
{
 ENVector2 rc[2];
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
 //Check all vertexes
 ENModel::ENModelHeader header=model->GetHeader();
 for(ENuint a=0;a<header.NumVertexes;a++)
 {
  ENVector vert2D=From3DTo2D(model->GetVertex(a,CurrentFrame),mspec);
  if(vert2D.v[0]>=rc[0].v[0]&&vert2D.v[0]<=rc[1].v[0]&&
     vert2D.v[1]>=rc[0].v[1]&&vert2D.v[1]<=rc[1].v[1]&&
     (type!=ENDrawClass::ENDRAW3D||
     (vert2D.v[2]>=0&&vert2D.v[2]<=1)))
     VertSelected[a]=1;
 }
}

void ENModelData::SelectCoord(ENVector2 a,ENVector2 b)
{
 ENVector2 rc[2];
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
 //Check all vertexes
 ENModel::ENModelHeader header=model->GetHeader();
 for(ENuint a=0;a<header.NumFaces;a++)
 {
  ENModel::ENModelFace face=GetFace(a);
  for(ENuint b=0;b<3;b++)
  {
   ENVector2 vert2D=face.Coord[b];
   if(vert2D.v[0]>=rc[0].v[0]&&vert2D.v[0]<=rc[1].v[0]&&
      vert2D.v[1]>=rc[0].v[1]&&vert2D.v[1]<=rc[1].v[1])
        CoordSelected[a*3+b]=1;
  }
 }
}

ENint ENModelData::SelectOneVert(ENVector2 p,ENDrawClass::ENDrawClassTypes type,
                                 ENMatrixSpec mspec)
{
 //Variables
 ENint res=-1;
 ENfloat depth=0.0f;
 ENModel::ENModelHeader header=model->GetHeader();
 //Check all vertexes
 for(ENuint a=0;a<header.NumVertexes;a++)
 {
  ENVector vert2D=From3DTo2D(model->GetVertex(a,CurrentFrame),mspec);
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

void ENModelData::AddVertexRaw(ENVector vec)
{
 ENModel::ENModelHeader header=model->GetHeader();
 MustSave=true;
 model->AddVertex(vec);
 ENubyte b=1;
 VertSelected=ENAddElement(VertSelected,1,header.NumVertexes,&b);
}

void ENModelData::AddVertexes(ENVector *verts,ENuint num)
{
 MustSave=true;
 ENModel::ENModelHeader header=model->GetHeader();
 ENubyte *b=new ENubyte[num];
 memset(b,1,num);
 VertSelected=ENAddElements(VertSelected,1,header.NumVertexes,b,num);
 model->AddVertexes(verts,num); 
 delete[] b;
}

void ENModelData::AddFace(ENuint Ind[])
{
 ENModel::ENModelHeader header=GetHeader();
 MustSave=true;
 ENModel::ENModelFace face;
 memcpy(face.Indices,Ind,sizeof(ENuint)*3);
 face.Coord[0]=ENVector2(0,0);
 face.Coord[1]=ENVector2(0,0);
 face.Coord[2]=ENVector2(0,0);
 model->AddFace(face);
 ///////////////
 header.NumFaces*=3;
 ENubyte b[3]={1,1,1};
 CoordSelected=ENAddElements(CoordSelected,1,header.NumFaces,b,3);
}

void ENModelData::AddFaces(ENModel::ENModelFace *faces,ENuint num)
{
 ENModel::ENModelHeader header=GetHeader();
 MustSave=true;
 model->AddFaces(faces,num);
 ///////////////
 ENubyte *b=new ENubyte[num*3];
 memset(b,1,num*3);
 header.NumFaces*=3;
 CoordSelected=ENAddElements(CoordSelected,1,header.NumFaces,b,num*3);
 delete[] b;
}

void ENModelData::AddFrame()
{
 MustSave=true;
 model->AddFrame(CurrentFrame);
}

void ENModelData::DeleteFrame()
{
 MustSave=true;
 model->DeleteFrame(CurrentFrame);
 ENModel::ENModelHeader header=GetHeader();
 CurrentFrame%=header.NumFrames;
}

ENVector ENModelData::From2DTo3D(ENVector2 mpos,ENfloat depth,ENMatrixSpec mspec)
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

ENVector ENModelData::From3DTo2D(ENVector vec,ENMatrixSpec mspec)
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

void ENModelData::SetGLVertexes()
{
 model->SetGLVertexPointer(CurrentFrame);
}

ENVector ENModelData::GetVertex(ENuint ind)
{
 return model->GetVertex(ind,CurrentFrame);
}

ENModel::ENModelFace ENModelData::GetFace(ENuint ind)
{
 return model->GetFace(ind);
}

ENfloat ENModelData::GetMinVal(char coord)
{
 switch(coord)
 {
  case 'x':
    return model->GetMinX(CurrentFrame);
  case 'y':
    return model->GetMinY(CurrentFrame);
  case 'z':
    return model->GetMinZ(CurrentFrame);
 }

 return 0.0f;
}

ENfloat ENModelData::GetMaxVal(char coord)
{
 switch(coord)
 {
  case 'x':
    return model->GetMaxX(CurrentFrame);
  case 'y':
    return model->GetMaxY(CurrentFrame);
  case 'z':
    return model->GetMaxZ(CurrentFrame);
 }

 return 0.0f;
}

ENModel::ENModelHeader ENModelData::GetHeader()
{
 return model->GetHeader();
}

void ENModelData::SelectAllCoord()
{
 ENModel::ENModelHeader header=model->GetHeader();
 memset(CoordSelected,1,header.NumFaces*3);
}

void ENModelData::SelectNoneCoord()
{
 ENModel::ENModelHeader header=model->GetHeader();
 memset(CoordSelected,0,header.NumFaces*3);
}

void ENModelData::SelectAll()
{
 ENModel::ENModelHeader header=model->GetHeader();
 memset(VertSelected,1,header.NumVertexes);
}

void ENModelData::SelectNone()
{
 ENModel::ENModelHeader header=model->GetHeader();
 memset(VertSelected,0,header.NumVertexes);
}

void ENModelData::InvertSelected()
{
 ENModel::ENModelHeader header=model->GetHeader();
 for(ENuint a=0;a<header.NumVertexes;a++)
   if(VertSelected[a])
     VertSelected[a]=0;
   else
     VertSelected[a]=1;
}

ENbool ENModelData::IsVertSelected(ENuint ind)
{
 return VertSelected[ind];
}

ENbool ENModelData::IsFaceSelected(ENuint ind)
{
 ENModel::ENModelFace face=model->GetFace(ind);
 if(VertSelected[face.Indices[0]]&&VertSelected[face.Indices[1]]
    &&VertSelected[face.Indices[2]])
    return true;
 else
    return false;
}

ENbool ENModelData::IsCoordSelected(ENuint ind)
{
 return CoordSelected[ind];
}

ENbool ENModelData::Save()
{
 if(CurrentFile[0]==0)
   return false;
 else
   Save(CurrentFile);

 return true;
}

void ENModelData::Save(char *FileName)
{
 if(!model->Write(FileName))
   ShowMessage("Error writing file!!!");
 else
 {
  strcpy(CurrentFile,FileName);
  MustSave=false;
 }
}

void ENModelData::Open(char *FileName)
{
 DeleteAllSkins();
 if(!model->Load(FileName))
   ShowMessage("Error loading file!!!");
 else
 {
  MustSave=false;
  strcpy(CurrentFile,FileName);
  ENModel::ENModelHeader header=model->GetHeader();
  CurrentFrame=0;
  if(header.NumSkins)
    CurrentSkin=0;
  else
    CurrentSkin=-1;
  ENFreeMemory(VertSelected);
  ENFreeMemory(CoordSelected);
  VertSelected=(ENubyte*)ENAllocMemory(header.NumVertexes);
  CoordSelected=(ENubyte*)ENAllocMemory(header.NumFaces*3);
  InitAllSkins();
  SelectNone();
  SelectNoneCoord();
 }
}

void ENModelData::Translate(ENVector Pos,ENbool selected)
{
 ENModel::ENModelHeader header;
 header=model->GetHeader();
 MustSave=true;
 for(ENuint i=0;i<header.NumVertexes;i++)
   if(!selected||(selected&&IsVertSelected(i)))
 {
  ENVector vec=model->GetVertex(i,CurrentFrame);
  model->SetVertex(i,CurrentFrame,vec+Pos);
 }
}

void ENModelData::TranslateCoord(ENVector2 Pos,ENbool selected)
{
 ENModel::ENModelHeader header;
 header=model->GetHeader();
 MustSave=true;
 for(ENuint i=0;i<header.NumFaces;i++)
 {
  ENModel::ENModelFace face=GetFace(i);
  for(ENuint j=0;j<3;j++)
    if(!selected||(selected&&IsCoordSelected(i*3+j)))
    {
     face.Coord[j]=face.Coord[j]+Pos;
     if(face.Coord[j].v[0]<0.0f) face.Coord[j].v[0]=0.0f;
     if(face.Coord[j].v[1]<0.0f) face.Coord[j].v[1]=0.0f;
     if(face.Coord[j].v[0]>1.0f) face.Coord[j].v[0]=1.0f;
     if(face.Coord[j].v[1]>1.0f) face.Coord[j].v[1]=1.0f;     
    }

  model->SetFace(i,face);
 }
}

void ENModelData::Scale(ENVector center,ENVector svec,ENbool selected)
{
 ENModel::ENModelHeader header;
 header=model->GetHeader();
 MustSave=true;
 for(ENuint i=0;i<header.NumVertexes;i++)
   if(!selected||(selected&&IsVertSelected(i)))
 {
  ENVector vec=model->GetVertex(i,CurrentFrame);
  vec=(vec-center)*(ENVector(1,1,1)+svec/100)+center;
  model->SetVertex(i,CurrentFrame,vec);
 }
}

void ENModelData::ScaleCoord(ENVector2 center,ENVector2 svec,ENbool selected)
{
 ENModel::ENModelHeader header;
 header=model->GetHeader();
 MustSave=true;
 for(ENuint i=0;i<header.NumFaces;i++)
 {
  ENModel::ENModelFace face=GetFace(i);
  for(ENuint j=0;j<3;j++)
    if(!selected||(selected&&IsCoordSelected(i*3+j)))
    {
     ENVector2 vec=face.Coord[j];
     vec=(vec-center)*(ENVector2(1,1)+svec)+center;
     face.Coord[j]=vec;
   }
  model->SetFace(i,face);
 }
}

void ENModelData::Rotate(ENVector center,ENfloat angle,char Axis,ENbool selected)
{
 //Get number of vertexes
 ENModel::ENModelHeader header;
 header=model->GetHeader();
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
 for(ENuint i=0;i<header.NumVertexes;i++)
   if(!selected||(selected&&IsVertSelected(i)))
 {
  ENVector vec=model->GetVertex(i,CurrentFrame);
  vec=rmat*(vec-center)+center;
  model->SetVertex(i,CurrentFrame,vec);
 }
}

void ENModelData::RotateCoord(ENVector2 center,ENfloat angle,ENbool selected)
{
 //Get number of coord's
 ENModel::ENModelHeader header;
 header=model->GetHeader();
 //set save flag
 MustSave=true;
 //Create help vector
 ENVector hcenter=ENVector(center.v[0],center.v[1],0.0f);
 //Get rotation matrix
 ENMatrix3X3 rmat=ENRotMatrixZ(angle);
 //Rotate coord
 for(ENuint i=0;i<header.NumFaces;i++)
 {
  ENModel::ENModelFace face=GetFace(i);
  for(ENuint j=0;j<3;j++)
    if(!selected||(selected&&IsCoordSelected(i*3+j)))
    {
     ENVector vec=ENVector(face.Coord[j].v[0],face.Coord[j].v[1],0.0f);
     vec=rmat*(vec-hcenter)+hcenter;
     face.Coord[j]=ENVector2(vec.v[0],vec.v[1]);
    }
  model->SetFace(i,face);
 }
}

ENbool ENModelData::HasToSave()
{
 return MustSave;
}

ENuint ENModelData::GetNumSelected()
{
 ENuint c=0;

 ENModel::ENModelHeader header;
 header=model->GetHeader();
 for(ENuint a=0;a<header.NumVertexes;a++)
   if(IsVertSelected(a))
     c++;

 return c;
}

ENuint ENModelData::GetNumSelectedCoord()
{
 ENuint c=0;

 ENModel::ENModelHeader header;
 header=model->GetHeader();
 for(ENuint a=0;a<header.NumFaces*3;a++)
   if(IsCoordSelected(a))
     c++;

 return c;
}

void ENModelData::GetSelectedVerts(ENVector *Vertexes)
{
 ENuint c=0;

 ENModel::ENModelHeader header;
 header=model->GetHeader();
 for(ENuint a=0;a<header.NumVertexes;a++)
   if(IsVertSelected(a))
   {
    Vertexes[c]=GetVertex(a);
    c++;
   }
}

void ENModelData::GetSelectedCoord(ENVector2 *Coord)
{
 ENuint c=0;

 ENModel::ENModelHeader header;
 header=model->GetHeader();
 for(ENuint a=0;a<header.NumFaces;a++)
 {
  ENModel::ENModelFace face=GetFace(a);
  for(ENuint b=0;b<3;b++)
    if(IsCoordSelected(a*3+b))
    {
     Coord[c]=face.Coord[b];
     c++;
    }
 }
}

void ENModelData::BoxSelected(ENVector &min,ENVector &max)
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

void ENModelData::BoxSelectedCoord(ENVector2 &min,ENVector2 &max)
{
 //Variables
 ENuint num,i;
 ENVector2 *verts=NULL;
 //Get number of selected coord
 num=GetNumSelectedCoord();
 if(num)
 {
  //Alloc space and get selected coord
  verts=new ENVector2[num];
  GetSelectedCoord(verts);
  //Init min and max
  min=verts[0];
  max=verts[0];
  //Get min and max vector
  for(i=1;i<num;i++)
  {
   ENVector2 vec=verts[i];
   //Min x
   if(vec.v[0]<min.v[0]) min.v[0]=vec.v[0];
   //Min y
   if(vec.v[1]<min.v[1]) min.v[1]=vec.v[1];
   //Max x
   if(vec.v[0]>max.v[0]) max.v[0]=vec.v[0];
   //Max y
   if(vec.v[1]>max.v[1]) max.v[1]=vec.v[1];
  }
  //Delete selected vertexes
  delete[] verts;
 }
 else
 {//No selected vertexes
  min=ENVector2(0,0);
  max=ENVector2(0,0);
 }
}

void ENModelData::Box(ENVector &min,ENVector &max)
{
 //Variables
 ENuint i;
 ENModel::ENModelHeader header;
 //Get header
 header=model->GetHeader();
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

ENVector ENModelData::Center(ENbool selected)
{
 ENVector min,max;
 if(selected)
   BoxSelected(min,max);
 else
   Box(min,max);

 return (min+max)/2;
}

void ENModelData::SetCurrentFrame(ENuint ind)
{
 ENModel::ENModelHeader header=GetHeader();
 CurrentFrame=ind%header.NumFrames;
}

ENuint ENModelData::GetCurrentFrame()
{
 return CurrentFrame;
}

AnsiString ENModelData::GetGroupName(ENuint ind)
{
 char GroupName[33];
 model->GetGroupName(ind,GroupName);
 return AnsiString(GroupName);
}

void ENModelData::CreateGroup(char *GroupName)
{
 if(!model->CreateGroup(GroupName))
   ShowMessage("Group name already exists!!!");
 else
   MustSave=true;
}

void ENModelData::RenameGroup(ENuint ind,char *GroupName)
{
 if(!model->RenameGroup(ind,GroupName))
   ShowMessage("Group name already exists!!!");
 else
   MustSave=true;
}

void ENModelData::DeleteGroup(ENuint ind)
{
 MustSave=true;
 model->DeleteGroup(ind);
}

void ENModelData::SelectGroup(ENuint ind)
{
 ENuint num=model->GetNumGroupIndices(ind);
 ENuint *Indices=new ENuint[num];
 model->GetGroupIndices(ind,Indices);
 for(ENuint a=0;a<num;a++)
   VertSelected[Indices[a]]=1;

 delete[] Indices;
}

void ENModelData::AddGeometryToGroup(ENuint ind)
{
 MustSave=true;
 ENuint num=GetNumSelected();
 ENModel::ENModelHeader header=GetHeader();
 if(num)
 {
  ENuint *Indices=new ENuint[num];
  ENuint c=0;
  for(ENuint b=0;b<header.NumVertexes;b++)
    if(VertSelected[b])
    {
     Indices[c]=b;
     c++;
    }

  model->AddGeometry2Group(ind,Indices,num);
  delete[] Indices;
 }
}

void ENModelData::DeleteSelectedFaces()
{
 MustSave=true;
 ENModel::ENModelHeader header=GetHeader();
 ENuint numFaces=header.NumFaces;
 ENuint ind=0;
 while(ind<numFaces)
 {
  if(IsFaceSelected(ind))
  {
   model->DeleteFace(ind);
   header=GetHeader();
   numFaces=header.NumFaces;
  }
  else
    ind++;
 }
}

void ENModelData::DeleteSelectedVertexes()
{
 MustSave=true;
 ENModel::ENModelHeader header=GetHeader();
 ENuint numVerts=header.NumVertexes;
 ENuint ind=0;
 while(ind<numVerts)
 {
  if(IsVertSelected(ind))
  {
   model->DeleteVertex(ind);
   header=GetHeader();
   numVerts=header.NumVertexes;
  }
  else
    ind++;
 }
}

void ENModelData::SelectConnected()
{
 ENModel::ENModelHeader header=GetHeader();
 for(ENuint a=0;a<header.NumVertexes;a++)
   if(VertSelected[a])
     SelectConnectedRe(a);
}

void ENModelData::SelectConnectedRe(ENuint index)
{
 ENModel::ENModelFace   face;
 ENModel::ENModelHeader header=GetHeader();
 VertSelected[index]=1;
  for(ENuint a=0;a<header.NumFaces;a++)
  {
   face=model->GetFace(a);
   if(face.Indices[0]==index||face.Indices[1]==index||face.Indices[2]==index)
    {
      if(!VertSelected[face.Indices[0]])
        SelectConnectedRe(face.Indices[0]);
      if(!VertSelected[face.Indices[1]])
        SelectConnectedRe(face.Indices[1]);
      if(!VertSelected[face.Indices[2]])
        SelectConnectedRe(face.Indices[2]);
    }
  }
}

void ENModelData::FlipNormals()
{
 MustSave=true;
 ENModel::ENModelHeader header=GetHeader();
 for(ENuint a=0;a<header.NumFaces;a++)
   if(IsFaceSelected(a))
   {
    //Get face
    ENModel::ENModelFace face=model->GetFace(a);
    //Flip indices
    face.Indices[0]^=face.Indices[2];
    face.Indices[2]^=face.Indices[0];
    face.Indices[0]^=face.Indices[2];
    //Flip texcoord
    ENVector2 tvec=face.Coord[0];
    face.Coord[0]=face.Coord[2];
    face.Coord[2]=tvec;
    //Set face
    model->SetFace(a,face);
   }
}

void ENModelData::ConnectAll(ENfloat tolerance)
{
 MustSave=true;
 ENModel::ENModelHeader header;
 ENuint  numVerts;

 header=GetHeader();
 numVerts=header.NumVertexes;

 for(ENuint a=0;a<numVerts;a++)
   for(ENuint b=a+1;b<numVerts;b++)
     if(ENDistance(GetVertex(a),GetVertex(b))<=tolerance)
     {
      ConnectVertexes(a,b);
      model->DeleteVertex(b);
      header=GetHeader();
      numVerts=header.NumVertexes;
     }
}

void ENModelData::ConnectVertexes(ENuint a,ENuint b)
{
 ENModel::ENModelHeader header=GetHeader();
 for(ENuint i=0;i<header.NumFaces;i++)
 {
  ENModel::ENModelFace face=model->GetFace(i);
  if(face.Indices[0]==b) face.Indices[0]=a;
  if(face.Indices[1]==b) face.Indices[1]=a;
  if(face.Indices[2]==b) face.Indices[2]=a;
  model->SetFace(i,face);
 }
}

void ENModelData::Optimize(ENbool UnusedVerts,ENbool Verts,ENfloat toleranz)
{
 //Variables
 ENModel::ENModelHeader header;
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
    model->DeleteVertex(c);
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

ENbool ENModelData::IsVertexUnused(ENuint ind)
{
 ENModel::ENModelHeader header=GetHeader();
 for(ENuint a=0;a<header.NumFaces;a++)
 {
  ENModel::ENModelFace face=model->GetFace(a);
  if(face.Indices[0]==ind||face.Indices[1]==ind||face.Indices[2]==ind)
    return false;
 }

 return true;
}

AnsiString ENModelData::GetSkinName(ENuint ind)
{
 char SkinName[33];
 model->GetSkinName(ind,SkinName);
 return AnsiString(SkinName);
}

void ENModelData::CreateSkin(char *SkinName,char *FileName)
{
 if(!model->CreateSkin(SkinName,FileName))
   ShowMessage("Cannot create skin!!!");
 else
 {
  MustSave=true;
  //Create new material
  ENMaterialLoaded *lmat;
  ENModel::ENModelHeader header=GetHeader();
  ENMaterial *mat=new ENMaterial();
  model->GetSkinData(mat,header.NumSkins-1);
  lmat=new ENMaterialLoaded(mat,true);
  delete mat;
  //Add new material
  header.NumSkins--;
  materials=(ENMaterialLoaded**)ENAddElement((ENubyte*)materials,
            sizeof(ENMaterialLoaded*),header.NumSkins,(ENubyte*)&lmat);
  //Check current skin
  if(CurrentSkin==-1) CurrentSkin=0;
 }
}

void ENModelData::CreateSkin(char *SkinName,ENMaterial *mat)
{
 if(!model->CreateSkin(SkinName,mat))
   ShowMessage("Cannot add skin!!!");
 else
 {
  MustSave=true;
  //Create new material
  ENMaterialLoaded *lmat;
  ENModel::ENModelHeader header=GetHeader();
  lmat=new ENMaterialLoaded(mat,true);
  //Add new material
  header.NumSkins--;
  materials=(ENMaterialLoaded**)ENAddElement((ENubyte*)materials,
            sizeof(ENMaterialLoaded*),header.NumSkins,(ENubyte*)&lmat);
  //Check current skin
  if(CurrentSkin==-1) CurrentSkin=0;
 }
}

void ENModelData::RenameSkin(ENuint ind,char *SkinName)
{
 if(!model->RenameSkin(ind,SkinName))
   ShowMessage("Skin name already exists!!!");
 else
   MustSave=true;
}

void ENModelData::DeleteSkin(ENuint ind)
{
 MustSave=true;
 //Delete material
 delete materials[ind];
 ENModel::ENModelHeader header=GetHeader();
 ENuint num=header.NumSkins;
 materials=(ENMaterialLoaded**)ENDelElement((ENubyte*)materials,
           sizeof(ENMaterialLoaded*),num,ind);
 //Delete skin
 model->DeleteSkin(ind);
 //Check current skin
 if(num)
   CurrentSkin%=num;
 else
   CurrentSkin=-1;
}

void ENModelData::DeleteAllSkins()
{
 ENModel::ENModelHeader header=GetHeader();

 for(ENuint a=0;a<header.NumSkins;a++)
   delete materials[a];

 ENFreeMemory(materials);
 materials=NULL;
}

void ENModelData::InitAllSkins()
{
 ENModel::ENModelHeader header=GetHeader();
 materials=(ENMaterialLoaded**)ENAllocMemory(sizeof(ENMaterialLoaded*)*header.NumSkins);
 for(ENuint a=0;a<header.NumSkins;a++)
 {
  ENMaterial *mat=new ENMaterial();
  model->GetSkinData(mat,a);
  materials[a]=new ENMaterialLoaded(mat,true);
  delete mat;
 }
}

ENint ENModelData::GetCurrentSkin()
{
 return CurrentSkin;
}

void ENModelData::SetCurrentSkin(ENint cskin)
{
 ENModel::ENModelHeader header=GetHeader();
 if(header.NumSkins)
   CurrentSkin=cskin%header.NumSkins;
}

void ENModelData::ActivateCurrentSkin()
{
 if(CurrentSkin>=0)
   materials[CurrentSkin]->SetBase();
 else
   glBindTexture(GL_TEXTURE_2D,0);
}

void ENModelData::ExportSkin(char *FileName,ENuint cskin)
{
 ENModel::ENModelHeader header=GetHeader();
 if(header.NumSkins)
 {
  cskin%=header.NumSkins;
  ENMaterial *mat=new ENMaterial();
  model->GetSkinData(mat,cskin);
  if(!mat->Save(FileName))
    ShowMessage("Cannot export material!!!");

  delete mat;    
 }
}

void ENModelData::MakeSkinGridTop()
{
 MustSave=true;
 //Variables
 ENVector min,max,rcvec;
 //Get Box of select vertexes
 BoxSelected(min,max);
 //Clear selected coord array
 SelectNoneCoord();
 //Create box vector
 rcvec=max-min;
 if(rcvec.v[0]>0.0f&&rcvec.v[2]>0.0f)
 {
  ENModel::ENModelHeader header=GetHeader();
  for(ENuint a=0;a<header.NumFaces;a++)
    if(IsFaceSelected(a))
    {
     ENModel::ENModelFace face=GetFace(a);
     for(ENuint b=0;b<3;b++)
     {
      CoordSelected[a*3+b]=true;     
      ENVector vec=GetVertex(face.Indices[b]);
      face.Coord[b].v[0]=(vec.v[0]-min.v[0])/rcvec.v[0];
      face.Coord[b].v[1]=(vec.v[2]-min.v[2])/rcvec.v[2];
     }
     model->SetFace(a,face);
    }
 }
}

void ENModelData::MakeSkinGridFront()
{
 MustSave=true;
 //Variables
 ENVector min,max,rcvec;
 //Get Box of select vertexes
 BoxSelected(min,max);
 //Clear selected coord array
 SelectNoneCoord();
 //Create box vector
 rcvec=max-min;
 if(rcvec.v[0]>0.0f&&rcvec.v[1]>0.0f)
 {
  ENModel::ENModelHeader header=GetHeader();
  for(ENuint a=0;a<header.NumFaces;a++)
    if(IsFaceSelected(a))
    {
     ENModel::ENModelFace face=GetFace(a);
     for(ENuint b=0;b<3;b++)
     {
      CoordSelected[a*3+b]=true;
      ENVector vec=GetVertex(face.Indices[b]);
      face.Coord[b].v[0]=(vec.v[0]-min.v[0])/rcvec.v[0];
      face.Coord[b].v[1]=1.0f-(vec.v[1]-min.v[1])/rcvec.v[1];
     }
     model->SetFace(a,face);
    }
 }
}

void ENModelData::MakeSkinGridSide()
{
 MustSave=true;
 //Variables
 ENVector min,max,rcvec;
 //Get Box of select vertexes
 BoxSelected(min,max);
 //Create box vector
 rcvec=max-min;
 //Clear selected coord array
 SelectNoneCoord();
 if(rcvec.v[2]>0.0f&&rcvec.v[1]>0.0f)
 {
  ENModel::ENModelHeader header=GetHeader();
  for(ENuint a=0;a<header.NumFaces;a++)
    if(IsFaceSelected(a))
    {
     ENModel::ENModelFace face=GetFace(a);
     for(ENuint b=0;b<3;b++)
     {
      CoordSelected[a*3+b]=true;
      ENVector vec=GetVertex(face.Indices[b]);
      face.Coord[b].v[0]=(vec.v[2]-min.v[2])/rcvec.v[2];
      face.Coord[b].v[1]=1.0f-(vec.v[1]-min.v[1])/rcvec.v[1];
     }
     model->SetFace(a,face);
    }
 }
}

void ENModelData::MakeSkinGridCube()
{
 MustSave=true;
 //Variables
 ENVector min,max,rcvec;
 //Get Box of select vertexes
 BoxSelected(min,max);
 //Clear selected coord array
 SelectNoneCoord();
 //Create box vector
 rcvec=max-min;
 ENModel::ENModelHeader header=GetHeader();
 for(ENuint a=0;a<header.NumFaces;a++)
   if(IsFaceSelected(a))
   {
    ENuint b;
    for(b=0;b<3;b++)
      CoordSelected[a*3+b]=true;
    ENModel::ENModelFace face=GetFace(a);
    ENVector vNormal=FaceNormal(face);
    vNormal=ENAbsolute(vNormal);
    //X(Side)
    if(vNormal.v[0]>=vNormal.v[1]&&vNormal.v[0]>=vNormal.v[2])
      for(b=0;b<3;b++)
      {
       ENVector vec=GetVertex(face.Indices[b]);
       face.Coord[b].v[0]=(vec.v[2]-min.v[2])/rcvec.v[2];
       face.Coord[b].v[1]=1.0f-(vec.v[1]-min.v[1])/rcvec.v[1];
      }
    //Y(Top)
    if(vNormal.v[1]>=vNormal.v[0]&&vNormal.v[1]>=vNormal.v[2])
      for(b=0;b<3;b++)
      {
       ENVector vec=GetVertex(face.Indices[b]);
       face.Coord[b].v[0]=(vec.v[0]-min.v[0])/rcvec.v[0];
       face.Coord[b].v[1]=(vec.v[2]-min.v[2])/rcvec.v[2];
      }
    //Z(Front)
    if(vNormal.v[2]>=vNormal.v[0]&&vNormal.v[2]>=vNormal.v[1])
      for(b=0;b<3;b++)
      {
       ENVector vec=GetVertex(face.Indices[b]);      
       face.Coord[b].v[0]=(vec.v[0]-min.v[0])/rcvec.v[0];
       face.Coord[b].v[1]=1.0f-(vec.v[1]-min.v[1])/rcvec.v[1];
      }
    model->SetFace(a,face);
   }
}

ENVector ENModelData::FaceNormal(ENModel::ENModelFace face)
{
 ENVector F[3];
 F[0]=GetVertex(face.Indices[0]);
 F[1]=GetVertex(face.Indices[1]);
 F[2]=GetVertex(face.Indices[2]);
 return ENNormal(F);
}

void ENModelData::ExportToASCII(char *FileName)
{
 ENASCIIMesh *eam=new ENASCIIMesh();
 //Get header data and alloc space
 ENModel::ENModelHeader header=GetHeader();
 ENASCIIMesh::ENMeshFace *faces=new ENASCIIMesh::ENMeshFace[header.NumFaces];
 ENVector *vertexes=new ENVector[header.NumVertexes];
 //Copy vertexes
 for(ENuint a=0;a<header.NumVertexes;a++)
   vertexes[a]=GetVertex(a);
 //Copy faces
 for(ENuint b=0;b<header.NumFaces;b++)
 {
  ENModel::ENModelFace face=GetFace(b);
  memcpy(faces[b].Indices,face.Indices,sizeof(ENuint)*3);
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

void ENModelData::ImportFromASCII(char *FileName)
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

ENint ENModelData::GetSelectedVertInd()
{
 ENModel::ENModelHeader header=model->GetHeader();
 for(ENuint a=0;a<header.NumVertexes;a++)
   if(VertSelected[a])
     return a;

 return -1;
}
