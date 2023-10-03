//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MainWndUnit.h"
#include "FontData.h"
#include "SetLetterDlgUnit.h"
#include "AboutDlgUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainWnd *MainWnd;
//---------------------------------------------------------------------------
__fastcall TMainWnd::TMainWnd(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TMainWnd::InitLetterBox()
{
 for(ENuint a=0;a<256;a++)
   LetterBox->Items->Add((char)a);
}

void TMainWnd::InitLetterPic()
{
 LetterImage->Picture->Bitmap->Width=1;
 LetterImage->Picture->Bitmap->Height=1;
 LetterImage->Picture->Bitmap->Canvas->Pixels[0][0]=clWhite;
}

void __fastcall TMainWnd::FormCreate(TObject *Sender)
{
 ENEngineFontData::Init();
 InitLetterBox();
 InitLetterPic();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::LetterBoxKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 LetterBox->ItemIndex=Key%256;
}
//---------------------------------------------------------------------------
void TMainWnd::SetCurrentLetter(ENuint ind)
{
 CurrentLetterEdt->Text=(char)ind;
 ENEngineFontData::DrawLetter(LetterImage->Picture->Bitmap,ind,true);
}
void __fastcall TMainWnd::LetterBoxClick(TObject *Sender)
{
 ENint ind;
 ind=LetterBox->ItemIndex;
 if(ind>=0)
   SetCurrentLetter((ENuint)ind);
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormDestroy(TObject *Sender)
{
 ENEngineFontData::Free();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Set1Click(TObject *Sender)
{
 if(LetterBox->ItemIndex>-1)
 {
  SetLetterDlg->SetLetterIndex(LetterBox->ItemIndex);
  SetLetterDlg->ShowModal();
  LetterBoxClick(Sender);
 }
 else
   ShowMessage("You must select a letter!!!");
}
//---------------------------------------------------------------------------


void __fastcall TMainWnd::Remove1Click(TObject *Sender)
{
 ENint ind=LetterBox->ItemIndex;
 if(ind>=0)
 {
  ENEngineFontData::DeleteLetter(ind);
  LetterBoxClick(Sender);
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::New1Click(TObject *Sender)
{
 ENEngineFontData::New();
 LetterBox->ItemIndex=-1;
 LetterImage->Picture->Bitmap->Width=1;
 LetterImage->Picture->Bitmap->Height=1;
 LetterImage->Picture->Bitmap->Canvas->Pixels[0][0]=clWhite;
 CurrentLetterEdt->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Open1Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
   ENEngineFontData::Open(OpenDialog1->FileName.c_str());

 LetterBox->ItemIndex=-1;
 LetterImage->Picture->Bitmap->Width=1;
 LetterImage->Picture->Bitmap->Height=1;
 LetterImage->Picture->Bitmap->Canvas->Pixels[0][0]=clWhite;
 CurrentLetterEdt->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Exit1Click(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Save1Click(TObject *Sender)
{
 if(SaveDialog1->Execute())
 {
  AnsiString FName=SaveDialog1->FileName;
  AnsiString FExt=ExtractFileExt(FName);
  if(FExt.LowerCase()!=".enf") FName+=".enf";
    ENEngineFontData::Save(FName.c_str());
 }
}
//---------------------------------------------------------------------------


void __fastcall TMainWnd::Help2Click(TObject *Sender)
{
 AboutDlg->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormShow(TObject *Sender)
{
 if(ParamCount())
   ENEngineFontData::Open(ParamStr(1).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Setdefaultfont1Click(TObject *Sender)
{
 if(FontDialog1->Execute())
   ENEngineFontData::SetFont(FontDialog1->Font);
}
//---------------------------------------------------------------------------

