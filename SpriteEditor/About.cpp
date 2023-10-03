//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "About.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutWnd *AboutWnd;
//---------------------------------------------------------------------------
__fastcall TAboutWnd::TAboutWnd(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAboutWnd::Button1Click(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
