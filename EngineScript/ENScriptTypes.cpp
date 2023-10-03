#include <iostream.h>
#include "ENScriptTypes.h"
#include "ENScriptParser.h"
#include "ENScriptCode.h"
#include "ENOperatorFuncs.h"

//////////STATIC VARIABLES////////////////
//Types
ENuint                               ENScriptTypeClass::NumDataTypes=0;
ENScriptTypeClass::ENScriptDataType *ENScriptTypeClass::DataTypes=NULL;
//Operators
ENuint                               ENScriptTypeClass::NumOperators=0;
ENScriptTypeClass::ENOPERATOR       *ENScriptTypeClass::Operators=NULL;   

void ENScriptTypeClass::InitBasicTypes()
{
 static ENbool InitBT=false;
 if(!InitBT)
 {
  InitBT=true;
  //Init basic types
  ENScriptDataType type;
  type.basic=true;
  type.Members=NULL;
  type.numMems=0; 
  type.pointer=false;
  //Integer
  strcpy(type.Name.str,"int");
  type.Size=sizeof(ENint); 
  AddType(type);
  //Float
  strcpy(type.Name.str,"float");
  type.Size=sizeof(ENfloat); 
  AddType(type);
  //Procedure var
  strcpy(type.Name.str,"procedurevar");
  type.Size=sizeof(ENint); 
  AddType(type);
  //string
  strcpy(type.Name.str,"string");
  type.Size=1024; 
  AddType(type); 
 }
}

ENuint ENScriptTypeClass::GetNum()
{
 return NumDataTypes;
}

ENbool ENScriptTypeClass::GetType(ENuint ind,ENScriptDataType *type)
{
 if(ind>=NumDataTypes) return false;

 *type=DataTypes[ind];
 return true;
}

ENint ENScriptTypeClass::ModifyType(ENint &type,ENint pos,char *mod)
{
 //VARS
 ENScriptDataType data,ndata;
 char  *nmod,cmod[EN_MAX_NAME_LENGTH];
 //Get type
 if(!GetType(type,&data)) return -1;
 //Get name
 strcpy(cmod,mod); 
 nmod=strchr(mod,'.'); 
 if(nmod)
 {
  cmod[nmod-mod]=0;
  nmod++;   
 } 
 //Modify 
 ENint tres=GetMember(data,cmod,ndata);
 if(tres==-1) return -1;
 //Return modification
 type=ndata.pos;
 if(nmod)
   return ModifyType(type,pos+tres,nmod);
 else
   return pos+tres;
}

ENint ENScriptTypeClass::GetTypeSize(ENint type)
{
 return DataTypes[type].Size;
}

ENbool ENScriptTypeClass::IsTypePtr(ENint type)
{
 return DataTypes[type].pointer;
}

ENint ENScriptTypeClass::GetMember(ENScriptDataType data,char *mod,ENScriptDataType &res)
{
 //VARS
 ENint pos=0;
 //Check
 if(data.basic) return -1;
 if(strcmpi(mod,"space")==0) return -1;
 //Search member
 for(ENuint a=0;a<data.numMems;a++)
 {
  //Get a member
	 if(!GetType(data.Members[a].lnk,&res)) return -1;
  //Found member
  if(strcmpi(data.Members[a].Name.str,mod)==0) return pos;
  //Increase address
  pos+=res.Size;
 }
 //Member doesn't exists
 return -1;
}

ENbool ENScriptTypeClass::FindType(char *Name,ENScriptDataType *type)
{
 for(ENuint a=0;a<NumDataTypes;a++)
   if(strcmpi(Name,DataTypes[a].Name.str)==0)
   {
	if(type) *type=DataTypes[a];
    return true;
   }

 return false;
}

void ENScriptTypeClass::RunOperator(ENint ind,ENubyte *ENStack,ENint &_ENStackPointer)
{
 Operators[ind].OperatorFunc(ENStack,_ENStackPointer);
}

ENbool ENScriptTypeClass::FindOperator(ENint t1,ENint t2,char *Op,ENOPERATOR *func)
{ 
 for(ENuint a=0;a<NumOperators;a++)
   if(Operators[a].t1==t1&&Operators[a].t2==t2&&strcmpi(Op,Operators[a].Op)==0)
   {
	if(func!=NULL) *func=Operators[a];		
	return true;
   }

 return false;
}

