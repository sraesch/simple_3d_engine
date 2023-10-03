//---------------------------------------------------------------------------
#ifndef AddSphereUnitH
#define AddSphereUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EngineSystem.h"
//---------------------------------------------------------------------------
class TAddSphereDlg : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TLabel *Label3;
        TEdit *EdtRange;
        TEdit *EdtDetails;
        TGroupBox *GroupBox2;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TEdit *EdtXPos;
        TEdit *EdtYPos;
        TEdit *EdtZPos;
        TButton *Button1;
        TButton *BtnClose;
        void __fastcall BtnCloseClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
        ENfloat ConvertStrToFloat(AnsiString str);
        ENint   ConvertStrToInt(AnsiString str);
public:		// User declarations
        __fastcall TAddSphereDlg(TComponent* Owner);
        void SetPosValues(ENVector pos);
};
//---------------------------------------------------------------------------
extern PACKAGE TAddSphereDlg *AddSphereDlg;
//---------------------------------------------------------------------------
#endif
