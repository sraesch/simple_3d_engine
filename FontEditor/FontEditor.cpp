//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("MainWndUnit.cpp", MainWnd);
USEFORM("SetLetterDlgUnit.cpp", SetLetterDlg);
USEFORM("AboutDlgUnit.cpp", AboutDlg);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TMainWnd), &MainWnd);
                 Application->CreateForm(__classid(TSetLetterDlg), &SetLetterDlg);
                 Application->CreateForm(__classid(TAboutDlg), &AboutDlg);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
