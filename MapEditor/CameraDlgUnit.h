//---------------------------------------------------------------------------

#ifndef CameraDlgUnitH
#define CameraDlgUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TCameraDlg : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TGroupBox *GroupBox2;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
        TEdit *EdtXPos;
        TEdit *EdtYPos;
        TEdit *EdtZPos;
        TGroupBox *GroupBox1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *EdtXAngle;
        TEdit *EdtYAngle;
        TEdit *EdtZAngle;
        TLabel *Label7;
        TEdit *EdtArc;
        TLabel *Label8;
        TEdit *EdtRange;
        TButton *ApplyBtn;
        TButton *CloseBtn;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall CloseBtnClick(TObject *Sender);
        void __fastcall ApplyBtnClick(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
        __fastcall TCameraDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCameraDlg *CameraDlg;
//---------------------------------------------------------------------------
#endif
