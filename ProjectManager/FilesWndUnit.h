//---------------------------------------------------------------------------

#ifndef FilesWndUnitH
#define FilesWndUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFilesWnd : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
        TPageControl *FilePages;
        TTabSheet *ScriptPage;
        TTabSheet *MapPage;
        TTabSheet *PackPage;
        TListBox *ScriptBox;
        TListBox *MapBox;
        TListBox *PackBox;
        TTabSheet *UnknownPage;
        TListBox *UnknownBox;
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
        void UpdateFiles();
        __fastcall TFilesWnd(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFilesWnd *FilesWnd;
//---------------------------------------------------------------------------
#endif
