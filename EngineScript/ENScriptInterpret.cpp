#include <iostream.h>
#include "ENScriptInterpret.h"
#include "ENScriptExpTree.h"
#include "EngineScript.h"
///////////////////////////ERROR's///////////////////////////////////
#define   EN_COMPILE_ERROR0    "Identifier is too long!!!"
#define   EN_COMPILE_ERROR1    "Unknown keyword!!!"
#define   EN_COMPILE_ERROR2    "Invalid string!!!"
#define   EN_COMPILE_ERROR3    "Undefined type!!!"
#define   EN_COMPILE_ERROR4    "Name already exists!!!"
#define   EN_COMPILE_ERROR5    "Identifier is already a keyword!!!"
#define   EN_COMPILE_ERROR6    "Identifier contain invalid charakters!!!"
#define   EN_COMPILE_ERROR7    "Invalid declaration!!!"
#define   EN_COMPILE_ERROR8    "Invalid header!!!"
#define   EN_COMPILE_ERROR9    "Invalid body!!!"
#define   EN_COMPILE_ERROR10   "Invalid expression!!!"
#define   EN_COMPILE_ERROR11   "Operator is too long!!!"
#define   EN_COMPILE_ERROR12   "This is not an action!!!"
#define   EN_COMPILE_ERROR13   "Action must be already defined!!!"
#define   EN_COMPILE_ERROR14   "Unknown statement!!!"
#define   EN_COMPILE_ERROR15   "Missing ;!!!"
#define   EN_COMPILE_ERROR16   "Types are incompatible!!!"
#define   EN_COMPILE_ERROR18   "Invalid Parameters!!!"
#define   EN_COMPILE_ERROR19   "Invalid assignment!!!"
#define   EN_COMPILE_ERROR20   "Invalid call!!!"
#define   EN_COMPILE_ERROR21   "Invalid command!!!"
#define   EN_COMPILE_ERROR22   "Function must be already defined!!!"
#define   EN_COMPILE_ERROR23   "Invalid if structur!!!"
/////////////////////////////////////////////////////////////////////
const ENuint ENScriptInterpret::NUMCOMMANDS=14;
const char *ENScriptInterpret::COMMANDS[]=
            {"include","package","path","var",
             "procedure","function","action",
             "exit","return","if","else","while",
             "wait","waitt"};

ENScriptInterpret::ENScriptInterpret()
                  :ENScriptParser()
{
 CompileLog[0]=0;
 NumGlobalVars=0;
 NumFuncs=0;
 GlobalVars=NULL;
 Funcs=NULL;
}

ENScriptInterpret::~ENScriptInterpret()
{
 //Delete global variables
 ENFreeMemory(GlobalVars);
 //Delete functions
 ENFreeMemory(Funcs);
}

ENbool ENScriptInterpret::Compile(ENScriptCode *code)
{
 //VARS
 ENTEXTSTR comstr;
 ENuint  timesave;
 char    TextBuffer[80];
 //Init 
 this->code=code;
 CompileLog[0]=0;
 timesave=GetTickCount();
 //Parse the script  
 while(!PointerEnd())
 {
  //Jump to next command
  JumpOverSpace();
  if(PointerEnd()) break;
  //Get command
  if(!GetIdentifier(comstr.str,true)) return false;  
  //Interpret command
  if(!InterpretCommand(comstr.str)) return false;
 }
 //Finished
 timesave=GetTickCount()-timesave;
 sprintf(TextBuffer,"Script compiled in %f seconds",timesave/1000.0f);
 AddMsg(TextBuffer);
 return true;
}

void ENScriptInterpret::GetCompileLog(ENuint len,char *dst)
{
 ENuint slen=strlen(CompileLog);
 if(slen>len-1)
 {
  memcpy(dst,CompileLog,len-1);
  CompileLog[len]=0;
 }
 else
   strcpy(dst,CompileLog);
}


ENbool ENScriptInterpret::GetIdentifier(char *res,ENbool error)
{
 if(!ENScriptParser::GetIdentifier(res))
 {
  if(error) AddError(EN_COMPILE_ERROR0);
  return false;
 }
 else
   return true;
}

ENbool ENScriptInterpret::GetBody(ENScriptRange &range)
{
 if(!ENScriptParser::GetBody(range))
 {
  AddError(EN_COMPILE_ERROR9);
  return false;
 }
 else
   return true;
}

void ENScriptInterpret::SetNewPointer(char *newp)
{
 SndPointer=Pointer;
 Pointer=newp;
}

void ENScriptInterpret::ResetPointer()
{
 Pointer=SndPointer;
}
/////////////////////////////////GENERAL FUNCTIONS/////////////////////////////////
ENbool ENScriptInterpret::InterpretCommand(char *com)
{
 //VARS
 ENKEYWORDS comID;
 ENScriptCode::ENACTIONREC action;
 //Get command id
 comID=CommandID(com);
 if(comID==ENKUNKNOWN)
 {
  AddError(EN_COMPILE_ERROR1);
  return false;
 }
 //Interpret command
 switch(comID)
 {
  case ENKINCLUDE:
    if(!ProcessInclude()) return false;
  break;
  case ENKPACKAGE:
    if(!ProcessPackage()) return false;
  break;
  case ENKPATH:
    if(!ProcessPath()) return false;
  break;
  case ENKVAR:
	if(!ProcessGlobalVar()) return false;
  break;
  case ENKPROCEDURE:
	if(!ProcessFunction(false,false)) return false;
    if(!CompileFunction(NumFuncs-1)) return false;
	//Look for main	
	if(Funcs[NumFuncs-1].NumParams==0&&strcmpi(Funcs[NumFuncs-1].Name,"main")==0)
	  code->SetMainFunc(Funcs[NumFuncs-1].Pos);
	//Look for load
	if(Funcs[NumFuncs-1].NumParams==0&&strcmpi(Funcs[NumFuncs-1].Name,"load")==0)
	  code->SetLoadFunc(Funcs[NumFuncs-1].Pos);
  break;
  case ENKFUNCTION:
	if(!ProcessFunction(false,true)) return false;
	if(!CompileFunction(NumFuncs-1)) return false;
  break;
  case ENKACTION:	
	if(!ProcessFunction(true,false)) return false;
	if(!CompileFunction(NumFuncs-1)) return false;
    //Register action
	strcpy(action.Name,Funcs[NumFuncs-1].Name);
	action.Pos=Funcs[NumFuncs-1].Pos;
	code->AddAction(action);
  break;
 };
 //Finished
 return true;
}

