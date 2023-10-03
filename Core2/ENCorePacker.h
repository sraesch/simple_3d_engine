//---------------------------------------------------------------------------
#ifndef ENCorePackerH
#define ENCorePackerH
//---------------------------------------------------------------------------
#include "EngineMedia.h"

class ENCorePacker
{
 public:
   static void   Init();
   static void   Free();
   static ENbool LoadStdPackage(char *FileName);
   static void   LoadMapPackage(char *FileName);
   static void*  GetSource(char *Name,ENPackerBase::ENPackageType type);
 private:
   static ENPackerLoaded *stdpacker;
   static ENPackerLoaded *mappacker;
};
#endif
