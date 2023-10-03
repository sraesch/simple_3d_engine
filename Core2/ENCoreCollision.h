//---------------------------------------------------------------------------
#ifndef ENCoreCollisionH
#define ENCoreCollisionH
//---------------------------------------------------------------------------
#include "EngineMedia.h"

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
   static ENbool SphereMap(ENCOLSPHEREHULL *sphere);
   static ENbool Sphere(ENCOLSPHEREHULL *sphere);
   static ENbool MoveSphere(ENCOLSPHEREHULL *sphere,ENVector2 Angles,ENfloat range);
   static ENbool SpherePortal(ENCOLSPHEREHULL *sphere,ENMapBuildLoader::ENPortal *portal);
   static ENbool SpherePortal(ENVector *pos,ENfloat range,
                              ENMapBuildLoader::ENPortal *portal);
 private:
   static const  ENfloat STEPSIZE;

   static ENbool SphereSphere(ENCOLSPHEREHULL *sphere1,ENCOLSPHEREHULL *sphere2);
};

#endif