ENScriptInterpret::ENKEYWORDS ENScriptInterpret::CommandID(char *com)
{
 for(ENuint a=0;a<NUMCOMMANDS;a++)
   if(strcmpi(COMMANDS[a],com)==0) return ENKEYWORDS(a);

 return ENKUNKNOWN;
}

void *ENScriptInterpret::AddToAddr(ENuint &num,ENuint elementsize,void *dst,void *src)
{
 num++;
 dst=ENReallocMemory(dst,num*elementsize);
 memcpy((ENubyte*)(dst)+(num-1)*elementsize,src,elementsize);
 return dst;
}

ENbool ENScriptInterpret::ValidName(char *name)
{	
 //VARS
 ENuint a;
 ENScriptTypeClass::ENScriptDataType type; 
 //Check charakters
 if(!ENScriptParser::ValidName(name,"")) {AddError(EN_COMPILE_ERROR6);return false;}
 //Check commands
 for(a=0;a<NUMCOMMANDS;a++)
   if(strcmpi(COMMANDS[a],name)==0) {AddError(EN_COMPILE_ERROR5);return false;}
 //Check types
 for(a=0;a<ENScriptTypeClass::GetNum();a++)
 {  
  ENScriptTypeClass::GetType(a,&type);  
  if(strcmpi(type.Name.str,name)==0) {AddError(EN_COMPILE_ERROR5);return false;}
 }
 //Check global variables 
 for(a=0;a<NumGlobalVars;a++)
	 if(strcmpi(GlobalVars[a].Name,name)==0) {AddError(EN_COMPILE_ERROR4);return false;}
 //Check extern variables 
 if(ENScriptExtern::FindExtVar(name,NULL)) {AddError(EN_COMPILE_ERROR4);return false;}
 //Check extern constant 
 if(ENScriptExtern::FindExtConst(name,NULL)) {AddError(EN_COMPILE_ERROR4);return false;}
 //Check functions
 for(a=0;a<NumFuncs;a++)
   if(strcmpi(Funcs[a].Name,name)==0) {AddError(EN_COMPILE_ERROR4);return false;}
 //Check extern functions 
 if(ENScriptExtern::FindExtFunc(name,NULL)) {AddError(EN_COMPILE_ERROR4);return false;}

 return true;
}

ENuint ENScriptInterpret::VarPos(ENuint NumVars,ENVAR *Vars)
{ 
 if(NumVars==0) 
   return 0;
 else
 {
  ENuint res=Vars[NumVars-1].addr;
  res+=ENScriptTypeClass::GetTypeSize(Vars[NumVars-1].type);
  return res;
 }
}

ENuint ENScriptInterpret::VarsSize(ENVAR *Vars,ENuint NumVars)
{
 ENuint res=0;
 for(ENuint a=0;a<NumVars;a++)
   res+=ENScriptTypeClass::GetTypeSize(Vars[a].type);
 
 return res;
}


ENbool ENScriptInterpret::GetExpression(ENScriptRange &range)
{  
 if(!ENScriptParser::GetExpression(range))
 {  
  AddError(EN_COMPILE_ERROR10);
  return false;
 }
 else
   return true;
}

ENbool ENScriptInterpret::GetOperator(char *Op)
{
 if(!ENScriptParser::GetOperator(Op))
 {
  AddError(EN_COMPILE_ERROR11);
  return false;
 }
 else
   return true;
}

void ENScriptInterpret::GetArgument(char *res)
{
 //Vars
 char _string[1024];
 ENuint a=0; 
 //Look for string   
 if(GetString(_string))
 {  
  sprintf(res,"%c%s%c",'"',_string,'"');
  return;
 }
 
 //Check first operator
 if(*Pointer!=_RETURN_&&*Pointer!=_RETURN2_&&
	*Pointer!=_SPACE_&&*Pointer!=';'&&
	*Pointer!='('&&*Pointer!=')'&&*Pointer!=','&&
	(strchr(OPERATORS,*Pointer)==NULL||*Pointer=='+'||*Pointer=='-'))
 {
  res[a]=*Pointer;
  Pointer++;
  a++;
 }
 //GO 
 while(*Pointer!=_RETURN_&&*Pointer!=_RETURN2_&&
	   *Pointer!=_SPACE_&&strchr(OPERATORS,*Pointer)==NULL&&
	   *Pointer!=';'&&*Pointer!='('&&*Pointer!=')'&&*Pointer!=',')
 {
  
  res[a]=*Pointer;
  Pointer++;
  a++;
 }

 res[a]=0;
}

ENbool ENScriptInterpret::IsProcAddr(char *Text,ENint &addr)
{ 
 if(strcmpi(Text,"NULL")==0)
 {
  addr=-1;
  return true;
 }

 for(ENuint a=0;a<NumFuncs;a++)
   if(strcmpi(Funcs[a].Name,Text)==0)
   {
	//Check if it is a call
	JumpOverSpace();
	if(*Pointer!='(')
    {
     if(Funcs[a].NumParams>0||!Funcs[a].procedure)
  	   AddError(EN_COMPILE_ERROR12);
     if(Funcs[a].Pos<0)
 	   AddError(EN_COMPILE_ERROR13);

	 addr=Funcs[a].Pos;
	 return true;
    }
   }   

 return false;
}

