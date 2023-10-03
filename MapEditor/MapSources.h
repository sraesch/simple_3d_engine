//---------------------------------------------------------------------------
#ifndef MapSourcesH
#define MapSourcesH
//---------------------------------------------------------------------------
#include "EngineMedia.h"

class ENMapSources
{
 public:
   static void          Init();
   static void          Free();
   static void          Clear();
   static void          Load(char *Path,char *FileName);
   static ENbool        Draw(char *Name);
   static void          GetPacks(TListBox *lb,ENPackerBase::ENPackageType type);
   static ENint         GetPack(char *Name);
   static ENVector      GetObjDim(char *Name);
 protected:
   static ENPackerLoaded *packer;
};


#endif
