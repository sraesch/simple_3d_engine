//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainWnd.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma comment (lib,"EngineScript.lib")
TMainDlg *MainDlg;
//---------------------------------------------------------------------------
__fastcall TMainDlg::TMainDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainDlg::LoadBtnClick(TObject *Sender)
{
 ENuint a=0;

 if(OpenDialog1->Execute())
 {
  AnsiString FName=OpenDialog1->FileName;
  ENScriptCode *code=new ENScriptCode();
  if(code->LoadFromFile(FName.c_str()))
  {
   //Clear field
   CodeField->Clear();
   //Write paths
   CodeField->Lines->Add("Paths:");
   for(a=0;a<code->GetNumPaths();a++)
   {
    ENScriptCode::ENPATH path;
    code->GetPath(a,path);
    CodeField->Lines->Add(path.Path);
   }
   //Write package
   char PackageFile[MAX_PATH];
   code->GetPackage(PackageFile);
   CodeField->Lines->Add("Package: "+AnsiString(PackageFile));
   //Write actions
   ENuint NumActions=code->GetNumActions();
   CodeField->Lines->Add("Actions:");
   for(a=0;a<NumActions;a++)
   {
    ENTEXTSTR str;
    str=code->GetActionName(a);
    CodeField->Lines->Add("Name: "+AnsiString(str.str)+" Position: "+
                          AnsiString(code->GetActionPos(str.str)));
   }
   //Write commands
   CodeField->Lines->Add("Code:");
   ENuint NumCommands=code->GetNumCommands();
   for(a=0;a<NumCommands;a++)
   {
    ENScriptCode::ENSCRIPTCOMMAND command;
    command=code->GetCommand(a);
    CodeField->Lines->Add(IntToStr(a)+": "+CommandToStr(command));
   }
   delete code;
  }
  else
  {
   ShowMessage("Cannot load "+FName+"!!!");
   delete code;
  }
 }
}

AnsiString TMainDlg::CommandToStr(ENScriptCode::ENSCRIPTCOMMAND com)
{
 AnsiString command;
 switch(com.comid)
 {
  case ENScriptCode::ENSCRIPTEXIT:
    command="exit";
  break;
  case ENScriptCode::ENSCRIPTJUMP:
    command="jump";
  break;
  case ENScriptCode::ENSCRIPTJUMPTRUE:
    command="jump if true";
  break;
  case ENScriptCode::ENSCRIPTJUMPFALSE:
    command="jump if false";
  break;
  case ENScriptCode::ENSCRIPTEXTERNCALL:
    command="extern call";
  break;
  case ENScriptCode::ENPUSH:
    command="push";
  break;
  case ENScriptCode::ENPUSHCONST:
    command="push const";
  break;                
  case ENScriptCode::ENPUSHRESULT:
    command="push result";
  break;
  case ENScriptCode::ENPUSHLOCAL:
    command="push local";
  break;
  case ENScriptCode::ENPUSHBYADDR:
    command="push by address";
  break;
  case ENScriptCode::ENPOP:
    command="pop";
  break;
  case ENScriptCode::ENPOPLOCAL:
    command="pop local";
  break;
  case ENScriptCode::ENPOPRESULT:
    command="pop result";
  break;
  case ENScriptCode::ENPOPLOGIC:
    command="pop logic";
  break;
  case ENScriptCode::ENPOPTIME:
    command="pop time value";
  break;
  case ENScriptCode::ENPOPBYADDR:
    command="pop by address";
  break;
  case ENScriptCode::ENSAVESTACK:
    command="save stack";
  break;
  case ENScriptCode::ENRESETSTACK:
    command="reset stack";
  break;
  case ENScriptCode::ENADDSTACK:
    command="add stack";
  break;
  case ENScriptCode::ENCONVIF:
    command="Convert integer to float";
  break;
  case ENScriptCode::ENCONVIS:
    command="Convert integer to string";
  break;
  case ENScriptCode::ENCONVFI:
    command="Convert float to integer";
  break;
  case ENScriptCode::ENCONVFS:
    command="Convert float to string";
  break;
  case ENScriptCode::ENCONVSI:
    command="Convert string to integer";
  break;
  case ENScriptCode::ENCONVSF:
    command="Convert string to float";
  break;
  case ENScriptCode::ENOPCOM:
    command="Run operator";
  break;                
  case ENScriptCode::ENRESETPOS:
    command="reset position";
  break;
  case ENScriptCode::ENSAVEPOS:
    command="save position";
  break;
  case ENScriptCode::ENWAIT:
    command="wait frame";
  break;
  case ENScriptCode::ENWAITT:
    command="wait time(ms)";
  break;
  case ENScriptCode::ENSETADDR:
    command="set address";
  break;
  default:
    command="UNKNOWN";
 };

 return command+"    "+IntToStr(com.a)+"||"+IntToStr(com.b);
}
//---------------------------------------------------------------------------

