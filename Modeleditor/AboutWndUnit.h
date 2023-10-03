//---------------------------------------------------------------------------
#ifndef AboutWndUnitH
#define AboutWndUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TAboutWnd : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TButton *Button1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TAboutWnd(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutWnd *AboutWnd;
//---------------------------------------------------------------------------
#endif
