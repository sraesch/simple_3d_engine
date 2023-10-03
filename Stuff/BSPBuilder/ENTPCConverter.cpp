//---------------------------------------------------------------------------
#include "ENTPCConverter.h"
//---------------------------------------------------------------------------
ENTPCConverter::ENTPCConverter()
{
 toprocess=new stack<ENTPCTriangle>;
 finished=new stack<ENTPCPolygon>;
}

ENTPCConverter::~ENTPCConverter()
{
 delete toprocess;
 delete finished;
}

void ENTPCConverter::addTriangles(ENuint n,ENVector *tris)
{
 //Set end pointer address
 const ENVector *endptr=tris+n*3;
 //Add triangles until, end pointer address
 while(tris<endptr)
 {
  ENTPCTriangle tpctri;
  //build tpc triangle
  memcpy(tpctri.tridata,tris,sizeof(ENVector)*3);
  tpctri.plane.BuildPlane(ENNormal(tpctri.tridata),*tpctri.tridata);
  //add tpc triangle
  toprocess->push(tpctri);
  tris+=3;
 }
}

ENbool       ENTPCConverter::isResEmpty()
{
 return finished->empty();
}

ENuint ENTPCConverter::getNumRes()
{
 return finished->size();
}

void ENTPCConverter::getRes(ENTPCPolygon &poly)
{
 poly=finished->top();
 finished->pop();
}

void ENTPCConverter::compute()
{
 //Init
 planeslist=new list<ENTPCPlaneData>;
 //Sort by planes
 SortDataByPlanes();
 //Compute all planes
 while(!planeslist->empty())
 {
  ComputePlaneData(planeslist->back());
  planeslist->pop_back();
 }
 //Release
 delete planeslist;
}

void ENTPCConverter::SortDataByPlanes()
{
 //Variables
 ENTPCTriangle triangle;
 //sort all triangles
 while(!toprocess->empty())
 {
  //get triangle
  triangle=toprocess->top();
  toprocess->pop();
  //Insert triangle, else create new plane
  if(!InsertTriangleIntoPlaneData(triangle))
  {
   ENTPCPlaneData pdata;
   pdata.plane=triangle.plane;
   pdata.triangles.push_back(triangle);
   planeslist->push_back(pdata);
  }
 }
}

ENbool ENTPCConverter::InsertTriangleIntoPlaneData(ENTPCTriangle tri)
{
 //Variables
 list<ENTPCPlaneData>::iterator i;
 ENuint num;
 //get data
 i=planeslist->begin();
 num=planeslist->size();
 //Check all planes
 for(ENuint a=0;a<num;a++)
 {
  //calculate angle between both planes
  ENfloat angle=ENDot(i->plane.normal,tri.plane.normal);
  //If the planes are nearly the same, return index
  if(ENAbsolute(angle)>0.9998f)
    if(ENAbsolute(i->plane.dist-tri.plane.dist)<0.001)
    {
     i->triangles.push_back(tri);
     return true;
    }
  //Next plane
  i++;
 }
 //No plane found
 return false;
}

void ENTPCConverter::ComputePlaneData(ENTPCPlaneData planedata)
{
 while(!planedata.triangles.empty())
 {
  //get triangle from back
  ENTPCTriangle tri=planedata.triangles.back();
  planedata.triangles.pop_back();
  //Create list and add new triangle
  list<ENTPCTriangle> *trianglegrp=new list<ENTPCTriangle>;
  trianglegrp->push_back(tri);
  //Collect triangles
  CollectTriangles(planedata.triangles,trianglegrp);
 }
}

void ENTPCConverter::CollectTriangles(list<ENTPCTriangle> triangles,list<ENTPCTriangle> *trianglegrp)
{
 //Variables
 list<ENTPCTriangle>::iterator it;
 //Get iterator
 it=triangles.begin();
 //Go
 //for(ENuint a=0;a<triangles.size();a++)
 //  if(TrianglesTouch(trianglegrp->front(),
 //{
 // it++;
// }
}

ENbool ENTPCConverter::TrianglesTouch(ENTPCTriangle tri1,ENTPCTriangle tri2)
{
 //Variables
 ENuint a,count=0;
 ENfloat dist[3];
 //Count edges
 for(a=0;a<3;a++)
   if(ENDistance(tri1.tridata[a],tri2.tridata[0])<0.01f||
      ENDistance(tri1.tridata[a],tri2.tridata[1])<0.01f||
      ENDistance(tri1.tridata[a],tri2.tridata[2])<0.01f)
        count++;
 //Finished
 return count>=2;        
}

