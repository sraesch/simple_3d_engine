//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FontData.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
ENFont *ENEngineFontData::fontobj=NULL;

void ENEngineFontData::Init()
{
 fontobj=new ENFont();
}

void ENEngineFontData::Free()
{
 delete fontobj;
}

void ENEngineFontData::New()
{
 fontobj->Clear();
}

void ENEngineFontData::DrawLetter(Graphics::TBitmap *pic,ENuint ind,ENbool base)
{
 ENPicture *lpic=new ENPicture();
 pic->PixelFormat=pf24bit;
 pic->Width=1;
 pic->Height=1;
 pic->Canvas->Pixels[0][0]=clWhite;
 if(fontobj->GetLetter(ind,base,lpic))
 {
  pic->Width=lpic->GetWidth();
  pic->Height=lpic->GetHeight();
  for(ENint y=0;y<pic->Height;y++)
  {
   Byte *ptr = (Byte*)pic->ScanLine[y];
   for(ENint x=0;x<pic->Width;x++)
   {
    ENuint pixel=lpic->GetPixel(x,y);
    char *comps=(char*)&pixel;
    memcpy(ptr+x*3+2,comps,1);
    memcpy(ptr+x*3+1,comps+1,1);
    memcpy(ptr+x*3,comps+2,1);
   }
  }
 }
  delete lpic;
}

void ENEngineFontData::DeleteLetter(ENuint ind)
{
 fontobj->DelLetter(ind,true);
 fontobj->DelLetter(ind,false);
}

void ENEngineFontData::SetLetter(ENuint ind,ENPicture *base,ENPicture *alpha)
{
 fontobj->SetLetter(ind,true,base);
 fontobj->SetLetter(ind,false,alpha);
}

void ENEngineFontData::SetLetter(ENuint ind,Graphics::TBitmap *bmp,TColor backcolor)
{
 //Variables
 ENubyte *data;
 ENuint w,h,x,y;
 ENPicture *base,*alpha;
 //Alloc space
 w=(ENuint)bmp->Width;
 h=(ENuint)bmp->Height;
 data=new ENubyte[w*h*3];
 //Get data
 for(x=0;x<w;x++)
   for(y=0;y<h;y++)
   {
    ENuint pixel;
    pixel=(ENuint)bmp->Canvas->Pixels[x][y];
    data[(y*w+x)*3]=GetRValue(pixel);
    data[(y*w+x)*3+1]=GetGValue(pixel);
    data[(y*w+x)*3+2]=GetBValue(pixel);
   }
 //Create base
 base=new ENPicture(3,w,h,data);
 delete data;
 base->SetTransparent(GetRValue(backcolor),GetGValue(backcolor),GetBValue(backcolor));
 //Create alpha
 alpha=new ENPicture(base);
 alpha->SetStdAlpha();
 //Set letter
 SetLetter(ind,base,alpha);
 //Free
 delete base,alpha;
}

void ENEngineFontData::SetFont(TFont *font)
{
 //Variables
 TColor backcolor;
 SIZE lettersize;
 Graphics::TBitmap *bmp=new Graphics::TBitmap();
 //Set font
 bmp->Canvas->Font=font;
 //Set backcolor
 if(font->Color==clWhite)
   backcolor=clBlack;
 else
   backcolor=clWhite;
 //Create new font
 for(ENuint a=0;a<256;a++)
 {
  //Get charakter
  char letter=(char)a;
  //Get size of charakter
  GetTextExtentPoint32(bmp->Canvas->Handle,&letter,1,&lettersize);
  //Resize bitmap
  bmp->Width=lettersize.cx;
  bmp->Height=lettersize.cy;
  //Clear
  bmp->Canvas->Brush->Color=backcolor;
  bmp->Canvas->Pen->Color=backcolor;
  bmp->Canvas->Rectangle(0,0,lettersize.cx,lettersize.cy);
  //Draw letter
  bmp->Canvas->TextOut(0,0,letter);
  //Set letter
  SetLetter(a,bmp,backcolor);
 }
 //Finished
 delete bmp;
}

void ENEngineFontData::Open(char *FileName)
{
 if(!fontobj->Load(FileName))
   ShowMessage("Cannot load file!!!");
}

void ENEngineFontData::Save(char *FileName)
{
 if(!fontobj->Write(FileName))
   ShowMessage("Cannot write file!!!");
}

