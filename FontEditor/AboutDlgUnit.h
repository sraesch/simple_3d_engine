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
__published:	// Von der IDE verwaltete Komponenten
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TButton *Button1;
        void __fastcall Button1Click(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
        __fastcall TAboutDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutDlg *AboutDlg;
//---------------------------------------------------------------------------
#endif
