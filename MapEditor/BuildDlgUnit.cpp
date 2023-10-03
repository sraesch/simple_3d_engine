//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "BuildDlgUnit.h"
#include "MapData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBuildDlg *BuildDlg;
//---------------------------------------------------------------------------
__fastcall TBuildDlg::TBuildDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TBuildDlg::Build(AnsiString FileName)
{
 FName=FileName;
 ShowModal();
}

void TBuildDlg::ReportFunc(char *Msg,ENuint pos,ENuint max)
{
 BuildDlg->MsgLbl->Caption=Msg;
 if(max!=(ENuint)BuildDlg->ProgBar->Max)
   BuildDlg->ProgBar->Max=max;
   
 BuildDlg->ProgBar->Position=pos;
 Application->ProcessMessages();
}


void __fastcall TBuildDlg::CloseBtnClick(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------

void __fastcall TBuildDlg::FormActivate(TObject *Sender)
{
 GoBtn->Enabled=false;
 char FileName[MAX_PATH];
 strcpy(FileName,FName.c_str());
 CloseBtn->Enabled=false;
 ENMapData::Build(FileName);
 CloseBtn->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TBuildDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
 GoBtn->Enabled=false;        
}
//---------------------------------------------------------------------------

void __fastcall TBuildDlg::FormShow(TObject *Sender)
{
 GoBtn->Enabled=true;        
}
//---------------------------------------------------------------------------

