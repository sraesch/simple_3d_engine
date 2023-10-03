//---------------------------------------------------------------------------
#ifndef SkinManagerUnitH
#define SkinManagerUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TSkinManagerDlg : public TForm
{
__published:	// IDE-managed Components
        TLabel *SkinsLbl;
        TListBox *SkinList;
        TLabel *NameLbl;
        TEdit *NameEdt;
        TButton *RenameBtn;
        TButton *CreateBtn;
        TButton *DeleteBtn;
        TOpenDialog *OpenDialog1;
        TLabel *CurrentSkinLbl;
        TEdit *CurrentSkinEdt;
        TButton *ExportBtn;
        TSaveDialog *SaveDialog1;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall CreateBtnClick(TObject *Sender);
        void __fastcall RenameBtnClick(TObject *Sender);
        void __fastcall DeleteBtnClick(TObject *Sender);
        void __fastcall SkinListClick(TObject *Sender);
        void __fastcall SkinListDblClick(TObject *Sender);
        void __fastcall ExportBtnClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
        void UpdateSkinList();
        void UpdateCurrentSkin();
public:		// User declarations
        __fastcall TSkinManagerDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSkinManagerDlg *SkinManagerDlg;
//---------------------------------------------------------------------------
#endif
