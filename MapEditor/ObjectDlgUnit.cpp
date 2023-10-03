//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdlib.h>
#pragma hdrstop

#include "ObjectDlgUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TObjectDlg *ObjectDlg;
//---------------------------------------------------------------------------
__fastcall TObjectDlg::TObjectDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
ENMapBase::ENMapObject TObjectDlg::GetObject()
{
 return obj;
}

ENbool TObjectDlg::isDataValid()
{
 return DataOkay;
}
void __fastcall TObjectDlg::CloseBtnClick(TObject *Sender)
{
 DataOkay=false;
 Close();        
}
//---------------------------------------------------------------------------

void __fastcall TObjectDlg::FormShow(TObject *Sender)
{
 ENMapSources::GetPacks(SpritesBox,ENPackerBase::ENSPRITE);
 ENMapSources::GetPacks(ModelsBox,ENPackerBase::ENMODEL);
}
//---------------------------------------------------------------------------
void TObjectDlg::NewObjectDlg(ENVector pos)
{
 //Init object
 obj.Pos=pos;
 obj.Angle=ENVector(0.0f,0.0f,0.0f);
 obj.Scale=ENVector(1.0f,1.0f,1.0f);
 *obj.Func=0;
 *obj.Name=0;
 //Show object data
 EdtXPos->Text=FloatToStr(obj.Pos.v[0]);
 EdtYPos->Text=FloatToStr(obj.Pos.v[1]);
 EdtZPos->Text=FloatToStr(obj.Pos.v[2]);
 EdtXDir->Text=FloatToStr(obj.Angle.v[0]);
 EdtYDir->Text=FloatToStr(obj.Angle.v[1]);
 EdtZDir->Text=FloatToStr(obj.Angle.v[2]);
 EdtXScale->Text=FloatToStr(obj.Scale.v[0]);
 EdtYScale->Text=FloatToStr(obj.Scale.v[1]);
 EdtZScale->Text=FloatToStr(obj.Scale.v[2]);
 FuncEdt->Text=obj.Func;
 NameEdt->Text=obj.Name;
 //Set new caption
 Caption="Add new object...";
 //Show dialog
 ShowModal();
}

void TObjectDlg::EditObjectDlg(ENMapBase::ENMapObject objdata)
{
 //Init object
 obj=objdata;
 //Show light data
 EdtXPos->Text=FloatToStr(obj.Pos.v[0]);
 EdtYPos->Text=FloatToStr(obj.Pos.v[1]);
 EdtZPos->Text=FloatToStr(obj.Pos.v[2]);
 EdtXDir->Text=FloatToStr(obj.Angle.v[0]);
 EdtYDir->Text=FloatToStr(obj.Angle.v[1]);
 EdtZDir->Text=FloatToStr(obj.Angle.v[2]);
 EdtXScale->Text=FloatToStr(obj.Scale.v[0]);
 EdtYScale->Text=FloatToStr(obj.Scale.v[1]);
 EdtZScale->Text=FloatToStr(obj.Scale.v[2]);
 FuncEdt->Text=obj.Func;
 NameEdt->Text=obj.Name;
 //Set new caption
 Caption="Edit object...";
 //Show dialog
 ShowModal();
}

void __fastcall TObjectDlg::ApplyBtnClick(TObject *Sender)
{
 //Get position
 obj.Pos.v[0]=atof(EdtXPos->Text.c_str());
 obj.Pos.v[1]=atof(EdtYPos->Text.c_str());
 obj.Pos.v[2]=atof(EdtZPos->Text.c_str());
 //Get angle
 obj.Angle.v[0]=atof(EdtXDir->Text.c_str());
 obj.Angle.v[1]=atof(EdtYDir->Text.c_str());
 obj.Angle.v[2]=atof(EdtZDir->Text.c_str());
 //Get scale
 obj.Scale.v[0]=atof(EdtXScale->Text.c_str());
 obj.Scale.v[1]=atof(EdtYScale->Text.c_str());
 obj.Scale.v[2]=atof(EdtZScale->Text.c_str());
 //Get function
 strcpy(obj.Func,FuncEdt->Text.c_str());
 //Get name
 strcpy(obj.Name,NameEdt->Text.c_str()); 
 //Finished
 DataOkay=true;
 Close();
}
//---------------------------------------------------------------------------

void __fastcall TObjectDlg::SpritesBoxClick(TObject *Sender)
{
 ModelsBox->ItemIndex=-1;
 if(SpritesBox>=0)
   NameEdt->Text=SpritesBox->Items->Strings[SpritesBox->ItemIndex];
}
//---------------------------------------------------------------------------

void __fastcall TObjectDlg::ModelsBoxClick(TObject *Sender)
{
 SpritesBox->ItemIndex=-1;
 if(ModelsBox>=0)
   NameEdt->Text=ModelsBox->Items->Strings[ModelsBox->ItemIndex];
}
//---------------------------------------------------------------------------

