//---------------------------------------------------------------------------
#ifndef MainWndUnitH
#define MainWndUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include "EngineSystem.h"
#include "EngineMedia.h"
#include <ComCtrls.hpp>
#include "EngineSystem.h"
#include <Menus.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMainWnd : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TOpenDialog *OpenDialog1;
        TProgressBar *ProgressBar1;
        TPopupMenu *PopupMenu1;
        TMenuItem *Loaddata1;
        TMenuItem *Compute1;
        TTimer *Timer1;
        TMemo *Memo1;
        TSaveDialog *SaveDialog1;
        TMenuItem *Loadbsp1;
        TOpenDialog *OpenDialog2;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormDblClick(TObject *Sender);
        void __fastcall Memo1DblClick(TObject *Sender);
        void __fastcall Loadbsp1Click(TObject *Sender);
private:	// Anwender-Deklarationen
        ENASCIIMesh    *eam;
        ENBSPBuilder   *bsp;
        ENGLClass      *globj;
        ENBSPTree      *bsptree;
        ENubyte        *colors;
        void TransformMatrices();
        void DrawScene();
        ENuint numfront,numback;
        void DrawTree(ENBSPTree *bt);
        void MakeStressTest();
public:		// Anwender-Deklarationen
        __fastcall TMainWnd(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMainWnd *MainWnd;
//---------------------------------------------------------------------------
#endif
