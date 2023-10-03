#ifndef       _EN_SCRIPTPARSER_H_
#define       _EN_SCRIPTPARSER_H_

#include "ENScriptTypes.h"

#define _RETURN_  10
#define _RETURN2_ 13
#define _SPACE_   32

class ENScriptParser
{
 public:
   struct ENScriptRange
   {
    char *start;
    char *end;
   };

   __declspec(dllexport) ENScriptParser();
   __declspec(dllexport) ~ENScriptParser();
   __declspec(dllexport) ENbool SetSource(char *Src);
   __declspec(dllexport) ENbool LoadSource(char *FileName);

   __declspec(dllexport) void   ResetPointer();
   __declspec(dllexport) ENuint CountChars(char Chr);
   __declspec(dllexport) ENbool JumpOverCommand();
   __declspec(dllexport) ENbool GetString(char *str);
   __declspec(dllexport) void   JumpOverSpace();
   __declspec(dllexport) ENbool GetIdentifier(char *res);
   __declspec(dllexport) ENbool GetText(char *res);
   __declspec(dllexport) ENbool GetExpression(ENScriptRange &range);
   __declspec(dllexport) ENbool GetOperator(char *Op);
   __declspec(dllexport) ENbool ValidName(char *Name,char *ExtCh);
   __declspec(dllexport) ENbool GetBody(ENScriptRange &range);
   __declspec(dllexport) ENbool PointerEnd();
   __declspec(dllexport) ENbool FindNext(char *Name,ENbool word);
   __declspec(dllexport) char   GetCurrentChar();
   __declspec(dllexport) void   IncPointer(ENint num);
   //Syntax Highlighting support
   __declspec(dllexport) static void GetOperatorString(char *str);
 protected:
   static const char OPERATORS[];

   char    curFileName[1024];
   char    *Source,*Pointer;
   ENuint  TextLength;   

   __declspec(dllexport) ENbool DeleteComments(char *Src);
};

#endif