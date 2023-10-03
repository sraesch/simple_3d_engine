//---------------------------------------------------------------------------
#include "ENCoreLoader.h"
#include "ENCoreMain.h"
//---------------------------------------------------------------------------
HWND            ENCoreLoader::DlgHandle=NULL;
HDC             ENCoreLoader::devicec=NULL;
ENuint          ENCoreLoader::NumMsgs=0;
const char      ENCoreLoader::ClassName[]="ENCoreLoader";
const char      ENCoreLoader::WindowTitle[]="Engine loader V1.0a";
const ENuint    ENCoreLoader::SizeX=500;
const ENuint    ENCoreLoader::SizeY=400;

ENbool ENCoreLoader::Init(HINSTANCE AppInst)
{
 //Register class
 if(!RegisterClass(AppInst))
 {
  ENCoreMain::ShowError("Cannot register engine loader!!!");
  return false;
 }
 //Init window
 if(!InitWindow(AppInst))
 {
  ENCoreMain::ShowError("Cannot init engine loader!!!");
  return false;
 }
 //Get device
 devicec=GetDC(DlgHandle);
 if(!devicec)
 {
  ENCoreMain::ShowError("Cannot create device for engine loader!!!");
  return false;
 }
 //Finished
 return true;
}

void ENCoreLoader::Free()
{
 ReleaseDC(DlgHandle,devicec);
 DestroyWindow(DlgHandle);
}

void   ENCoreLoader::AddMsg(char *Msg)
{
 //Write message
 TextOut(devicec,10,10+NumMsgs*20,Msg,strlen(Msg));
 //Increase number of messages
 NumMsgs++;
 //Process messages
 ProcessMessages();
}

ENbool ENCoreLoader::RegisterClass(HINSTANCE AppInst)
{
 //Variables
 HBRUSH brush;
 WNDCLASSEX wndclass;
 //Create white brush
 brush=(HBRUSH)CreateSolidBrush(0xFFFFFF);
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

ENbool ENCoreLoader::InitWindow(HINSTANCE AppInst)
{
 //Calculate window position
 ENuint Pos[2];
 Pos[0]=(GetSystemMetrics(SM_CXSCREEN)-SizeX)/2;
 Pos[1]=(GetSystemMetrics(SM_CYSCREEN)-SizeY)/2;
 //Create engine loader window
 DlgHandle=CreateWindowEx(WS_EX_TOPMOST,
                          ClassName,WindowTitle,
                          WS_POPUPWINDOW | WS_VISIBLE,
                          Pos[0],Pos[1],
                          SizeX,SizeY,
                          NULL,NULL,AppInst,NULL);
 //Check if it works
 if(!DlgHandle) return false;
 //Update window
 UpdateWindow(DlgHandle);
 //Finished
 return true;
}

LRESULT CALLBACK ENCoreLoader::WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
 return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

void ENCoreLoader::ProcessMessages()
{
 //Variables
 MSG msg;
 //Get message
 PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE);
 //Process message
 TranslateMessage(&msg);
 DispatchMessage(&msg);
}


