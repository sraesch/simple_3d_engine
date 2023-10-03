//---------------------------------------------------------------------------
#ifndef ENCoreCollisionH
#define ENCoreCollisionH
//---------------------------------------------------------------------------
#include "EngineMedia.h"
#include "EngineScript.h"

class ENCoreCollision
{
 public:
   struct ENCOLSPHEREHULL
   {
    ENVector pos;
    ENfloat  range;
    void Init(ENVector p,ENfloat r)
    {
     pos=p;
     range=r;
    }
   };
   static ENbool SphereSphere(ENVector p1,ENVector p2,ENfloat r1,ENfloat r2);
   static ENbool SphereSphere(ENCOLSPHEREHULL *sphere1,ENCOLSPHEREHULL *sphere2);

   static ENbool SpherePoint(ENCOLSPHEREHULL *sphere1,ENVector point);

   static ENbool SphereBoxRaw(ENCOLSPHEREHULL *sphere,ENVector vBox[]);
   static ENbool SphereBox(ENCOLSPHEREHULL *sphere,ENVector vBox[],ENVector *SurfaceVec);

   static ENbool SphereLine(ENVector vLine[],ENVector pos,ENfloat range);
   static ENbool SphereLine(ENVector vLine[],ENVector pos,ENfloat range,ENVector &vec);
   static ENbool SphereLine(ENVector vLine[],ENCOLSPHEREHULL *sphere);

   static ENbool SphereMap(ENCOLSPHEREHULL *sphere);
   static ENbool SphereObjects(ENCOLSPHEREHULL *sphere,void *src);
   static ENbool Sphere(ENCOLSPHEREHULL *sphere,void *src);
   static ENbool MoveSphere(ENCOLSPHEREHULL *sphere,ENVector2 Angles,ENfloat range,void *src);
   static ENbool MoveSphereSmooth(ENCOLSPHEREHULL *sphere,ENVector2 Angles,ENfloat range,void *src);
   static ENbool MoveSphere(ENCOLSPHEREHULL *sphere,ENVector dir,void *src);
   static ENbool MoveSphereSmooth(ENCOLSPHEREHULL *sphere,ENVector dir,void *src);
   static ENbool MoveObject(ENScriptObject *obj,ENVector2 Angles,ENfloat range,ENbool smooth);
   static ENbool MoveObject(ENScriptObject *obj,ENVector dir,ENbool smooth);
   static ENbool SpherePortal(ENCOLSPHEREHULL *sphere,ENMapBuildLoader::ENPortal *portal);
   static ENbool SpherePortal(ENVector *pos,ENfloat range,
                              ENMapBuildLoader::ENPortal *portal);
   static void   Trace(ENVector A,ENVector B,ENint options,ENint func);

   static ENVector ComputeReflectVector(ENVector vNormal);
   static void     SetBspTree(ENBSPTree *tree);
 private:
   static ENBSPTree *bsptree;
   static ENVector ReflectVec;
   static ENVector SurfaceVec;
   static ENVector DirectionVec;
   static const  ENfloat DEFAULT_STEP_SIZE;
};

#endif
