//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SoundDlgUnit.h"
#include "MapData.h"
#include "MapSources.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSoundDlg *SoundDlg;
//---------------------------------------------------------------------------
__fastcall TSoundDlg::TSoundDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSoundDlg::CloseBtnClick(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
ENbool TSoundDlg::CheckSoundData()
{
 //Sound range
 if(sound.range<=0.0f)
 {
  ShowMessage("Sound range must be over 0.0!!!");
  return false;
 }
 //Sound speed
 if(sound.speed<=0.0f)
 {
  ShowMessage("Sound speed must be over 0.0!!!");
  return false;
 }
 //Sound volume
 if(sound.volume<0.0f)
 {
  ShowMessage("Sound speed must be over 0.0 or 0.0!!!");
  return false;
 }
 //Sound angle
 if(sound.angle<0.0f||sound.angle>360.0f)
 {
  ShowMessage("Sound angle must in the range between 0.0 and 360.0!!!");
  return false;
 }
 //Finished
 return true;
}

void TSoundDlg::NewSoundDlg(ENVector pos)
{
 //Init sound
 sound.pos=pos;
 sound.dir=ENVector2(0.0f,0.0f);
 sound.range=100.0f;
 sound.speed=1.0f;
 sound.volume=1.0f;
 sound.angle=360.0f;
 sound.loop=false;
 *sound.src=0;
 //Show sound data
 EdtXPos->Text=FloatToStr(sound.pos.v[0]);
 EdtYPos->Text=FloatToStr(sound.pos.v[1]);
 EdtZPos->Text=FloatToStr(sound.pos.v[2]);
 EdtXDir->Text=FloatToStr(sound.dir.v[0]);
 EdtYDir->Text=FloatToStr(sound.dir.v[1]);
 EdtRange->Text=FloatToStr(sound.range);
 EdtSpot->Text=FloatToStr(sound.angle);
 EdtVol->Text=FloatToStr(sound.volume);
 EdtSpeed->Text=FloatToStr(sound.speed);
 LoopSnd->Checked=sound.loop;
 EdtSndSrc->Text=sound.src;
 //Set new caption
 Caption="Add new sound...";
 //Show dialog
 ShowModal();
}

void TSoundDlg::EditSoundDlg(ENSoundSrc sounddata)
{
 //Init sound
 sound=sounddata;
 //Show sound data
 EdtXPos->Text=FloatToStr(sound.pos.v[0]);
 EdtYPos->Text=FloatToStr(sound.pos.v[1]);
 EdtZPos->Text=FloatToStr(sound.pos.v[2]);
 EdtXDir->Text=FloatToStr(sound.dir.v[0]);
 EdtYDir->Text=FloatToStr(sound.dir.v[1]);
 EdtRange->Text=FloatToStr(sound.range);
 EdtSpot->Text=FloatToStr(sound.angle);
 EdtVol->Text=FloatToStr(sound.volume);
 EdtSpeed->Text=FloatToStr(sound.speed);
 LoopSnd->Checked=sound.loop;
 EdtSndSrc->Text=sound.src;
 //Set new caption
 Caption="Edit sound...";
 //Show dialog
 ShowModal();
}

ENSoundSrc TSoundDlg::GetSound()
{
 return sound;
}

ENbool TSoundDlg::isDataValid()
{
 return DataOkay;
}



void __fastcall TSoundDlg::OKBtnClick(TObject *Sender)
{
 //Get position
 sound.pos.v[0]=atof(EdtXPos->Text.c_str());
 sound.pos.v[1]=atof(EdtYPos->Text.c_str());
 sound.pos.v[2]=atof(EdtZPos->Text.c_str());
 //Get Direction
 sound.dir.v[0]=atof(EdtXDir->Text.c_str());
 sound.dir.v[1]=atof(EdtYDir->Text.c_str());
 //Get sound source
 strcpy(sound.src,EdtSndSrc->Text.c_str());
 //Get sound range and spot cutoff
 sound.range=atof(EdtRange->Text.c_str());
 sound.angle=atof(EdtSpot->Text.c_str());
 //Get loop flag, volume and speed
 sound.speed=atof(EdtSpeed->Text.c_str());
 sound.volume=atof(EdtVol->Text.c_str());
 sound.loop=LoopSnd->Checked;
 //Check data
 DataOkay=CheckSoundData();
 //Finished
 Close();
}
//---------------------------------------------------------------------------

void __fastcall TSoundDlg::FormShow(TObject *Sender)
{
 ENMapSources::GetPacks(SoundsBox,ENPackerBase::ENSOUND);
}
//---------------------------------------------------------------------------

void __fastcall TSoundDlg::SoundsBoxClick(TObject *Sender)
{
 EdtSndSrc->Text=SoundsBox->Items->Strings[SoundsBox->ItemIndex];
}
//---------------------------------------------------------------------------

