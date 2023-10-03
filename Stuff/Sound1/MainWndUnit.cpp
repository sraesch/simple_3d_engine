//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MainWndUnit.h"

#pragma comment (lib,"EngineSystem.lib")
#pragma comment (lib,"EngineMedia.lib")
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainWnd *MainWnd;
//---------------------------------------------------------------------------
__fastcall TMainWnd::TMainWnd(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::FormCreate(TObject *Sender)
{
 snddev=new ENSoundDevice();
 lpack=new ENPackerLoaded();
 //Load sound
 if(lpack->Load("packages\\pack.enginepack"))
 {
  ENSoundLoaded *sndl=(ENSoundLoaded*)lpack->GetSource("sound1",ENPackerBase::ENSOUND);
  if(sndl)
  {
   ssnd=new ENSoundSource(sndl);
   ssnd->SetRange(100.0f);
   ssnd->SetPosition(ENVector(0.0f,0.0f,-4.0f));
   ssnd->SetSpeed(1.0f);
   ssnd->SetVolume(1.0f);
   ssnd->SetDir(ENVecDir(1.0f,ENVector2(0.0f,0.0f)));
   ssnd->SetAngle(360.0f);
   ssnd->SetLoopMode(1);
   ssnd->Play();
  }
  else
    ShowMessage("Cannot find resource sound1!!!");
 }
 else
   ShowMessage("Cannot load package!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormDestroy(TObject *Sender)
{
 if(ssnd) delete ssnd;
 delete lpack;
 delete snddev;
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::Button2Click(TObject *Sender)
{
 if(ssnd)
   ssnd->Play();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Button1Click(TObject *Sender)
{
 if(ssnd)
   ssnd->Stop();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Button3Click(TObject *Sender)
{
 if(ssnd)
   ssnd->Pause();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::CheckBox1Click(TObject *Sender)
{
 if(ssnd)
   ssnd->SetLoopMode(CheckBox1->Checked);        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::TrackBar1Change(TObject *Sender)
{
 DistLbl->Caption=TrackBar1->Position;
 if(ssnd)
   ssnd->SetPosition(ENVector(TrackBar1->Position,0.0f,0.0f));       
}
//---------------------------------------------------------------------------

