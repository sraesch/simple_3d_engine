//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#pragma comment (lib,"EngineSystem.lib")
#pragma comment (lib,"EngineMedia.lib")
//---------------------------------------------------------------------------
USEFORM("MainWndUnit.cpp", MainWnd);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TMainWnd), &MainWnd);
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
