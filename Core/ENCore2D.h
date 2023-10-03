//---------------------------------------------------------------------------
#ifndef ENCore2DH
#define ENCore2DH
//---------------------------------------------------------------------------
#include "EngineMedia.h"

class ENCore2D
{
 public:
   static ENbool Init();
   static void   Free();
   static void   Draw();
   static void   SetCursorBmp(char *Name);
 private:
   static ENMatrixSpec oldmatrices;

   static void InitMatrices();
   static void ResetMatrices();
   static ENbool RegisterScript();
};

#endif
