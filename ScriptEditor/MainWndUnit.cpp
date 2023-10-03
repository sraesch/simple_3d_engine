//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MainWndUnit.h"
#include "ScriptFormContainer.h"
#include "CompileDlgUnit.h"
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
void __fastcall TMainWnd::Exit1Click(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------


void __fastcall TMainWnd::FormCreate(TObject *Sender)
{
 ScriptFormClass::Init(this,ScriptFileBox);        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormDestroy(TObject *Sender)
{
 ScriptFormClass::Free();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::New1Click(TObject *Sender)
{
 ScriptFormClass::AddForm();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Saveas1Click(TObject *Sender)
{
 if(!ScriptFormClass::SaveAs())
   ShowMessage("No file to save!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Open1Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
   for(ENint a=0;a<OpenDialog1->Files->Count;a++)
   {
    AnsiString FName=OpenDialog1->Files->Strings[a];
    if(FileExists(FName))
      ScriptFormClass::Load(FName);
    else
      ShowMessage(FName+" doesn't exists!!!");
   }
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Save1Click(TObject *Sender)
{
 if(ScriptFormClass::GetCurrent())
   ScriptFormClass::Save();
 else
   ShowMessage("No file to save!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Saveall1Click(TObject *Sender)
{
 ScriptFormClass::SaveAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Compile1Click(TObject *Sender)
{
 if(MDIChildCount)
 {
  ScriptFormClass::SaveAll();

  AnsiString MainFile=ScriptFormClass::GetMainFile();
  if(MainFile.Length())
  {
   CompileDlg->MainFile=MainFile;
   CompileDlg->ShowModal();
  }
  else
    ShowMessage("Invalid main script!!!");    
 }
 else
   ShowMessage("No script files!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Undo1Click(TObject *Sender)
{
 ScriptFormClass::Undo();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Cut1Click(TObject *Sender)
{
 ScriptFormClass::CutText();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Copy1Click(TObject *Sender)
{
 ScriptFormClass::CopyText();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Paste1Click(TObject *Sender)
{
 ScriptFormClass::PasteText();         
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::About1Click(TObject *Sender)
{
 AboutDlg->ShowModal();        
}
//---------------------------------------------------------------------------


void __fastcall TMainWnd::Cascade1Click(TObject *Sender)
{
 Cascade();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Title1Click(TObject *Sender)
{
 Tile();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::ScriptFileBoxDblClick(TObject *Sender)
{
 ENint ind=ScriptFileBox->ItemIndex;
 if(ind>-1) ScriptFormClass::Activate(ind);
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormShow(TObject *Sender)
{
 for(ENint a=1;a<=ParamCount();a++)
 {
  AnsiString FName=ParamStr(a);
  if(FileExists(FName))
    ScriptFormClass::Load(FName);
  else
    ShowMessage(FName+" doesn't exists!!!");
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Search1Click(TObject *Sender)
{
 FindDialog1->Execute();        
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::FindDialog1Find(TObject *Sender)
{
 ScriptFormClass::Search((TFindDialog*)Sender);
 ((TFindDialog*)Sender)->CloseDialog();        
}
//---------------------------------------------------------------------------