ENbool ENScriptInterpret::IsInteger(char *Text,ENint &ConstInd)
{
 ENuint a,len=strlen(Text);
 //Check common things
 if(len<=0) return false;
 //Check if it is a constant
 ENScriptExtern::ENEXTCONST vconst;
 if(ENScriptExtern::FindExtConst(Text,&vconst))
 {
  ConstInd=vconst.Pos;
  return true;
 }
 //Check the beginning
 if(!(*Text>='0'&&*Text<='9')&&*Text!='-'&&*Text!='+')
   return false;
 //Check the rest of the string
 for(a=1;a<len;a++)
   if(Text[a]<'0'||Text[a]>'9')
	 return false;

 ConstInd=-1;
 return true;
}

ENbool ENScriptInterpret::IsFloat(char *Text)
{
 ENuint len=strlen(Text);
 //Check common things
 if(len<=0) return false;
 //Check the beginning
 if(!(*Text>='0'&&*Text<='9')&&*Text!='-'&&*Text!='+')
   return false;
 //Check the rest of the string
 for(ENuint a=1;a<len;a++)
   if(!(Text[a]>='0'&&Text[a]<='9')&&Text[a]!='.')
	 return false;

 return true;
}

ENbool ENScriptInterpret::IsString(char *Text,char *str)
{
 ENuint len=strlen(Text);
 if(*Text!='"') return false; 
 if(Text[len-1]!='"') return false; 
 for(ENuint a=1;a<len-1;a++)
	 if(Text[a]=='"') return false;

 memcpy(str,Text+1,len-2);
 str[len-2]=0;
 return true;
}

ENbool ENScriptInterpret::FindVar(char *Text,ENVAR *vars,ENuint NumVars,ENVAR &res,ENuint ind)
{ 
 //Vars
 ENuint a;
 ENint  b=-1;
 ENbool found=false;
 ENScriptExtern::ENREGEXTERNVAR extvar;
 char Temp[EN_MAX_NAME_LENGTH]; 
 //Create Temp string
 strcpy(Temp,Text);
 for(a=0;a<strlen(Text);a++)
   if(Temp[a]=='.')
   {
    Temp[a]=0;
	b=(ENint)a;
	break;
   }

 //Check all global variables      
 for(a=0;a<NumGlobalVars;a++)
   if(strcmpi(Temp,GlobalVars[a].Name)==0)
   {
	res=GlobalVars[a];
	found=true;
	break;
   }
 //Check all extern variables   
 if(ENScriptExtern::FindExtVar(Temp,&extvar))
 {
  res.addr=extvar.Pos;
  res.Local=false;
  strcpy(res.Name,extvar.Name);  
  res.type=extvar.type;
  found=true;
 }
 //Check all parameter 
 for(a=0;a<Funcs[ind].NumParams;a++)
   if(strcmpi(Temp,Funcs[ind].Params[a].Name)==0)
   {
	res=Funcs[ind].Params[a];
	found=true;
	break;
   }
 //Check all local variables
 for(a=0;a<NumVars;a++)
   if(strcmpi(Temp,vars[a].Name)==0)
   {
	res=vars[a];
	found=true;
	break;
   }
 //Check for a modification of variable      
 res.Ptr=false;
 if(found)
 {
  if(b==-1)
	return true;
  else  
  {
   ENint tmp;
   if(ENScriptTypeClass::IsTypePtr(res.type))
   {//pointer	
	tmp=ENScriptTypeClass::ModifyType(res.type,0,Text+b+1);
	if(tmp==-1) return false;
	res.Ptr=true;
	res.Pos=tmp;
   }
   else
   {//not pointer	   
	tmp=ENScriptTypeClass::ModifyType(res.type,res.addr,Text+b+1);
	if(tmp==-1) return false;	
    res.addr=tmp;
   }      
   return true;
  }
 }
 else
   return false;
}

ENbool ENScriptInterpret::FindFunc(char *Text,ENFUNCPROC &func)
{
 for(ENuint a=0;a<NumFuncs;a++)
   if(strcmpi(Text,Funcs[a].Name)==0)
   {
	func=Funcs[a];
	return true;
   }
 
 return false;
}

ENbool ENScriptInterpret::JumpToBraket()
{
 //VARS
 ENuint cb=1,cc=0;
 //Go trough script
 while(!PointerEnd())
 {
  if(*Pointer=='"') cc++;
  if(cc%2==0&&*Pointer=='(') cb++;
  if(cc%2==0&&*Pointer==')') cb--;
  if(!cb) return true;
  Pointer++;
 }
 //Not found
 return false; 
}
/////////////////////////////////PROCESS FUNCTIONS/////////////////////////////////
ENbool ENScriptInterpret::ProcessInclude()
{
 JumpOverSpace();
 if(!GetString(NULL))
 {
  AddError(EN_COMPILE_ERROR2);
  return false;
 }

 return true;
}

ENbool ENScriptInterpret::ProcessPackage()
{
 //VARS
 char str[256];
 //
 JumpOverSpace();
 if(!GetString(str))
 {
  AddError(EN_COMPILE_ERROR2);
  return false;
 }

 code->SetPackage(str);
 return true;
}

ENbool ENScriptInterpret::ProcessPath()
{
 //VARS
 ENScriptCode::ENPATH path; 
 //
 JumpOverSpace();
 if(!GetString(path.Path))
 {
  AddError(EN_COMPILE_ERROR2);
  return false;
 }

 code->AddPath(path);
 return true;
}

