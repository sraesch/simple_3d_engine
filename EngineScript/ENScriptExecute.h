#ifndef     _EN_SCRIPTEXECUTE_H_
#define     _EN_SCRIPTEXECUTE_H_

#include "ENScriptTypes.h"
#include "ENScriptCode.h"
#include "ENScriptExtern.h"

class ENStackClass
{
 public:
   ENStackClass(ENint *StackPointer);
   ~ENStackClass();

   void ENPush(void *src,ENint size);
   void ENPop(void *dst,ENint size);
   ENubyte *ENGetStack();
   void ENCheckAddr(ENubyte *Addr);
 protected:
   ENubyte *Stack;
   ENint   StackSize;
   ENint   *StackPointer;
   void  ENCheckStackSize(ENint size);
};

class ENScriptExecute
{
 public:
   enum ENSCRIPTEXECUTEMODE {ENRUNNING,ENWAITFRAME,ENWAITTIME,ENFINISHED};
   
   __declspec(dllexport) ENScriptExecute(ENint start,ENScriptCode *code);
   __declspec(dllexport) ~ENScriptExecute();
   __declspec(dllexport) ENbool Execute();
   __declspec(dllexport) ENbool IsFinished();
   __declspec(dllexport) void   SetFinished();
 protected:   
   ENint   CurrentPos;
   ENint   StartPos;
   ENSCRIPTEXECUTEMODE mode;
   ENint   Wait;
   //Stack
   ENStackClass   *StackObj;
   ENint          _ENStackPointer;
   //Stack of stack pointer
   ENint   _ENStackESP[_EN_SCRIPT_NUM_BACK_ESP];
   ENuint  _ENNumStackESP;
   //Stack of back addresses
   ENuint  _ENBackPOS[_EN_SCRIPT_NUM_BACK_POS];
   ENint   _ENNumBackPOS;
   //Variables for time calculating
   ENuint   _TempTime;
   //Register
   ENubyte                _AReg[2048];//For results
   ENint                  _BReg;//For solved logic expressions
   ENint                  _CReg;//For solved time expressions
   ENint                  _DReg;//For address objects
   //Extern classes
   ENScriptCode      *code;
   //Functions
   __declspec(dllexport) ENbool RunCommand();
};

#endif