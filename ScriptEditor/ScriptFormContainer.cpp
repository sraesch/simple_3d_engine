//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ScriptFormContainer.h"
#include "ScriptWndUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
TForm *ScriptFormClass::parent=NULL;
TList *ScriptFormClass::FormContainer=NULL;
TListBox *ScriptFormClass::ScriptFileBox=NULL;

void ScriptFormClass::Init(TForm *parent,TListBox *ScriptFileBox)
{
 ScriptFormClass::parent=parent;
 ScriptFormClass::ScriptFileBox=ScriptFileBox;
 FormContainer=new TList();
}

void ScriptFormClass::Free()
{
 delete FormContainer;
}

TForm *ScriptFormClass::AddForm()
{
 TScriptWnd *scriptform;
 scriptform=new TScriptWnd(parent);
 scriptform->Show();
 FormContainer->Add(scriptform);
 ScriptFileBox->Items->Add(scriptform->Caption);
 return (TForm*)scriptform;
}

void ScriptFormClass::Load(AnsiString FileName)
{
 TScriptWnd *sform=(TScriptWnd*)AddForm();
 sform->ScriptField->Lines->LoadFromFile(FileName);
 sform->SetFileName(FileName);
}

void ScriptFormClass::DeleteForm(void *form)
{
 ENint ind=FormContainer->IndexOf(form);
 if(ind>=0)
 {
  FormContainer->Delete(ind);
  ScriptFileBox->Items->Delete(ind);
 }
}

TForm *ScriptFormClass::GetCurrent()
{
 for(ENint a=0;a<FormContainer->Count;a++)
 {
  TForm *form=(TForm*)FormContainer->Items[a];
   if(form->Active)
     return form;
 }
 return NULL;
}

ENbool ScriptFormClass::Save()
{
 //Get current script form
 TScriptWnd *sform=(TScriptWnd*)GetCurrent();
 if(!sform) return false;
 //Save
 sform->Save();
 //Finished
 return true;
}

ENbool ScriptFormClass::SaveAs()
{
 //Get current script form
 TScriptWnd *sform=(TScriptWnd*)GetCurrent();
 if(!sform) return false;
 //Save
 sform->SaveAs();
 //Finished
 return true;
}

void ScriptFormClass::SaveAll()
{
 for(ENint a=0;a<FormContainer->Count;a++)
 {
  TScriptWnd *sform=(TScriptWnd*)FormContainer->Items[a];
  sform->Save();
 }
}

AnsiString ScriptFormClass::GetMainFile()
{
 AnsiString res="";
 TScriptWnd *sform=(TScriptWnd*)GetCurrent();
 if(sform)
   res=sform->GetFileName();

 return res;   
}

void ScriptFormClass::Undo()
{
 TScriptWnd *sform=(TScriptWnd*)GetCurrent();
 if(sform)
   sform->ScriptField->Undo();
}

void ScriptFormClass::Search(TFindDialog *fd)
{
 TScriptWnd *sform=(TScriptWnd*)GetCurrent();
 if(sform)
 {
  //Variables
  ENint FoundAt,StartPos,ToEnd;
  //Set start
  if(sform->ScriptField->SelLength)
    StartPos=sform->ScriptField->SelStart+sform->ScriptField->SelLength;
  else
    StartPos = 0;
  //Set end
  ToEnd=sform->ScriptField->Text.Length()-StartPos;
  //Search
  FoundAt=sform->ScriptField->FindText(fd->FindText,StartPos,ToEnd,
                                       TSearchTypes()<<stMatchCase);
  //Set result
  if(FoundAt!=-1)
  {
   sform->ScriptField->SetFocus();
   sform->ScriptField->SelStart=FoundAt;
   sform->ScriptField->SelLength=fd->FindText.Length();
   sform->SetFocus();
  }
 }
}

void ScriptFormClass::CopyText()
{
 TScriptWnd *sform=(TScriptWnd*)GetCurrent();
 if(sform)
   sform->ScriptField->CopyToClipboard();
}

void ScriptFormClass::CutText()
{
 TScriptWnd *sform=(TScriptWnd*)GetCurrent();
 if(sform)
   sform->ScriptField->CutToClipboard();
}

void ScriptFormClass::PasteText()
{
 TScriptWnd *sform=(TScriptWnd*)GetCurrent();
 if(sform)
   sform->ScriptField->PasteFromClipboard();
}

void ScriptFormClass::Activate(ENuint ind)
{
 TScriptWnd *sform=(TScriptWnd*)FormContainer->Items[ind];
 sform->Show();
}

void ScriptFormClass::SetMyTitle(TObject *Sender,AnsiString str)
{
 ENint ind=FormContainer->IndexOf(Sender);
 if(ind>-1)
   ScriptFileBox->Items->Strings[ind]=str;
}

