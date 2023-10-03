//---------------------------------------------------------------------------
#ifndef ObjectDlgUnitH
#define ObjectDlgUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "MapData.h"
#include "MapSources.h"
//---------------------------------------------------------------------------
class TObjectDlg : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *PositionBox;
        TLabel *XLbl;
        TLabel *YLbl;
        TLabel *ZLbl;
        TEdit *EdtXPos;
        TEdit *EdtYPos;
        TEdit *EdtZPos;
        TGroupBox *AngleBox;
        TLabel *AngleXLbl;
        TLabel *AngleYLbl;
        TEdit *EdtXDir;
        TEdit *EdtYDir;
        TLabel *AngleZLbl;
        TEdit *EdtZDir;
        TGroupBox *ScaleBox;
        TLabel *ScaleXLbl;
        TLabel *ScaleYLbl;
        TLabel *ScaleZLbl;
        TEdit *EdtXScale;
        TEdit *EdtYScale;
        TEdit *EdtZScale;
        TLabel *SpritesLbl;
        TListBox *SpritesBox;
        TLabel *ModelsLbl;
        TListBox *ModelsBox;
        TLabel *FuncLbl;
        TEdit *FuncEdt;
        TLabel *NameLbl;
        TEdit *NameEdt;
        TButton *ApplyBtn;
        TButton *CloseBtn;
        void __fastcall CloseBtnClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall ApplyBtnClick(TObject *Sender);
        void __fastcall SpritesBoxClick(TObject *Sender);
        void __fastcall ModelsBoxClick(TObject *Sender);
private:	// User declarations
        ENbool DataOkay;
        ENMapBase::ENMapObject obj;
public:		// User declarations
        void NewObjectDlg(ENVector pos);
        void EditObjectDlg(ENMapBase::ENMapObject objdata);
        ENMapBase::ENMapObject GetObject();
        ENbool  isDataValid();
        __fastcall TObjectDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TObjectDlg *ObjectDlg;
//---------------------------------------------------------------------------
#endif
