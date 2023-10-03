//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdlib.h>
#pragma hdrstop

#include "LightDlgUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLightDlg *LightDlg;
//---------------------------------------------------------------------------
__fastcall TLightDlg::TLightDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TLightDlg::CloseBtnClick(TObject *Sender)
{
 DataOkay=false;
 Close();        
}
//---------------------------------------------------------------------------
void TLightDlg::NewLightDlg(ENVector pos)
{
 //Init light
 light.pos=pos;
 light.color=ENColor(255,255,255,255);
 light.dir=ENVector2(0,0);
 light.range=100.0f;
 light.spotcut=180.0f;
 //Show light data
 EdtXPos->Text=FloatToStr(light.pos.v[0]);
 EdtYPos->Text=FloatToStr(light.pos.v[1]);
 EdtZPos->Text=FloatToStr(light.pos.v[2]);
 EdtXDir->Text=FloatToStr(light.dir.v[0]);
 EdtYDir->Text=FloatToStr(light.dir.v[1]);
 EdtRange->Text=FloatToStr(light.range);
 EdtSpot->Text=FloatToStr(light.spotcut);
 ColorPnl->Color=(TColor)RGB(light.color.r,light.color.g,light.color.b);
 //Set new caption
 Caption="Add new light...";
 //Show dialog
 ShowModal();
}

void TLightDlg::EditLightDlg(ENLight lightdata)
{
 //Init light
 light=lightdata;
 //Show light data
 EdtXPos->Text=FloatToStr(light.pos.v[0]);
 EdtYPos->Text=FloatToStr(light.pos.v[1]);
 EdtZPos->Text=FloatToStr(light.pos.v[2]);
 EdtXDir->Text=FloatToStr(light.dir.v[0]);
 EdtYDir->Text=FloatToStr(light.dir.v[1]);
 EdtRange->Text=FloatToStr(light.range);
 EdtSpot->Text=FloatToStr(light.spotcut);
 ColorPnl->Color=(TColor)RGB(light.color.r,light.color.g,light.color.b);
 //Set new caption
 Caption="Edit light...";
 //Show dialog
 ShowModal();
}

ENLight TLightDlg::GetLight()
{
 return light;
}

void __fastcall TLightDlg::ColorPnlClick(TObject *Sender)
{
 ColorDialog1->Color=ColorPnl->Color;
 if(ColorDialog1->Execute())
   ColorPnl->Color=ColorDialog1->Color;        
}
//---------------------------------------------------------------------------
ENbool TLightDlg::CheckLightData()
{
 //Light spot cutoff
 if(light.spotcut<0.0f||light.spotcut>180.0f)
 {
  ShowMessage("Spot cutoff must be in the range of 0.0 to 180.0!!!");
  return false;
 }
 //Finished
 return true;
}

void __fastcall TLightDlg::OKBtnClick(TObject *Sender)
{
 //Get light color
 light.color.r=GetRValue(ColorPnl->Color);
 light.color.g=GetGValue(ColorPnl->Color);
 light.color.b=GetBValue(ColorPnl->Color);
 //Get position
 light.pos.v[0]=atof(EdtXPos->Text.c_str());
 light.pos.v[1]=atof(EdtYPos->Text.c_str());
 light.pos.v[2]=atof(EdtZPos->Text.c_str());
 //Get Direction
 light.dir.v[0]=atof(EdtXDir->Text.c_str());
 light.dir.v[1]=atof(EdtYDir->Text.c_str());
 //Get light range and spot cutoff
 light.range=atof(EdtRange->Text.c_str());
 light.spotcut=atof(EdtSpot->Text.c_str());
 //Check data
 DataOkay=CheckLightData();
 //Finished
 Close();
}

ENbool TLightDlg::isDataValid()
{
 return DataOkay;
}
//---------------------------------------------------------------------------

