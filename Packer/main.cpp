//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "PackerData.h"
#include "AboutWndUnit.h"
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
void __fastcall TMainWnd::FormCreate(TObject *Sender)
{
 PackItems->Cells[0][0]="Name";
 PackItems->Cells[1][0]="Type";
 PackItems->Cells[2][0]="Size";
 ENPackerData::Init();
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::FormDestroy(TObject *Sender)
{
 SaveQuestion();
 ENPackerData::Free();        
}
//---------------------------------------------------------------------------
AnsiString TMainWnd::Type2Str(ENPackerBase::ENPackageType type)
{
 switch(type)
 {
  case ENPackerBase::ENFONT:
    return "Font";
  case ENPackerBase::ENMODEL:
    return "Model";
  case ENPackerBase::ENSPRITE:
    return "Sprite";
  case ENPackerBase::ENPICTURE:
    return "Picture";
  case ENPackerBase::ENSOUND:
    return "Sound";    
  default:
    return "Unknown";    
 };
}

void TMainWnd::UpdateList()
{
 ENPackerBase::ENPackerHeader header=ENPackerData::GetHeader();
 PackItems->RowCount=header.NumPackages+1;
 for(ENuint a=0;a<header.NumPackages;a++)
 {
  ENPacker::ENPackageHeader ph=ENPackerData::GetPackageHeader(a);
  PackItems->Cells[0][a+1]=ph.Name;
  PackItems->Cells[1][a+1]=Type2Str(ph.Type);
  PackItems->Cells[2][a+1]=ph.Size;
 }
}


void __fastcall TMainWnd::AddFBtnClick(TObject *Sender)
{
 if(OpenDialog1->Execute())
   ENPackerData::AddPackage(OpenDialog1->FileName,NameEdt->Text);

 UpdateList();
}
//---------------------------------------------------------------------------


void __fastcall TMainWnd::PackItemsSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
 ENPackerBase::ENPackerHeader header=ENPackerData::GetHeader();
 if(ARow-1<(ENint)header.NumPackages&&ARow)
 {
  ENPacker::ENPackageHeader ph=ENPackerData::GetPackageHeader(ARow-1);
  NameEdt->Text=ph.Name;
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::DeleteBtnClick(TObject *Sender)
{
 ENPackerData::DeletePackage(NameEdt->Text);
 UpdateList();
}
//---------------------------------------------------------------------------



void __fastcall TMainWnd::New1Click(TObject *Sender)
{
 SaveQuestion();
 ENPackerData::New();
 UpdateList();        
}
//---------------------------------------------------------------------------
void TMainWnd::SaveQuestion()
{
 if(ENPackerData::GetMustSave())
 {
  ENint btn=Application->MessageBox("Do you want to save?","Save...",MB_YESNO);
  if(btn==IDYES)
    Save1Click(NULL);
 }
}

void __fastcall TMainWnd::Open1Click(TObject *Sender)
{
 SaveQuestion();
 if(OpenDialog2->Execute())
 {
  AnsiString FName=OpenDialog2->FileName;
  ENPackerData::Load(FName);
  UpdateList();
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Save1Click(TObject *Sender)
{
 if(ENPackerData::GetCanSave())
   ENPackerData::Save();
 else
   Saveas1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Saveas1Click(TObject *Sender)
{
 if(SaveDialog1->Execute())
 {
  AnsiString FName=SaveDialog1->FileName;
  AnsiString FExt=ExtractFileExt(FName);
  if(FExt.LowerCase()!=".enginepack")
    FName+=".enginepack";
  ENPackerData::Save(FName);
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::RenameBtnClick(TObject *Sender)
{
 ENPackerData::RenamePackage(NameEdt->Text,RenameEdt->Text);
 UpdateList();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Exit1Click(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::About1Click(TObject *Sender)
{
 AboutDlg->ShowModal();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::ExportBtnClick(TObject *Sender)
{
 ENint ind=ENPackerData::SearchPackage(NameEdt->Text);
 if(ind>=0)
 {
  ENPacker::ENPackageHeader header;
  header=ENPackerData::GetPackageHeader((ENuint)ind);
  AnsiString Filter="All files(*.*)|*.*";
  switch(header.Type)
  {
   case ENPackerBase::ENMODEL:
     Filter="Engine Model(*.emo)|*.emo";
   break;
   case ENPackerBase::ENSPRITE:
     Filter="Engine Sprite(*.esp)|*.esp";
   break;
   case ENPackerBase::ENFONT:
     Filter="Engine Font(*.efn)|*.efn";
   break;
   case ENPackerBase::ENPICTURE:
     Filter="Engine Picture(*.enp)|*.enp";
   break;
   case ENPackerBase::ENSOUND:
     Filter="Engine Sound(*.ensnd)|*.ensnd";
   break;
  }
  SaveDialog2->Filter=Filter;
  if(SaveDialog2->Execute())
    ENPackerData::ExportPackage((ENuint)ind,SaveDialog2->FileName);
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormShow(TObject *Sender)
{
 //If open command
 if(ParamCount())
 {
  AnsiString FName=ParamStr(1);
  ENPackerData::Load(FName);
  UpdateList();
 }
}
//---------------------------------------------------------------------------

