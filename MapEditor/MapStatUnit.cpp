//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MapStatUnit.h"
#include "MapData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMapStatDlg *MapStatDlg;
//---------------------------------------------------------------------------
__fastcall TMapStatDlg::TMapStatDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMapStatDlg::FormShow(TObject *Sender)
{
 ENMapBase::ENMapHeader header=ENMapData::GetHeader();
 StatTextField->Lines->Clear();
 StatTextField->Lines->Add("Map status:");
 StatTextField->Lines->Add("-------------------");
 StatTextField->Lines->Add("ID: "+AnsiString(header.ID));
 StatTextField->Lines->Add("Package: "+AnsiString(header.PackageFile));
 StatTextField->Lines->Add("Number of Vertexes: "+IntToStr(header.NumVertexes));
 StatTextField->Lines->Add("Number of Faces: "+IntToStr(header.NumFaces));
 StatTextField->Lines->Add("Number of Textures: "+IntToStr(header.NumTextures));
 StatTextField->Lines->Add("Number of Groups: "+IntToStr(header.NumGroups));
 StatTextField->Lines->Add("Number of Lights: "+IntToStr(header.NumLights));
 StatTextField->Lines->Add("Number of Objects: "+IntToStr(header.NumObjects));
 StatTextField->Lines->Add("Number of Sounds: "+IntToStr(header.NumSounds));
}
//---------------------------------------------------------------------------

void __fastcall TMapStatDlg::OKBtnClick(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------

