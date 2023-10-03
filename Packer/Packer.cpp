//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("Packer.res");
USEFORM("main.cpp", MainWnd);
USEUNIT("PackerData.cpp");
USEFORM("AboutWndUnit.cpp", AboutDlg);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TMainWnd), &MainWnd);
                 Application->CreateForm(__classid(TAboutDlg), &AboutDlg);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
