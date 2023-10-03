//---------------------------------------------------------------------------
#ifndef ModelStatUnitH
#define ModelStatUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TModelStatDlg : public TForm
{
__published:	// IDE-managed Components
        TMemo *StatTextField;
        TButton *OKBtn;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall OKBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TModelStatDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TModelStatDlg *ModelStatDlg;
//---------------------------------------------------------------------------
#endif
