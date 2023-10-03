/*
 Engine Sound dll
 Written by Scratchy
 E-Mail: ScratchyItsch@hotmail.com
 Version 0.1.1.x
*/
//---------------------------------------------------------------------------
#include <windows.h>
#include "EngineSound.h"
//---------------------------------------------------------------------------
#pragma comment (lib,"EngineSystem.lib")

#pragma argsused

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
 //Check the reason
 if(reason!=DLL_PROCESS_ATTACH)
   return true;
 else
 {
  if(ENSoundAPI::Init())
    return true;
  else
  {
   MessageBox(NULL,"Cannot initialize EngineSound.dll","EngineSound.dll Error",
              MB_TASKMODAL | MB_TOPMOST | MB_ICONERROR);
   return false;
  }
 }
}
//---------------------------------------------------------------------------
 