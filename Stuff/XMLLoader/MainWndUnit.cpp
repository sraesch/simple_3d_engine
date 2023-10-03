//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainWndUnit.h"
#pragma comment(lib,"EngineSystem.lib")
#pragma comment(lib,"EngineMedia.lib")
#pragma comment(lib,"EngineScript.lib")
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
void __fastcall TMainWnd::Loadxml1Click(TObject *Sender)
{
 OpenDialog1->Execute();        
}
//---------------------------------------------------------------------------
