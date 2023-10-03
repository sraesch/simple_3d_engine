//---------------------------------------------------------------------------
#ifndef ENBSPBuilderH
#define ENBSPBuilderH
//---------------------------------------------------------------------------
#include "EngineSystem.h"
#include <list.h>
#include <stack.h>

struct ENBSPPolygon
{
 ENBSPPolygon();
 ENBSPPolygon(const ENBSPPolygon& poly);
 ENVector *verts;
 ENuint    num;
};

class ENBSPTree
{
 public:
  ENBSPTree();
  ~ENBSPTree();
  ENbool WriteBsp(char *FileName);
  ENbool WriteBsp(ENFile handle);
  ENbool LoadBsp(char *FileName);
  ENbool LoadBsp(ENFile handle);
  ENbool SphereCollision(const ENVector &pos,const ENfloat range);

  ENPlane         partition;
  ENuint          numpolys;
  ENBSPPolygon    *polylist;
  ENBSPTree       *front;
  ENBSPTree       *back;
 protected:
  void WriteTree(ENFile handle,ENBSPTree *bsp);
  void LoadTree(ENFile handle,ENBSPTree *bsp);
};

typedef void (*ENBSPBUILDERMSG)(const char *msg,ENint pos,ENint max);

class ENBSPBuilder
{
 public:
   ENBSPBuilder(ENBSPBUILDERMSG msgfunc);
   ~ENBSPBuilder();
   void AddPolygon(const ENBSPPolygon& poly);
   void AddPolygon(const ENBSPPolygon *polygons,const ENuint num);
   ENBSPTree *BuildTree();
 protected:
   ENBSPBUILDERMSG  msgfunc;
   ENint            msgvar[2];
   list<ENBSPPolygon> *polygonStack;

   void             BuildPlane(const ENVector poly[],ENPlane &partition);
   void             BuildTreeIntern(ENBSPTree *tree,list<ENBSPPolygon> *polygons,ENPlane *parent);
   void             SplitPolygon(const ENPlane& partition,const ENBSPPolygon& poly,
                                  list<ENBSPPolygon> *front,list<ENBSPPolygon> *back);
   void             GetRootPolygon(list<ENBSPPolygon> *polygons,ENBSPPolygon &root,ENPlane *parent);
   ENfloat          RootRanking(list<ENBSPPolygon> *polygons,const ENBSPPolygon& poly,
                                ENBSPPolygon *root,ENPlane *parent);

   void             UpdateState(char *msg,ENuint pos,ENuint max);
};
#endif
