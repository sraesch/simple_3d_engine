//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdlib.h>
#pragma hdrstop

#include "AddCubeUnit.h"
#include "ModelPrefabs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAddCubeDlg *AddCubeDlg;
//---------------------------------------------------------------------------
__fastcall TAddCubeDlg::TAddCubeDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAddCubeDlg::Button2Click(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
void TAddCubeDlg::SetPosValues(ENVector pos)
{
 EdtXPos->Text=FloatToStr(pos.v[0]);
 EdtYPos->Text=FloatToStr(pos.v[1]);
 EdtZPos->Text=FloatToStr(pos.v[2]);
}

ENfloat TAddCubeDlg::ConvertStrToFloat(AnsiString str)
{
 str=str.TrimLeft();
 return atof(str.c_str());
}


void __fastcall TAddCubeDlg::Button1Click(TObject *Sender)
{
 ENVector size,pos;
 //Convert from string to floating point
 pos.v[0]=ConvertStrToFloat(EdtXPos->Text);
 pos.v[1]=ConvertStrToFloat(EdtYPos->Text);
 pos.v[2]=ConvertStrToFloat(EdtZPos->Text);
 size.v[0]=ConvertStrToFloat(EdtXSize->Text);
 size.v[1]=ConvertStrToFloat(EdtYSize->Text);
 size.v[2]=ConvertStrToFloat(EdtZSize->Text);
 //Check values
 if(size.v[0]*size.v[1]*size.v[2]!=0.0f)
 {
  ENModelPrefabs::AddCube(pos,size);
  Close();
 }
 else
   ShowMessage("Error: Invalid values!!!");
}
//---------------------------------------------------------------------------

