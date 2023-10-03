#ifndef        _EN_SCRIPTEXTERN_H_
#define        _EN_SCRIPTEXTERN_H_

#include "ENScriptTypes.h"

class ENScriptExtern
{
 public:
   typedef void (*ENSCRIPTEXTERNFUNCTION)(ENubyte *Data,void *res);

   struct ENREGEXTERNVAR
   {
    char   Name[EN_MAX_NAME_LENGTH+1];
    ENint  type;
    ENuint Pos; 
   };

   struct ENREGEXTERNFUNCTION
   {
    char Name[EN_MAX_NAME_LENGTH+1];
    ENbool procedure;
    ENVAR *Params;
    ENuint NumParams;
    ENuint Pos;
    ENint  res;
    ENSCRIPTEXTERNFUNCTION Function;
   };

   struct ENEXTCONST
   {
    char Name[EN_MAX_NAME_LENGTH+1];
    ENint Pos;
    ENint Const;
   };
   __declspec(dllexport) static ENbool LoadExtern(char *Src);
   __declspec(dllexport) static ENbool AddExtVar(ENREGEXTERNVAR var);   
   __declspec(dllexport) static ENbool AddExtFunc(ENREGEXTERNFUNCTION func);   
   __declspec(dllexport) static ENbool AddExtConst(ENEXTCONST vconst);   
   __declspec(dllexport) static ENbool FindExtVar(char *Name,ENREGEXTERNVAR *var);
   __declspec(dllexport) static ENbool FindExtFunc(char *Name,ENREGEXTERNFUNCTION *func);
   __declspec(dllexport) static ENbool FindExtConst(char *Name,ENEXTCONST *vconst);
   __declspec(dllexport) static ENbool SetExtFunc(char *Name,ENSCRIPTEXTERNFUNCTION exfunc);
   __declspec(dllexport) static void   RunExtFunc(ENint ind,ENubyte *Data,void *res);
   __declspec(dllexport) static ENint  GetExtVarsSize();
   __declspec(dllexport) static ENint  GetConstValue(ENint ConstInd);
   __declspec(dllexport) static ENint  GetExtVarPos(char *Name);
 protected:
   //Extern variables
   static ENREGEXTERNVAR *ExtVars;
   static ENuint          NumExtVars;
   //Extern functions
   static ENREGEXTERNFUNCTION *ExtFuncs;
   static ENuint          NumExtFuncs;
   //Constants
   static ENEXTCONST    *ExtConst;
   static ENuint         NumConst;
   //Functions
   __declspec(dllexport) static ENbool InterpretConst(ENTEXTSTR ident,void *parser);
   __declspec(dllexport) static ENbool InterpretExternFunc(ENTEXTSTR ident,void *parser);
   __declspec(dllexport) static ENbool InterpretExternVar(ENTEXTSTR ident,void *parser,ENScriptTypeClass::ENScriptDataType type);   
   __declspec(dllexport) static ENbool ProcessParams(void *parser,ENREGEXTERNFUNCTION &func);

};

#endif