ENbool ENScriptInterpret::ProcessVar(ENVAR &var)
{
 //Vars
 ENTEXTSTR Temp;
 ENScriptTypeClass::ENScriptDataType type;
 //Jump to type
 JumpOverSpace();
 //Read out type
 if(!GetIdentifier(Temp.str,true)) return false;
 if(!ENScriptTypeClass::FindType(Temp.str,&type))
 {
  AddError(EN_COMPILE_ERROR3);
  return false;
 }
 var.type=type.pos;
 //Jump to identifier
 JumpOverSpace();
 //Read out identifier 
 if(!GetIdentifier(var.Name,true)) return false;
 if(!ValidName(var.Name)) return false;
 //Jump to ';'
 JumpOverSpace();
 //Check for ';'
 if(*Pointer!=';')
 {
  AddError(EN_COMPILE_ERROR7);
  return false;
 }
 else
   Pointer++;
 //Finished
 return true;
}

ENbool ENScriptInterpret::ProcessGlobalVar()
{
 //Vars
 ENVAR var;
 ////////////////// 
 //Get variable
 if(!ProcessVar(var)) return false;
 //Set position  
 var.addr=VarPos(NumGlobalVars,GlobalVars)+code->GetStackPointer();
 //Set local flag false
 var.Local=false;
 //Add variable 
 GlobalVars=(ENVAR*)AddToAddr(NumGlobalVars,sizeof(ENVAR),GlobalVars,&var);
 //Add text if the variable is a text
 ENScriptTypeClass::ENScriptDataType type;
 ENScriptTypeClass::GetType(var.type,&type);
 if(strcmpi(type.Name.str,"text")==0)
   code->AddText(var.addr);
 //Add panel if the variable is a panel
 if(strcmpi(type.Name.str,"panel")==0)
   code->AddPanel(var.addr);
 //Finished 
 return true;
}

ENbool ENScriptInterpret::ProcessFunction(ENbool action,ENbool result)
{
 //Vars
 ENFUNCPROC       func;
 ENScriptTypeClass::ENScriptDataType type;
 char             Temp[EN_MAX_NAME_LENGTH+1];
 //Init result
 func.Intepreted=false;
 if(!action&&result)
   func.procedure=false;
 else
   func.procedure=true;
   
 func.Pos=-1;
 //Jump to identifier
 JumpOverSpace();
 //Read out identifier 
 if(!GetIdentifier(func.Name,true)) return false;
 if(!ValidName(func.Name)) return false; 
 //Read out parameter's
 if(!action)
 {
  JumpOverSpace();
  if(!ProcessParams(func)) return false;
 }
 else
 {
  func.NumParams=0;
  func.Params=NULL;
 }
 //Get result type
 if(result&&!action)
 {  
  JumpOverSpace();
  if(!GetIdentifier(Temp,true)) return false;
  if(!ENScriptTypeClass::FindType(Temp,&type))
  {
   AddError(EN_COMPILE_ERROR3);
   return false;
  }
  func.res=type.pos;
 }
 //Find body
 JumpOverSpace(); 
 if(!GetBody(func.range)) return false;
 //Add procedure 
 Funcs=(ENFUNCPROC*)AddToAddr(NumFuncs,sizeof(ENFUNCPROC),Funcs,&func); 
 //Finished
 return true; 
}

ENbool ENScriptInterpret::ProcessParams(ENFUNCPROC &func)
{
 //Vars
 ENuint a;
 ENVAR param;
 ENScriptTypeClass::ENScriptDataType type;
 char Temp[EN_MAX_NAME_LENGTH+1];
 //Check if there is a clip
 if(*Pointer!='(')
 {
  AddError(EN_COMPILE_ERROR8);
  return false;
 }
 Pointer++;
 //Init result
 func.NumParams=0;
 func.Params=NULL; 
 //Read out all parameters
 JumpOverSpace();
 while(*Pointer!=')'&&*Pointer!=0)
 {
  //Get type  
  if(!GetIdentifier(Temp,true)) return false;
  if(!ENScriptTypeClass::FindType(Temp,&type))
  {
   AddError(EN_COMPILE_ERROR3);
   return false;
  }
  param.type=type.pos;
  //Set parameter position  
  if(!func.NumParams)
	param.addr=-(ENint)type.Size;
  else
	param.addr=func.Params[func.NumParams-1].addr-(ENint)type.Size;
  //Get name of variable
  JumpOverSpace();
  if(!GetIdentifier(param.Name,true)) return false;
  //Check name  
  if(!ValidName(param.Name)) return false;
  for(a=0;a<func.NumParams;a++)
	if(strcmpi(func.Params[a].Name,param.Name)==0)
    {
     AddError(EN_COMPILE_ERROR4);
     return false;
	}
  //Jump to next
  JumpOverSpace();
  if(*Pointer==',')
  {
   Pointer++;
   JumpOverSpace();
  }
  else
    if(*Pointer!=')')
	{
	 AddError(EN_COMPILE_ERROR8);
     return false;
	}  
  //Set local flag true
  param.Local=true;
  //Add parameter
  func.Params=(ENVAR*)AddToAddr(func.NumParams,sizeof(ENVAR),func.Params,&param);
 } 
 //Jump over clip
 Pointer++;
 //Finished
 return true;
}

