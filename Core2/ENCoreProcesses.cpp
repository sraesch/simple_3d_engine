//---------------------------------------------------------------------------
#include "ENCoreProcesses.h"
#include "ENCoreScript.h"
#include "ENCoreLoader.h"
#include "ENCoreMain.h"
//---------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////
/// Engine core base process class
///////////////////////////////////////////////////////////////////
ENCoreBaseProcess::ENCoreBaseProcess(ENint addr)
                 : ENScriptExecute(addr,ENCoreScript::GetCodeObj())
{
 TimeStamp=0;
 type=ENPROCESSBASE;
 ID=(ENuint)this;
}

inline ENuint ENCoreBaseProcess::GetID()
{
 return ID;
}

inline ENCoreBaseProcess::ProcessType ENCoreBaseProcess::GetType()
{
 return type;
}

void ENCoreBaseProcess::Execute(ENuint TimeStamp)
{
 if(TimeStamp!=this->TimeStamp)
 {
  this->TimeStamp=TimeStamp;
  ENScriptExecute::Execute();
 }
}
///////////////////////////////////////////////////////////////////
/// Engine core object process class
///////////////////////////////////////////////////////////////////
ENScriptObject **ENCoreObjectProcess::ScriptMe=NULL;
ENScriptObject **ENCoreObjectProcess::ScriptYou=NULL;
ENint           *ENCoreObjectProcess::ScriptTrigger=NULL;

ENCoreObjectProcess::ENCoreObjectProcess(ENint addr,TriggerType t,
                                         ENScriptObject *me,ENScriptObject *you)
                   : ENCoreBaseProcess(addr)
{
 type=ENPROCESSOBJECT;
 trigger=t;
 this->me=me;
 this->you=you;
}

ENbool ENCoreObjectProcess::Init()
{
 //Me
 ScriptMe=(ENScriptObject**)ENCoreScript::GetScriptVar("me");
 if(!ScriptMe) return false;
 //You
 ScriptYou=(ENScriptObject**)ENCoreScript::GetScriptVar("you");
 if(!ScriptYou) return false;
 //Trigger
 ScriptTrigger=(ENint*)ENCoreScript::GetScriptVar("Trigger");
 if(!ScriptTrigger) return false;
 //Init
 *ScriptMe=NULL;
 *ScriptYou=NULL;
 *ScriptTrigger=-1;
 //Finished
 return true;
}

void ENCoreObjectProcess::Execute(ENuint TimeStamp)
{
 //Save old trigger variables
 SaveMe=*ScriptMe;
 SaveYou=*ScriptYou;
 SaveTrigger=*ScriptTrigger;
 //Set new trigger variables
 *ScriptMe=me;
 *ScriptYou=you;
 *ScriptTrigger=trigger;
 //Execute code
 ENCoreBaseProcess::Execute(TimeStamp);
 //Reset old trigger variables
 *ScriptMe=SaveMe;
 *ScriptYou=SaveYou;
 *ScriptTrigger=SaveTrigger;
}

void ENCoreObjectProcess::CheckObject(ENScriptObject *obj)
{
 //Check object variables
 if(me==obj)            me=NULL;
 if(you==obj)           you=NULL;
 //Check save variables
 if(SaveMe==obj)        SaveMe=NULL;
 if(SaveYou==obj)       SaveYou=NULL;
}

void ENCoreObjectProcess::CheckScript(ENScriptObject *obj)
{
 //Check script variables
 if(*ScriptMe==obj)            *ScriptMe=NULL;
 if(*ScriptYou==obj)           *ScriptYou=NULL;
}

///////////////////////////////////////////////////////////////////
/// Engine core process manager class
///////////////////////////////////////////////////////////////////
ENArray *ENCoreProcessManager::Processes=NULL;
ENuint   ENCoreProcessManager::TimeStamp=1;

