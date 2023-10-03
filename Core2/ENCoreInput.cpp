//---------------------------------------------------------------------------
#include "ENCoreInput.h"
#include "ENCoreScript.h"
#include "ENCoreMain.h"
#include "ENCoreWindow.h"
#include "ENCoreProcesses.h"
#include "ENCoreLoader.h"
//---------------------------------------------------------------------------
ENbool                          ENCoreInput::KeyStats[256];
ENCoreInput::ENInputFunc        ENCoreInput::KeyFuncs[256];
ENint                          *ENCoreInput::CursorX=NULL;
ENint                          *ENCoreInput::CursorY=NULL;
ENVector2                      *ENCoreInput::CursorForce=NULL;
ENint                          *ENCoreInput::CursorMode=NULL;

ENbool ENCoreInput::Init()
{
 //Variables
 ENuint a;
 //Loader message
 ENCoreLoader::AddMsg("Initialize engine core Inputs...");
 //Register script variables
 if(!RegisterScript())
 {
  ENCoreMain::ShowError("Cannot initialize input script variables!!!");
  return false;
 }
 //Init key stats
 memset(KeyStats,0,sizeof(ENbool)*256);
 //Init cursor position
 *CursorX=0;
 *CursorY=0;
 //Init cursor force
 *CursorForce=ENVector2(0.0f,0.0f);
 //Init CursorMode
 *CursorMode=EN_CURSOR_SHOW;
 //Init key funcs
 for(a=0;a<256;a++)
 {
  KeyFuncs[a].down=-1;
  KeyFuncs[a].up=-1;
 }
 //Finished
 return true;
}

void ENCoreInput::UpdateKey(ENuint key,ENbool stat)
{
 key%=256;
 //Set new key stat
 KeyStats[key]=stat;
 //Run key function
 RunKeyFunc(key,stat);
}

void ENCoreInput::UpdateCursor(ENint x,ENint y)
{
 //Calculate cursor force
 if((*CursorMode)!=EN_CURSOR_SHOW)
   UpdateCursorForce(x,y);
 //Set cursor position
 *CursorX=x;
 *CursorY=y;
}

void   ENCoreInput::SetKeyFunc(ENuint key,ENbool stat,ENint func)
{
 if(stat)
   KeyFuncs[key%256].down=func;
 else
   KeyFuncs[key%256].up  =func;
}

ENint  ENCoreInput::GetCursorMode()
{
 return (*CursorMode);
}

void   ENCoreInput::GetCursorPos(ENint &x,ENint &y)
{
 x=*CursorX;
 y=*CursorY;
}

ENbool ENCoreInput::GetKeyStat(ENuint key)
{
 return KeyStats[key%256];
}

ENbool ENCoreInput::RegisterScript()
{
 //CursorX
 CursorX=(ENint*)ENCoreScript::GetScriptVar("CursorX");
 if(!CursorX) return false;
 //CursorY
 CursorY=(ENint*)ENCoreScript::GetScriptVar("CursorY");
 if(!CursorY) return false;
 //CursorForce
 CursorForce=(ENVector2*)ENCoreScript::GetScriptVar("CursorForce");
 if(!CursorForce) return false;
 //CursorMode
 CursorMode=(ENint*)ENCoreScript::GetScriptVar("CursorMode");
 if(!CursorMode) return false;
 //Finished
 return true;
}

void ENCoreInput::UpdateCursorForce(ENint &x,ENint &y)
{
 //Variables
 ENuint wx,wy;
 ENVector2 Force;
 //Get window size
 ENCoreWindow::GetWindowSize(wx,wy);
 //Calculate cursor force
 Force.x=(ENfloat)(x-CursorX[0]);
 Force.y=(ENfloat)(y-CursorY[0]);
 //Reduce cursor force
 Force=Force*0.1f;
 //Check ranges
 x=GetSystemMetrics(SM_CXSCREEN)/2;
 y=GetSystemMetrics(SM_CYSCREEN)/2;
 SetCursorPos(x,y);
 /*
 if(x==0||x==GetSystemMetrics(SM_CXSCREEN)-1)
 {
  x=GetSystemMetrics(SM_CXSCREEN)/2;
  SetCursorPos(x,y);
 }
 if(y==0||y==GetSystemMetrics(SM_CYSCREEN)-1)
 {
  y=GetSystemMetrics(SM_CYSCREEN)/2;
  SetCursorPos(x,y);
 }
 */
 //Set cursor force
 *CursorForce=Force;
}

void ENCoreInput::RunKeyFunc(ENuint key,ENbool pressed)
{
 //Variables
 ENint func;
 //Get key func
 if(pressed)
   func=KeyFuncs[key].down;
 else
   func=KeyFuncs[key].up;
 //Execute key function
 if(func>-1)
   ENCoreProcessManager::AddBaseProcess(func,true);
 else
   ExecStdKeyFunc(key,pressed);    
}

void ENCoreInput::ExecStdKeyFunc(ENuint key,ENbool pressed)
{
 switch(key)
 {
  case VK_ESCAPE:
    if(pressed) ENCoreMain::StopApplication();
  break;
 };
}
