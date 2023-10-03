//---------------------------------------------------------------------------
#ifndef ENCoreLoaderH
#define ENCoreLoaderH
//---------------------------------------------------------------------------
#include "EngineSystem.h"

class ENCoreLoader
{
 public:
   static ENbool Init(HINSTANCE AppInst);
   static void   Free();
   static void   AddMsg(char *Msg);
 private:
   static const char ClassName[];
   static const char WindowTitle[];
   static const ENuint SizeX,SizeY;
   static HWND DlgHandle;
   static HDC  devicec;
   static ENuint NumMsgs;

   static ENbool RegisterClass(HINSTANCE AppInst);
   static ENbool InitWindow(HINSTANCE AppInst);
   static LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
   static void    ProcessMessages();
};
#endif
