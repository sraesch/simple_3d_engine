//---------------------------------------------------------------------------
#ifndef SpriteDataH
#define SpriteDataH
//---------------------------------------------------------------------------
#include <vcl.h>
#include "EngineMedia.h"
#pragma comment (lib,"EngineMedia.lib")
#pragma comment (lib,"EngineSystem.lib")

#define SPRITE_DRAW_NORMAL 0
#define SPRITE_DRAW_TRANS  1
#define SPRITE_DRAW_ALPHA  2

class ENSpriteData
{
 public:
   static void   Init();
   static void   Free();
   static void   Clear();
   static void   Load(char *FileName);
   static void   Draw(ENuint mode,ENuint w,ENuint h);
   static ENbool AddFrame(char *BaseFile,char *AlphaFile,ENubyte red,ENubyte green,ENubyte blue);
   static void   DeleteFrame();
   static void   SetCurrentFrame(ENuint frame);
   static ENuint GetCurrent();
   static ENSprite *SpriteData;
 protected:
   static ENuint    CurrentFrame,NumIDs,BackGrdID;
   static ENuint   *TexIDs;
   static char CurrentFile[MAX_PATH];

   static void FreeTexIDs();
   static void NewTexID();
   static void DeleteTexID(ENuint ind);
   static void InitTexIDs();
   static void DrawQuad(ENuint w,ENuint h);
};

#endif
