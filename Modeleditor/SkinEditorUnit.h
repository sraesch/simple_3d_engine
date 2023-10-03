//---------------------------------------------------------------------------
#ifndef SkinEditorUnitH
#define SkinEditorUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include "EngineSystem.h"
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TSkinEdt : public TForm
{
__published:	// IDE-managed Components
        TPopupMenu *SkinEdtMenu;
        TMenuItem *SkinManager1;
        TMenuItem *N1;
        TMenuItem *MakeSkingrid1;
        TMenuItem *Top1;
        TMenuItem *Front1;
        TMenuItem *Side1;
        TMenuItem *Cube1;
        TMenuItem *Select1;
        TMenuItem *Selection1;
        TMenuItem *Selectall1;
        TMenuItem *Selectnone1;
        TMenuItem *N2;
        TMenuItem *Move1;
        TMenuItem *Rotate1;
        TMenuItem *Scale1;
        TMenuItem *N3;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall SkinManager1Click(TObject *Sender);
        void __fastcall Top1Click(TObject *Sender);
        void __fastcall Front1Click(TObject *Sender);
        void __fastcall Side1Click(TObject *Sender);
        void __fastcall Cube1Click(TObject *Sender);
        void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall Selection1Click(TObject *Sender);
        void __fastcall Selectall1Click(TObject *Sender);
        void __fastcall Selectnone1Click(TObject *Sender);
        void __fastcall Move1Click(TObject *Sender);
        void __fastcall Rotate1Click(TObject *Sender);
        void __fastcall Scale1Click(TObject *Sender);
public:		// User declarations
        enum ENSKINTOOLS {ENSKINTOOLSELECT,ENSKINTOOLMOVE,ENSKINTOOLROT,ENSKINTOOLSCALE};
        __fastcall TSkinEdt(TComponent* Owner);
private:	// User declarations
        ENbool   Selection,MoveCoord,RotateCoord,ScaleCoord;
        ENGLClass  *globj;
        ENVector2  Current,VecDir,CenterVec;
        ENVector2  SelectFrom,SelectTo;
        ENSKINTOOLS CurrentTool;
        void  Draw();
        ENMatrixSpec mspec;
        void Transform();
        void DrawSkin();
        void DrawSkinVertexes();
        void DrawSkinFaces();
        void DrawSelectTool();
        void DrawSkinGrid();
};
//---------------------------------------------------------------------------
extern PACKAGE TSkinEdt *SkinEdt;
//---------------------------------------------------------------------------
#endif
