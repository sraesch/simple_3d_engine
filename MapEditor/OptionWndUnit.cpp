//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop

#include "OptionWndUnit.h"
#include "MapData.h"
#include "SkyDlgUnit.h"
#include "CameraDlgUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOptionWnd *OptionWnd;
//---------------------------------------------------------------------------
__fastcall TOptionWnd::TOptionWnd(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
TColor TOptionWnd::VecToColor(ENVector col)
{
 return (TColor)RGB(col.v[0]*255,col.v[1]*255,col.v[2]*255);
}

ENVector TOptionWnd::ColorToVec(TColor col)
{
 ENuint r,g,b;
 r=GetRValue(col);
 g=GetGValue(col);
 b=GetBValue(col);
 return ENVector((ENfloat)r/255.0f,(ENfloat)g/255.0f,(ENfloat)b/255.0f);
}

void __fastcall TOptionWnd::FormShow(TObject *Sender)
{
 drawoptions=ENDrawClass::GetOptions();
 //Set draw flags
 Cullfacing->Checked=drawoptions.Cullfacing;
 EnableVerts->Checked=drawoptions.EnableDrawVertexes;
 EnableFaces->Checked=drawoptions.EnableDrawFaces;
 EnableGrid->Checked=drawoptions.EnableDrawGrid;
 EnableOrigin->Checked=drawoptions.EnableDrawOrigin;
 VertexColor->Color=VecToColor(drawoptions.VertexColor);
 SVertexColor->Color=VecToColor(drawoptions.SVertexColor);
 FaceColor->Color=VecToColor(drawoptions.FaceColor);
 SFaceColor->Color=VecToColor(drawoptions.SFaceColor);
 GridColor->Color=VecToColor(drawoptions.GridColor);
 ToolColor->Color=VecToColor(drawoptions.ToolColor);
 BackColor->Color=VecToColor(drawoptions.BackColor);
 SLightColor->Color=VecToColor(drawoptions.SLightColor);
 ObjColor->Color=VecToColor(drawoptions.ObjColor);
 SObjColor->Color=VecToColor(drawoptions.SObjColor);
 NumGridLines->Position=drawoptions.NumGridLines;
 SizeGridBar->Position=ENLogBase2((ENuint)drawoptions.GridSize);
 //Set map settings
 ENMapBase::ENMapHeader header=ENMapData::GetHeader();
 PackEdt->Text=header.PackageFile;
}
//---------------------------------------------------------------------------
void __fastcall TOptionWnd::ApplyBtnClick(TObject *Sender)
{
 //Editor options
 ENDrawClass::SetOptions(drawoptions);
 //Map settings
 char PackageFile[MAX_PATH];
 strcpy(PackageFile,PackEdt->Text.c_str());
 if(*PackageFile) ENMapData::SetPackage(PackageFile);
 //Close window
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TOptionWnd::CullfacingClick(TObject *Sender)
{
 drawoptions.Cullfacing=Cullfacing->Checked;
}
//---------------------------------------------------------------------------
void TOptionWnd::ChangePanelColor(TPanel *panel)
{
 ColorDialog1->Color=panel->Color;
 if(ColorDialog1->Execute())
   panel->Color=ColorDialog1->Color;


}

void __fastcall TOptionWnd::VertexColorClick(TObject *Sender)
{
 ChangePanelColor(VertexColor);
 drawoptions.VertexColor=ColorToVec(VertexColor->Color);
}
//---------------------------------------------------------------------------
void __fastcall TOptionWnd::SVertexColorClick(TObject *Sender)
{
 ChangePanelColor(SVertexColor);
 drawoptions.SVertexColor=ColorToVec(SVertexColor->Color);
}
//---------------------------------------------------------------------------
void __fastcall TOptionWnd::FaceColorClick(TObject *Sender)
{
 ChangePanelColor(FaceColor);
 drawoptions.FaceColor=ColorToVec(FaceColor->Color);
}
//---------------------------------------------------------------------------
void __fastcall TOptionWnd::SFaceColorClick(TObject *Sender)
{
 ChangePanelColor(SFaceColor);
 drawoptions.SFaceColor=ColorToVec(SFaceColor->Color);
}
//---------------------------------------------------------------------------
void __fastcall TOptionWnd::CloseBtnClick(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
void __fastcall TOptionWnd::GridColorClick(TObject *Sender)
{
 ChangePanelColor(GridColor);
 drawoptions.GridColor=ColorToVec(GridColor->Color);
}
//---------------------------------------------------------------------------
void __fastcall TOptionWnd::ToolColorClick(TObject *Sender)
{
 ChangePanelColor(ToolColor);
 drawoptions.ToolColor=ColorToVec(ToolColor->Color);
}
//---------------------------------------------------------------------------
void __fastcall TOptionWnd::BackColorClick(TObject *Sender)
{
 ChangePanelColor(BackColor);
 drawoptions.BackColor=ColorToVec(BackColor->Color);
}
//---------------------------------------------------------------------------
void __fastcall TOptionWnd::EnableVertsClick(TObject *Sender)
{
 drawoptions.EnableDrawVertexes=EnableVerts->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TOptionWnd::EnableFacesClick(TObject *Sender)
{
 drawoptions.EnableDrawFaces=EnableFaces->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TOptionWnd::EnableOriginClick(TObject *Sender)
{
 drawoptions.EnableDrawOrigin=EnableOrigin->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TOptionWnd::EnableGridClick(TObject *Sender)
{
 drawoptions.EnableDrawGrid=EnableGrid->Checked;        
}
//---------------------------------------------------------------------------

void __fastcall TOptionWnd::DefaultBtnClick(TObject *Sender)
{
 ENDrawClass::SetDefaultOptions();
 FormShow(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TOptionWnd::NumGridLinesChange(TObject *Sender)
{
 drawoptions.NumGridLines=NumGridLines->Position;
 GridLinesLbl->Caption=IntToStr(NumGridLines->Position);
}
//---------------------------------------------------------------------------

void __fastcall TOptionWnd::SizeGridBarChange(TObject *Sender)
{
 ENuint res=1;
 for(ENint a=0;a<SizeGridBar->Position;a++)
   res*=2;

 drawoptions.GridSize=(ENfloat)res;
 SizeGridLbl->Caption=IntToStr(res);
}
//---------------------------------------------------------------------------
ENuint TOptionWnd::ENLogBase2(ENuint x)
{
 ENuint res=0;
 while(x>1)
 {
  x=x>>1;
  res++;
 }
 return res;
}
void __fastcall TOptionWnd::SLightColorClick(TObject *Sender)
{
 ChangePanelColor(SLightColor);
 drawoptions.SLightColor=ColorToVec(SLightColor->Color);
}
//---------------------------------------------------------------------------

void __fastcall TOptionWnd::Button1Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
   PackEdt->Text=OpenDialog1->FileName;        
}
//---------------------------------------------------------------------------

void __fastcall TOptionWnd::ObjColorClick(TObject *Sender)
{
 ChangePanelColor(ObjColor);
 drawoptions.ObjColor=ColorToVec(ObjColor->Color);
}
//---------------------------------------------------------------------------

void __fastcall TOptionWnd::SObjColorClick(TObject *Sender)
{
 ChangePanelColor(SObjColor);
 drawoptions.SObjColor=ColorToVec(SObjColor->Color);
}
//---------------------------------------------------------------------------

void __fastcall TOptionWnd::SkyBtnClick(TObject *Sender)
{
 SkyDlg->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TOptionWnd::CamBtnClick(TObject *Sender)
{
 CameraDlg->ShowModal();        
}
//---------------------------------------------------------------------------

