//---------------------------------------------------------------------------
#include "ENCoreWindow.h"
#include "ENCoreInput.h"
#include "ENCoreMain.h"
#include "ENCoreScript.h"
#include "ENCoreInput.h"
#include "ENCoreGL.h"
#include "ENCoreLoader.h"
//---------------------------------------------------------------------------
HWND                            ENCoreWindow::WndHandle=NULL;
ENint                          *ENCoreWindow::SizeX=0;
ENint                          *ENCoreWindow::SizeY=0;
ENint                           ENCoreWindow::Cursor[2]={0,0};
const char                      ENCoreWindow::ClassName[]="ENCoreWindow";
const char                      ENCoreWindow::WindowTitle[]="Engine Core modul";

ENbool ENCoreWindow::Init(HINSTANCE AppInst)
{
 //Loader message
 ENCoreLoader::AddMsg("Initialize engine core window...");
 //Register script variables
 if(!RegisterScript())
 {
  ENCoreMain::ShowError("Window cannot register script variables");
  return false;
 }
 //Register window class
 if(!RegisterClass(AppInst))
 {
  ENCoreMain::ShowError("Cannot register window class!!!");
  return false;
 }
 //Create and show window
 if(!InitWindow(AppInst))
 {
  ENCoreMain::ShowError("Cannot create window class!!!");
  return false;
 }
 //Init OpenGL context
 if(!ENCoreGL::Init(WndHandle)) return false;
 //Finished
 return true;
}

void ENCoreWindow::Free()
{
 //Delete OpenGL context
 ENCoreGL::Free();
 //Destroy me
 DestroyWindow(WndHandle);
}

void ENCoreWindow::FullScreen()
{
 MoveWindow(WndHandle,0,0,
            GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
            false);
}

void ENCoreWindow::Idle()
{
 //Variables
 MSG msg;
 //Get message
 PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE);
 //Process message
 if(msg.message!=WM_QUIT)
 {
  TranslateMessage(&msg);
  DispatchMessage(&msg);
 }
 else
   ENCoreMain::StopApplication();
 //Update cursor position
 ENCoreInput::UpdateCursor(Cursor[0],Cursor[1]);
}

void   ENCoreWindow::GetWindowSize(ENuint &w,ENuint &h)
{
 w=(ENuint)(*SizeX);
 h=(ENuint)(*SizeY);
}

HDC    ENCoreWindow::CreateDevice()
{
 return GetDC(WndHandle);
}

void   ENCoreWindow::DeleteDevice(HDC dc)
{
 ReleaseDC(WndHandle,dc);
}

ENbool ENCoreWindow::RegisterScript()
{
 //SizeX
 SizeX=(ENint*)ENCoreScript::GetScriptVar("ScreenX");
 if(!SizeX) return false;
 //SizeY
 SizeY=(ENint*)ENCoreScript::GetScriptVar("ScreenY");
 if(!SizeY) return false;
 //Finished
 return true;
}

ENbool ENCoreWindow::RegisterClass(HINSTANCE AppInst)
{
 //Variables
 HBRUSH brush;
 WNDCLASSEX wndclass;
 //Create black brush
 brush=(HBRUSH)CreateSolidBrush(0);
 //Init window class record
 memset (&wndclass, 0, sizeof(WNDCLASSEX));
 //Fill record
 wndclass.lpszClassName = ClassName;
 wndclass.cbSize = sizeof(WNDCLASSEX);
 wndclass.style = CS_HREDRAW | CS_VREDRAW;
 wndclass.lpfnWndProc = WindowProc;
 wndclass.hInstance = AppInst;
 wndclass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
 wndclass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
 wndclass.hCursor = LoadCursor (NULL, IDC_ARROW);
 wndclass.hbrBackground = brush;
 return (RegisterClassEx (&wndclass)!=0);
}

ENbool ENCoreWindow::InitWindow(HINSTANCE AppInst)
{
 //Create engine window
 WndHandle=CreateWindowEx(WS_EX_TOPMOST,ClassName,WindowTitle,
                          WS_POPUP,
                          0,0,
                          GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
                          NULL,NULL,AppInst,NULL);
 //Check if it works
 if(!WndHandle) return false;
 //Finished
 return true;
}

void ENCoreWindow::Show()
{
 //Show window and don't show cursor
 ShowWindow(WndHandle,SW_SHOW);
 UpdateWindow(WndHandle);
 ShowCursor(FALSE);
}

LRESULT CALLBACK ENCoreWindow::WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
 switch(uMsg)
 {
  case WM_KEYDOWN:
  case WM_KEYUP:
    ProcessKeys(uMsg,wParam);
  break;
  case WM_MOUSEMOVE:
    Cursor[0]=LOWORD(lParam);
    Cursor[1]=HIWORD(lParam);
  break;
  case WM_SIZE:
    *SizeX=LOWORD(lParam);
    *SizeY=HIWORD(lParam);
  break;
 };
 return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

void ENCoreWindow::ProcessKeys(UINT uMsg,WPARAM wParam)
{
 //Variables
 static ENint LastKey=-1;
 static ENint LastKeyStat=-1;
 //Process
 if(uMsg==WM_KEYDOWN)
 {
  if(LastKey!=(ENint)wParam||LastKeyStat!=1)
  {
   LastKey=wParam;
   LastKeyStat=1;
   ENCoreInput::UpdateKey(wParam,true);
  }
 }
 else
 {
  if(LastKey!=(ENint)wParam||LastKeyStat!=0)
  {
   LastKey=wParam;
   LastKeyStat=0;
   ENCoreInput::UpdateKey(wParam,false);
  }
 }
}
