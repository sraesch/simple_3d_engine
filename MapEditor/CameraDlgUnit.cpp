//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CameraDlgUnit.h"
#include "MapData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCameraDlg *CameraDlg;
//---------------------------------------------------------------------------
__fastcall TCameraDlg::TCameraDlg(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCameraDlg::FormShow(TObject *Sender)
{
 ENCamera cam=ENMapData::GetCamera();
 //position
 EdtXPos->Text=cam.Pos.v[0];
 EdtYPos->Text=cam.Pos.v[1];
 EdtZPos->Text=cam.Pos.v[2];
 //angle
 EdtXAngle->Text=cam.Angle.v[0];
 EdtYAngle->Text=cam.Angle.v[1];
 EdtZAngle->Text=cam.Angle.v[2];
 //Arc and range
 EdtArc->Text=cam.Arc;
 EdtRange->Text=cam.Range;
}
//---------------------------------------------------------------------------
void __fastcall TCameraDlg::CloseBtnClick(TObject *Sender)
{
 Close();
}
//---------------------------------------------------------------------------
void __fastcall TCameraDlg::ApplyBtnClick(TObject *Sender)
{
 ENCamera cam;
 //position
 cam.Pos.v[0]=atof(EdtXPos->Text.c_str());
 cam.Pos.v[1]=atof(EdtYPos->Text.c_str());
 cam.Pos.v[2]=atof(EdtZPos->Text.c_str());
 //angle
 cam.Angle.v[0]=atof(EdtXAngle->Text.c_str());
 cam.Angle.v[1]=atof(EdtYAngle->Text.c_str());
 cam.Angle.v[2]=atof(EdtZAngle->Text.c_str());
 //Arc and range
 cam.Arc=atof(EdtArc->Text.c_str());
 cam.Range=atof(EdtRange->Text.c_str());
 //Set new camera data
 ENMapData::SetCamera(cam);
 Close();
}
//---------------------------------------------------------------------------
