//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Import3DSInterface.h"
#include "Process3DSUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TImport3DSDlg *Import3DSDlg;
//---------------------------------------------------------------------------
__fastcall TImport3DSDlg::TImport3DSDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TImport3DSDlg::BrowseBtnClick(TObject *Sender)
{
 OpenDialog1->Execute();        
}
//---------------------------------------------------------------------------
void __fastcall TImport3DSDlg::OpenDialog1CanClose(TObject *Sender,
      bool &CanClose)
{
 FNameEdt->Text=OpenDialog1->FileName;        
}
//---------------------------------------------------------------------------
void __fastcall TImport3DSDlg::CloseBtnClick(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
void __fastcall TImport3DSDlg::FormShow(TObject *Sender)
{
 StatusBar->Enabled=false;
 StatusBar->Position=0;        
}
//---------------------------------------------------------------------------
void __fastcall TImport3DSDlg::ApplyBtnClick(TObject *Sender)
{
 ENProcess3DS::Import(FNameEdt->Text,StatusLbl,StatusBar,ImportMatChb);
 StatusBar->Position=0;
 Close();        
}
//---------------------------------------------------------------------------

