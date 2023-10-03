#include "ENScriptExtern.h"
#include "ENScriptParser.h"
#include "ENSysFuncs.h"

///////////////STATIC VARIABLES///////////////////////
ENuint ENScriptExtern::NumExtVars=0;
ENuint ENScriptExtern::NumExtFuncs=0;
ENuint ENScriptExtern::NumConst=0;

ENScriptExtern::ENREGEXTERNVAR      *ENScriptExtern::ExtVars=NULL;
ENScriptExtern::ENREGEXTERNFUNCTION *ENScriptExtern::ExtFuncs=NULL;
ENScriptExtern::ENEXTCONST          *ENScriptExtern::ExtConst=NULL;

ENint ENScriptExtern::GetExtVarsSize()
{
 if(!NumExtVars)
   return 0;
 else
 {  
  ENScriptTypeClass::ENScriptDataType type;  
  ENScriptTypeClass::GetType(ExtVars[NumExtVars-1].type,&type);
  return ExtVars[NumExtVars-1].Pos+type.Size;
 }
}

ENint ENScriptExtern::GetConstValue(ENint ConstInd)
{
 return ExtConst[ConstInd].Const;
}

ENint ENScriptExtern::GetExtVarPos(char *Name)
{
 ENREGEXTERNVAR var;
 if(!FindExtVar(Name,&var))
   return -1;
 else
   return var.Pos;
}

ENbool ENScriptExtern::FindExtVar(char *Name,ENREGEXTERNVAR *var)
{ 
 for(ENuint a=0;a<NumExtVars;a++)
   if(strcmpi(Name,ExtVars[a].Name)==0)
   {
	if(var) *var=ExtVars[a];
    return true;
   }

 return false;
}

ENbool ENScriptExtern::FindExtConst(char *Name,ENEXTCONST *vconst)
{ 
 for(ENuint a=0;a<NumConst;a++)
   if(strcmpi(Name,ExtConst[a].Name)==0)
   {
	if(vconst) *vconst=ExtConst[a];
    return true;
   }

 return false;
}

void ENScriptExtern::RunExtFunc(ENint ind,ENubyte *Data,void *res)
{
 ExtFuncs[ind].Function(Data,res);
}

ENbool ENScriptExtern::FindExtFunc(char *Name,ENREGEXTERNFUNCTION *func)
{
 for(ENuint a=0;a<NumExtFuncs;a++)
   if(strcmpi(Name,ExtFuncs[a].Name)==0)
   {
	if(func) *func=ExtFuncs[a];
    return true;
   }

 return false; 
}

ENbool ENScriptExtern::SetExtFunc(char *Name,ENSCRIPTEXTERNFUNCTION exfunc)
{
 //Set extern function
 for(ENuint a=0;a<NumExtFuncs;a++)
   if(strcmpi(Name,ExtFuncs[a].Name)==0)
   {
	ExtFuncs[a].Function=exfunc;
    return true;
   }
 //Failed
 return false;
}

ENbool ENScriptExtern::AddExtVar(ENREGEXTERNVAR var)
{
 ENScriptTypeClass::ENScriptDataType type;
 //Check if name is unique
 if(FindExtVar(var.Name,NULL)) return false;  
 //Set position of var 
 if(NumExtVars)
 {
  if(!ENScriptTypeClass::GetType(ExtVars[NumExtVars-1].type,&type)) return false;
  var.Pos=type.Size+ExtVars[NumExtVars-1].Pos;
 }
 else
   var.Pos=0;
 //Add new extern variable
 NumExtVars++;
 ExtVars=(ENREGEXTERNVAR*)ENReallocMemory(ExtVars,
	               NumExtVars*sizeof(ENREGEXTERNVAR));
 ExtVars[NumExtVars-1]=var; 
 //Finished
 return true;
}

ENbool ENScriptExtern::AddExtFunc(ENREGEXTERNFUNCTION func)
{
 //Init func
 func.Pos=NumExtFuncs;
 //Check if name is unique
 if(FindExtFunc(func.Name,NULL)) return false;  
 //Add new extern function
 NumExtFuncs++;
 ExtFuncs=(ENREGEXTERNFUNCTION*)ENReallocMemory(ExtFuncs,
	               NumExtFuncs*sizeof(ENREGEXTERNFUNCTION));
 ExtFuncs[NumExtFuncs-1]=func;
 //Finished
 return true;
}

ENbool ENScriptExtern::AddExtConst(ENEXTCONST vconst)
{
 //Check if name is unique	
 if(FindExtConst(vconst.Name,NULL)) return false;  
 //Set pos
 vconst.Pos=NumConst;
 //Add new extern constant 
 NumConst++; 
 ExtConst=(ENEXTCONST*)ENReallocMemory(ExtConst,
	               NumConst*sizeof(ENEXTCONST));
 ExtConst[NumConst-1]=vconst;
 //Finished
 return true;
}

ENbool ENScriptExtern::InterpretExternVar(ENTEXTSTR ident,void *parser,ENScriptTypeClass::ENScriptDataType type)
{
 ENREGEXTERNVAR   var;
 ENScriptParser *ph=(ENScriptParser*)parser; 
 //Config var
 var.type=type.pos;
 //Get name
 ph->JumpOverSpace();
 if(!ph->GetIdentifier(var.Name)) return false;
 //Add extern var 
 return AddExtVar(var);
}

