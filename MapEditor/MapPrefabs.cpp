//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "MapPrefabs.h"
#include "MapData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void ENMapPrefabs::AddCube(ENVector Pos,ENVector Size)
{
  //Variables
  ENMapBase::ENMapHeader header;
  ENVector Temp;
  //Select None
  ENMapData::SelectNone();
  //Add Vertexes
  Temp=Pos;
  Temp.v[1]+=Size.v[1];
  Temp.v[0]-=Size.v[0];
  Temp.v[2]+=Size.v[2];
  //Top
  ENMapData::AddVertexRaw(Temp);
  Temp.v[0]+=2*Size.v[0];
  ENMapData::AddVertexRaw(Temp);
  Temp.v[2]-=2*Size.v[2];
  ENMapData::AddVertexRaw(Temp);
  Temp.v[0]-=2*Size.v[0];
  ENMapData::AddVertexRaw(Temp);
  //Bottom
  Temp.v[1]-=2*Size.v[1];
  ENMapData::AddVertexRaw(Temp);
  Temp.v[2]+=2*Size.v[2];
  ENMapData::AddVertexRaw(Temp);
  Temp.v[0]+=2*Size.v[0];
  ENMapData::AddVertexRaw(Temp);
  Temp.v[2]-=2*Size.v[2];
  ENMapData::AddVertexRaw(Temp);
  //Get header
  header=ENMapData::GetHeader();
  //Add Faces
  ENuint Ind[3];
  //Top  
  Ind[0]=header.NumVertexes-8;
  Ind[1]=header.NumVertexes-7;
  Ind[2]=header.NumVertexes-6;
  ENMapData::AddFace(Ind);
  Ind[0]=header.NumVertexes-6;
  Ind[1]=header.NumVertexes-5;
  Ind[2]=header.NumVertexes-8;
  ENMapData::AddFace(Ind);
  //Bottom
  Ind[0]=header.NumVertexes-2;
  Ind[1]=header.NumVertexes-3;
  Ind[2]=header.NumVertexes-4;
  ENMapData::AddFace(Ind);
  Ind[0]=header.NumVertexes-4;
  Ind[1]=header.NumVertexes-1;
  Ind[2]=header.NumVertexes-2;
  ENMapData::AddFace(Ind);
  //Front
  Ind[0]=header.NumVertexes-5;
  Ind[1]=header.NumVertexes-6;
  Ind[2]=header.NumVertexes-1;
  ENMapData::AddFace(Ind);
  Ind[0]=header.NumVertexes-1;
  Ind[1]=header.NumVertexes-4;
  Ind[2]=header.NumVertexes-5;
  ENMapData::AddFace(Ind);
  //Back
  Ind[0]=header.NumVertexes-8;
  Ind[1]=header.NumVertexes-3;
  Ind[2]=header.NumVertexes-2;
  ENMapData::AddFace(Ind);
  Ind[0]=header.NumVertexes-2;
  Ind[1]=header.NumVertexes-7;
  Ind[2]=header.NumVertexes-8;
  ENMapData::AddFace(Ind);
  //Right
  Ind[0]=header.NumVertexes-6;
  Ind[1]=header.NumVertexes-7;
  Ind[2]=header.NumVertexes-2;
  ENMapData::AddFace(Ind);
  Ind[0]=header.NumVertexes-2;
  Ind[1]=header.NumVertexes-1;
  Ind[2]=header.NumVertexes-6;
  ENMapData::AddFace(Ind);
  //Left
  Ind[0]=header.NumVertexes-5;
  Ind[1]=header.NumVertexes-4;
  Ind[2]=header.NumVertexes-3;
  ENMapData::AddFace(Ind);
  Ind[0]=header.NumVertexes-3;
  Ind[1]=header.NumVertexes-8;
  Ind[2]=header.NumVertexes-5;
  ENMapData::AddFace(Ind);
}

