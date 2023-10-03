//---------------------------------------------------------------------------
#ifndef OptimizeUnitH
#define OptimizeUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TOptimizeDlg : public TForm
{
__published:	// IDE-managed Components
        TCheckBox *UnVertexesCh;
        TCheckBox *VertexesCh;
        TLabel *ToleranceLbl;
        TEdit *ToleranzEdt;
        TButton *Button1;
        void __fastcall VertexesChClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TOptimizeDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TOptimizeDlg *OptimizeDlg;
//---------------------------------------------------------------------------
#endif