ENbool ENScriptInterpret::ProcessArgument(char *Text,ENVAR *Vars,ENuint NumVars,
                          ENuint ind,ENARG &arg)
{ 
 //Vars
 ENint tmpaddr,ConstInd;
 ENVAR var; 
 ENFUNCPROC func;
 ENScriptExtern::ENREGEXTERNFUNCTION exfunc;

 //Check if it is a adress 
 if(IsProcAddr(Text,tmpaddr))
 {  
  arg.addr=code->AddConst((ENubyte*)&tmpaddr,sizeof(ENint));
  arg.SrcType=ENCONST;
  arg.type=ENPROCEDUREVAR;    
  return true;
 }
 //Check if it is a variable  
 if(FindVar(Text,Vars,NumVars,var,ind))
 {
  arg.type=var.type;  
  arg.addr=var.addr;
  arg.IsPointer=var.Ptr;
  if(var.Ptr)
    arg.Pos=var.Pos;
  
  if(var.Local)
 	arg.SrcType=ENLOCAL;
  else
	arg.SrcType=ENGLOBAL; 

  return true;
 }
 //Check if it is an intern call  
 if(FindFunc(Text,func))
 {
  arg.func=func;
  arg.SrcType=ENINTERNCALL;
  arg.type=func.res;
  return true;
 }
 //Check if it is an extern call  
 if(ENScriptExtern::FindExtFunc(Text,&exfunc))
 {
  arg.exfunc=exfunc;
  arg.SrcType=ENEXTERNCALL;
  arg.type=exfunc.res;
  return true;
 }
 //Check if it is a constant integer 
 if(IsInteger(Text,ConstInd))
 {
  ENint _int;  
  if(ConstInd==-1)
    _int=atoi(Text);
  else
	_int=ENScriptExtern::GetConstValue(ConstInd);
  arg.addr=code->AddConst((ENubyte*)&_int,sizeof(ENint));
  arg.SrcType=ENCONST;
  arg.type=ENINT;  
  return true;
 }
 //Check if it is a constant float
 if(IsFloat(Text))
 {
  ENfloat _float;
  _float=(ENfloat)atof(Text);
  arg.addr=code->AddConst((ENubyte*)&_float,sizeof(ENfloat));
  arg.SrcType=ENCONST;
  arg.type=ENFLOAT;  
  return true;
 } 
 //Check if it is a constant string 
 char _string[1024];
 if(IsString(Text,_string))
 {
  arg.addr=code->AddConst((ENubyte*)_string,1024);    
  arg.SrcType=ENCONST;
  arg.type=ENSTRING;
  return true;
 } 
 //Type was unknown 
 AddError(EN_COMPILE_ERROR14); 
 arg.addr=0;
 return false;
}

ENbool ENScriptInterpret::ProcessExit()
{
 //Jump over space
 JumpOverSpace();
 //Check for ';'
 if(*Pointer!=';')
 {
  AddError(EN_COMPILE_ERROR15);
  return false;
 }
 //Jump over ';'
 Pointer++;
 //Finished 
 code->AddCommand(ENScriptCode::ENSCRIPTEXIT,0,0);
 return true;
}

ENbool ENScriptInterpret::ProcessReturn(ENVAR *vars,ENuint NumVars,ENuint ind)
{
 //Vars
 ENint   restype;
 ENScriptRange range;
 //Beginning
 range.start=Pointer;
 //Find end
 if(!JumpOverCommand()) return false;
 range.end=Pointer-1;
 //Process expression
 if(!ProcessExpression(range,vars,NumVars,restype,ind)) return false;
 //Conversion
 if(Funcs[ind].res!=restype)
   if(!ProcessConversion(restype,Funcs[ind].res)) return false;
 //Pop result from stack to register eax
 code->AddCommand(ENScriptCode::ENPOPRESULT,ENScriptTypeClass::GetTypeSize(Funcs[ind].res),0);
 //End function
 EndCurrentFunc(ind);
 return true;
}

ENbool ENScriptInterpret::ProcessConversion(ENint from,ENint to)
{
 ENScriptTypeClass::ENOPERATOR func; 
 //Find conversion
 if(!ENScriptTypeClass::FindOperator(to,from,"=",&func))
 {
  AddError(EN_COMPILE_ERROR16);
  return false;
 }
 //Finished
 code->AddCommand(ENScriptCode::ENOPCOM,func.Pos,0);
 return true;
}

ENbool ENScriptInterpret::ProcessWait(ENVAR *vars,ENuint NumVars,ENuint ind,ENbool RealTime)
{
 //Vars
 ENScriptRange range;
 ENint   restype;
 //Beginning
 range.start=Pointer;
 //Find end
 if(!JumpOverCommand()) return false;
 range.end=Pointer-1;
 
 if(!ProcessExpression(range,vars,NumVars,restype,ind)) return false;
 //Conversion 
 if(ENINT!=restype)
   if(!ProcessConversion(restype,ENINT))
   {  
	AddError(EN_COMPILE_ERROR18);
    return false;
   }
 //Pop result from stack to the C Register
 code->AddCommand(ENScriptCode::ENPOPTIME,0,0);
 //Add Wait command
 if(RealTime)
   code->AddCommand(ENScriptCode::ENWAITT,0,0);
 else
   code->AddCommand(ENScriptCode::ENWAIT,0,0);
 //Finished
 return true;
}