void ENMapPrefabs::AddCone(ENVector Pos,ENfloat Range,ENfloat Height,ENuint Details)
{
 //Variables
 ENVector *ring,*vertexes;
 ENMapBase::ENMapFace *faces;
 ENMapBase::ENMapHeader header;
 ENuint c,a;
 ENuint numVerts,numFaces;
 //Get header
 header=ENMapData::GetHeader();
 //Calculate 2D ring
 ring=Create2DRing(Details,Range,c);
 //Calculate number of faces and vertexes and alloc space
 numVerts=c+2;
 numFaces=c*2;
 vertexes=(ENVector*)ENAllocMemory(sizeof(ENVector)*numVerts);
 faces=(ENMapBase::ENMapFace*)ENAllocMemory(sizeof(ENMapBase::ENMapFace)*numFaces);
 //Create vertexes
 for(a=0;a<c;a++)
   vertexes[a]=ring[a]+Pos;

 vertexes[c]=Pos;
 vertexes[c+1]=Pos+ENVector(0.0f,Height,0.0f);
 //Create faces
 for(a=0;a<c;a++)
 {
  //Top
  InitFace(faces[a*2]);
  ENuint *Ind=faces[a*2].indices;
  Ind[0]=a+header.NumVertexes;
  Ind[1]=(a+1)%c+header.NumVertexes;
  Ind[2]=c+header.NumVertexes;
  //Bottom
  InitFace(faces[a*2+1]);
  Ind=faces[a*2+1].indices;
  Ind[0]=(a+1)%c+header.NumVertexes;
  Ind[1]=a+header.NumVertexes;
  Ind[2]=c+1+header.NumVertexes;
 }
 //Select None
 ENMapData::SelectNone();
 //Finished
 ENMapData::AddVertexes(vertexes,numVerts);
 ENMapData::AddFaces(faces,numFaces);
 ENFreeMemory(vertexes);
 ENFreeMemory(faces);
 ENFreeMemory(ring);
}

void ENMapPrefabs::AddCylinder(ENVector Pos,ENfloat Range,ENfloat Height,ENuint Details)
{
 //Variables
 ENVector *ring,*vertexes;
 ENMapBase::ENMapFace *faces;
 ENMapBase::ENMapHeader header;
 ENuint d,c,a;
 ENuint numVerts,numFaces;
 //Get header
 header=ENMapData::GetHeader();
 //Calculate 2D ring
 ring=Create2DRing(Details,Range,c);
 //Calculate number of faces and vertexes and alloc space
 numVerts=c*2+2;
 numFaces=c*4;
 vertexes=(ENVector*)ENAllocMemory(sizeof(ENVector)*numVerts);
 faces=(ENMapBase::ENMapFace*)ENAllocMemory(sizeof(ENMapBase::ENMapFace)*numFaces);
 //Create Vertexes
 for(a=0;a<c;a++)
   vertexes[a]=ring[a]+Pos;

 for(a=0;a<c;a++)
   vertexes[c+a]=ring[a]+Pos+ENVector(0.0f,Height,0.0f);

 vertexes[c*2]=Pos;
 vertexes[c*2+1]=Pos+ENVector(0.0f,Height,0.0f);
 //Create faces
 for(d=0;d<c;d++)
 {
  //Outside
  InitFace(faces[d*4]);
  ENuint *Ind=faces[d*4].indices;
  Ind[0]=c+(d+1)%c+header.NumVertexes;
  Ind[1]=(d+1)%c+header.NumVertexes;
  Ind[2]=d+header.NumVertexes;

  InitFace(faces[d*4+1]);
  Ind=faces[d*4+1].indices;
  Ind[0]=d+header.NumVertexes;
  Ind[1]=c+d+header.NumVertexes;
  Ind[2]=c+(d+1)%c+header.NumVertexes;
  //Bottom and top
  InitFace(faces[d*4+2]);
  Ind=faces[d*4+2].indices;
  Ind[0]=d+header.NumVertexes;
  Ind[1]=(d+1)%c+header.NumVertexes;
  Ind[2]=c*2+header.NumVertexes;

  InitFace(faces[d*4+3]);
  Ind=faces[d*4+3].indices;
  Ind[0]=c+(d+1)%c+header.NumVertexes;
  Ind[1]=c+d+header.NumVertexes;
  Ind[2]=c*2+1+header.NumVertexes;
 }
 //Select None
 ENMapData::SelectNone();
 //Finished
 ENMapData::AddVertexes(vertexes,numVerts);
 ENMapData::AddFaces(faces,numFaces);
 ENFreeMemory(vertexes);
 ENFreeMemory(faces);
 ENFreeMemory(ring);
}

