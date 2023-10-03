//---------------------------------------------------------------------------
#ifndef AddSpriteFrameH
#define AddSpriteFrameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TAddFrameWnd : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *BaseFName;
        TLabel *Label2;
        TEdit *AlphaFName;
        TButton *Button1;
        TButton *Button2;
        TOpenDialog *OpenDialog1;
        TButton *Button3;
        TButton *Button4;
        TPanel *Panel1;
        TColorDialog *ColorDialog1;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Panel1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TAddFrameWnd(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAddFrameWnd *AddFrameWnd;
//---------------------------------------------------------------------------
#endif
