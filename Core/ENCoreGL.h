//---------------------------------------------------------------------------
#ifndef ENCoreGLH
#define ENCoreGLH
//---------------------------------------------------------------------------
#include "EngineMedia.h"

class ENCoreGL
{
 public:
   static ENbool Init(HWND WndHandle);
   static void   Free();
   static void   BeginDraw();
   static void   EndDraw();
   static void   Activate();
 private:
   static ENGLClass *gldevice;
   static void   ConfigGL();
   static void   TransformMatrices();   
};
#endif