void ENMapPrefabs::AddSphere(ENVector Pos,ENfloat Range,ENuint Details)
{
 //Variables
 ENVector *ring,*vertexes;
 ENMapBase::ENMapFace *faces;
 ENMapBase::ENMapHeader header;
 ENuint a,b,c;
 ENuint numVerts,numFaces;
 ENfloat interval;
 //Get header
 header=ENMapData::GetHeader();
 //Calculate 2D ring
 ring=Create2DRing(Details,Range,c);
 //Calculate number of faces and vertexes and alloc space
 numVerts=Details*c+2;
 numFaces=Details*c*2;
 vertexes=(ENVector*)ENAllocMemory(sizeof(ENVector)*numVerts);
 faces=(ENMapBase::ENMapFace*)ENAllocMemory(sizeof(ENMapBase::ENMapFace)*numFaces);
 //Create vertexes
 interval=Range*2/(Details+1);
 for(a=1;a<=Details;a++)
 {
  //Factor
  ENfloat f=interval*a-Range;
  f=(ENfloat)sqrt(Range*Range-f*f);
  f/=Range;
  //Now create vertexes
   for(b=0;b<c;b++)
   {
    vertexes[(a-1)*c+b].v[0]=ring[b].v[0]*f;
    vertexes[(a-1)*c+b].v[1]=interval*a-Range;
    vertexes[(a-1)*c+b].v[2]=ring[b].v[2]*f;
    vertexes[(a-1)*c+b]=vertexes[(a-1)*c+b]+Pos;
   }
 }

 vertexes[numVerts-2]=Pos+ENVector(0.0f, Range,0.0f);
 vertexes[numVerts-1]=Pos+ENVector(0.0f,-Range,0.0f);
 //Create Faces(Bottom and top)
 for(a=0;a<c;a++)
 {
  InitFace(faces[a*2]);
  ENuint *Ind=faces[a*2].indices;
  Ind[0]=header.NumVertexes+numVerts-1;
  Ind[1]=a+header.NumVertexes;
  Ind[2]=(a+1)%c+header.NumVertexes;

  InitFace(faces[a*2+1]);
  Ind=faces[a*2+1].indices;
  Ind[0]=header.NumVertexes+numVerts-2;
  Ind[1]=(a+1)%c+(Details-1)*c+header.NumVertexes;
  Ind[2]=a+(Details-1)*c+header.NumVertexes;
 }
 //Create Faces(Side)
 for(a=1;a<Details;a++)
 {
  for(b=0;b<c;b++)
  {
   InitFace(faces[(a*c+b)*2]);
   ENuint *Ind=faces[(a*c+b)*2].indices;
   Ind[2]=(a-1)*c+b+header.NumVertexes;
   Ind[1]=(a-1)*c+(b+1)%c+header.NumVertexes;
   Ind[0]=a*c+(b+1)%c+header.NumVertexes;

   InitFace(faces[(a*c+b)*2+1]);
   Ind=faces[(a*c+b)*2+1].indices;
   Ind[2]=a*c+(b+1)%c+header.NumVertexes;
   Ind[1]=a*c+b+header.NumVertexes;
   Ind[0]=(a-1)*c+b+header.NumVertexes;
  }
 }
 //Select None
 ENMapData::SelectNone();
 //Finished
 ENMapData::AddVertexes(vertexes,numVerts);
 ENMapData::AddFaces(faces,numFaces);
 ENFreeMemory(vertexes);
 ENFreeMemory(faces);
 ENFreeMemory(ring);
}

ENVector *ENMapPrefabs::Create2DRing(ENuint details,ENfloat range,ENuint &num)
{
 //Variables
 ENVector *ring=NULL,vec;
 ENMatrix3X3  RotMatrix;
 //Check details
 if(details<=0) details=1;
 //Calculate number of vertexes and allocate space
 num=2+2*details;
 ring=(ENVector*)ENAllocMemory(sizeof(ENVector)*num);
 //Get rotation matrix
 RotMatrix=ENRotMatrixY(2*M_PI/num);
 //Init rotate vector
 vec=ENVector(range,0.0f,0.0f);
 //Calculate vertexes
 for(ENuint c=0;c<num;c++)
 {
  ring[c]=vec;
  vec=RotMatrix*vec;
 }
 //Finished
 return ring;
}

void ENMapPrefabs::InitFace(ENMapBase::ENMapFace &face)
{
 ENMapData::CubeCoord(&face,ENVector2(0.0f,0.0f),ENVector2(1.0f,1.0f),0.0f);
 face.props.Passable=false;
 face.props.Invisible=false;
 face.props.Lighting=true;
 face.props.Shadow=true;
 face.props.Mode=EN_SHOW_NORMAL;
}

