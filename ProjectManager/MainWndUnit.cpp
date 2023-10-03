//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MainWndUnit.h"
#include "ProjectClass.h"
#include "FilesWndUnit.h"

#pragma comment (lib,"EngineSystem.lib")
#pragma comment (lib,"EngineMedia.lib")
#pragma comment (lib,"EngineScript.lib")
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
void __fastcall TMainWnd::Exit1Click(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::FormDestroy(TObject *Sender)
{
 ENProjectClass::Free();        
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::FormShow(TObject *Sender)
{
 FilesWnd->Show();
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::Addfile1Click(TObject *Sender)
{
 if(AddFileDlg->Execute())
   for(ENuint a=0;a<AddFileDlg->Files->Count;a++)
   {
    ENProjectClass::AddFile(AddFileDlg->Files->Strings[a]);
    FilesWnd->UpdateFiles();
   }
}
//---------------------------------------------------------------------------
