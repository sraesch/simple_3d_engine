#include <iostream.h>
#include "ENScriptExecute.h"

//STACK CLASS
ENStackClass::ENStackClass(ENint *StackPointer)
{
 this->StackPointer=StackPointer;
 StackSize=0;
 Stack=NULL;
}

ENStackClass::~ENStackClass()
{
 if(StackSize) ENFreeMemory(Stack);
}

void ENStackClass::ENPush(void *src,ENint size)
{
 ENCheckStackSize(size);
 if(src) memcpy(Stack+StackPointer[0],src,size);
 StackPointer[0]+=size;
}

void ENStackClass::ENPop(void *dst,ENint size)
{
 if(dst) memcpy(dst,Stack+StackPointer[0]-size,size);
 StackPointer[0]-=size;
}

ENubyte *ENStackClass::ENGetStack()
{
 return this->Stack;
}

void ENStackClass::ENCheckAddr(ENubyte *Addr)
{
 ENCheckStackSize(Addr-Stack);
}

void ENStackClass::ENCheckStackSize(ENint size)
{
 if((StackPointer[0]+size)>StackSize)
 {
  StackSize+=0x800;
  Stack=(ENubyte*)ENReallocMemory(Stack,StackSize);  
  ENCheckStackSize(size);
 }
}
//EXECUTE CLASS
ENScriptExecute::ENScriptExecute(ENint start,ENScriptCode *code)
{
 //Set extern classes
 this->code=code;
 //Init
 _ENNumBackPOS=0;
 _ENNumStackESP=0;
 _ENStackPointer=0;
 CurrentPos=start;
 StartPos=start;
 Wait=0;
 mode=ENRUNNING;
 StackObj=new ENStackClass(&_ENStackPointer);
}

ENScriptExecute::~ENScriptExecute()
{
 delete StackObj;
}

ENbool ENScriptExecute::Execute()
{
 //Wait frames
 if(mode==ENWAITFRAME)
 {
  Wait--;
  if(Wait<=0)
    mode=ENRUNNING;
  else
	return true;
 }
 //Wait time
 if(mode==ENWAITTIME)
 {
  if((_TempTime+Wait)<=GetTickCount())
    mode=ENRUNNING;
  else
	return true;
 }

 if(CurrentPos<0)
   mode=ENFINISHED;
 //Process for commands
 while(mode==ENRUNNING)
   if(!RunCommand()) 
	 return false;

 return true;
}

