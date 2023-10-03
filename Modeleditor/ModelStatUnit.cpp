//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ModelStatUnit.h"
#include "ModelData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TModelStatDlg *ModelStatDlg;
//---------------------------------------------------------------------------
__fastcall TModelStatDlg::TModelStatDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TModelStatDlg::FormShow(TObject *Sender)
{
 ENModel::ENModelHeader header=ENModelData::GetHeader();
 StatTextField->Lines->Clear();
 StatTextField->Lines->Add("Model status:");
 StatTextField->Lines->Add("-------------------");
 StatTextField->Lines->Add("Number of Frames: "+IntToStr(header.NumFrames));
 StatTextField->Lines->Add("Number of Vertexes: "+IntToStr(header.NumVertexes));
 StatTextField->Lines->Add("Number of Faces: "+IntToStr(header.NumFaces));
 StatTextField->Lines->Add("Number of Skins: "+IntToStr(header.NumSkins));
 StatTextField->Lines->Add("Number of Groups: "+IntToStr(header.NumGroups));
}
//---------------------------------------------------------------------------

void __fastcall TModelStatDlg::OKBtnClick(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------

