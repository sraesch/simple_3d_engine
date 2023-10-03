#ifndef      _EN_SCRIPTLINKER_H_
#define      _EN_SCRIPTLINKER_H_

#include "ENScriptParser.h"

class ENScriptLinker
{
 public:
   __declspec(dllexport) ENScriptLinker();
   __declspec(dllexport) ~ENScriptLinker();
   __declspec(dllexport) ENbool    Link(char *FileName,char *Path);
   __declspec(dllexport) ENuint    GetNum();
   __declspec(dllexport) ENTEXTSTR GetFile(ENuint ind);
   __declspec(dllexport) void      GetError(ENuint len,char *dst);
 protected:
   struct ENLinks
   {
    ENTEXTSTR  FName;
    ENuint     NumLinks;
    ENLinks *Links;
    ENLinks *Parent;
   };
   ENScriptParser  *parser;
   ENTEXTSTR        Path;
   ENuint           NumFiles;
   ENTEXTSTR       *Files;
   ENLinks          Links;
   char             ErrorMsg[1024];
   __declspec(dllexport) ENbool PharseFile(ENLinks *Link);
   __declspec(dllexport) ENbool GetInclude(char *inc);
   __declspec(dllexport) ENbool IsRedundant(ENLinks *link);
   __declspec(dllexport) void   AddFile(char *fname);
   __declspec(dllexport) void   AddError(char *Error);
   __declspec(dllexport) void   DeleteLinks(ENLinks *lnks);
};

#endif