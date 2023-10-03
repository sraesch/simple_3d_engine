//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CompileDlgUnit.h"
#include "EngineScript.h"

#pragma comment(lib,"EngineScript.lib")
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCompileDlg *CompileDlg;
//---------------------------------------------------------------------------
__fastcall TCompileDlg::TCompileDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCompileDlg::FormShow(TObject *Sender)
{
 MainFNEdt->Text=MainFile;
 StatusText->Clear();
 DstFileEdt->Focused();
}
//---------------------------------------------------------------------------
void __fastcall TCompileDlg::CloseBtnClick(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------
void ENPrintLog(char *Msg)
{
 CompileDlg->StatusText->Lines->Add(Msg);
 CompileDlg->StatusText->SetFocus();
 Application->ProcessMessages();
}
void __fastcall TCompileDlg::CompileBtnClick(TObject *Sender)
{
 StatusText->Clear();

 char ScriptFile[128];
 char DstFile[MAX_PATH];
 char Path[MAX_PATH];
 strcpy(ScriptFile,ExtractFileName(MainFile).c_str());
 strcpy(Path,ExtractFilePath(MainFile).c_str());
 strcpy(DstFile,DstFileEdt->Text.c_str());
 //Convert Path string
 ENuint len=strlen(Path);
 Path[len-1]=0;
 //build
 ENScriptBuilder *builder=new ENScriptBuilder(ENPrintLog);
 ENuint t=GetTickCount();
 if(!builder->Build(ScriptFile,Path,DstFile))
   ENPrintLog("Cannot compile!!!");
 else
 {
  ENfloat tw=(ENfloat)(GetTickCount()-t)/1000.0f;
  ENPrintLog(AnsiString("Finished in "+FloatToStr(tw)+" seconds!!!").c_str());
 }
 delete builder;
}
//---------------------------------------------------------------------------

void __fastcall TCompileDlg::Button1Click(TObject *Sender)
{
 if(SaveDialog1->Execute())
   DstFileEdt->Text=SaveDialog1->FileName;
}
//---------------------------------------------------------------------------

