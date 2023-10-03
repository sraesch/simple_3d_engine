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
#include "MaterialData.h"
#include <Dialogs.hpp>
#include "EngineSystem.h"

#pragma comment(lib,"EngineSystem.lib")
//---------------------------------------------------------------------------
class TENMainWnd : public TForm
{
__published:	// IDE-managed Components
        TMainMenu *MainMenu1;
        TMenuItem *File1;
        TMenuItem *Exit1;
        TMenuItem *New1;
        TMenuItem *Edit1;
        TMenuItem *Setbase1;
        TOpenDialog *OpenDialog1;
        TColorDialog *ColorDialog1;
        TMenuItem *View1;
        TMenuItem *Base1;
        TMenuItem *Transparent1;
        TMenuItem *Alpha1;
        TMenuItem *Normalmap1;
        TMenuItem *Setalpha1;
        TMenuItem *SetNormalmap1;
        TOpenDialog *OpenDialog2;
        TMenuItem *Save1;
        TSaveDialog *SaveDialog1;
        TMenuItem *Load1;
        TMenuItem *Help1;
        TMenuItem *About1;
        TMenuItem *Luminancemap1;
        TMenuItem *Setluminancemap1;
        TMenuItem *Export1;
        TMenuItem *basemap1;
        TMenuItem *alphamap1;
        TMenuItem *normalmap2;
        TMenuItem *lumimap;
        TSaveDialog *SaveDialog2;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Exit1Click(TObject *Sender);
        void __fastcall New1Click(TObject *Sender);
        void __fastcall Setbase1Click(TObject *Sender);
        void __fastcall SwitchMode(TObject *Sender);
        void __fastcall Setalpha1Click(TObject *Sender);
        void __fastcall SetNormalmap1Click(TObject *Sender);
        void __fastcall Save1Click(TObject *Sender);
        void __fastcall Load1Click(TObject *Sender);
        void __fastcall About1Click(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall Setluminancemap1Click(TObject *Sender);
        void __fastcall basemap1Click(TObject *Sender);
        void __fastcall alphamap1Click(TObject *Sender);
        void __fastcall normalmap2Click(TObject *Sender);
        void __fastcall lumimapClick(TObject *Sender);
private:	// User declarations
        ENuint mode;
        ENGLClass  *globj;
        void            Draw();
public:		// User declarations
        __fastcall TENMainWnd(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TENMainWnd *ENMainWnd;
//---------------------------------------------------------------------------
#endif
