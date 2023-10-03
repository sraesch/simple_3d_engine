//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SkyDlgUnit.h"
#include "MapData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSkyDlg *SkyDlg;
//---------------------------------------------------------------------------
__fastcall TSkyDlg::TSkyDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSkyDlg::FormShow(TObject *Sender)
{
 SkyBoxList->ItemIndex=0;
 UpdateSky();        
}
//---------------------------------------------------------------------------
void TSkyDlg::UpdateSky()
{
 ENPicture *pic=new ENPicture();
 ENMapData::GetSky((ENuint)SkyBoxList->ItemIndex,pic);
 ENMapData::GetPreview(PreviewImage->Picture->Bitmap,pic,128,128);
 delete pic;
}

void __fastcall TSkyDlg::SkyBoxListClick(TObject *Sender)
{
 UpdateSky();        
}
//---------------------------------------------------------------------------

void __fastcall TSkyDlg::ClearBtnClick(TObject *Sender)
{
 ENPicture *pic=new ENPicture();
 ENMapData::SetSky((ENuint)SkyBoxList->ItemIndex,pic);
 delete pic;

 UpdateSky();
}
//---------------------------------------------------------------------------


void __fastcall TSkyDlg::SetBtnClick(TObject *Sender)
{
 char cFileName[MAX_PATH];

 if(OpenDialog1->Execute())
 {
  //Get filename
  AnsiString FName=OpenDialog1->FileName;
  strcpy(cFileName,FName.c_str());
  //Try to load filename
  ENPicture *pic=new ENPicture();
  if(!pic->Load(cFileName))
  {
   ShowMessage("Cannot read "+FName+"!!!");
   delete pic;
  }
  else
  {
   ENMapData::SetSky((ENuint)SkyBoxList->ItemIndex,pic);
   delete pic;
   UpdateSky();
  }
 }
}
//---------------------------------------------------------------------------


void TSkyDlg::UpdateSun()
{
 ENMapBase::ENMapSun sun;
 ENMapData::GetSun(&sun);
 SunColorPnl->Color=(TColor)RGB(sun.SunColor[0],sun.SunColor[1],sun.SunColor[2]);
 SunChb->Checked=sun.Enable;
 EdtXDir->Text=sun.dir.v[0];
 EdtYDir->Text=sun.dir.v[1];
}

ENfloat TSkyDlg::ConvertStrToFloat(AnsiString str)
{
 str=str.TrimLeft();
 return atof(str.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TSkyDlg::SunSheetShow(TObject *Sender)
{
 UpdateSun();        
}
//---------------------------------------------------------------------------

void __fastcall TSkyDlg::ResetSunBtnClick(TObject *Sender)
{
 UpdateSun();        
}
//---------------------------------------------------------------------------


void __fastcall TSkyDlg::ApplySunBtnClick(TObject *Sender)
{
 ENMapBase::ENMapSun sun;
 sun.Enable=SunChb->Checked;
 sun.dir.v[0]=ConvertStrToFloat(EdtXDir->Text);
 sun.dir.v[1]=ConvertStrToFloat(EdtYDir->Text);

 ENuint suncol=SunColorPnl->Color;
 sun.SunColor[0]=(ENubyte)GetRValue(suncol);
 sun.SunColor[1]=(ENubyte)GetGValue(suncol);
 sun.SunColor[2]=(ENubyte)GetBValue(suncol);

 ENMapData::SetSun(&sun);
}
//---------------------------------------------------------------------------

void __fastcall TSkyDlg::SunColorPnlClick(TObject *Sender)
{
 ColorDialog1->Color=SunColorPnl->Color;
 if(ColorDialog1->Execute())
   SunColorPnl->Color=ColorDialog1->Color;
}
//---------------------------------------------------------------------------

void __fastcall TSkyDlg::FormCreate(TObject *Sender)
{
 OpenDialog1->Filter=ENPicture::GetImageExtStr();
}
//---------------------------------------------------------------------------

