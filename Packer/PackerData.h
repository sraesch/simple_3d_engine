//---------------------------------------------------------------------------
#ifndef PackerDataH
#define PackerDataH
//---------------------------------------------------------------------------
#include "EngineSound.h"
#include "EngineMedia.h"
#pragma comment(lib,"EngineSound.lib")
#pragma comment(lib,"EngineMedia.lib")

class ENPackerData
{
 public:
   static void Init();
   static void Free();
   static void New();
   static ENPackerBase::ENPackerHeader GetHeader();
   static ENPacker::ENPackageHeader GetPackageHeader(ENuint ind);
   static void AddPackage(AnsiString FName,AnsiString Name);
   static void DeletePackage(AnsiString Name);
   static void RenamePackage(AnsiString Name,AnsiString NewName);
   static ENint SearchPackage(AnsiString Name);
   static void ExportPackage(ENuint ind,AnsiString FileName); 
   static ENbool GetCanSave();
   static ENbool GetMustSave();
   static void   Save();
   static void   Save(AnsiString FileName);
   static void   Load(AnsiString FileName);
 private:
   static ENPacker * packer;
   static ENbool     MustSave;
   static AnsiString CurrentFile;
};

#endif
