//---------------------------------------------------------------------------
#ifndef FontDataH
#define FontDataH
//---------------------------------------------------------------------------
#include "EngineMedia.h"
#pragma comment(lib,"EngineMedia.lib")

class ENEngineFontData
{
 public:
   static void Init();
   static void Free();
   static void New();
   static void Open(char *FileName);
   static void Save(char *FileName);
   static void DrawLetter(Graphics::TBitmap *pic,ENuint ind,ENbool base);
   static void DeleteLetter(ENuint ind);
   static void SetLetter(ENuint ind,ENPicture *base,ENPicture *alpha);
   static void SetFont(TFont *font);
 private:
   static ENFont *fontobj;

   static void SetLetter(ENuint ind,Graphics::TBitmap *bmp,TColor backcolor);   
};

#endif
