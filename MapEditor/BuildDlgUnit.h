//---------------------------------------------------------------------------
#ifndef BuildDlgUnitH
#define BuildDlgUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "EngineSystem.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TBuildDlg : public TForm
{
__published:	// IDE-managed Components
        TButton *CloseBtn;
        TPanel *CompilePanel;
        TLabel *CurStepLbl;
        TLabel *ProgLbl;
        TLabel *MsgLbl;
        TProgressBar *ProgBar;
        TButton *GoBtn;
        void __fastcall CloseBtnClick(TObject *Sender);
        void __fastcall FormActivate(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
        AnsiString FName;
public:		// User declarations
        void Build(AnsiString FileName);
        __fastcall TBuildDlg(TComponent* Owner);

        static void ReportFunc(char *Msg,ENuint pos,ENuint max);
};
//---------------------------------------------------------------------------
extern PACKAGE TBuildDlg *BuildDlg;
//---------------------------------------------------------------------------
#endif