ENbool ENScriptInterpret::ProcessExpression(ENScriptRange range,ENVAR *vars,ENuint NumVars,
						                    ENint &type,ENuint ind)
{
 //Vars
 char *SavePointer=Pointer;
 char Argument[4096],TempOp[3]; 
 char **_Ops=NULL;
 ENARG *Args=NULL,arg;
 ENuint a,nop=0,narg=0;
 //--------Read out the expression
 Pointer=range.start; 
 while(Pointer<=range.end)
 {
  //Jump over space  	 
  JumpOverSpace();
  if(Pointer>range.end) break;
  //Get argument    
  if(*Pointer!='(')
  {   
   GetArgument(Argument);
   if(!ProcessArgument(Argument,vars,NumVars,ind,arg)) return false;   
   if(arg.SrcType==ENINTERNCALL||arg.SrcType==ENEXTERNCALL)//If it is a call
   {	
	Pointer++;
    if(!GetExpression(arg.Expr)) return false;	
   }
  }
  else
  {
   Pointer++;
   if(!GetExpression(arg.Expr)) return false;
   arg.SrcType=ENEXPRESSION;
   arg.type=type;   
  }  
  //Add argument  
  Args=(ENARG*)AddToAddr(narg,sizeof(ENARG),Args,&arg);
  //Jump over space  
  JumpOverSpace();
  //Check of end
  if(Pointer>=range.end) break;
  //Get operator  
  if(!GetOperator(TempOp)) return false;
  //Add operator
  nop++;
  _Ops=(char**)ENReallocMemory(_Ops,sizeof(char*)*nop);
  _Ops[nop-1]=(char*)ENAllocMemory(sizeof(char)*3);
  strcpy(_Ops[nop-1],TempOp);
 }
 //--------Check arguments and operators
 //Check number 
 if(narg<1||narg-1!=nop)
 { 
  AddError(EN_COMPILE_ERROR10);
  return false;
 }
 //---------Solve expression
 ENScriptExpTree *exptree=new ENScriptExpTree(code);  
 if(!exptree->SolveExpTree(vars,NumVars,_Ops,Args,nop,narg,type,ind,this))
 {
  for(a=0;a<nop;a++)
	ENFreeMemory(_Ops[a]);

  ENFreeMemory(Args);
  ENFreeMemory(_Ops);  
  return false;
 }
 delete exptree;
 //----------Free space
 for(a=0;a<nop;a++)
   ENFreeMemory(_Ops[a]);

 ENFreeMemory(Args);
 ENFreeMemory(_Ops);

 Pointer=SavePointer;
 return true;
}

ENbool ENScriptInterpret::ProcessParamsCall(ENuint ind,ENVAR *Vars,ENuint NumVars,
											ENuint NumParam,ENVAR *Param)
{
 //Vars
 ENint restype;
 //Jump over space
 JumpOverSpace();
 //Check clip  
 if(*Pointer!='(') 
 {  
  AddError(EN_COMPILE_ERROR18);  
  return false;
 }
  Pointer++; 
 //GO trough all parameters
 for(ENuint a=0;a<NumParam;a++)
 {
  ENScriptRange range;
  range.start=Pointer;
  //Search the end
  if((ENint)a<(ENint)(NumParam-1))
  {
   if(!EndOfParameter()) return false;
  }
  else  
  {
   if(!EndOfParameter2()) return false;
  }  
  //Interpret parameter    
  range.end=Pointer;  
  if(!ProcessExpression(range,Vars,NumVars,restype,ind)) return false;
  //Convert  
  if(Param[a].type!=restype)
    if(!ProcessConversion(restype,Param[a].type)) return false;

  Pointer+=2;  
 } 
 //If no parametes
 if(!NumParam)
 {
  //Jump over space
  JumpOverSpace();
  //Check clip
  if(*Pointer!=')') 
  {
   AddError(EN_COMPILE_ERROR18);
   return false;
  }
   Pointer++; 
 }
 //Everything works
 return true;
}

ENbool ENScriptInterpret::EndOfParameter()
{
 //Vars
 ENuint a,len,c=0;
 ENbool OK=true;
 //Find out length
 len=strlen(Pointer); 
 for(a=0;a<len;a++)
 {
  if(Pointer[a]=='(') c++;
  if(Pointer[a]==')') c--;
  if(Pointer[a]=='"') OK=!OK;
  if(OK&&c==0&&Pointer[a]==',') {Pointer+=a-1;return true;}
 }
 //No end where found
 AddError(EN_COMPILE_ERROR18);
 return false;
}

ENbool ENScriptInterpret::EndOfParameter2()
{
 //Vars
 ENuint a,len,b=1;
 ENbool OK=true;
 //Find out length 
 len=strlen(Pointer);
 for(a=0;a<len;a++)
 {
  if(Pointer[a]=='"') OK=!OK;
  if(OK&&Pointer[a]=='(') b++;
  if(OK&&Pointer[a]==')') b--;
  if(b==0) {Pointer+=a-1;return true;}
 }
 //No end where found
 AddError(EN_COMPILE_ERROR18);
 return false;
}

ENbool ENScriptInterpret::ProcessExternCall(ENuint ind,ENScriptExtern::ENREGEXTERNFUNCTION func,
											ENVAR *Vars,ENuint NumVars)
{
 //Interpret parameter
 if(!ProcessParamsCall(ind,Vars,NumVars,func.NumParams,func.Params))
   return false;
 //Add call command's
 code->AddCommand(ENScriptCode::ENSCRIPTEXTERNCALL,func.Pos,0);
 //Stack minus all params
 code->AddCommand(ENScriptCode::ENADDSTACK,-(ENint)VarsSize(func.Params,func.NumParams),0);
 //Everything works
 return true; 
}

ENbool ENScriptInterpret::ProcessInternCall(ENuint ind,ENFUNCPROC func,ENVAR *Vars,
					                        ENuint NumVars)
{ 
 //Check, if function or procedure is already set
 if(func.Pos<0)
 {
  AddError(EN_COMPILE_ERROR22);
  return false;
 }
 //Interpret parameter 
 if(!ProcessParamsCall(ind,Vars,NumVars,func.NumParams,func.Params)) return false;
 //Add save current pos command
 code->AddCommand(ENScriptCode::ENSAVEPOS,0,0);
 //Add call command's
 code->AddCommand(ENScriptCode::ENSCRIPTJUMP,func.Pos,0);
 //Everything works
 return true;
}

