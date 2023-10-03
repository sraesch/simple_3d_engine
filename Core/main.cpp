/*
 Engine core program
 Written by Scratchy
 E-Mail: ScratchyItsch@hotmail.com
 Version 0.1.1.x
*/
//---------------------------------------------------------------------------
#include <windows.h>
#pragma hdrstop
#include "ENCoreMain.h"

#pragma comment (lib,"EngineSystem.lib")
#pragma comment (lib,"EngineMedia.lib")
#pragma comment (lib,"EngineScript.lib")
#pragma comment (lib,"EngineSound.lib")
//---------------------------------------------------------------------------

#pragma argsused
WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
 return ENCoreMain::Run(hInstance);
}
//---------------------------------------------------------------------------
 