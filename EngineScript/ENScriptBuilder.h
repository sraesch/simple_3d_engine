#ifndef         _EN_SCRIPTBUILDER_H_
#define         _EN_SCRIPTBUILDER_H_

#include "ENScriptLinker.h"
#include "ENScriptCode.h"
#include "ENScriptTypes.h"
#include "ENScriptExtern.h"
#include "ENScriptInterpret.h"

class ENScriptBuilder
{
 public:
   typedef void (*ENSCRIPTMSGCALLBACK) (char *Msg);
   
   __declspec(dllexport) ENScriptBuilder(ENSCRIPTMSGCALLBACK MsgFunc);
   __declspec(dllexport) ~ENScriptBuilder();
   __declspec(dllexport) ENbool Build(char *FileName,char *Path,char *Dst);
 protected:
   ENScriptLinker    *linker;
   ENScriptCode      *code;
   ENScriptInterpret *interp;
   ENSCRIPTMSGCALLBACK MsgFunc; 
};

#endif