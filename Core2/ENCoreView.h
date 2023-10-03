//---------------------------------------------------------------------------
#ifndef ENCoreViewH
#define ENCoreViewH
//---------------------------------------------------------------------------
#include "EngineSystem.h"

class ENCoreView
{
 public:
   static ENbool  Init();
   static void    TransformProj(ENfloat aspect);
   static void    TransformModel();
   static void    RotateModel();
 private:
   static ENCamera *camera;
};

#endif
