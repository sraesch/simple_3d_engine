//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ImportQuake3Dlg.h"
#include "MapData.h"
#include "ControlViews.h"
#include "MainWndUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TImportQuake3 *ImportQuake3;
//---------------------------------------------------------------------------
__fastcall TImportQuake3::TImportQuake3(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TImportQuake3::SearchBtnClick(TObject *Sender)
{
 if(OpenDialog1->Execute())
   FileNameEdt->Text=OpenDialog1->FileName;
}
//---------------------------------------------------------------------------
void __fastcall TImportQuake3::CloseBtnClick(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
void __fastcall TImportQuake3::ApplyBtnClick(TObject *Sender)
{
 ENImportQ3Flags flags;

 flags.mesh=MeshChb->Checked;
 flags.billb=BillChb->Checked;
 flags.patches=PatchChb->Checked;
 flags.textures=TexChb->Checked;
 flags.polygones=PolyChb->Checked;
 strcpy(flags.TexturePath,DirEdt->Text.c_str());

 MainWnd->SaveQuestion();
 ENMapData::New();
 ENMapData::ImportFromQuake3(FileNameEdt->Text.c_str(),flags);

 ENViewControl::DrawAll();
 MainWnd->UpdateTextureList();
 MainWnd->UpdateGroupList();

 Close();
}
//---------------------------------------------------------------------------
