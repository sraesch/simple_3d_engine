//---------------------------------------------------------------------------

#ifndef SkyDlgUnitH
#define SkyDlgUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include "EngineSystem.h"
//---------------------------------------------------------------------------
class TSkyDlg : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TPageControl *SettingsControl;
        TTabSheet *SunSheet;
        TTabSheet *SkySheet;
        TListBox *SkyBoxList;
        TLabel *SideLbl;
        TLabel *PreviewLbl;
        TImage *PreviewImage;
        TButton *ClearBtn;
        TButton *SetBtn;
        TOpenDialog *OpenDialog1;
        TButton *ApplySunBtn;
        TCheckBox *SunChb;
        TButton *ResetSunBtn;
        TGroupBox *DirectionBox;
        TLabel *AngleXLbl;
        TLabel *AngleYLbl;
        TEdit *EdtXDir;
        TEdit *EdtYDir;
        TPanel *SunColorPnl;
        TLabel *SunColorLbl;
        TColorDialog *ColorDialog1;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall SkyBoxListClick(TObject *Sender);
        void __fastcall ClearBtnClick(TObject *Sender);
        void __fastcall SetBtnClick(TObject *Sender);
        void __fastcall SunSheetShow(TObject *Sender);
        void __fastcall ResetSunBtnClick(TObject *Sender);
        void __fastcall ApplySunBtnClick(TObject *Sender);
        void __fastcall SunColorPnlClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
private:	// Anwender-Deklarationen
        void    UpdateSun();
        ENfloat ConvertStrToFloat(AnsiString str);
public:		// Anwender-Deklarationen
        __fastcall TSkyDlg(TComponent* Owner);

        void UpdateSky();
};
//---------------------------------------------------------------------------
extern PACKAGE TSkyDlg *SkyDlg;
//---------------------------------------------------------------------------
#endif
