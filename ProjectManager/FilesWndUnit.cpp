//---------------------------------------------------------------------------
#include <vcl.h>
#include <process.h>
#pragma hdrstop

#include "FilesWndUnit.h"
#include "ProjectClass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFilesWnd *FilesWnd;
//---------------------------------------------------------------------------
__fastcall TFilesWnd::TFilesWnd(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFilesWnd::FormResize(TObject *Sender)
{
 FilePages->Width=ClientWidth;
 FilePages->Height=ClientHeight;
}
//---------------------------------------------------------------------------
void TFilesWnd::UpdateFiles()
{
 //Clear all
 ScriptBox->Clear();
 MapBox->Clear();
 PackBox->Clear();
 UnknownBox->Clear();
 //Add elements
 for(ENuint a=0;a<ENProjectClass::GetNumFiles();a++)
 {
  ENProjectFile file=ENProjectClass::GetProjectFile(a);
  switch(file.type)
  {
   case ENProjectFile::ENSCRIPT:
     ScriptBox->Items->Add(file.FileName);
   break;
   case ENProjectFile::ENMAP:
     MapBox->Items->Add(file.FileName);
   break;
   case ENProjectFile::ENPACKAGE:
     PackBox->Items->Add(file.FileName);
   break;
   case ENProjectFile::ENUNKNOWN:
     UnknownBox->Items->Add(file.FileName);
   break;
  }
 }
}
void __fastcall TFilesWnd::FormShow(TObject *Sender)
{
 UpdateFiles();
}
//---------------------------------------------------------------------------