ENbool ENScriptExecute::RunCommand()
{
 //Variables
 ENScriptCode::ENSCRIPTCOMMAND com;
 //Set current command  
 com=code->GetCommand(CurrentPos);
 //Increase current pos
 CurrentPos++;
 //////////////////////
 switch(com.comid)
 {
  //Exit command
  case ENScriptCode::ENSCRIPTEXIT:
	mode=ENFINISHED;
  break;
  //Wait frame command
  case ENScriptCode::ENWAIT:
    Wait=_CReg;
	mode=ENWAITFRAME;
	_TempTime=0;
  break;
  //Wait time command
  case ENScriptCode::ENWAITT:
    Wait=_CReg;
	mode=ENWAITTIME;
	_TempTime=GetTickCount();
  break;
  //Jump in the code
  case ENScriptCode::ENSCRIPTJUMP:
	CurrentPos=com.a;
  break;
  //Jump in the code if B register is true
  case ENScriptCode::ENSCRIPTJUMPTRUE:
	if(_BReg)
	  CurrentPos=com.a;
  break;
  //Jump in the code if B register is false
  case ENScriptCode::ENSCRIPTJUMPFALSE:
	if(!_BReg)
	  CurrentPos=com.a;
  break;
  //Call an extern function
  case ENScriptCode::ENSCRIPTEXTERNCALL:
	ENScriptExtern::RunExtFunc(com.a,StackObj->ENGetStack()+_ENStackPointer,_AReg);
  break;
  //Push something onto the stack
  case ENScriptCode::ENPUSH:
	StackObj->ENPush((void*)(code->GetStack()+com.a),com.b);
  break;
  //Push a constant onto the stack
  case ENScriptCode::ENPUSHCONST:
	StackObj->ENPush((void*)(code->GetConstAddr()+com.a),com.b);
  break;
  //Push a local address onto the stack
  case ENScriptCode::ENPUSHLOCAL:
	StackObj->ENCheckAddr(StackObj->ENGetStack()+_ENStackESP[_ENNumStackESP-1]+
		             com.a+com.b);
	StackObj->ENPush(StackObj->ENGetStack()+_ENStackESP[_ENNumStackESP-1]+
		             com.a,com.b);
  break;
  //Push something onto the stack by using address
  case ENScriptCode::ENPUSHBYADDR:
	if(_DReg)
	  StackObj->ENPush((void*)(_DReg+com.a),com.b);
	else
	  StackObj->ENPush(NULL,com.b);
  break;
  //Push the result from the A register to the stack
  case ENScriptCode::ENPUSHRESULT:
    StackObj->ENPush(_AReg,com.a);
  break;  
 //Pop something from the stack
  case ENScriptCode::ENPOP:
	StackObj->ENPop((void*)(code->GetStack()+com.a),com.b);	
  break;
  //Pop a local address from the stack
  case ENScriptCode::ENPOPLOCAL:
	StackObj->ENCheckAddr(StackObj->ENGetStack()+_ENStackESP[_ENNumStackESP-1]+
		             com.a+com.b);
	StackObj->ENPop(StackObj->ENGetStack()+_ENStackESP[_ENNumStackESP-1]+
		             com.a,com.b);
  break;
  //Pop the result from the stack to the A register
  case ENScriptCode::ENPOPRESULT:
	StackObj->ENPop(_AReg,com.a);
  break;
  //Pop the solved logic expression from the stack to the B register
  case ENScriptCode::ENPOPLOGIC:
	StackObj->ENPop(&_BReg,sizeof(ENint));
  break;
  //Pop the solved time expression from the stack to the C register
  case ENScriptCode::ENPOPTIME:
	StackObj->ENPop(&_CReg,sizeof(ENint));
  break;
  //Pop address to D register
  case ENScriptCode::ENSETADDR:
    StackObj->ENPop(&_DReg,sizeof(ENint));
  break;
  //Pop something from the stack by using address
  case ENScriptCode::ENPOPBYADDR:
	if(_DReg)
	  StackObj->ENPop((void*)(_DReg+com.a),com.b);
	else
	  StackObj->ENPop(NULL,com.b);
  break;
  //Save the Stack pointer
  case ENScriptCode::ENSAVESTACK:
    _ENStackESP[_ENNumStackESP]=_ENStackPointer;
	_ENNumStackESP++;
  break;
  //Reset the Stack pointer
  case ENScriptCode::ENRESETSTACK:
    _ENStackPointer=_ENStackESP[_ENNumStackESP-1];
	_ENNumStackESP--;
  break;
  //Save the position after jump
  case ENScriptCode::ENSAVEPOS:
	_ENBackPOS[_ENNumBackPOS]=CurrentPos+1;
	_ENNumBackPOS++;
  break;
  //Reset the current pos
  case ENScriptCode::ENRESETPOS:
    if(!_ENNumBackPOS)
	{
	 mode=ENFINISHED;
	 return true;
	}

	CurrentPos=_ENBackPOS[_ENNumBackPOS-1];
	_ENNumBackPOS--;
  break;
  //Add the Stack pointer
  case ENScriptCode::ENADDSTACK:
	_ENStackPointer+=com.a;
  break;  
  //Operator command
  case ENScriptCode::ENOPCOM:		
	  ENScriptTypeClass::RunOperator(com.a,StackObj->ENGetStack(),_ENStackPointer);
  break;
 };

 return true;
}

ENbool ENScriptExecute::IsFinished()
{
 return (mode==ENFINISHED);
}

void ENScriptExecute::SetFinished()
{
 mode=ENFINISHED;
}