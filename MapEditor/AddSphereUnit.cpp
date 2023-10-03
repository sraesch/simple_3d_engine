//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdlib.h>
#pragma hdrstop

#include "AddSphereUnit.h"
#include "MapPrefabs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAddSphereDlg *AddSphereDlg;
//---------------------------------------------------------------------------
__fastcall TAddSphereDlg::TAddSphereDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TAddSphereDlg::SetPosValues(ENVector pos)
{
 EdtXPos->Text=FloatToStr(pos.v[0]);
 EdtYPos->Text=FloatToStr(pos.v[1]);
 EdtZPos->Text=FloatToStr(pos.v[2]);
}

ENfloat TAddSphereDlg::ConvertStrToFloat(AnsiString str)
{
 str=str.TrimLeft();
 return atof(str.c_str());
}

ENint TAddSphereDlg::ConvertStrToInt(AnsiString str)
{
 str=str.TrimLeft();
 return atoi(str.c_str());
}
void __fastcall TAddSphereDlg::BtnCloseClick(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
void __fastcall TAddSphereDlg::Button1Click(TObject *Sender)
{
 ENVector pos;
 ENfloat range;
 ENuint  details;
 //Convert from string to floating point
 pos.v[0]=ConvertStrToFloat(EdtXPos->Text);
 pos.v[1]=ConvertStrToFloat(EdtYPos->Text);
 pos.v[2]=ConvertStrToFloat(EdtZPos->Text);
 range=ConvertStrToFloat(EdtRange->Text);
 details=ConvertStrToInt(EdtDetails->Text);
 //Check values
 if(range!=0.0f||details>0||details<=10000)
 {
  ENMapPrefabs::AddSphere(pos,range,details);
  Close();
 }
 else
   ShowMessage("Error: Invalid values!!!");
}
//---------------------------------------------------------------------------
