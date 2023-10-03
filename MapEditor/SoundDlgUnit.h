//---------------------------------------------------------------------------

#ifndef SoundDlgUnitH
#define SoundDlgUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EngineSystem.h"
//---------------------------------------------------------------------------
class TSoundDlg : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TGroupBox *PositionBox;
        TLabel *XLbl;
        TLabel *YLbl;
        TLabel *ZLbl;
        TEdit *EdtXPos;
        TEdit *EdtYPos;
        TEdit *EdtZPos;
        TGroupBox *DirectionBox;
        TLabel *AngleXLbl;
        TLabel *AngleYLbl;
        TEdit *EdtXDir;
        TEdit *EdtYDir;
        TLabel *SRangeLbl;
        TLabel *SSpotLbl;
        TEdit *EdtSpot;
        TEdit *EdtRange;
        TButton *OKBtn;
        TButton *CloseBtn;
        TCheckBox *LoopSnd;
        TLabel *SoundsLbl;
        TListBox *SoundsBox;
        TLabel *VolLbl;
        TEdit *EdtVol;
        TLabel *SpeedLbl;
        TEdit *EdtSpeed;
        TLabel *Label1;
        TEdit *EdtSndSrc;
        void __fastcall CloseBtnClick(TObject *Sender);
        void __fastcall OKBtnClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall SoundsBoxClick(TObject *Sender);
private:	// Anwender-Deklarationen
        ENbool  DataOkay;
        ENSoundSrc sound;
        ENbool CheckSoundData();
public:		// Anwender-Deklarationen
        void NewSoundDlg(ENVector pos);
        void EditSoundDlg(ENSoundSrc sounddata);
        ENbool isDataValid();
        ENSoundSrc GetSound();
        __fastcall TSoundDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSoundDlg *SoundDlg;
//---------------------------------------------------------------------------
#endif
