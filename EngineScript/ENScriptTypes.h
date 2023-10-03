#ifndef    _EN_SCRIPTTYPES_H_
#define    _EN_SCRIPTTYPES_H_

#include "../EngineSystem/EngineSystem.h"
//BASIC TYPES
#define ENINT          0
#define ENFLOAT        1
#define ENPROCEDUREVAR 2
#define ENSTRING       3
/////////////////////Defines/////////////////////////
#define _EN_SCRIPT_STACK_SIZE        0xFFFFF
#define _EN_SCRIPT_NUM_BACK_ESP      0xFF
#define _EN_SCRIPT_NUM_BACK_POS      0xFF
#define EN_MAX_NAME_LENGTH           0x80
#define EN_MAX_OPERATOR_LENGTH       0x3
#define EN_SCRIPT_INTERPRET_MAX_MSG  0x1000
/////////////////////Records///////////////////////////
struct ENTEXTSTR
{
 char str[EN_MAX_NAME_LENGTH+1];
};

struct ENVAR
{
 ENint  type,addr;
 char   Name[EN_MAX_NAME_LENGTH+1];
 ENuint Pos;
 ENbool Local,Ptr;
};

class ENScriptTypeClass
{
 public:
   struct ENScriptDataMember
   {
    ENTEXTSTR Name;
    ENuint    lnk;
   };
   struct ENScriptDataType
   {
    ENTEXTSTR Name;
    ENuint    Size;
    ENbool    basic;
    ENuint    pos;
    ENuint    numMems; 
    ENbool    pointer;
    ENScriptDataMember   *Members;
   };
   typedef void (*ENSCRIPTOPFUNCTION)(ENubyte *ENStack,ENint &_ENStackPointer);
   struct ENOPERATOR
   {
    ENint         t1,t2,res;
    char          Op[3];
    ENuint        Pos;
    ENSCRIPTOPFUNCTION OperatorFunc;
   };
   __declspec(dllexport) static ENbool SetTypes(char *Text);
   __declspec(dllexport) static ENbool AddType(ENScriptDataType type);
   __declspec(dllexport) static ENbool AddOperator(char *res,char *t1,char *t2,char *Op,
							                ENSCRIPTOPFUNCTION OpFunc);
   __declspec(dllexport) static ENbool FindType(char *Name,ENScriptDataType *type);
   __declspec(dllexport) static ENbool FindOperator(ENint t1,ENint t2,char *Op,ENOPERATOR *func);
   __declspec(dllexport) static void   RunOperator(ENint ind,ENubyte *ENStack,ENint &_ENStackPointer);
   __declspec(dllexport) static ENbool GetType(ENuint ind,ENScriptDataType *type);
   __declspec(dllexport) static ENuint GetNum();
   __declspec(dllexport) static ENint  ModifyType(ENint &type,ENint pos,char *mod);   
   __declspec(dllexport) static ENint  GetTypeSize(ENint type);
   __declspec(dllexport) static ENbool IsTypePtr(ENint type);
 protected:
   //Types
   static ENuint            NumDataTypes;
   static ENScriptDataType *DataTypes;
   //Operators
   static ENuint            NumOperators;
   static ENOPERATOR       *Operators;   

   __declspec(dllexport) static void   InitBasicTypes();
   __declspec(dllexport) static ENbool InterpretType(ENTEXTSTR ident,void *parser);
   __declspec(dllexport) static ENbool InterpretBasic(void *parser);
   __declspec(dllexport) static ENbool InterpretPack(void *parser);
   __declspec(dllexport) static ENint  GetMember(ENScriptDataType data,char *mod,ENScriptDataType &res);
};

#endif