ENbool ENScriptInterpret::ProcessBlock(ENuint ind,ENuint &NumVars,ENVAR **Vars)
{
 //Vars
 ENVAR var;
 ENFUNCPROC func;
 ENScriptExtern::ENREGEXTERNFUNCTION exfunc;
 char *SavePointer=Pointer;
 char Temp[EN_MAX_NAME_LENGTH];
 ENKEYWORDS command;
 //GO
 Pointer=Funcs[ind].range.start;
 while(Pointer<Funcs[ind].range.end)
 {  
  //Jump over space
  JumpOverSpace();
  //Get keyword  
  if(!GetIdentifier(Temp,true)) return false;
  //Check for end
  if(Pointer>=Funcs[ind].range.end) break;
  //Look if it is a command
  command=CommandID(Temp);
  if(command==ENKUNKNOWN)
  {   
   //Look if it is an assignment
   if(FindVar(Temp,*Vars,NumVars,var,ind))
   {
	if(!ProcessAssignment(ind,var,*Vars,NumVars)) return false;
   }
   else
   {
	//Look if it is a intern call	
    if(FindFunc(Temp,func))
    {	 
	 if(!ProcessInternCall(ind,func,*Vars,NumVars)) return false;
	 //Go to the end
	 JumpOverSpace();
	 if(*Pointer!=';')
	 {	  
	  AddError(EN_COMPILE_ERROR20);
	  return false;
	 }

	 Pointer++;
    }
    else
	{
     if(ENScriptExtern::FindExtFunc(Temp,&exfunc))
     {	  
	  if(!ProcessExternCall(ind,exfunc,*Vars,NumVars)) return false;
	  //Go to the end
	  JumpOverSpace();
	  if(*Pointer!=';')
	  {	  
	   AddError(EN_COMPILE_ERROR20);
	   return false;
	  }
  	  Pointer++;
     }
     else
	 {		  
	  AddError(EN_COMPILE_ERROR1);
      return false;
	 }
	}
   }
  }
  else
  {   
   //Interpret command
   if(!ProcessCommand(ind,command,Vars,NumVars)) return false;
  }
 }

 Pointer=SavePointer;
 return true;
}

ENbool ENScriptInterpret::ProcessCommand(ENuint ind,ENKEYWORDS command,
						                 ENVAR **vars,ENuint &NumVars)
{
 //Vars
 ENVAR var;
 //Interpret command
 switch(command)
 {
  case ENKVAR:	
	if(!ProcessVar(var)) return false;	
    //Set position of the variable   	
    var.addr=VarPos(NumVars,vars[0]);	
    //Set local flag true
    var.Local=true;
    //Add variable	
    NumVars++;
    vars[0]=(ENVAR*)ENReallocMemory(vars[0],NumVars*sizeof(ENVAR));
    vars[0][NumVars-1]=var;	
	//Add stack command    	
	code->AddCommand(ENScriptCode::ENADDSTACK,ENScriptTypeClass::GetTypeSize(var.type),0);
  break;
  case ENKEXIT:
	if(!ProcessExit()) return false;
  break;
  case ENKRETURN:
	if(!ProcessReturn(*vars,NumVars,ind)) return false;
  break;
  case ENKIF:
	if(!ProcessIf(vars,NumVars,ind)) return false;
  break;  
  case ENKWAIT:
	if(!ProcessWait(*vars,NumVars,ind,false)) return false;
  break;
  case ENKWAITT:
	if(!ProcessWait(*vars,NumVars,ind,true)) return false;
  break;
  case ENKWHILE:	
	if(!ProcessWhile(vars,NumVars,ind)) return false;
  break;
  default:
	AddError(EN_COMPILE_ERROR21);
	return false;
 };

 return true;
}

ENbool ENScriptInterpret::ProcessAssignment(ENuint ind,ENVAR var,ENVAR *vars,ENuint NumVars)
{
 //Vars
 ENint restype;
 ENScriptRange range;
 //Jump over space
 JumpOverSpace();
 //Look for operator 
 if(*Pointer!='=')
 {
  AddError(EN_COMPILE_ERROR19);
  return false;
 } 
 Pointer++;
 //Interpret argument   
 range.start=Pointer;
 if(!JumpOverCommand()) return false;
 range.end=Pointer-1;
 if(!ProcessExpression(range,vars,NumVars,restype,ind)) return false;
 //Convertion
 if(var.type!=restype)
   if(!ProcessConversion(restype,var.type)) return false; 
 //Pop stack element to variable
 ENint ts;
 ts=ENScriptTypeClass::GetTypeSize(var.type);

 if(var.Ptr)
 {
  //Push address onto stack
  if(var.Local)
	code->AddCommand(ENScriptCode::ENPUSHLOCAL,var.addr,sizeof(ENint));
  else
	code->AddCommand(ENScriptCode::ENPUSH,var.addr,sizeof(ENint));
  //Set address
  code->AddCommand(ENScriptCode::ENSETADDR,0,0);
  //Now pop
  code->AddCommand(ENScriptCode::ENPOPBYADDR,var.Pos,ts);
 }
 else
 {
  if(var.Local)
    code->AddCommand(ENScriptCode::ENPOPLOCAL,var.addr,ts);
  else
    code->AddCommand(ENScriptCode::ENPOP,var.addr,ts);
 }
 return true;
}

