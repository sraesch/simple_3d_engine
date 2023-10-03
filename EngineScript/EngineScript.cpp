/*
 Engine script dll
 Written by Scratchy
 E-Mail: ScratchyItsch@hotmail.com
 Version 0.1.1.1
*/
//INCLUDES
#include <vcl.h>
#include "EngineScript.h"
#include "EngineMedia.h"
#include "ENScriptInitDll.h"
//PRECOMPILER OPTIONS
#pragma comment (lib,"EngineSystem.lib")
#pragma comment (lib,"EngineMedia.lib")
//UNITS
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
 //Check the reason
 if(reason!=DLL_PROCESS_ATTACH)
   return true;
 else
 {
  if(ENScriptInitDll::Init())
    return true;
  else
  {
   ENScriptInitDll::ShowError("Cannot initialize EngineScript.dll");
   return false;
  }
 }
}
//---------------------------------------------------------------------------

