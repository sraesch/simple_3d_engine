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
 private:
   static ENbool InterpConfig(ENFile handle);
   static ENbool ReadShadowData(char FileBuffer[]);
   static void   StrToLower(char FileBuffer[]);
   static ENbool ReadVal(char FileBuffer[],const char *Name,ENint &val);

   static GLenum ShadowPrecision;
   static ENint  ShadowSize;
};

#endif
