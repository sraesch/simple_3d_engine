//---------------------------------------------------------------------------
#ifndef ENCoreDepthBufferH
#define ENCoreDepthBufferH
//---------------------------------------------------------------------------
#include "EngineSystem.h"
#pragma comment(lib,"EngineSystem.lib")

class ENCoreDepthBuffer
{
 public:
   static ENbool       Init(HDC vdc,HGLRC vrc,HWND hwnd);
   static void         Free();
   static void         SetShadow();
   static void         Begin();
   static void         End(ENbool toTexture);
 private:
   static ENuint       TexID;
   static ENint        Size;
   static HDC          dc;
   static HGLRC        rc;
   static HWND         hwnd;
   static HPBUFFERARB  buffer;
   static ENbool       InitBuffer(HDC vdc,HGLRC vrc,HWND hwnd);
   static void         Config();
};
#endif
