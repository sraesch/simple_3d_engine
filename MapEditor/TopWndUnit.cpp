//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TopWndUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTopWnd *TopWnd;
//---------------------------------------------------------------------------
__fastcall TTopWnd::TTopWnd(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TTopWnd::FormCreate(TObject *Sender)
{
 drawobj=new ENDrawClass(this,ENDrawClass::ENDRAWTOP);
}
//---------------------------------------------------------------------------

void __fastcall TTopWnd::FormDestroy(TObject *Sender)
{
 delete drawobj;        
}
//---------------------------------------------------------------------------

void __fastcall TTopWnd::FormResize(TObject *Sender)
{
 drawobj->ConfigProjMatrix();
 drawobj->Draw();      
}
//---------------------------------------------------------------------------
void TTopWnd::Draw()
{
 drawobj->Draw();
}

void TTopWnd::StdView()
{
 drawobj->ConfigStdMatrix();
}

void __fastcall TTopWnd::FormPaint(TObject *Sender)
{
 Draw();        
}
//---------------------------------------------------------------------------


void __fastcall TTopWnd::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if(Button==mbLeft)
   drawobj->OnMouseDown(X,Y,Shift.Contains(ssShift));
}
//---------------------------------------------------------------------------

void __fastcall TTopWnd::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
 drawobj->OnMouseMove(X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TTopWnd::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 if(Button==mbLeft)
   drawobj->OnMouseUp();        
}
//---------------------------------------------------------------------------





