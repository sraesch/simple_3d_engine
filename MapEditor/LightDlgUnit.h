//---------------------------------------------------------------------------
#ifndef LightDlgUnitH
#define LightDlgUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EngineSystem.h"
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TLightDlg : public TForm
{
__published:	// IDE-managed Components
        TButton *OKBtn;
        TButton *CloseBtn;
        TGroupBox *PositionBox;
        TLabel *XLbl;
        TLabel *YLbl;
        TLabel *ZLbl;
        TEdit *EdtXPos;
        TEdit *EdtYPos;
        TEdit *EdtZPos;
        TLabel *LightColLbl;
        TPanel *ColorPnl;
        TGroupBox *DirectionBox;
        TLabel *AngleXLbl;
        TEdit *EdtXDir;
        TEdit *EdtRange;
        TEdit *EdtSpot;
        TLabel *LSpotLbl;
        TLabel *LRangeLbl;
        TColorDialog *ColorDialog1;
        TLabel *AngleYLbl;
        TEdit *EdtYDir;
        void __fastcall CloseBtnClick(TObject *Sender);
        void __fastcall ColorPnlClick(TObject *Sender);
        void __fastcall OKBtnClick(TObject *Sender);
private:	// User declarations
        ENbool  DataOkay;
        ENLight light;
        ENbool CheckLightData();
public:		// User declarations
        void NewLightDlg(ENVector pos);
        void EditLightDlg(ENLight lightdata);
        ENLight GetLight();
        ENbool  isDataValid();
        __fastcall TLightDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLightDlg *LightDlg;
//---------------------------------------------------------------------------
#endif