ENbool ENScriptTypeClass::AddType(ENScriptDataType type)
{
 if(FindType(type.Name.str,NULL)) return false;

 //Check for pointer
 if(type.pointer)
   type.Size=sizeof(ENint);
 //Set position
 type.pos=NumDataTypes;
 NumDataTypes++;
 DataTypes=(ENScriptDataType*)ENReallocMemory(DataTypes,sizeof(ENScriptDataType)*NumDataTypes);
 DataTypes[NumDataTypes-1]=type;
 return true;
}

ENbool ENScriptTypeClass::AddOperator(char *res,char *t1,char *t2,char *Op,
									  ENSCRIPTOPFUNCTION OpFunc)
{ 
 ENOPERATOR op;
 ENScriptDataType a,b,r;
 //Get types
 if(res==NULL)
   r.pos=0;
 else
   if(!FindType(res,&r)) return false;
 
 if(!FindType(t1,&a)) return false;
 if(!FindType(t2,&b)) return false;
 //Init operator
 strcpy(op.Op,Op);
 op.OperatorFunc=OpFunc;
 op.Pos=NumOperators;
 op.res=r.pos;
 op.t1=a.pos;
 op.t2=b.pos;
 //Check if operator is unique
 if(FindOperator(a.pos,b.pos,Op,NULL)) return false;
 //Add new operator
 NumOperators++;
 Operators=(ENOPERATOR*)ENReallocMemory(Operators,
	               NumOperators*sizeof(ENOPERATOR));
 Operators[NumOperators-1]=op;
 //Finished
 return true;
}

ENbool ENScriptTypeClass::InterpretBasic(void *parser)
{
 ENScriptDataType data; 
 ENTEXTSTR        str;
 ENScriptParser *ph=(ENScriptParser*)parser; 
 //Config data
 data.basic=true;
 //Get name
 ph->JumpOverSpace();
 if(!ph->GetIdentifier(data.Name.str)) return false;
 //Get size
 ph->JumpOverSpace();
 if(!ph->GetIdentifier(str.str)) return false;
 data.Size=atoi(str.str);
 if(!data.Size) return false;
 //Add new data type
 return AddType(data);
}

ENbool ENScriptTypeClass::InterpretPack(void *parser)
{
 ENScriptDataType data,tmp; 
 ENScriptDataMember tmp2;
 ENTEXTSTR        str,typ;
 ENScriptParser *ph=(ENScriptParser*)parser; 
 //Config data
 data.basic=false;
 data.Members=NULL;
 data.numMems=0;
 data.Size=0;
 //Jump over space
 ph->JumpOverSpace();
 //Get type specification
 if(!ph->GetIdentifier(typ.str)) return false;
 if(strcmpi(typ.str,"ref")==0)
   data.pointer=false;
 else
 {
  if(strcmpi(typ.str,"ptr")==0)
	data.pointer=true;
  else
	return false;
 }
 //Get name
 ph->JumpOverSpace();
 if(!ph->GetIdentifier(data.Name.str)) return false;
 //Get elements
 while(!ph->PointerEnd())
 {
  //Get type
  ph->JumpOverSpace();
  if(!ph->GetIdentifier(str.str)) return false;
  if(strcmpi("END",str.str)==0) break;
  //Find type
  if(!FindType(str.str,&tmp)) return false;
  tmp2.lnk=tmp.pos;
  data.Size+=tmp.Size;
  //Get name
  ph->JumpOverSpace();
  if(!ph->GetIdentifier(tmp2.Name.str)) return false;
  //Add member
  data.numMems++;  
  data.Members=(ENScriptDataMember*)ENReallocMemory(data.Members,sizeof(ENScriptDataMember)*data.numMems);
  data.Members[data.numMems-1]=tmp2;

 }
 //Add new data type 
 return AddType(data);
}

ENbool ENScriptTypeClass::InterpretType(ENTEXTSTR ident,void *parser)
{
 if(strcmpi(ident.str,"basic")==0)
   return InterpretBasic(parser);

 if(strcmpi(ident.str,"pack")==0)
   return InterpretPack(parser);

 return false;
}

ENbool ENScriptTypeClass::SetTypes(char *Text)
{
 //VARS
 ENScriptParser *parser=new ENScriptParser();
 //Init basic types
 InitBasicTypes();
 //Set text   
 if(!parser->SetSource(Text)) return false; 
 //Interpret config  
 while(!parser->PointerEnd())
 {
  ENTEXTSTR ident;
  parser->JumpOverSpace();
  if(parser->PointerEnd()) break;
  if(!parser->GetIdentifier(ident.str)) return false;  
  if(!InterpretType(ident,parser)) return false;
 }
 //Finished
 delete parser;
 return true;
}
