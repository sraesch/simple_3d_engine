//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AboutWndUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutDlg *AboutDlg;
//---------------------------------------------------------------------------
__fastcall TAboutDlg::TAboutDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAboutDlg::OKBtnClick(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------

