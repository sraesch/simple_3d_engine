//---------------------------------------------------------------------------
#ifndef MainWndUnitH
#define MainWndUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMainWnd : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *New1;
        TMenuItem *N1;
        TMenuItem *Open1;
        TMenuItem *N3;
        TMenuItem *Save1;
        TMenuItem *Saveas1;
        TMenuItem *Saveall1;
        TMenuItem *N4;
        TMenuItem *Compile1;
        TMenuItem *N2;
        TMenuItem *Exit1;
        TMenuItem *Edit1;
        TMenuItem *Undo1;
        TMenuItem *N5;
        TMenuItem *Cut1;
        TMenuItem *Copy1;
        TMenuItem *Paste1;
        TMenuItem *Help1;
        TMenuItem *About1;
        TOpenDialog *OpenDialog1;
        TMenuItem *Window1;
        TMenuItem *Cascade1;
        TMenuItem *Title1;
        TListBox *ScriptFileBox;
        TSplitter *Splitter1;
        TFindDialog *FindDialog1;
        TMenuItem *N6;
        TMenuItem *Search1;
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall New1Click(TObject *Sender);
        void __fastcall Saveas1Click(TObject *Sender);
        void __fastcall Open1Click(TObject *Sender);
        void __fastcall Save1Click(TObject *Sender);
        void __fastcall Saveall1Click(TObject *Sender);
        void __fastcall Compile1Click(TObject *Sender);
        void __fastcall Undo1Click(TObject *Sender);
        void __fastcall Cut1Click(TObject *Sender);
        void __fastcall Copy1Click(TObject *Sender);
        void __fastcall Paste1Click(TObject *Sender);
        void __fastcall About1Click(TObject *Sender);
        void __fastcall Cascade1Click(TObject *Sender);
        void __fastcall Title1Click(TObject *Sender);
        void __fastcall ScriptFileBoxDblClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Search1Click(TObject *Sender);
        void __fastcall FindDialog1Find(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TMainWnd(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainWnd *MainWnd;
//---------------------------------------------------------------------------
#endif
