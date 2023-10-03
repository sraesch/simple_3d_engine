//---------------------------------------------------------------------------

#ifndef ImportQuake3DlgH
#define ImportQuake3DlgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TImportQuake3 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TCheckBox *MeshChb;
        TCheckBox *PolyChb;
        TCheckBox *PatchChb;
        TCheckBox *BillChb;
        TCheckBox *TexChb;
        TLabel *FileNameLbl;
        TEdit *FileNameEdt;
        TButton *SearchBtn;
        TButton *ApplyBtn;
        TButton *CloseBtn;
        TOpenDialog *OpenDialog1;
        TLabel *Label1;
        TEdit *DirEdt;
        void __fastcall SearchBtnClick(TObject *Sender);
        void __fastcall CloseBtnClick(TObject *Sender);
        void __fastcall ApplyBtnClick(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
        __fastcall TImportQuake3(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TImportQuake3 *ImportQuake3;
//---------------------------------------------------------------------------
#endif
