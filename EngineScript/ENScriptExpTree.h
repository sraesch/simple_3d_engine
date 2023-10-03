#ifndef        _EN_SCRIPTEXPTREE_H_
#define        _EN_SCRIPTEXPTREE_H_

#include "ENScriptCode.h"
#include "ENScriptTypes.h"
#include "ENScriptExtern.h"
#include "ENScriptInterpret.h"

class ENScriptExpTree
{
 public:
   __declspec(dllexport) ENScriptExpTree(ENScriptCode *code);
   __declspec(dllexport) ~ENScriptExpTree();
   __declspec(dllexport) ENbool SolveExpTree(ENVAR *vars,ENuint NumVars,char **_Ops,ENScriptInterpret::ENARG *Args,
						 ENuint nop,ENuint narg,ENint &type,ENuint ind,void *Interpret);
 protected:
   struct ENEXPNODE
   {
    ENbool end;
    char Op[3];
    ENScriptInterpret::ENARG   arg;
    ENEXPNODE *left,*right;
   };

   static const char *Operators1[];
   static const char *Operators2[];
   static const char *Operators3[];
   static const char *Operators4[];
   static const ENint NumOps[];

   ENScriptCode      *code;
   __declspec(dllexport) ENint  OperatorRanking(char *Op);
   __declspec(dllexport) ENbool PushArgument(ENScriptInterpret::ENARG arg,ENVAR *vars,ENuint NumVars,ENint &type,
	                     ENuint ind,void *Interpret);
   __declspec(dllexport) void   DeleteExpTree(ENEXPNODE *node);
   __declspec(dllexport) ENbool CreateExpTree(ENEXPNODE *node,ENScriptInterpret::ENARG *Args,char **_Ops,
						 ENuint sop,ENuint eop);
   __declspec(dllexport) ENbool ProcessExpTree(ENVAR *vars,ENuint NumVars,ENint &type,
						 ENuint ind,ENEXPNODE *node,void *Interpret);

};

#endif