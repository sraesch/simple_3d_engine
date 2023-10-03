//---------------------------------------------------------------------------
#ifndef ProjectClassH
#define ProjectClassH
//---------------------------------------------------------------------------
#include "EngineSystem.h"
#include <Classes.hpp>
struct ENProjectFile
{
 enum FileType {ENUNKNOWN,ENSCRIPT,ENPACKAGE,ENMAP};
 FileType type;
 char   FileName[MAX_PATH];
 void GetFileType(AnsiString FileName,FileType &type);
};

class ENProjectClass
{
 public:
   static void          Free();
   static ENuint        GetNumFiles();
   static ENProjectFile GetProjectFile(ENuint ind);
   static void          AddFile(AnsiString FileName);
 private:
   static ENbool MustSave;
   static TList  *ProjectFiles;
   static char   ProjectFile[MAX_PATH];
};

#endif
