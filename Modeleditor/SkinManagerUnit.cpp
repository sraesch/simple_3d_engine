//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SkinManagerUnit.h"
#include "ModelData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSkinManagerDlg *SkinManagerDlg;
//---------------------------------------------------------------------------
__fastcall TSkinManagerDlg::TSkinManagerDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TSkinManagerDlg::UpdateSkinList()
{
 ENModel::ENModelHeader header=ENModelData::GetHeader();
 SkinList->Clear();
 for(ENuint a=0;a<header.NumSkins;a++)
 {
  AnsiString SkinName=ENModelData::GetSkinName(a);
  SkinList->Items->Add(SkinName);
 }
}
void __fastcall TSkinManagerDlg::FormShow(TObject *Sender)
{
 UpdateSkinList();
 NameEdt->Clear();
 UpdateCurrentSkin();
}
//---------------------------------------------------------------------------

void __fastcall TSkinManagerDlg::CreateBtnClick(TObject *Sender)
{
 if(OpenDialog1->Execute())
 {
  AnsiString FName=OpenDialog1->FileName;
  AnsiString SName=NameEdt->Text;
  ENModelData::CreateSkin(SName.c_str(),FName.c_str());
  UpdateSkinList();
  UpdateCurrentSkin();
 }
}
//---------------------------------------------------------------------------

void __fastcall TSkinManagerDlg::RenameBtnClick(TObject *Sender)
{
 AnsiString SName=NameEdt->Text;
 if(SkinList->ItemIndex>=0)
 {
  ENModelData::RenameSkin(SkinList->ItemIndex,SName.c_str());
  UpdateSkinList();
 }
 else
   ShowMessage("You must select a skin!!!");
}
//---------------------------------------------------------------------------

void __fastcall TSkinManagerDlg::DeleteBtnClick(TObject *Sender)
{
 if(SkinList->ItemIndex>=0)
 {
  ENModelData::DeleteSkin(SkinList->ItemIndex);
  UpdateSkinList();
  UpdateCurrentSkin();
 }
 else
   ShowMessage("You must select a skin!!!");
}
//---------------------------------------------------------------------------

void __fastcall TSkinManagerDlg::SkinListClick(TObject *Sender)
{
 if(SkinList->ItemIndex>=0)
 {
  AnsiString SName=SkinList->Items->Strings[SkinList->ItemIndex];
  NameEdt->Text=SName;
 }
}
//---------------------------------------------------------------------------

void __fastcall TSkinManagerDlg::SkinListDblClick(TObject *Sender)
{
 if(SkinList->ItemIndex>=0)
 {
  ENModelData::SetCurrentSkin(SkinList->ItemIndex);
  UpdateCurrentSkin();
 }
}
//---------------------------------------------------------------------------
void TSkinManagerDlg::UpdateCurrentSkin()
{
 if(ENModelData::GetCurrentSkin()!=-1)
 {
  AnsiString SName=ENModelData::GetSkinName(ENModelData::GetCurrentSkin());
  CurrentSkinEdt->Text=SName;
 }
 else
   CurrentSkinEdt->Clear();
}
void __fastcall TSkinManagerDlg::ExportBtnClick(TObject *Sender)
{
 if(SkinList->ItemIndex>=0)
 {
  if(SaveDialog1->Execute())
  {
   AnsiString FName=SaveDialog1->FileName;
   ENModelData::ExportSkin(FName.c_str(),(ENuint)SkinList->ItemIndex);
  }
 }
 else
   ShowMessage("You must select a skin!!!");
}
//---------------------------------------------------------------------------

void __fastcall TSkinManagerDlg::FormCreate(TObject *Sender)
{
 AnsiString FileExt;
 FileExt=ENPicture::GetImageExtStr();
 FileExt+="|Engine material(*.emat)|*.emat";
 OpenDialog1->Filter=FileExt;
}
//---------------------------------------------------------------------------

