//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "About.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TENMainWnd *ENMainWnd;
//---------------------------------------------------------------------------
__fastcall TENMainWnd::TENMainWnd(TComponent* Owner)
        : TForm(Owner)
{
 globj=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TENMainWnd::FormCreate(TObject *Sender)
{
 globj=new ENGLClass(Handle,true,false);
 if(!globj->Init())
   ShowMessage("Cannot init OpenGL!!!");

 glEnable(GL_TEXTURE_2D);   
 mode=EN_MATERIALDRAW_BASE;
 ENMaterialData::Init();

 OpenDialog1->Filter=ENPicture::GetImageExtStr();
}
//---------------------------------------------------------------------------

void __fastcall TENMainWnd::FormDestroy(TObject *Sender)
{
 ENMaterialData::Free();
 delete globj;
 globj=NULL;
}
//---------------------------------------------------------------------------

void __fastcall TENMainWnd::Exit1Click(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------

void __fastcall TENMainWnd::New1Click(TObject *Sender)
{
 ENMaterialData::Clear();
 Draw();        
}
//---------------------------------------------------------------------------



void __fastcall TENMainWnd::Setbase1Click(TObject *Sender)
{
 char errorstr[80];

 if(OpenDialog1->Execute())
 {
  AnsiString FName=OpenDialog1->FileName;
  if(ColorDialog1->Execute())
  {
   TColor tcol=ColorDialog1->Color;
   ENubyte r=GetRValue(tcol);
   ENubyte g=GetGValue(tcol);
   ENubyte b=GetBValue(tcol);
   ENPicture *pic=new ENPicture();
   if(pic->Load(FName.c_str()))
   {
    ENMaterialData::mat->SetBaseData(pic,r,g,b);
    ENMaterialData::SetTexture(0);
    delete pic;
   }
   else
   {
    pic->GetLastErrorStr(errorstr);
    ShowMessage(errorstr);
   }
  }
 }

 Draw();
}
//---------------------------------------------------------------------------
void __fastcall TENMainWnd::SwitchMode(TObject *Sender)
{
 mode=((TMenuItem*)Sender)->Tag;
 Base1->Checked=false;
 Transparent1->Checked=false;
 Alpha1->Checked=false;
 Normalmap1->Checked=false;
 Luminancemap1->Checked=false;
 switch(mode)
 {
  case EN_MATERIALDRAW_BASE:
    Base1->Checked=true;
  break;
  case EN_MATERIALDRAW_TRANS:
    Transparent1->Checked=true;
  break;
  case EN_MATERIALDRAW_ALPHA:
    Alpha1->Checked=true;
  break;
  case EN_MATERIALDRAW_BUMP:
    Normalmap1->Checked=true;
  break;
  case EN_MATERIALDRAW_LUMI:
    Luminancemap1->Checked=true;
  break;
 };

 Draw();
}
//---------------------------------------------------------------------------
void __fastcall TENMainWnd::Setalpha1Click(TObject *Sender)
{
 char errorstr[80];

 if(OpenDialog1->Execute())
 {
  AnsiString FName=OpenDialog1->FileName;
  ENPicture *pic=new ENPicture();
  if(pic->Load(FName.c_str()))
  {
   if(pic->GetBpp()<=3)
     pic->SetStdAlpha();
   if(!ENMaterialData::mat->SetAlphaData(pic))
     ShowMessage("Cannot set alpha data!!!");
   else
     ENMaterialData::SetTexture(1);

   delete pic;
  }
  else
  {
   pic->GetLastErrorStr(errorstr);
   ShowMessage(errorstr);
  }
 }
 Draw();
}
//---------------------------------------------------------------------------

void __fastcall TENMainWnd::SetNormalmap1Click(TObject *Sender)
{
 char errorstr[80];

 if(OpenDialog1->Execute())
 {
  AnsiString FName=OpenDialog1->FileName;
  ENPicture *pic=new ENPicture();
  if(pic->Load(FName.c_str()))
  {
   ENMaterialData::mat->SetBumpData(pic);
   ENMaterialData::SetTexture(2);
   delete pic;
  }
  else
  {
   pic->GetLastErrorStr(errorstr);
   ShowMessage(errorstr);
  }
 }
 Draw();
}
//---------------------------------------------------------------------------

void __fastcall TENMainWnd::Save1Click(TObject *Sender)
{
 if(SaveDialog1->Execute())
 {
  AnsiString FName=SaveDialog1->FileName;
  AnsiString ext=ExtractFileExt(FName);
  if(ext.LowerCase()!=".emat")
    FName+=".emat";

  if(!ENMaterialData::mat->Save(FName.c_str()))
    ShowMessage("Cannot save file!!!");
 }
}
//---------------------------------------------------------------------------

void __fastcall TENMainWnd::Load1Click(TObject *Sender)
{
 if(OpenDialog2->Execute())
 {
  AnsiString FName=OpenDialog2->FileName;
  if(!ENMaterialData::mat->Load(FName.c_str()))
    ShowMessage("Cannot load file!!!");
  else
  {
   ENMaterialData::SetTexture(0);
   ENMaterialData::SetTexture(1);
   ENMaterialData::SetTexture(2);
   ENMaterialData::SetTexture(3);
  }
 }
 Draw();
}
//---------------------------------------------------------------------------

void __fastcall TENMainWnd::About1Click(TObject *Sender)
{
 AboutWnd->ShowModal();
}
//---------------------------------------------------------------------------

void TENMainWnd::Draw()
{
 if(globj)
 {
  globj->ConfigMatrix(0,0,ClientWidth,ClientHeight);
  globj->Start();
  ENMaterialData::Draw(mode,ClientWidth,ClientHeight);
  globj->End();
 }
}
//---------------------------------------------------------------------------

void __fastcall TENMainWnd::FormResize(TObject *Sender)
{
 Draw();        
}
//---------------------------------------------------------------------------

void __fastcall TENMainWnd::FormPaint(TObject *Sender)
{
 Draw();
}
//---------------------------------------------------------------------------

void __fastcall TENMainWnd::FormShow(TObject *Sender)
{
 if(ParamCount())
 {
  AnsiString FName=ParamStr(1);
  if(!ENMaterialData::mat->Load(FName.c_str()))
    ShowMessage("Cannot load file!!!");
  else
  {
   ENMaterialData::SetTexture(0);
   ENMaterialData::SetTexture(1);
   ENMaterialData::SetTexture(2);
   ENMaterialData::SetTexture(3);
  }
 }
}
//---------------------------------------------------------------------------

void __fastcall TENMainWnd::Setluminancemap1Click(TObject *Sender)
{
 char errorstr[80];

 if(OpenDialog1->Execute())
 {
  AnsiString FName=OpenDialog1->FileName;
  ENPicture *pic=new ENPicture();
  if(pic->Load(FName.c_str()))
  {
   ENMaterialData::mat->SetLumiData(pic);
   ENMaterialData::SetTexture(3);
   delete pic;
  }
  else
  {
   pic->GetLastErrorStr(errorstr);
   ShowMessage(errorstr);
  }
 }
 Draw();
}
//---------------------------------------------------------------------------

void __fastcall TENMainWnd::basemap1Click(TObject *Sender)
{
 if(SaveDialog2->Execute())
 {
  AnsiString FName=SaveDialog2->FileName;
  ENPicture *pic=new ENPicture();
  ENMaterialData::mat->GetBase(pic);
  pic->WritePNG(FName.c_str());
  delete pic;
 }
}
//---------------------------------------------------------------------------

void __fastcall TENMainWnd::alphamap1Click(TObject *Sender)
{
 if(SaveDialog2->Execute())
 {
  AnsiString FName=SaveDialog2->FileName;
  ENPicture *pic=new ENPicture();
  ENMaterialData::mat->GetAlpha(pic);
  pic->WritePNG(FName.c_str());
  delete pic;
 }
}
//---------------------------------------------------------------------------

void __fastcall TENMainWnd::normalmap2Click(TObject *Sender)
{
 if(SaveDialog2->Execute())
 {
  AnsiString FName=SaveDialog2->FileName;
  ENPicture *pic=new ENPicture();
  ENMaterialData::mat->GetBump(pic);
  pic->WritePNG(FName.c_str());
  delete pic;
 }
}
//---------------------------------------------------------------------------

void __fastcall TENMainWnd::lumimapClick(TObject *Sender)
{
 if(SaveDialog2->Execute())
 {
  AnsiString FName=SaveDialog2->FileName;
  ENPicture *pic=new ENPicture();
  ENMaterialData::mat->GetLumi(pic);
  pic->WritePNG(FName.c_str());
  delete pic;
 }
}
//---------------------------------------------------------------------------

