
#pragma hdrstop
#include <condefs.h>
#include <conio.h>
#include <iostream.h>
#include "EngineScript.h"

#pragma comment (lib,"EngineSystem.lib")
#pragma comment (lib,"EngineScript.lib")
//---------------------------------------------------------------------------
#pragma argsused
ENScriptBuilder       *builder;
ENScriptExecute       *execute;
ENScriptCode          *code;
ENCamera              *camptr;
ENScriptObject        **obj;
ENScriptObject        eobj;

char *curDir="G:\\Current Engine\\Runtimes\\Scripts";
char *dstDir="G:\\Current Engine\\Runtimes\\Scripts\\ENGINECODE";

void StringAusgabe(ENubyte *Data,void *res)
{
 char str[1024];
 memcpy(str,Data-1024,1024); 
 printf("%s\n",str); 
}

bool InitExtern()
{	
 //Add print function
 ENScriptExtern::ENREGEXTERNFUNCTION reg;
 reg.Function=StringAusgabe;
 strcpy(reg.Name,"Print");
 reg.NumParams=1;
 reg.Params=new ENVAR;
 reg.Params->type=ENSTRING;
 reg.procedure=true;
 if(!ENScriptExtern::AddExtFunc(reg))
 {
  cout<<"Cannot init the function print"<<endl;
  return false;
 }

 return true;
}


void Ausgabe(char *Msg)
{
 cout<<Msg<<endl;
}

bool Build()
{
 builder=new ENScriptBuilder(Ausgabe); 
 return builder->Build("Script.ens",curDir,dstDir);
}

bool RunScript()
{
 code=new ENScriptCode();
 if(!code->LoadFromFile(dstDir))
 {
  cout<<"Cannot load compiled code!!!"<<endl;
  return false;
 } 
 //Get pointer
 camptr=(ENCamera*)(code->GetStack()+ENScriptExtern::GetExtVarPos("view"));
 //Set pointer
 obj=(ENScriptObject**)(code->GetStack()+ENScriptExtern::GetExtVarPos("me"));
 obj[0]=&eobj;
 eobj.Pos.x=13;
 //Write all actions
 cout<<code->GetNumActions()<<endl;
 for(ENuint a=0;a<code->GetNumActions();a++)
   cout<<code->GetActionName(a).str<<endl;
 //
 execute=new ENScriptExecute(code->GetMainFunc(),code);
 while(!execute->IsFinished())
 {
  Sleep(100);
  if(!execute->Execute()) return false;
 }
 execute->Execute();

 return true;
}

int main(int argc, char* argv[])
{
 //Load extern
 if(!InitExtern())
 {
  getch();
  return -2;
 }
 //Build
 if(!Build())
 {
  getch();
  return -3;
 }
 //Run script
 getch();
 system("cls");
 if(!RunScript())
 {
  cout<<"Error: Execute script!!!"<<endl;
  getch();
  return -1;
 }
 //Test
 cout<<"Programm: "<<eobj.Pos.x<<endl;
 //Delete
 delete builder;
 delete code;
 //Wait
 getch();
 return 0;
}
