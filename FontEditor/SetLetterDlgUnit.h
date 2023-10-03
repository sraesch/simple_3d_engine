//---------------------------------------------------------------------------
#ifndef SetLetterDlgUnitH
#define SetLetterDlgUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include "FontData.h"
//---------------------------------------------------------------------------
class TSetLetterDlg : public TForm
{
__published:	// IDE-managed Components
        TLabel *BaseLbl;
        TEdit *BaseEdt;
        TButton *SearchBtn1;
        TLabel *AlphaLbl;
        TEdit *AlphaEdt;
        TButton *SearchBtn2;
        TCheckBox *AlphaChb;
        TButton *OKBtn;
        TButton *CloseBtn;
        TOpenDialog *OpenDialog1;
        TPanel *ColorPnl;
        TLabel *TransLbl;
        TButton *Button1;
        TColorDialog *ColorDialog1;
        void __fastcall CloseBtnClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall AlphaChbClick(TObject *Sender);
        void __fastcall SearchBtn1Click(TObject *Sender);
        void __fastcall SearchBtn2Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall OKBtnClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
        ENuint LetterInd;
public:		// User declarations
        void SetLetterIndex(ENuint ind);
        __fastcall TSetLetterDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSetLetterDlg *SetLetterDlg;
//---------------------------------------------------------------------------
#endif
