//---------------------------------------------------------------------------
#ifndef MainWndUnitH
#define MainWndUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include "EngineSystem.h"
#include "EngineSound.h"
#include "EngineMedia.h"

#pragma comment(lib,"EngineSystem.lib")
#pragma comment(lib,"EngineSound.lib")
#pragma comment(lib,"EngineMedia.lib")
//---------------------------------------------------------------------------
class TMainWnd : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TButton *Button2;
        TButton *Button1;
        TButton *Button3;
        TCheckBox *CheckBox1;
        TTrackBar *TrackBar1;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *DistLbl;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall CheckBox1Click(TObject *Sender);
        void __fastcall TrackBar1Change(TObject *Sender);
private:	// Anwender-Deklarationen
      ENSoundDevice  *snddev;
      ENSoundSource  *ssnd;
      ENPackerLoaded *lpack;
      ENSoundLoaded  *lsound;
public:		// Anwender-Deklarationen
        __fastcall TMainWnd(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainWnd *MainWnd;
//---------------------------------------------------------------------------
#endif
