//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdlib.h>
#pragma hdrstop

#include "AddConeUnit.h"
#include "MapPrefabs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAddConeDlg *AddConeDlg;
//---------------------------------------------------------------------------
__fastcall TAddConeDlg::TAddConeDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAddConeDlg::BtnCloseClick(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
void TAddConeDlg::SetPosValues(ENVector pos)
{
 EdtXPos->Text=FloatToStr(pos.v[0]);
 EdtYPos->Text=FloatToStr(pos.v[1]);
 EdtZPos->Text=FloatToStr(pos.v[2]);
}

ENfloat TAddConeDlg::ConvertStrToFloat(AnsiString str)
{
 str=str.TrimLeft();
 return atof(str.c_str());
}

ENint TAddConeDlg::ConvertStrToInt(AnsiString str)
{
 str=str.TrimLeft();
 return atoi(str.c_str());
}

void __fastcall TAddConeDlg::Button1Click(TObject *Sender)
{
 ENVector pos;
 ENfloat range,height;
 ENuint  details;
 //Convert from string to floating point
 pos.v[0]=ConvertStrToFloat(EdtXPos->Text);
 pos.v[1]=ConvertStrToFloat(EdtYPos->Text);
 pos.v[2]=ConvertStrToFloat(EdtZPos->Text);
 range=ConvertStrToFloat(EdtRange->Text);
 height=ConvertStrToFloat(EdtHeight->Text);
 details=ConvertStrToInt(EdtDetails->Text);
 //Check values
 if(range!=0.0f||details>0||details<=10000)
 {
  ENMapPrefabs::AddCone(pos,range,height,details);
  Close();
 }
 else
   ShowMessage("Error: Invalid values!!!");
}
//---------------------------------------------------------------------------
