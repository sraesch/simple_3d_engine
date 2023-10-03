//---------------------------------------------------------------------------
#ifndef ModelStatUnitH
#define ModelStatUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TMapStatDlg : public TForm
{
__published:	// IDE-managed Components
        TMemo *StatTextField;
        TButton *OKBtn;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall OKBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TMapStatDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMapStatDlg *MapStatDlg;
//---------------------------------------------------------------------------
#endif
