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
//---------------------------------------------------------------------------
class TMainWnd : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *N1;
        TMenuItem *Exit1;
        TMenuItem *Project1;
        TMenuItem *Addfile1;
        TOpenDialog *AddFileDlg;
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Addfile1Click(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
        __fastcall TMainWnd(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainWnd *MainWnd;
//---------------------------------------------------------------------------
#endif
