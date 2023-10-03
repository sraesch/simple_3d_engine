//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AddSpriteFrame.h"
#include "EngineMedia.h"
#include "SpriteData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAddFrameWnd *AddFrameWnd;
//---------------------------------------------------------------------------
__fastcall TAddFrameWnd::TAddFrameWnd(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAddFrameWnd::Button1Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
   BaseFName->Text=OpenDialog1->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TAddFrameWnd::Button2Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
   AlphaFName->Text=OpenDialog1->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TAddFrameWnd::Button4Click(TObject *Sender)
{
 this->Close();        
}
//---------------------------------------------------------------------------

void __fastcall TAddFrameWnd::Button3Click(TObject *Sender)
{
 ENuint tcol=(ENuint)Panel1->Color;
 ENubyte r=GetRValue(tcol);
 ENubyte g=GetGValue(tcol);
 ENubyte b=GetBValue(tcol);
 if(ENSpriteData::AddFrame(BaseFName->Text.c_str(),AlphaFName->Text.c_str(),r,g,b))
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TAddFrameWnd::Panel1Click(TObject *Sender)
{
 ColorDialog1->Color=Panel1->Color;
 if(ColorDialog1->Execute())
   Panel1->Color=ColorDialog1->Color;        
}
//---------------------------------------------------------------------------


void __fastcall TAddFrameWnd::FormCreate(TObject *Sender)
{
 OpenDialog1->Filter=ENPicture::GetImageExtStr();
}
//---------------------------------------------------------------------------

