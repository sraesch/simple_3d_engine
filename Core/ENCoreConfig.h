//---------------------------------------------------------------------------
#ifndef ENCoreConfigH
#define ENCoreConfigH
//---------------------------------------------------------------------------
#include "EngineSystem.h"

class ENCoreConfig
{
 public:
   static ENbool Load();

   static void   GetShadowData(GLenum &precision,ENint &size);
   static ENbool GetShadowEnable();
 private:
   static ENbool InterpConfig(ENFile handle);
   static ENbool ReadData(char FileBuffer[]);
   static void   StrToLower(char FileBuffer[]);
   static ENbool ReadVal(char FileBuffer[],const char *Name,ENint &val);

   static GLenum ShadowPrecision;
   static ENint  ShadowSize;
   static ENint  ShadowEnable;
   static ENint  MaxLights;
   static ENint  MaxShadows;
};

#endif
