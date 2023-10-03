//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("MaterialEditor.res");
USEFORM("main.cpp", ENMainWnd);
USEUNIT("MaterialData.cpp");
USEFORM("C:\Dokumente und Einstellungen\Me\Eigene Dateien\C++\WINAPILoadBmp\About.cpp", AboutWnd);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TENMainWnd), &ENMainWnd);
                 Application->CreateForm(__classid(TAboutWnd), &AboutWnd);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
