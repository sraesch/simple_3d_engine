//---------------------------------------------------------------------------

#ifndef MainWndH
#define MainWndH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include "EngineScript.h"
//---------------------------------------------------------------------------
class TMainDlg : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TMemo *CodeField;
        TToolBar *ToolBar1;
        TButton *LoadBtn;
        TOpenDialog *OpenDialog1;
        void __fastcall LoadBtnClick(TObject *Sender);
private:	// Anwender-Deklarationen
        AnsiString CommandToStr(ENScriptCode::ENSCRIPTCOMMAND com);
public:		// Anwender-Deklarationen
        __fastcall TMainDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainDlg *MainDlg;
//---------------------------------------------------------------------------
#endif
