//---------------------------------------------------------------------------
#ifndef AboutDlgUnitH
#define AboutDlgUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TAboutDlg : public TForm
{
__published:	// IDE-managed Components
        TLabel *TitleLbl;
        TLabel *AuthorLbl;
        TLabel *EMailLbl;
        TButton *OKBtn;
        void __fastcall OKBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TAboutDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutDlg *AboutDlg;
//---------------------------------------------------------------------------
#endif
