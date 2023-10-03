//---------------------------------------------------------------------------
#ifndef ENScriptInitDllH
#define ENScriptInitDllH
//---------------------------------------------------------------------------
#include "EngineMedia.h"

class ENScriptInitDll
{
 public:
   static ENbool Init();
   static void   ShowError(char *Msg);
 private:
   static ENPacker *packer;

   static ENbool LoadPackage();
   static char  *LoadSource(char *Name);
};

#endif
