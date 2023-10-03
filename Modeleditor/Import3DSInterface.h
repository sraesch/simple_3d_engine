//---------------------------------------------------------------------------
#ifndef Import3DSInterfaceH
#define Import3DSInterfaceH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TImport3DSDlg : public TForm
{
__published:	// IDE-managed Components
        TLabel *FNameLbl;
        TEdit *FNameEdt;
        TButton *BrowseBtn;
        TOpenDialog *OpenDialog1;
        TCheckBox *ImportMatChb;
        TButton *ApplyBtn;
        TButton *CloseBtn;
        TProgressBar *StatusBar;
        TLabel *StatusLbl;
        void __fastcall BrowseBtnClick(TObject *Sender);
        void __fastcall OpenDialog1CanClose(TObject *Sender,
          bool &CanClose);
        void __fastcall CloseBtnClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall ApplyBtnClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TImport3DSDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TImport3DSDlg *Import3DSDlg;
//---------------------------------------------------------------------------
#endif
