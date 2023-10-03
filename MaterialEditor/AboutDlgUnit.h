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
        TButton *Button1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TAboutDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutDlg *AboutDlg;
//---------------------------------------------------------------------------
#endif
