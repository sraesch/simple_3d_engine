//---------------------------------------------------------------------------
#ifndef BspTreeH
#define BspTreeH
//---------------------------------------------------------------------------
#include <list.h>
#include "EngineSystem.h"

struct ENBSPPolygon
{
 __declspec(dllexport) ENBSPPolygon();
 __declspec(dllexport) ENBSPPolygon(const ENBSPPolygon& poly);
 ENVector *verts;
 ENuint    num;
};

class ENBSPTree
{
 public:
  __declspec(dllexport) ENBSPTree();
  __declspec(dllexport) ~ENBSPTree();
  __declspec(dllexport) ENbool WriteBsp(char *FileName);
  __declspec(dllexport) ENbool WriteBsp(ENFile handle);
  __declspec(dllexport) ENbool LoadBsp(char *FileName);
  __declspec(dllexport) ENbool LoadBsp(ENFile handle);
  __declspec(dllexport) static ENbool SphereCollision(const ENBSPTree *tree,const ENVector &pos,const ENfloat range,
                                                      ENVector *vNormal);

  ENPlane         partition;
  ENuint          numpolys;
  ENBSPPolygon    *polylist;
  ENBSPTree       *front;
  ENBSPTree       *back;
 protected:
  __declspec(dllexport) void WriteTree(ENFile handle,ENBSPTree *bsp);
  __declspec(dllexport) void LoadTree(ENFile handle,ENBSPTree *bsp);
};

typedef void (*ENBSPBUILDERMSG)(char *msg,ENuint pos,ENuint max);

class ENBSPBuilder
{
 public:
   __declspec(dllexport) ENBSPBuilder(ENBSPBUILDERMSG msgfunc);
   __declspec(dllexport) ~ENBSPBuilder();
   __declspec(dllexport) void AddPolygon(const ENBSPPolygon& poly);
   __declspec(dllexport) void AddPolygon(const ENBSPPolygon *polygons,const ENuint num);
   __declspec(dllexport) ENBSPTree *BuildTree();
 protected:
   ENBSPBUILDERMSG  msgfunc;
   ENint            msgvar[2];
   list<ENBSPPolygon> *polygonStack;

   __declspec(dllexport) ENVector         BuildMiddleVec(list<ENBSPPolygon> *polygons);
   __declspec(dllexport) ENVector         BuildFloodVec(list<ENBSPPolygon> *polygons);
   __declspec(dllexport) void             BuildPlane(const ENVector poly[],ENPlane &partition);
   __declspec(dllexport) void             BuildTreeIntern(ENBSPTree *tree,list<ENBSPPolygon> *polygons,ENPlane *parent);
   __declspec(dllexport) void             SplitPolygon(const ENPlane& partition,const ENBSPPolygon& poly,
                                                       list<ENBSPPolygon> *front,list<ENBSPPolygon> *back);
   __declspec(dllexport) void             GetRootPolygon(list<ENBSPPolygon> *polygons,ENBSPPolygon &root,ENPlane *parent);
   __declspec(dllexport) ENfloat          RootRanking(list<ENBSPPolygon> *polygons,const ENBSPPolygon& poly,
                                                      ENBSPPolygon *root,ENPlane *parent);

   __declspec(dllexport) void             UpdateState(char *msg,ENuint pos,ENuint max);
};

#endif
 