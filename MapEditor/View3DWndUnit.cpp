//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "View3DWndUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TView3DWnd *View3DWnd;
//---------------------------------------------------------------------------
__fastcall TView3DWnd::TView3DWnd(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TView3DWnd::FormCreate(TObject *Sender)
{
 drawobj=new ENDrawClass(this,ENDrawClass::ENDRAW3D);
}
//---------------------------------------------------------------------------
void __fastcall TView3DWnd::FormDestroy(TObject *Sender)
{
 delete drawobj;
}
//---------------------------------------------------------------------------
void TView3DWnd::Draw()
{
 drawobj->Draw();
}

void TView3DWnd::StdView()
{
 drawobj->ConfigStdMatrix();
}

void __fastcall TView3DWnd::FormPaint(TObject *Sender)
{
 Draw();
}
//---------------------------------------------------------------------------
void __fastcall TView3DWnd::FormResize(TObject *Sender)
{
 drawobj->ConfigProjMatrix();
 drawobj->Draw(); 
}
//---------------------------------------------------------------------------

void __fastcall TView3DWnd::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if(Button==mbLeft)
   drawobj->OnMouseDown(X,Y,Shift.Contains(ssShift));
}
//---------------------------------------------------------------------------

void __fastcall TView3DWnd::FormMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
 drawobj->OnMouseMove(X,Y);        
}
//---------------------------------------------------------------------------

void __fastcall TView3DWnd::FormMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if(Button==mbLeft)
   drawobj->OnMouseUp();
}
//---------------------------------------------------------------------------

