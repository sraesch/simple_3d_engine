//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "AddSpriteFrame.h"
#include "SpriteData.h"
#include "About.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainWnd *MainWnd;
//---------------------------------------------------------------------------
__fastcall TMainWnd::TMainWnd(TComponent* Owner)
        : TForm(Owner)
{
 globj=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::FormCreate(TObject *Sender)
{
 globj=new ENGLClass(Handle,true,false);
 if(!globj->Init())
   ShowMessage("Cannot initialize OpenGL!!!");
 glEnable(GL_TEXTURE_2D);
 ENSpriteData::Init();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormDestroy(TObject *Sender)
{
 delete globj;
 globj=NULL;
 ENSpriteData::Free();        
}
//---------------------------------------------------------------------------
void TMainWnd::Draw()
{
 if(globj)
 {
  ENuint mode;
  //Get current modus
  if(Normal1->Checked)
    mode=SPRITE_DRAW_NORMAL;
  if(Transparent1->Checked)
    mode=SPRITE_DRAW_TRANS;
  if(Alpha1->Checked)
    mode=SPRITE_DRAW_ALPHA;

  globj->ConfigMatrix(0,0,ClientWidth,ClientHeight);
  globj->Start();
  ENSpriteData::Draw(mode,ClientWidth,ClientHeight);
  globj->End();
 }  
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::Exit1Click(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::AddFrame1Click(TObject *Sender)
{
 AddFrameWnd->ShowModal();
 if(ENSpriteData::SpriteData->GetNumFrames())
   ScrollBar1->Max=ENSpriteData::SpriteData->GetNumFrames()-1;
 else
   ScrollBar1->Max=0;

 Draw();
}
//---------------------------------------------------------------------------


void __fastcall TMainWnd::Normal1Click(TObject *Sender)
{
 Normal1->Checked=false;
 Transparent1->Checked=false;
 Alpha1->Checked=false;
 TMenuItem *m=(TMenuItem*)Sender;
 switch(m->Tag)
 {
  case 0:
    Normal1->Checked=true;
  break;
  case 1:
    Transparent1->Checked=true;
  break;
  case 2:
    Alpha1->Checked=true;
  break;
 };

 Draw();
}
//---------------------------------------------------------------------------


void __fastcall TMainWnd::DeleteFrame1Click(TObject *Sender)
{
 ENSpriteData::DeleteFrame();
 if(ENSpriteData::SpriteData->GetNumFrames())
   ScrollBar1->Max=ENSpriteData::SpriteData->GetNumFrames()-1;
 else
   ScrollBar1->Max=ENSpriteData::SpriteData->GetNumFrames();

 ScrollBar1->Position=ENSpriteData::GetCurrent();
 StatusBar1->Panels->Items[0]->Text="Frame: "+IntToStr(ENSpriteData::GetCurrent());
 Draw(); 
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::New1Click(TObject *Sender)
{
 ENSpriteData::Clear();
 Draw();
 ScrollBar1->Max=0;
 ScrollBar1->Position=0;
 StatusBar1->Panels->Items[0]->Text="Frame: "+IntToStr(ENSpriteData::GetCurrent());
}
//---------------------------------------------------------------------------


void __fastcall TMainWnd::ScrollBar1Scroll(TObject *Sender,
      TScrollCode ScrollCode, int &ScrollPos)
{
 ENSpriteData::SetCurrentFrame(ScrollBar1->Position);
 StatusBar1->Panels->Items[0]->Text="Frame: "+IntToStr(ENSpriteData::GetCurrent());
 Draw();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Load1Click(TObject *Sender)
{
 OpenDialog1->Execute();
}
//---------------------------------------------------------------------------


void __fastcall TMainWnd::Save1Click(TObject *Sender)
{
 SaveDialog1->Execute();        
}
//---------------------------------------------------------------------------



void __fastcall TMainWnd::SaveDialog1CanClose(TObject *Sender,
      bool &CanClose)
{
 AnsiString FName=SaveDialog1->FileName;
 AnsiString ext=ExtractFileExt(FName);
 ext.LowerCase();
 if(ext!=".esp")
   FName+=".esp";

 if(!ENSpriteData::SpriteData->Save(FName.c_str()))
   ShowMessage("Cannot save file!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::OpenDialog1CanClose(TObject *Sender,
      bool &CanClose)
{
 ENSpriteData::Load(OpenDialog1->FileName.c_str());
 if(ENSpriteData::SpriteData->GetNumFrames())
   ScrollBar1->Max=ENSpriteData::SpriteData->GetNumFrames()-1;
 else
   ScrollBar1->Max=0;

 ScrollBar1->Position=0;
 StatusBar1->Panels->Items[0]->Text="Frame: "+IntToStr(ENSpriteData::GetCurrent());
 Draw();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::N1Click(TObject *Sender)
{
 AboutWnd->ShowModal();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormPaint(TObject *Sender)
{
 Draw();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormResize(TObject *Sender)
{
 Draw();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormShow(TObject *Sender)
{
 //If open command
 if(ParamCount())
 {
  ENSpriteData::Load(ParamStr(1).c_str());
  if(ENSpriteData::SpriteData->GetNumFrames())
    ScrollBar1->Max=ENSpriteData::SpriteData->GetNumFrames()-1;
  else
    ScrollBar1->Max=0;

  ScrollBar1->Position=0;
  StatusBar1->Panels->Items[0]->Text="Frame: "+IntToStr(ENSpriteData::GetCurrent());
  Draw();
 }
}
//---------------------------------------------------------------------------

