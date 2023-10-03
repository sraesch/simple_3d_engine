//---------------------------------------------------------------------------
#ifndef FaceDlgUnitH
#define FaceDlgUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "EngineSystem.h"
//---------------------------------------------------------------------------
class TFaceDlg : public TForm
{
__published:	// IDE-managed Components
        TCheckBox *InvisibleChb;
        TCheckBox *PassableChb;
        TCheckBox *LightingChb;
        TCheckBox *ShadowChb;
        TRadioGroup *ModeGroup;
        TButton *ApplyBtn;
        TButton *CloseBtn;
        TCheckBox *AnimatedChb;
        TGroupBox *AnimateGroup;
        TLabel *NumFramesLbl;
        TEdit *NumFramesEdt;
        TLabel *FrameLenLbl;
        TEdit *FrameLenEdt;
        TGroupBox *TexPosGroup;
        TLabel *XPosLbl;
        TLabel *YPosLbl;
        TEdit *EdtXPos;
        TEdit *EdtYPos;
        TGroupBox *TexScaleGroup;
        TLabel *XScaleLbl;
        TLabel *YScaleLbl;
        TEdit *EdtXScale;
        TEdit *EdtYScale;
        TLabel *TexAngleLbl;
        TEdit *EdtAngle;
        void __fastcall CloseBtnClick(TObject *Sender);
        void __fastcall ApplyBtnClick(TObject *Sender);
private:	// User declarations
        void ComputeFlag(ENint flag,ENint &flt);
        AnsiString FltToStr(ENfloat flt);
        TCheckBoxState FlagToState(ENint flag);
public:		// User declarations
        __fastcall TFaceDlg(TComponent* Owner);
        int __fastcall ShowModal(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFaceDlg *FaceDlg;
//---------------------------------------------------------------------------
#endif