ENbool ENCoreProcessManager::Init()
{
 //Loader message
 ENCoreLoader::AddMsg("Initialize engine core process manager...");
 //Set first time stamp
 TimeStamp=1;
 //Init array object for processes
 Processes=new ENArray(sizeof(ENCoreBaseProcess*));
 //Init core object processes
 if(!ENCoreObjectProcess::Init())
 {
  ENCoreMain::ShowError("Cannot initialize core object processes!!!");
  return false;
 }
 //Finished
 return true;
}

void ENCoreProcessManager::Free()
{
 //Delete processes
 for(ENuint a=0;a<Processes->GetNum();a++)
   delete GetProcess(0);
 //Delete process list
 delete Processes;
}

ENuint ENCoreProcessManager::AddBaseProcess(ENint addr,ENbool immediately)
{
 //Variables
 ENCoreBaseProcess *process;
 //Check process
 if(addr<0) return NULL;
 //Create process
 process=new ENCoreBaseProcess(addr);
 //Add new process to the process list
 AddProcess(process);
 //Execute process immediately if needed
 if(immediately) process->Execute(TimeStamp);
 //Finished
 return process->GetID();
}

ENuint ENCoreProcessManager::AddObjectProcess(ENint addr,ENScriptObject *me,
                                              ENScriptObject *you,ENbool immediately)
{
 //Variables
 ENCoreObjectProcess *process;
 //Check process
 if(addr<0) return NULL;
 //Create process
 process=new ENCoreObjectProcess(addr,ENCoreObjectProcess::ENCREATEOBJECT,
                                 me,you);
 //Add new process to the process list
 AddProcess(process);
 //Execute process immediately if needed
 if(immediately) process->Execute(TimeStamp);
 //Finished
 return process->GetID();
}

void ENCoreProcessManager::Execute()
{
 //Variables
 ENuint pcounter=0;
 ENCoreBaseProcess *bprocess=NULL;
 //Execute all processes
 while(pcounter<Processes->GetNum())
 {
  //Get current process
  bprocess=GetProcess(pcounter);
  //Execute process
  bprocess->Execute(TimeStamp);
  //Next process
  if(!bprocess->IsFinished())
    pcounter++;
  else
    KillProcess(pcounter);
 }
 //Increase time stamp
 TimeStamp++;
 if(!TimeStamp) TimeStamp=1;
}

void ENCoreProcessManager::FreeProcess(ENuint ID)
{
 //Variables
 ENuint nump=Processes->GetNum();
 ENCoreBaseProcess *bprocess=NULL;
 //Search process
 for(ENuint a=0;a<nump;a++)
 {
  bprocess=GetProcess(a);
  //Found process, so finish process
  if(bprocess->GetID()==ID)
  {
   bprocess->SetFinished();
   return;
  }
 }
}

void ENCoreProcessManager::CheckProcessForObject(ENScriptObject *obj)
{
 //Variables
 ENuint nump=Processes->GetNum();
 ENCoreBaseProcess   *bprocess=NULL;
 ENCoreObjectProcess *oprocess=NULL;
 //Check processes
 for(ENuint a=0;a<nump;a++)
 {
  bprocess=GetProcess(a);
  //Check for object process
  if(bprocess->GetType()==ENCoreBaseProcess::ENPROCESSOBJECT)
  {
   oprocess=(ENCoreObjectProcess*)bprocess;
   oprocess->CheckObject(obj);
  }
 }
 //Check script variables
 ENCoreObjectProcess::CheckScript(obj);
}

ENCoreBaseProcess *ENCoreProcessManager::GetProcess(ENuint ind)
{
 //Variables
 ENCoreBaseProcess *res;
 //Get process
 Processes->GetElement(ind,(ENubyte*)&res);
 //Finished
 return res;
}

void ENCoreProcessManager::AddProcess(ENCoreBaseProcess *process)
{
 Processes->AddElement((ENubyte*)&process);
}

void ENCoreProcessManager::KillProcess(ENuint ind)
{
 //Delete process
 delete GetProcess(ind);
 //Delete process from list
 Processes->DelElement(ind);
}

