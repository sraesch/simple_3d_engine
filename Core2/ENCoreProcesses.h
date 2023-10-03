//---------------------------------------------------------------------------
#ifndef ENCoreProcessesH
#define ENCoreProcessesH
//---------------------------------------------------------------------------
#include "EngineScript.h"
#include "ENCoreObjects.h"
///////////////////////////////////////////////////////////////////
/// Engine core base process class
///////////////////////////////////////////////////////////////////
class ENCoreBaseProcess : public ENScriptExecute
{
 public:
   enum ProcessType {ENPROCESSBASE,ENPROCESSOBJECT};
   ENCoreBaseProcess(ENint addr);
   inline ENuint      GetID();
   inline ENCoreBaseProcess::ProcessType GetType();
   virtual void   Execute(ENuint TimeStamp);
 protected:
   ProcessType type;
   ENuint      ID;
   ENuint      TimeStamp;
};
///////////////////////////////////////////////////////////////////
/// Engine core object process class
///////////////////////////////////////////////////////////////////
class ENCoreObjectProcess : public ENCoreBaseProcess
{
 public:
   enum TriggerType {ENCREATEOBJECT=0};
   static ENbool Init();

   ENCoreObjectProcess(ENint addr,TriggerType t,
                       ENScriptObject *me,ENScriptObject *you);
   virtual void        Execute(ENuint TimeStamp);
   void                CheckObject(ENScriptObject *obj);
   static  void        CheckScript(ENScriptObject *obj);
 protected:
   //Object variables
   TriggerType            trigger;
   ENScriptObject         *me,*you;
   //Save variables
   ENScriptObject         *SaveMe,*SaveYou;
   ENint                  SaveTrigger;
   //Static script variables
   static ENScriptObject  **ScriptMe,**ScriptYou;
   static ENint           *ScriptTrigger;
};
///////////////////////////////////////////////////////////////////
/// Engine core process manager class
///////////////////////////////////////////////////////////////////
class ENCoreProcessManager
{
 public:
   static ENbool Init();
   static void   Free();
   static ENuint AddBaseProcess(ENint addr,ENbool immediately);
   static ENuint AddObjectProcess(ENint addr,ENScriptObject *me,
                                  ENScriptObject *you,ENbool immediately);
   static void   Execute();
   static void   FreeProcess(ENuint ID);
   static void   CheckProcessForObject(ENScriptObject *obj);
 private:
   static ENArray *Processes;
   static ENuint   TimeStamp;

   static ENCoreBaseProcess *GetProcess(ENuint ind);
   static void               AddProcess(ENCoreBaseProcess *process);
   static void               KillProcess(ENuint ind);
};

#endif
