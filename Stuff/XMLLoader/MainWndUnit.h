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
#include "XMLClass.h"
//---------------------------------------------------------------------------
class TMainWnd : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TMemo *Memo1;
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *Loadxml1;
        TOpenDialog *OpenDialog1;
        void __fastcall Loadxml1Click(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
        __fastcall TMainWnd(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainWnd *MainWnd;
//---------------------------------------------------------------------------
#endif
