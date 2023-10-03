//---------------------------------------------------------------------------
#ifndef FrontWndUnitH
#define FrontWndUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ENDrawClass.h"
//---------------------------------------------------------------------------
class TFrontWnd : public TForm
{
__published:	// IDE-managed Components
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
protected:	// User declarations
        ENDrawClass *drawobj;
public:		// User declarations
        __fastcall TFrontWnd(TComponent* Owner);
        void StdView();        
        void Draw();
};
//---------------------------------------------------------------------------
extern PACKAGE TFrontWnd *FrontWnd;
//---------------------------------------------------------------------------
#endif
