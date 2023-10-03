//---------------------------------------------------------------------------
#ifndef ENTPCConverterH
#define ENTPCConverterH
//---------------------------------------------------------------------------
#include <stack.h>
#include <list.h>
#include "EngineSystem.h"

struct ENTPCPolygon
{
 ENuint         num;
 ENVector       *vecs;
};

class ENTPCConverter
{
 public:
   ENTPCConverter();
   ~ENTPCConverter();

   //Add triangles
   void                 addTriangles(ENuint n,ENVector *tris);
   //Get result
   ENbool               isResEmpty();
   ENuint               getNumRes();
   void                 getRes(ENTPCPolygon &poly);
   //Compute
   void                 compute();
 protected:
   struct ENTPCTriangle
   {
    ENVector       tridata[3];
    ENPlane        plane;
   };

   void         SortDataByPlanes();
   ENbool       InsertTriangleIntoPlaneData(ENTPCTriangle tri);

   stack<ENTPCTriangle> *toprocess;
   stack<ENTPCPolygon>  *finished;
   //Data to compute
   struct ENTPCPlaneData
   {
    ENPlane plane;
    list<ENTPCTriangle> triangles;
   };
   list<ENTPCPlaneData> *planeslist;

   void         ComputePlaneData(ENTPCPlaneData planedata);
   void         CollectTriangles(list<ENTPCTriangle> triangles,list<ENTPCTriangle> *trianglegrp);
   ENbool       TrianglesTouch(ENTPCTriangle tri1,ENTPCTriangle tri2);
};
#endif
