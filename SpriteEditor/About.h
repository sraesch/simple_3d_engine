//---------------------------------------------------------------------------
#ifndef AboutH
#define AboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TAboutWnd : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TButton *Button1;
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
