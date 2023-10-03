//---------------------------------------------------------------------------
#ifndef CompileDlgUnitH
#define CompileDlgUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TCompileDlg : public TForm
{
__published:	// IDE-managed Components
        TEdit *MainFNEdt;
        TLabel *MainFNLbl;
        TLabel *Label1;
        TMemo *StatusText;
        TButton *CompileBtn;
        TButton *CloseBtn;
        TEdit *DstFileEdt;
        TLabel *Label2;
        TButton *Button1;
        TSaveDialog *SaveDialog1;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall CloseBtnClick(TObject *Sender);
        void __fastcall CompileBtnClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        //void ENPrintLog(char *Msg);
        AnsiString MainFile;
        __fastcall TCompileDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCompileDlg *CompileDlg;
//---------------------------------------------------------------------------
#endif