ENbool ENScriptInterpret::ProcessIf(ENVAR **vars,ENuint &NumVars,ENuint ind)
{
 //VARS
 ENuint jmp1,jmp2;
 ENScriptRange range,tmp;
 ENint restype;
 ENScriptRange b1,b2;
 ENbool SndExists;
 char Temp[EN_MAX_NAME_LENGTH];
 //Jump to expression
 JumpOverSpace();
 //Look first braket  
 if(*Pointer!='(')
 {
  AddError(EN_COMPILE_ERROR23);
  return false;
 }

 Pointer++; 
 range.start=Pointer;
 //Look for closing braket
 if(!JumpToBraket())
 {
  AddError(EN_COMPILE_ERROR23);
  return false;
 } 
 range.end=Pointer;
 //Solve logic expression
 if(!ProcessExpression(range,*vars,NumVars,restype,ind)) return false;
 if(restype!=ENINT)
 {
  AddError(EN_COMPILE_ERROR23);
  return false;
 } 
 Pointer++;
 //Pop result to BReg
 code->AddCommand(ENScriptCode::ENPOPLOGIC,0,0);
 //Find first body  
 JumpOverSpace(); 
 if(!GetBody(b1)) return false; 
 //Look for second body
 SndExists=false;
 JumpOverSpace(); 
 if(!PointerEnd())
   if(GetIdentifier(Temp,false))
   {    
    if(strcmpi(Temp,"else")==0)
	{	  
	 SndExists=true;
	 JumpOverSpace();
	 if(!GetBody(b2)) return false;
	}
	else
	  Pointer-=strlen(Temp);	
   }

 //Add fist jump 
 jmp1=code->GetNumCommands();
 code->AddCommand(ENScriptCode::ENSCRIPTJUMPFALSE,0,0);
 //Pharse fist block 
 tmp=Funcs[ind].range;
 Funcs[ind].range=b1;
 if(!ProcessBlock(ind,NumVars,vars)) return false;
 Funcs[ind].range=tmp;
 //Add second block if needed jump
 if(SndExists)
 {
  jmp2=code->GetNumCommands();
  code->AddCommand(ENScriptCode::ENSCRIPTJUMP,0,0);
 }
 //Set destination of fist jump
 code->SetCommandParamA(jmp1,code->GetNumCommands());
 //If second block exists 
 if(SndExists)
 {
  //Pharse second block
  tmp=Funcs[ind].range;
  Funcs[ind].range=b2;
  if(!ProcessBlock(ind,NumVars,vars)) return false;
  Funcs[ind].range=tmp;
  //Set destination of second jump
  code->SetCommandParamA(jmp2,code->GetNumCommands());
  //Correct Pointer
  Pointer=b2.end+1;
 }
 //Finished
 return true;
}

ENbool ENScriptInterpret::ProcessWhile(ENVAR **vars,ENuint &NumVars,ENuint ind)
{
 //VARS
 ENuint jmp1,jmp2;
 ENScriptRange range,tmp,b;
 ENint restype;
 //Save first jump address
 jmp1=code->GetNumCommands();
 //Jump to expression
 JumpOverSpace();
 //Look first braket  
 if(*Pointer!='(')
 {
  AddError(EN_COMPILE_ERROR23);
  return false;
 }

 Pointer++; 
 range.start=Pointer;
 //Look for closing braket
 if(!JumpToBraket())
 {
  AddError(EN_COMPILE_ERROR23);
  return false;
 } 
 range.end=Pointer;
 //Solve logic expression
 if(!ProcessExpression(range,*vars,NumVars,restype,ind)) return false;
 if(restype!=ENINT)
 {
  AddError(EN_COMPILE_ERROR23);
  return false;
 } 
 Pointer++;
 //Pop result to BReg
 code->AddCommand(ENScriptCode::ENPOPLOGIC,0,0);
 //Find first body  
 JumpOverSpace(); 
 if(!GetBody(b)) return false; 
 //Add fist jump  
 jmp2=code->GetNumCommands();
 code->AddCommand(ENScriptCode::ENSCRIPTJUMPFALSE,0,0);
 //Pharse block 
 tmp=Funcs[ind].range;
 Funcs[ind].range=b;
 if(!ProcessBlock(ind,NumVars,vars)) return false;
 Funcs[ind].range=tmp;
 //Add jump back to beginning
 code->AddCommand(ENScriptCode::ENSCRIPTJUMP,jmp1,0);
 //Set destination of fist jump
 code->SetCommandParamA(jmp2,code->GetNumCommands());
 //Finished
 return true;
}

ENbool ENScriptInterpret::CompileFunction(ENuint ind)
{ 
 //First check
 if(Funcs[ind].Intepreted) return true;
 //Vars
 ENuint NumVars=0;
 ENVAR *Vars=NULL;  
 //Set position of the function  
 Funcs[ind].Pos=code->GetNumCommands();
 //Save stack
 code->AddCommand(ENScriptCode::ENSAVESTACK,0,0);
 //GO  
 if(!ProcessBlock(ind,NumVars,&Vars)) return false; 
 //Release variables
 ENFreeMemory(Vars);
 //End function
 EndCurrentFunc(ind);
 //Finished
 Funcs[ind].Intepreted=true; 
 return true;
}

void ENScriptInterpret::EndCurrentFunc(ENint ind)
{
 //Reset stack
 code->AddCommand(ENScriptCode::ENRESETSTACK,0,0);
 //Stack minus all params
 code->AddCommand(ENScriptCode::ENADDSTACK,-(ENint)VarsSize(Funcs[ind].Params,Funcs[ind].NumParams),0);
 //GO back to last position
 code->AddCommand(ENScriptCode::ENRESETPOS,0,0);
}

void ENScriptInterpret::AddError(char *Error)
{
 char Buffer[1024];
 ENuint line=CountChars(10)+1;
 sprintf(Buffer,"%s(%i) compile error: %s\n",curFileName,line,Error);
 strcat(CompileLog,Buffer); 
}

void ENScriptInterpret::AddMsg(char *msg)
{
 char Buffer[1024];
 sprintf(Buffer,"%s: %s\n",curFileName,msg);
 strcat(CompileLog,Buffer); 
}

ENuint ENScriptInterpret::GetNumCommmands()
{
 return NUMCOMMANDS;
}

void   ENScriptInterpret::GetCommands(char **coms)
{
 for(ENuint a=0;a<NUMCOMMANDS;a++)
  strcpy(coms[a],COMMANDS[a]);
}
