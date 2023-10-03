//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ScriptWndUnit.h"
#include "ScriptFormContainer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TScriptWnd *ScriptWnd;
//---------------------------------------------------------------------------
__fastcall TScriptWnd::TScriptWnd(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TScriptWnd::FormClose(TObject *Sender,
      TCloseAction &Action)
{
 if(ScriptField->Modified)
   SaveQuestion();
 else
   ScriptFormClass::DeleteForm(this);
 delete this;
}
//---------------------------------------------------------------------------
void __fastcall TScriptWnd::FormCreate(TObject *Sender)
{
 FileName="";        
}
//---------------------------------------------------------------------------
void TScriptWnd::SetFileName(AnsiString FileName)
{
 this->FileName=FileName;
 Caption=FileName;
 ScriptFormClass::SetMyTitle(this,FileName);
}

AnsiString TScriptWnd::GetFileName()
{
 return FileName;
}

void TScriptWnd::SaveAs()
{
 if(SaveDialog1->Execute())
 {
  AnsiString FName=SaveDialog1->FileName;
  AnsiString FExt=ExtractFileExt(FName);
  if(FExt.LowerCase()!=".ens")
    FName+=".ens";

  ScriptField->Lines->SaveToFile(FName);
  SetFileName(FName);
 }
}

void TScriptWnd::Save()
{
 if(!FileName.Length())
   SaveAs();
 else
   ScriptField->Lines->SaveToFile(FileName);
}

void TScriptWnd::SaveQuestion()
{
 ENint button;
 button=Application->MessageBox("Do you want to save?","Save...",MB_YESNO);
 if(button==IDYES)
   Save();
}


void __fastcall TScriptWnd::ScriptFieldChange(TObject *Sender)
{
 TPoint pos=ScriptField->CaretPos;
 char Buffer[80];
 sprintf(Buffer,"Pos: (%i,%i)",pos.x,pos.y);
 StatHeader->Panels->Items[0]->Text=Buffer;
}
//---------------------------------------------------------------------------

void __fastcall TScriptWnd::ScriptFieldKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 ScriptFieldChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TScriptWnd::ScriptFieldMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
 ScriptFieldChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TScriptWnd::ScriptFieldMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 ScriptFieldChange(Sender);        
}
//---------------------------------------------------------------------------

