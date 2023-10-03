//---------------------------------------------------------------------------
#ifndef ENCoreWindowH
#define ENCoreWindowH
//---------------------------------------------------------------------------
#include "EngineSystem.h"

class ENCoreWindow
{
 public:
   static ENbool Init(HINSTANCE AppInst);
   static void   Free();
   static void   Show();
   static void   FullScreen();
   static void   Idle();
   static void   GetWindowSize(ENuint &w,ENuint &h);
   static HDC    CreateDevice();
   static void   DeleteDevice(HDC dc);
 private:
   static HWND          WndHandle;
   static ENint        *SizeX,*SizeY;
   static ENint         Cursor[2];
   static const char    ClassName[];
   static const char    WindowTitle[];

   static ENbool           RegisterScript();
   static ENbool           RegisterClass(HINSTANCE AppInst);
   static ENbool           InitWindow(HINSTANCE AppInst);
   static LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
   static void             ProcessKeys(UINT uMsg,WPARAM wParam);
};

#endif
