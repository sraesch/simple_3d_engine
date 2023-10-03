//---------------------------------------------------------------------------
#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include "EngineMedia.h"
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
//---------------------------------------------------------------------------
class TMainWnd : public TForm
{
__published:	// IDE-managed Components
        TStringGrid *PackItems;
        TLabel *PackagesLbl;
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TOpenDialog *OpenDialog1;
        TGroupBox *PackageBox;
        TEdit *NameEdt;
        TLabel *NameLbl;
        TButton *AddFBtn;
        TButton *DeleteBtn;
        TMenuItem *New1;
        TMenuItem *N1;
        TMenuItem *Open1;
        TOpenDialog *OpenDialog2;
        TMenuItem *N2;
        TMenuItem *Save1;
        TSaveDialog *SaveDialog1;
        TMenuItem *Saveas1;
        TButton *RenameBtn;
        TLabel *NewNameLbl;
        TEdit *RenameEdt;
        TMenuItem *N3;
        TMenuItem *Exit1;
        TMenuItem *Help1;
        TMenuItem *About1;
        TButton *ExportBtn;
        TSaveDialog *SaveDialog2;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall AddFBtnClick(TObject *Sender);
        void __fastcall PackItemsSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall DeleteBtnClick(TObject *Sender);
        void __fastcall New1Click(TObject *Sender);
        void __fastcall Open1Click(TObject *Sender);
        void __fastcall Save1Click(TObject *Sender);
        void __fastcall Saveas1Click(TObject *Sender);
        void __fastcall RenameBtnClick(TObject *Sender);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall About1Click(TObject *Sender);
        void __fastcall ExportBtnClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
        void SaveQuestion();
        AnsiString Type2Str(ENPackerBase::ENPackageType type);
public:		// User declarations
        void UpdateList();
        __fastcall TMainWnd(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainWnd *MainWnd;
//---------------------------------------------------------------------------
#endif
