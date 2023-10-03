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
   enum TriggerType {ENCREATE=0,ENCREATED=1,ENCOLLISION=2,ENTOUCH=3,
                     ENTRACE=4,ENTRACED=5,ENSCAN=6,ENSCANED=7};
   static ENbool Init();

   ENCoreObjectProcess(ENint addr,TriggerType t,
                       ENScriptObject *me,ENScriptObject *you);
   virtual void        Execute(ENuint TimeStamp);
   void                CheckObject(ENScriptObject *obj);
   void                SetReflectVec(ENVector vec);
   void                SetSurfaceVec(ENVector vec);
   void                SetTraceVec(ENVector vec);
   static  void        CheckScript(ENScriptObject *obj);
   static ENScriptObject *GetScriptYou();
   static ENScriptObject *GetScriptMe();
 protected:
   //Object variables
   TriggerType            trigger;
   ENVector               ReflectVec,TraceVec,SurfaceVec;
   ENScriptObject         *me,*you;
   //Save variables
   ENScriptObject         *SaveMe,*SaveYou;
   ENVector               SaveReflectVec,SaveTraceVec,SaveSurfaceVec;
   ENint                  SaveTrigger;
   //Static script variables
   static ENScriptObject  **ScriptMe,**ScriptYou;
   static ENint           *ScriptTrigger;
   static ENVector        *ScriptReflectVec,*ScriptTraceVec,*ScriptSurfaceVec;
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
   static void   AddSimpleTrigger(ENint addr,ENScriptObject *me,ENScriptObject *you,
                                  ENCoreObjectProcess::TriggerType trigger);
   static void   AddCollisionTrigger(ENint addr,ENScriptObject *me,ENScriptObject *you,
                                     ENVector ReflectVec,ENVector SurfaceVec);
   static void   AddTraceTrigger(ENint addr,ENScriptObject *me,ENScriptObject *you,
                                 ENVector TraceVec,ENbool traced);
   static void   Execute();
   static void   FreeProcess(ENuint ID);
   static void   CheckProcessForObject(ENScriptObject *obj);

   static ENuint GetNumProcesses();
 private:
   static ENArrayBasic *Processes;
   static ENuint   TimeStamp;

   static ENCoreBaseProcess *GetProcess(ENuint ind);
   static void               AddProcess(ENCoreBaseProcess *process);
   static void               KillProcess(ENuint ind);
};

#endif
