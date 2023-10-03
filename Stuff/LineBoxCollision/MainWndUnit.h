//---------------------------------------------------------------------------

#ifndef MainWndUnitH
#define MainWndUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "EngineSystem.h"
#include "EngineMedia.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TTimer *Timer1;
        TButton *Button1;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall FormDblClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
private:	// Anwender-Deklarationen
   ENuint        QueryID;
   ENGLClass    *globj;
   void          DrawScene();
   void          DrawBox(ENVector Box[]);
   void          TransformMatrices();
   ENbool        ENBoxLineIntersect(ENVector vBox[],ENVector vLine[]);
   ENbool        ENBoxLineIntersectNew(ENVector vBox[],ENVector vLine[]);
   ENbool        ENBoxLineIntersect2(ENVector vBox[],ENVector vLine[]);
   ENbool        ENTriangleLineIntersect(ENVector vTri[],ENVector vLine[]);
public:		// Anwender-Deklarationen
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
