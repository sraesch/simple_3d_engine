#include <iostream.h>
#include  "ENScriptExpTree.h"
#include  "ENScriptInterpret.h"

const char *ENScriptExpTree::Operators1[]={"*","/","%","|"};
const char *ENScriptExpTree::Operators2[]={"+","-"};
const char *ENScriptExpTree::Operators3[]={"=","==","!=","<",">","<=",">="};
const char *ENScriptExpTree::Operators4[]={"&","||"};
const ENint ENScriptExpTree::NumOps[]={4,2,7,2};

ENScriptExpTree::ENScriptExpTree(ENScriptCode *code)
{
 this->code=code;
}

ENScriptExpTree::~ENScriptExpTree()
{
}

ENint ENScriptExpTree::OperatorRanking(char *Op)
{
 ENint a;
 //Operator ranking 1
 for(a=0;a<NumOps[0];a++)
   if(strcmpi(Op,Operators1[a])==0) return 1;
 //Operator ranking 2
 for(a=0;a<NumOps[1];a++)
   if(strcmpi(Op,Operators2[a])==0) return 2;
 //Operator ranking 3
 for(a=0;a<NumOps[2];a++)
   if(strcmpi(Op,Operators3[a])==0) return 3;
 //Operator ranking 4
 for(a=0;a<NumOps[3];a++)
   if(strcmpi(Op,Operators4[a])==0) return 4;
 //Nothing found
 return 0;
}

ENbool ENScriptExpTree::PushArgument(ENScriptInterpret::ENARG arg,ENVAR *vars,ENuint NumVars,ENint &type,
					                 ENuint ind,void *Interpret)
{
 ENScriptTypeClass::ENScriptDataType tmp;
 ENScriptInterpret *inter=(ENScriptInterpret*)Interpret;
 //Process case
 switch(arg.SrcType)
 {
  case ENScriptInterpret::ENCONST:
	if(!ENScriptTypeClass::GetType(arg.type,&tmp)) return false;
	code->AddCommand(ENScriptCode::ENPUSHCONST,arg.addr,tmp.Size);
	type=arg.type;
  break;
  case ENScriptInterpret::ENLOCAL:
    if(!ENScriptTypeClass::GetType(arg.type,&tmp)) return false;
	if(!arg.IsPointer)
	  code->AddCommand(ENScriptCode::ENPUSHLOCAL,arg.addr,tmp.Size);
    else
	  code->AddCommand(ENScriptCode::ENPUSHLOCAL,arg.addr,sizeof(ENint));

	type=arg.type;
  break;
  case ENScriptInterpret::ENGLOBAL:
    if(!ENScriptTypeClass::GetType(arg.type,&tmp)) return false;
	if(!arg.IsPointer)
	  code->AddCommand(ENScriptCode::ENPUSH,arg.addr,tmp.Size);
    else
	  code->AddCommand(ENScriptCode::ENPUSH,arg.addr,sizeof(ENint));

	type=arg.type;
  break;  
  case ENScriptInterpret::ENEXPRESSION:
	if(!inter->ProcessExpression(arg.Expr,vars,NumVars,type,ind)) return false;	
  break;
  case ENScriptInterpret::ENINTERNCALL:
	inter->SetNewPointer(arg.Expr.start-1);
    if(!inter->ProcessInternCall(ind,arg.func,vars,NumVars)) return false;
	inter->ResetPointer();

	code->AddCommand(ENScriptCode::ENPUSHRESULT,ENScriptTypeClass::GetTypeSize(arg.func.res),0);
	//Set type
	type=arg.func.res;
  break;
  case ENScriptInterpret::ENEXTERNCALL:	
	inter->SetNewPointer(arg.Expr.start-1);
    if(!inter->ProcessExternCall(ind,arg.exfunc,vars,NumVars))
 	  return false;

	inter->ResetPointer();
	code->AddCommand(ENScriptCode::ENPUSHRESULT,ENScriptTypeClass::GetTypeSize(arg.exfunc.res),0);
	//Set type
	type=arg.exfunc.res;
  break;  
 }; 
 //For pointer
 if((arg.SrcType==ENScriptInterpret::ENLOCAL||arg.SrcType==ENScriptInterpret::ENGLOBAL)&&arg.IsPointer)
 {  
  code->AddCommand(ENScriptCode::ENSETADDR,0,0);
  code->AddCommand(ENScriptCode::ENPUSHBYADDR,arg.Pos,tmp.Size);
 }
 
 return true;
}

