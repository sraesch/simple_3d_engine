//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdlib.h>
#pragma hdrstop

#include "FaceDlgUnit.h"
#include "MapData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFaceDlg *FaceDlg;
//---------------------------------------------------------------------------
__fastcall TFaceDlg::TFaceDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFaceDlg::CloseBtnClick(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------
int __fastcall TFaceDlg::ShowModal(void)
{
 ENMapBase::ENMapHeader header=ENMapData::GetHeader();
 ENbool IsSomethingSelected=false;
 ENint Flags[8]={-2,-2,-2,-2,-2,-2,-2,-2};
 for(ENuint a=0;a<header.NumFaces;a++)
   if(ENMapData::IsFaceSelected(a))
   {
    ENMapBase::ENMapFace face=ENMapData::GetFace(a);
    IsSomethingSelected=true;
    //Boolean values
    ComputeFlag(face.props.Passable,Flags[0]);
    ComputeFlag(face.props.Invisible,Flags[1]);
    ComputeFlag(face.props.Lighting,Flags[2]);
    ComputeFlag(face.props.Shadow,Flags[3]);
    ComputeFlag(face.props.Animated,Flags[4]);
    //Integer values
    ComputeFlag(face.props.Mode,Flags[5]);
    ComputeFlag(face.numframes,Flags[6]);
    ComputeFlag(face.framelen,Flags[7]);
   }
 //Boolean values
 PassableChb->State=FlagToState(Flags[0]);
 InvisibleChb->State=FlagToState(Flags[1]);
 LightingChb->State=FlagToState(Flags[2]);
 ShadowChb->State=FlagToState(Flags[3]);
 AnimatedChb->State=FlagToState(Flags[4]);
 //Integer values
 if(Flags[5]>=0) ModeGroup->ItemIndex=Flags[5]; else ModeGroup->ItemIndex=-1;
 if(Flags[6]>=0) NumFramesEdt->Text=Flags[6];   else NumFramesEdt->Clear();
 if(Flags[7]>=0) FrameLenEdt->Text=Flags[7];    else FrameLenEdt->Clear();
 //Floating point values
 EdtXPos->Clear();
 EdtYPos->Clear();
 EdtXScale->Clear();
 EdtYScale->Clear();
 EdtAngle->Clear();
 //Check if something has been selected
 if(!IsSomethingSelected)
 {
  ShowMessage("No mesh is selected!!!");
  return -1;
 }
 else
   return TForm::ShowModal();
}

void TFaceDlg::ComputeFlag(ENint flag,ENint &flt)
{
 if(flt==-2)
   flt=flag;
 else
 {
  if(flt!=flag&&flt!=-1) flt=-1;
 }
}

AnsiString TFaceDlg::FltToStr(ENfloat flt)
{
 char Buffer[30];
 sprintf(Buffer,"%f",flt);
 return AnsiString(Buffer);
}

TCheckBoxState TFaceDlg::FlagToState(ENint flag)
{
 switch(flag)
 {
  case 0:
    return cbUnchecked;
  case 1:
    return cbChecked;
  case -1:
    return cbGrayed;
 };

 return cbGrayed;
}
//---------------------------------------------------------------------------
void __fastcall TFaceDlg::ApplyBtnClick(TObject *Sender)
{
 ENMapBase::ENMapHeader header=ENMapData::GetHeader();
 for(ENuint a=0;a<header.NumFaces;a++)
   if(ENMapData::IsFaceSelected(a))
   {
    ENMapBase::ENMapFace face=ENMapData::GetFace(a);
    //Boolean values
    if(PassableChb->State!=cbGrayed) face.props.Passable=PassableChb->Checked;
    if(InvisibleChb->State!=cbGrayed) face.props.Invisible=InvisibleChb->Checked;
    if(LightingChb->State!=cbGrayed) face.props.Lighting=LightingChb->Checked;
    if(ShadowChb->State!=cbGrayed) face.props.Shadow=ShadowChb->Checked;
    if(AnimatedChb->State!=cbGrayed) face.props.Animated=AnimatedChb->Checked;
    //Integer values
    if(ModeGroup->ItemIndex>=0)    face.props.Mode=ModeGroup->ItemIndex;
    if(NumFramesEdt->Text.Length()) face.numframes=atoi(NumFramesEdt->Text.c_str());
    if(FrameLenEdt->Text.Length()) face.framelen=atoi(FrameLenEdt->Text.c_str());
    //Floating point values
    ENbool TexChange=false;
    ENVector2 texoffset=ENVector2(0.0f,0.0f),texscale=ENVector2(1.0f,1.0f);
    ENfloat texangle=0.0f;
    if(EdtXPos->Text.Length())     {TexChange=true;texoffset.v[0]=atof(EdtXPos->Text.c_str());}
    if(EdtYPos->Text.Length())     {TexChange=true;texoffset.v[1]=atof(EdtYPos->Text.c_str());}
    if(EdtXScale->Text.Length())   {TexChange=true;texscale.v[0]=atof(EdtXScale->Text.c_str());}
    if(EdtYScale->Text.Length())   {TexChange=true;texscale.v[1]=atof(EdtYScale->Text.c_str());}
    if(EdtAngle->Text.Length())    {TexChange=true;texangle=atof(EdtAngle->Text.c_str());}
    if(TexChange)                   ENMapData::CubeCoord(&face,texoffset,texscale,texangle);
    //Set face data
    ENMapData::SetFace(a,face);
   }

 Close();
}
//---------------------------------------------------------------------------
