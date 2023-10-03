#include "ENScriptBuilder.h"
#include <iostream.h>

ENScriptBuilder::ENScriptBuilder(ENSCRIPTMSGCALLBACK MsgFunc)
{
 this->MsgFunc=MsgFunc;
}

ENScriptBuilder::~ENScriptBuilder()
{
}

ENbool ENScriptBuilder::Build(char *FileName,char *Path,char *Dst)
{
 char _msg[1024];
 char _fname[1024];
 ENbool tmpbool;
 //Link
 linker=new ENScriptLinker(); 
 if(!linker->Link(FileName,Path))
 {
  linker->GetError(1024,_msg);
  if(MsgFunc) MsgFunc(_msg);
  return false;
 }
 else
 {
  if(MsgFunc) {sprintf(_msg,"Files linked!!!\n");MsgFunc(_msg);}
 }
 //Code object
 code=new ENScriptCode();  
 code->SetStackPointer(ENScriptExtern::GetExtVarsSize());
 //Init script object
 interp=new ENScriptInterpret();
 //Compile script
 for(ENuint l=0;l<linker->GetNum();l++)
 {  
  //Set filename
  sprintf(_fname,"%s\\%s",Path,linker->GetFile(l).str);
  //Load source  
  if(!interp->LoadSource(_fname))
  {
   sprintf(_msg,"Cannot load %s\n",_fname);
   if(MsgFunc) MsgFunc(_msg);
   return false;
  }
  //Compile
  tmpbool=interp->Compile(code);
  interp->GetCompileLog(1024,_msg);
  if(MsgFunc) MsgFunc(_msg);
  if(!tmpbool) return false;
 }
 //Finished 
 delete linker;
 ENbool res=code->SaveToFile(Dst);
 delete code;
 return res;
}