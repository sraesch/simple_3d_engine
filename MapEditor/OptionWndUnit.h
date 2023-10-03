//---------------------------------------------------------------------------
#ifndef OptionWndUnitH
#define OptionWndUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ENDrawClass.h"
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TOptionWnd : public TForm
{
__published:	// IDE-managed Components
        TButton *ApplyBtn;
        TColorDialog *ColorDialog1;
        TButton *CloseBtn;
        TGroupBox *ColorGroup;
        TLabel *Label1;
        TPanel *VertexColor;
        TLabel *Label3;
        TPanel *FaceColor;
        TLabel *Label5;
        TPanel *GridColor;
        TLabel *Label7;
        TPanel *BackColor;
        TLabel *Label2;
        TPanel *SVertexColor;
        TLabel *Label4;
        TPanel *SFaceColor;
        TLabel *Label6;
        TPanel *ToolColor;
        TGroupBox *DrawFlagsGroup;
        TCheckBox *Cullfacing;
        TCheckBox *EnableVerts;
        TCheckBox *EnableFaces;
        TCheckBox *EnableOrigin;
        TCheckBox *EnableGrid;
        TButton *DefaultBtn;
        TGroupBox *GridBox;
        TLabel *NumGridLinesLbl;
        TTrackBar *NumGridLines;
        TLabel *GridLinesLbl;
        TLabel *TitleSizeGridLbl;
        TLabel *SizeGridLbl;
        TTrackBar *SizeGridBar;
        TLabel *Label8;
        TPanel *SLightColor;
        TGroupBox *MapBox;
        TLabel *PackLbl;
        TEdit *PackEdt;
        TButton *Button1;
        TOpenDialog *OpenDialog1;
        TPanel *ObjColor;
        TLabel *Label9;
        TLabel *Label10;
        TPanel *SObjColor;
        TButton *SkyBtn;
        TButton *CamBtn;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall ApplyBtnClick(TObject *Sender);
        void __fastcall CullfacingClick(TObject *Sender);
        void __fastcall VertexColorClick(TObject *Sender);
        void __fastcall SVertexColorClick(TObject *Sender);
        void __fastcall FaceColorClick(TObject *Sender);
        void __fastcall SFaceColorClick(TObject *Sender);
        void __fastcall CloseBtnClick(TObject *Sender);
        void __fastcall GridColorClick(TObject *Sender);
        void __fastcall ToolColorClick(TObject *Sender);
        void __fastcall BackColorClick(TObject *Sender);
        void __fastcall EnableVertsClick(TObject *Sender);
        void __fastcall EnableFacesClick(TObject *Sender);
        void __fastcall EnableOriginClick(TObject *Sender);
        void __fastcall EnableGridClick(TObject *Sender);
        void __fastcall DefaultBtnClick(TObject *Sender);
        void __fastcall NumGridLinesChange(TObject *Sender);
        void __fastcall SizeGridBarChange(TObject *Sender);
        void __fastcall SLightColorClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall ObjColorClick(TObject *Sender);
        void __fastcall SObjColorClick(TObject *Sender);
        void __fastcall SkyBtnClick(TObject *Sender);
        void __fastcall CamBtnClick(TObject *Sender);
private:	// User declarations
    ENDrawClass::ENDrawOptions drawoptions;
    TColor VecToColor(ENVector col);
    ENVector ColorToVec(TColor col);
    void ChangePanelColor(TPanel *panel);
    ENuint ENLogBase2(ENuint x);
public:		// User declarations
        __fastcall TOptionWnd(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOptionWnd *OptionWnd;
//---------------------------------------------------------------------------
#endif
