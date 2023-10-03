//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SideWndUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSideWnd *SideWnd;
//---------------------------------------------------------------------------
__fastcall TSideWnd::TSideWnd(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSideWnd::FormCreate(TObject *Sender)
{
 drawobj=new ENDrawClass(this,ENDrawClass::ENDRAWSIDE);
}
//---------------------------------------------------------------------------

void __fastcall TSideWnd::FormDestroy(TObject *Sender)
{
 delete drawobj;        
}
//---------------------------------------------------------------------------

void __fastcall TSideWnd::FormResize(TObject *Sender)
{
 drawobj->ConfigProjMatrix();
 drawobj->Draw();         
}
//---------------------------------------------------------------------------
void TSideWnd::Draw()
{
 drawobj->Draw();
}

void TSideWnd::StdView()
{
 drawobj->ConfigStdMatrix();
}

void __fastcall TSideWnd::FormPaint(TObject *Sender)
{
 Draw();
}
//---------------------------------------------------------------------------

void __fastcall TSideWnd::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if(Button==mbLeft)
   drawobj->OnMouseDown(X,Y,Shift.Contains(ssShift));
}
//---------------------------------------------------------------------------

void __fastcall TSideWnd::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
 drawobj->OnMouseMove(X,Y);        
}
//---------------------------------------------------------------------------

void __fastcall TSideWnd::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 if(Button==mbLeft)
   drawobj->OnMouseUp();
}
//---------------------------------------------------------------------------

