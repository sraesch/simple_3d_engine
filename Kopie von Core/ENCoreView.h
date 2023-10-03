//---------------------------------------------------------------------------
#ifndef ENCoreViewH
#define ENCoreViewH
//---------------------------------------------------------------------------
#include "EngineSystem.h"

class ENCoreView
{
 public:
   static ENbool  Init();
   static void    TransformProj();
   static void    TransformModel();
   static void    RotateModel();
   static ENVector From3DTo2D(ENVector vec);
   static ENVector From3DTo2D2(ENVector vec);
   static ENVector From3DTo2D3(ENVector vec);
   static ENVector From2DTo3D(ENVector vec);
   static void     SetCamera(ENCamera cam);
   static void     UpdateListener();
   static ENVector GetCamPos();
 private:
   static ENdouble  ModelMat[16];
   static ENdouble  ProjMat[16];
   static ENint     Viewport[4];
   static ENCamera *camera;
};

#endif
