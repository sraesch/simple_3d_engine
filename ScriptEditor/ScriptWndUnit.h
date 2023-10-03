//---------------------------------------------------------------------------
#ifndef ScriptWndUnitH
#define ScriptWndUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TScriptWnd : public TForm
{
__published:	// IDE-managed Components
        TSaveDialog *SaveDialog1;
        TStatusBar *StatHeader;
        TRichEdit *ScriptField;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall ScriptFieldChange(TObject *Sender);
        void __fastcall ScriptFieldKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall ScriptFieldMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
        void __fastcall ScriptFieldMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
        AnsiString FileName;
        void SaveQuestion();
public:		// User declarations
       void SaveAs();
       void Save();
       void SetFileName(AnsiString FileName);
       AnsiString GetFileName();
        __fastcall TScriptWnd(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TScriptWnd *ScriptWnd;
//---------------------------------------------------------------------------
#endif
