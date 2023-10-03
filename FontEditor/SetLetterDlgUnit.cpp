//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "EngineMedia.h"
#include "SetLetterDlgUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSetLetterDlg *SetLetterDlg;
//---------------------------------------------------------------------------
__fastcall TSetLetterDlg::TSetLetterDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSetLetterDlg::CloseBtnClick(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
void __fastcall TSetLetterDlg::FormShow(TObject *Sender)
{
 AlphaEdt->Enabled=false;
 AlphaLbl->Enabled=false;
 AlphaChb->Checked=false;
 SearchBtn2->Enabled=false;
 ColorPnl->Color=clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TSetLetterDlg::AlphaChbClick(TObject *Sender)
{
 AlphaEdt->Enabled=AlphaChb->Checked;
 AlphaLbl->Enabled=AlphaChb->Checked;
 SearchBtn2->Enabled=AlphaChb->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TSetLetterDlg::SearchBtn1Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
   BaseEdt->Text=OpenDialog1->FileName;
}
//---------------------------------------------------------------------------

void __fastcall TSetLetterDlg::SearchBtn2Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
   AlphaEdt->Text=OpenDialog1->FileName;
}
//---------------------------------------------------------------------------
void TSetLetterDlg::SetLetterIndex(ENuint ind)
{
 LetterInd=ind;
}

void __fastcall TSetLetterDlg::Button1Click(TObject *Sender)
{
 ColorDialog1->Color=ColorPnl->Color;
 if(ColorDialog1->Execute())
   ColorPnl->Color=ColorDialog1->Color;
}
//---------------------------------------------------------------------------

void __fastcall TSetLetterDlg::OKBtnClick(TObject *Sender)
{
 //Get values
 char   BaseFN[MAX_PATH];
 char   AlphaFN[MAX_PATH];
 ENbool EnAlpha=AlphaChb->Checked;
 ENuint transcol=(ENuint)ColorPnl->Color;
 strcpy(BaseFN,BaseEdt->Text.c_str());
 strcpy(AlphaFN,AlphaEdt->Text.c_str());
 //Create base
 ENPicture *pic=new ENPicture();
 if(!pic->Load(BaseFN))
 {
  delete pic;
  ShowMessage("Cannot load base file!!!");
  return;
 }
 pic->SetTransparent(GetRValue(transcol),GetGValue(transcol),GetBValue(transcol));
 //Create alpha
 ENPicture *pic2=new ENPicture(pic);
 pic2->SetStdAlpha();
 if(EnAlpha)
 {
  ENPicture *alpha=new ENPicture();
  if(!alpha->Load(AlphaFN))
  {
   delete pic,pic2,alpha;
   ShowMessage("Cannot load alpha file!!!");
   return;
  }
  alpha->Resize(pic->GetWidth(),pic->GetHeight());
  for(ENuint x=0;x<pic->GetWidth();x++)
    for(ENuint y=0;y<pic->GetHeight();y++)
      pic2->SetAlpha(x,y,alpha->GetAlpha(x,y));

  delete alpha;      
 }
 //Add new letter
 ENEngineFontData::SetLetter(LetterInd,pic,pic2);
 delete pic,pic2;
 //Finished
 Close();
}
//---------------------------------------------------------------------------

void __fastcall TSetLetterDlg::FormCreate(TObject *Sender)
{
 OpenDialog1->Filter=ENPicture::GetImageExtStr();        
}
//---------------------------------------------------------------------------

