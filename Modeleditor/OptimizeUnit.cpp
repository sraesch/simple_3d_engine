//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdlib.h>
#pragma hdrstop

#include "OptimizeUnit.h"
#include "ModelData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOptimizeDlg *OptimizeDlg;
//---------------------------------------------------------------------------
__fastcall TOptimizeDlg::TOptimizeDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TOptimizeDlg::VertexesChClick(TObject *Sender)
{
 ToleranzEdt->Enabled=VertexesCh->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptimizeDlg::FormShow(TObject *Sender)
{
 UnVertexesCh->Checked=true;
 VertexesCh->Checked=true;
 ToleranzEdt->Enabled=true;         
}
//---------------------------------------------------------------------------
void __fastcall TOptimizeDlg::Button1Click(TObject *Sender)
{
 ENbool UnusedVerts=UnVertexesCh->Checked;
 ENbool Verts=VertexesCh->Checked;
 ENfloat toleranz=ENAbsolute((atof(ToleranzEdt->Text.c_str())));
 ENModelData::Optimize(UnusedVerts,Verts,toleranz);
 Close();
}
//---------------------------------------------------------------------------