void ENScriptExpTree::DeleteExpTree(ENEXPNODE *node)
{
 if(!node->end)
 {
  //Delete left side 
  DeleteExpTree(node->left);
  //Delete right side
  DeleteExpTree(node->right);
 }

 ENFreeMemory(node);
}

ENbool ENScriptExpTree::CreateExpTree(ENEXPNODE *node,ENScriptInterpret::ENARG *Args,char **_Ops,
								    ENuint sop,ENuint eop)
{ 
 //VARS
 ENuint a,r;
 ENint    t;
 //Init node
 node->end=false;
 node->left=(ENEXPNODE*)ENAllocMemory(sizeof(ENEXPNODE));
 node->right=(ENEXPNODE*)ENAllocMemory(sizeof(ENEXPNODE)); 
 //Search operator with highest ranking
 r=sop;
 for(a=sop+1;a<=eop;a++)
 {
  //Ranking of the operator
  t=OperatorRanking(_Ops[a]);  
  if(!t) return false;
  //Check ranking   
  if(t>=OperatorRanking(_Ops[r])) r=a;
 }
 //Fill node
 strcpy(node->Op,_Ops[r]);
 //Check left side
 if(r==sop)
 {   
  node->left->end=true;
  node->left->arg=Args[sop];
  node->left->left=NULL;
  node->left->right=NULL;  
 }
 else
   if(!CreateExpTree(node->left,Args,_Ops,sop,r-1)) return false;
 //Check right side
 if(r==eop)
 {   
  node->right->end=true;
  node->right->arg=Args[eop+1];
  node->right->left=NULL;
  node->right->right=NULL;
 }
 else
   if(!CreateExpTree(node->right,Args,_Ops,r+1,eop)) return false;
 //Finished
 return true;
}

ENbool ENScriptExpTree::ProcessExpTree(ENVAR *vars,ENuint NumVars,ENint &type,
						       ENuint ind,ENEXPNODE *node,void *Interpret)
{
 //VARS
 ENint   t1,t2;
 ENScriptTypeClass::ENOPERATOR      OpFunc;
 //
 if(node->end)
 {//Last node: push argument onto stack
  if(!PushArgument(node->arg,vars,NumVars,type,ind,Interpret))
	return false;
 }
 else
 {//Not last node: Search operator and add command
  if(!ProcessExpTree(vars,NumVars,t1,ind,node->left,Interpret)) return false;
  if(!ProcessExpTree(vars,NumVars,t2,ind,node->right,Interpret)) return false;  
  //Search operator
  if(ENScriptTypeClass::FindOperator(t1,t2,node->Op,&OpFunc))
   {
	type=OpFunc.res;
	//Create command
	code->AddCommand(ENScriptCode::ENOPCOM,OpFunc.Pos,0);
   }
   else
	 return false;
 }

 return true;
}

ENbool ENScriptExpTree::SolveExpTree(ENVAR *vars,ENuint NumVars,char **_Ops,ENScriptInterpret::ENARG *Args,
						ENuint nop,ENuint narg,ENint &type,ENuint ind,void *Interpret)
{ 
 if(nop)
 {
  //Create first node
  ENEXPNODE *node=(ENEXPNODE*)ENAllocMemory(sizeof(ENEXPNODE));
  //Create tree  
  if(!CreateExpTree(node,Args,_Ops,0,nop-1)) return false;
  //Process tree
  if(!ProcessExpTree(vars,NumVars,type,ind,node,Interpret)) return false;
  //Delete tree
  DeleteExpTree(node);  
 }
  else
    return PushArgument(Args[0],vars,NumVars,type,ind,Interpret);

 return true;
}