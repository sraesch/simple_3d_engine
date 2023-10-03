#ifndef        _EN_SCRIPTINTERPRET_H_
#define        _EN_SCRIPTINTERPRET_H_

#include "ENScriptParser.h"
#include "ENScriptCode.h"
#include "ENScriptExtern.h"

class ENScriptInterpret : public ENScriptParser
{
 public:
  enum ENSCRIPTARGTYPES {ENGLOBAL,ENCONST,ENLOCAL,ENEXPRESSION,ENINTERNCALL,ENEXTERNCALL,
                         ENEXTERNVAR};
  enum ENKEYWORDS {ENKINCLUDE,ENKPACKAGE,ENKPATH,ENKVAR,ENKPROCEDURE,ENKFUNCTION,ENKACTION,
                   ENKEXIT,ENKRETURN,ENKIF,ENKELSE,ENKWHILE,ENKWAIT,ENKWAITT,ENKUNKNOWN};
  struct ENFUNCPROC
  {
   ENbool procedure;
   char Name[EN_MAX_NAME_LENGTH+1];
   ENuint NumParams;
   ENint  res;
   ENVAR *Params;
   ENint Pos;
   ENScriptRange range;
   ENbool Intepreted;
  };
  struct ENARG
  {
   ENint  SrcType;
   ENbool IsPointer;
   ENuint addr,Pos,Ptr;
   ENint  type;
   ENFUNCPROC func;
   ENScriptExtern::ENREGEXTERNFUNCTION exfunc;
   ENScriptRange Expr;
  };

   __declspec(dllexport) ENScriptInterpret();
   __declspec(dllexport) ~ENScriptInterpret();
   __declspec(dllexport) ENbool Compile(ENScriptCode *code);
   __declspec(dllexport) void   GetCompileLog(ENuint len,char *dst);   

   __declspec(dllexport) ENbool GetIdentifier(char *res,ENbool error);
   __declspec(dllexport) ENbool GetBody(ENScriptRange &range);
   __declspec(dllexport) void   SetNewPointer(char *newp);
   __declspec(dllexport) void   ResetPointer();
   //Process
   __declspec(dllexport) ENbool ProcessExpression(ENScriptRange range,ENVAR *vars,ENuint NumVars,
						                          ENint &type,ENuint ind);
   __declspec(dllexport) ENbool ProcessExternCall(ENuint ind,ENScriptExtern::ENREGEXTERNFUNCTION func,
											      ENVAR *Vars,ENuint NumVars);
   __declspec(dllexport) ENbool ProcessInternCall(ENuint ind,ENFUNCPROC func,ENVAR *Vars,
					                        ENuint NumVars);
 protected:
   //Extern classes
   ENScriptCode      *code;
   //General data
   char       *SndPointer;
   ENuint      NumGlobalVars,NumFuncs;
   ENVAR      *GlobalVars;
   ENFUNCPROC *Funcs;
   //Compiler log
   char CompileLog[1024];
   //General functions
   __declspec(dllexport) ENbool InterpretCommand(char *com);
   __declspec(dllexport) ENScriptInterpret::ENKEYWORDS CommandID(char *com);
   __declspec(dllexport) void  *AddToAddr(ENuint &num,ENuint elementsize,void *dst,void *src);
   __declspec(dllexport) ENbool ValidName(char *name);
   __declspec(dllexport) ENuint VarPos(ENuint NumVars,ENVAR *Vars);  
   __declspec(dllexport) ENbool GetExpression(ENScriptRange &range);
   __declspec(dllexport) ENbool GetOperator(char *Op);
   __declspec(dllexport) void   GetArgument(char *res);
   __declspec(dllexport) ENbool IsProcAddr(char *Text,ENint &addr);
   __declspec(dllexport) ENbool IsInteger(char *Text,ENint &ConstInd);
   __declspec(dllexport) ENbool IsFloat(char *Text);
   __declspec(dllexport) ENbool IsString(char *Text,char *str);
   __declspec(dllexport) ENbool FindVar(char *Text,ENVAR *vars,ENuint NumVars,ENVAR &res,ENuint ind);
   __declspec(dllexport) ENbool EndOfParameter();
   __declspec(dllexport) ENbool EndOfParameter2();
   __declspec(dllexport) ENbool FindFunc(char *Text,ENFUNCPROC &func);
   __declspec(dllexport) ENuint VarsSize(ENVAR *Vars,ENuint NumVars);
   __declspec(dllexport) ENbool CompileFunction(ENuint ind);
   __declspec(dllexport) ENbool JumpToBraket();
   __declspec(dllexport) void   EndCurrentFunc(ENint ind);
   //Process functions
   __declspec(dllexport) ENbool ProcessInclude();
   __declspec(dllexport) ENbool ProcessPackage();
   __declspec(dllexport) ENbool ProcessPath();
   __declspec(dllexport) ENbool ProcessGlobalVar();
   __declspec(dllexport) ENbool ProcessFunction(ENbool action,ENbool result);
   __declspec(dllexport) ENbool ProcessVar(ENVAR &var);
   __declspec(dllexport) ENbool ProcessParams(ENFUNCPROC &func);  
   __declspec(dllexport) ENbool ProcessArgument(char *Text,ENVAR *Vars,ENuint NumVars,
	                                            ENuint ind,ENARG &arg);
   __declspec(dllexport) ENbool ProcessExit();
   __declspec(dllexport) ENbool ProcessReturn(ENVAR *vars,ENuint NumVars,ENuint ind);
   __declspec(dllexport) ENbool ProcessConversion(ENint from,ENint to);
   __declspec(dllexport) ENbool ProcessWait(ENVAR *vars,ENuint NumVars,ENuint ind,
	                                        ENbool RealTime);
   __declspec(dllexport) ENbool ProcessParamsCall(ENuint ind,ENVAR *Vars,ENuint NumVars,
											      ENuint NumParam,ENVAR *Param);
   __declspec(dllexport) ENbool ProcessBlock(ENuint ind,ENuint &NumVars,ENVAR **Vars);
   __declspec(dllexport) ENbool ProcessAssignment(ENuint ind,ENVAR var,ENVAR *vars,ENuint NumVars);
   __declspec(dllexport) ENbool ProcessCommand(ENuint ind,ENKEYWORDS command,
						                       ENVAR **vars,ENuint &NumVars);
   __declspec(dllexport) ENbool ProcessIf(ENVAR **vars,ENuint &NumVars,ENuint ind);
   __declspec(dllexport) ENbool ProcessWhile(ENVAR **vars,ENuint &NumVars,ENuint ind);
   //Message functions
   __declspec(dllexport) void AddMsg(char *msg);
   __declspec(dllexport) void AddError(char *Error);
   //Syntax Highlighting support
   __declspec(dllexport) static ENuint GetNumCommmands();
   __declspec(dllexport) static void   GetCommands(char **coms);
  private:
    static const ENuint NUMCOMMANDS;
    static const char  *COMMANDS[];

};

#endif