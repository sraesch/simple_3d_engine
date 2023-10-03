//---------------------------------------------------------------------------
#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include "EngineSystem.h"
//---------------------------------------------------------------------------
class TMainWnd : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *Edit1;
        TMenuItem *Help1;
        TMenuItem *AddFrame1;
        TMenuItem *DeleteFrame1;
        TStatusBar *StatusBar1;
        TMenuItem *Exit1;
        TMenuItem *View1;
        TMenuItem *Normal1;
        TMenuItem *Transparent1;
        TMenuItem *Alpha1;
        TMenuItem *New1;
        TScrollBar *ScrollBar1;
        TMenuItem *Save1;
        TMenuItem *Load1;
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        TMenuItem *N1;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall AddFrame1Click(TObject *Sender);
        void __fastcall Normal1Click(TObject *Sender);
        void __fastcall DeleteFrame1Click(TObject *Sender);
        void __fastcall New1Click(TObject *Sender);
        void __fastcall ScrollBar1Scroll(TObject *Sender,
          TScrollCode ScrollCode, int &ScrollPos);
        void __fastcall Load1Click(TObject *Sender);
        void __fastcall Save1Click(TObject *Sender);
        void __fastcall SaveDialog1CanClose(TObject *Sender,
          bool &CanClose);
        void __fastcall OpenDialog1CanClose(TObject *Sender,
          bool &CanClose);
        void __fastcall N1Click(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
        ENGLClass *globj;
public:		// User declarations
        void Draw();
        __fastcall TMainWnd(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainWnd *MainWnd;
//---------------------------------------------------------------------------
#endif