ENbool ENScriptExtern::LoadExtern(char *Src)
{
 //VARS
 ENScriptTypeClass::ENScriptDataType type;
 ENScriptParser *parser=new ENScriptParser();
 //Load text 
 if(!parser->SetSource(Src)) return false;
 //Interpret config  
 while(!parser->PointerEnd())
 {
  ENTEXTSTR ident;
  parser->JumpOverSpace();
  if(parser->PointerEnd()) break;
  if(!parser->GetIdentifier(ident.str)) return false;  
  if(!InterpretExternFunc(ident,parser))
	if(!InterpretConst(ident,parser))
	{
     if(!ENScriptTypeClass::FindType(ident.str,&type)) return false;
     if(!InterpretExternVar(ident,parser,type)) return false;
	}    
 }
 //Finished
 delete parser;
 return true;
}

ENbool ENScriptExtern::ProcessParams(void *parser,ENREGEXTERNFUNCTION &func)
{
 //Vars
 ENuint a;
 ENVAR param;
 ENScriptTypeClass::ENScriptDataType type;
 char Temp[EN_MAX_NAME_LENGTH+1];
 ENScriptParser *ph=(ENScriptParser*)parser;
 //Check if there is a clip 
 if(ph->GetCurrentChar()!='(')
   return false;

 ph->IncPointer(1);
 //Init result
 func.NumParams=0;
 func.Params=NULL; 
 //Read out all parameters
 ph->JumpOverSpace();
 while(ph->GetCurrentChar()!=')'&&ph->GetCurrentChar()!=0)
 {
  //Get type  
  if(!ph->GetIdentifier(Temp)) return false;
  if(!ENScriptTypeClass::FindType(Temp,&type))
    return false;
 
  param.type=type.pos;
  //Set parameter position  
  if(!func.NumParams)
	param.Pos=-(ENint)type.Size;
  else
	param.Pos=func.Params[func.NumParams-1].Pos-(ENint)type.Size;
  //Get name of variable
  ph->JumpOverSpace();
  if(!ph->GetIdentifier(param.Name)) return false;
  //Check name  
  if(!ph->ValidName(param.Name,"")) return false;
  for(a=0;a<func.NumParams;a++)
	if(strcmpi(func.Params[a].Name,param.Name)==0)
      return false;
  //Jump to next
  ph->JumpOverSpace();
  if(ph->GetCurrentChar()==',')
  {
   ph->IncPointer(1);
   ph->JumpOverSpace();
  }
  else
    if(ph->GetCurrentChar()!=')')
      return false;
  //Set local flag true
  param.Local=true;
  //Add parameter
  func.NumParams++;
  func.Params=(ENVAR*)ENReallocMemory(func.Params,func.NumParams*sizeof(ENVAR));
  func.Params[func.NumParams-1]=param;
 } 
 //Jump over clip
 ph->IncPointer(1);
 //Finished
 return true;
}

ENbool ENScriptExtern::InterpretConst(ENTEXTSTR ident,void *parser)
{
 ENEXTCONST vconst;
 char Temp[EN_MAX_NAME_LENGTH+1];
 ENScriptParser *ph=(ENScriptParser*)parser;
 //Check for key word
 if(strcmpi(ident.str,"const")!=0) return false;
 //Jump to name
 ph->JumpOverSpace();
 //Get identifier
 if(!ph->GetIdentifier(vconst.Name)) return false;
 //Jump to value
 ph->JumpOverSpace();
 //Get value
 if(!ph->GetIdentifier(Temp)) return false;
 vconst.Const=atoi(Temp);
 //Add constant
 return AddExtConst(vconst);
}

ENbool ENScriptExtern::InterpretExternFunc(ENTEXTSTR ident,void *parser)
{
 ENREGEXTERNFUNCTION func;
 ENScriptTypeClass::ENScriptDataType type;
 char Temp[EN_MAX_NAME_LENGTH+1];
 ENScriptParser *ph=(ENScriptParser*)parser;
 //Init function
 func.Function=NULL;
 //Check for function
 if(strcmpi(ident.str,"procedure")==0)
   func.procedure=true;  
 else
   if(strcmpi(ident.str,"function")==0)
	 func.procedure=false;
   else
	 return false;
 //Get name
 ph->JumpOverSpace(); 
 if(!ph->GetIdentifier(func.Name)) return false;
 //Check name
 if(!ph->ValidName(func.Name,"")) return false;
 if(FindExtFunc(func.Name,NULL)) return false;
 //Get all params
 ph->JumpOverSpace();
 if(!ProcessParams(parser,func)) return false;
 //Get result if it is a function
 if(!func.procedure)
 {
  ph->JumpOverSpace();
 }
 //Get result type
 if(!func.procedure)
 {  
  ph->JumpOverSpace();
  if(!ph->GetIdentifier(Temp)) return false;
  if(!ENScriptTypeClass::FindType(Temp,&type))
    return false;

  func.res=type.pos;
 }
 //Finished  
 return AddExtFunc(func);
}