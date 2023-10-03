//---------------------------------------------------------------------------
#ifndef ModelPrefabsH
#define ModelPrefabsH
//---------------------------------------------------------------------------
#include "EngineSystem.h"
#include "EngineMedia.h"

class ENModelPrefabs
{
 public:
   static void AddCube(ENVector Pos,ENVector Size);
   static void AddCone(ENVector Pos,ENfloat Range,ENfloat Height,ENuint Details);
   static void AddCylinder(ENVector Pos,ENfloat Range,ENfloat Height,ENuint Details);
   static void AddSphere(ENVector Pos,ENfloat Range,ENuint Details);
 private:
   static ENVector *Create2DRing(ENuint details,ENfloat range,ENuint &num);
   static void      InitFace(ENModel::ENModelFace &face);
};

#endif
