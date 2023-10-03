//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "FrontWndUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrontWnd *FrontWnd;
//---------------------------------------------------------------------------
__fastcall TFrontWnd::TFrontWnd(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrontWnd::FormCreate(TObject *Sender)
{
 drawobj=new ENDrawClass(this,ENDrawClass::ENDRAWFRONT);
}
//---------------------------------------------------------------------------

void __fastcall TFrontWnd::FormDestroy(TObject *Sender)
{
 delete drawobj;        
}
//---------------------------------------------------------------------------

void __fastcall TFrontWnd::FormResize(TObject *Sender)
{
 drawobj->ConfigProjMatrix();
 drawobj->Draw();         
}
//---------------------------------------------------------------------------
void TFrontWnd::Draw()
{
 drawobj->Draw();
}

void TFrontWnd::StdView()
{
 drawobj->ConfigStdMatrix();
}

void __fastcall TFrontWnd::FormPaint(TObject *Sender)
{
 Draw();        
}
//---------------------------------------------------------------------------

void __fastcall TFrontWnd::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if(Button==mbLeft)
   drawobj->OnMouseDown(X,Y,Shift.Contains(ssShift));        
}
//---------------------------------------------------------------------------

void __fastcall TFrontWnd::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
 drawobj->OnMouseMove(X,Y);        
}
//---------------------------------------------------------------------------

void __fastcall TFrontWnd::FormMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if(Button==mbLeft)
   drawobj->OnMouseUp();
}
//---------------------------------------------------------------------------

