//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AboutDlgUnit.h"
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
void __fastcall TAboutDlg::Button1Click(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
