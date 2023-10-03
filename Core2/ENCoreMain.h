//---------------------------------------------------------------------------
#ifndef ENCoreMainH
#define ENCoreMainH
//---------------------------------------------------------------------------
#include "EngineSystem.h"

class ENCoreMain
{
 public:
   static ENbool Run(HINSTANCE AppInst);

   static void   ShowError(char *error);
   static void   StopApplication();
   static ENbool SearchFile(char *FileName,char *res);
 private:
   static ENbool         AppRunning;
   static ENint         *Fps;
   static ENfloat       *TimeFac;
   static char           SysDir[MAX_PATH];

   static ENbool Init(HINSTANCE AppInst);
   static ENbool RegisterScript();
   static ENbool LoadStdPack();
   static void Free();
   static void Idle();
   static void Draw();
   static void UpdateFps();
   static void UpdateTimeFac();
   static ENbool CheckFile(char *FileName);
   static void   ExtractPath(char *FName,char *Path);
};


#endif
