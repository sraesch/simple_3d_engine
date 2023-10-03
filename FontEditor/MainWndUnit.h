//---------------------------------------------------------------------------
#ifndef MainWndUnitH
#define MainWndUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EngineSystem.h"
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TMainWnd : public TForm
{
__published:	// IDE-managed Components
        TListBox *LetterBox;
        TLabel *LetterLbl;
        TGroupBox *CurrentLetterBox;
        TEdit *CurrentLetterEdt;
        TPanel *LetterPnl;
        TImage *LetterImage;
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *Letter1;
        TMenuItem *Help1;
        TMenuItem *Remove1;
        TMenuItem *Set1;
        TMenuItem *New1;
        TMenuItem *Open1;
        TOpenDialog *OpenDialog1;
        TMenuItem *Save1;
        TMenuItem *Exit1;
        TSaveDialog *SaveDialog1;
        TMenuItem *Help2;
        TFontDialog *FontDialog1;
        TMenuItem *Setdefaultfont1;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall LetterBoxKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall LetterBoxClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Set1Click(TObject *Sender);
        void __fastcall Remove1Click(TObject *Sender);
        void __fastcall New1Click(TObject *Sender);
        void __fastcall Open1Click(TObject *Sender);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall Save1Click(TObject *Sender);
        void __fastcall Help2Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Setdefaultfont1Click(TObject *Sender);
private:	// User declarations
   void InitLetterBox();
   void InitLetterPic();
   void SetCurrentLetter(ENuint ind);
public:		// User declarations
        __fastcall TMainWnd(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainWnd *MainWnd;
//---------------------------------------------------------------------------
#endif
