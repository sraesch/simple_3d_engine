//---------------------------------------------------------------------------
#ifndef AddCubeUnitH
#define AddCubeUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EngineSystem.h"
//---------------------------------------------------------------------------
class TAddCubeDlg : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TEdit *EdtXSize;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *EdtYSize;
        TEdit *EdtZSize;
        TLabel *Label3;
        TGroupBox *GroupBox2;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TEdit *EdtXPos;
        TEdit *EdtYPos;
        TEdit *EdtZPos;
        TButton *Button1;
        TButton *Button2;
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
        ENfloat ConvertStrToFloat(AnsiString str);
public:		// User declarations
        __fastcall TAddCubeDlg(TComponent* Owner);
        void SetPosValues(ENVector pos);
};
//---------------------------------------------------------------------------
extern PACKAGE TAddCubeDlg *AddCubeDlg;
//---------------------------------------------------------------------------
#endif
