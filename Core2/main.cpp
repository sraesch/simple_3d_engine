//---------------------------------------------------------------------------
#include <windows.h>
#pragma hdrstop
#include "ENCoreMain.h"

#pragma comment (lib,"EngineSystem.lib")
#pragma comment (lib,"EngineMedia.lib")
#pragma comment (lib,"EngineScript.lib")
//---------------------------------------------------------------------------

#pragma argsused
WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
 return ENCoreMain::Run(hInstance);
}
//---------------------------------------------------------------------------
 