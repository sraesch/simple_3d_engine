//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("SpriteEditor.res");
USEFORM("main.cpp", MainWnd);
USEUNIT("SpriteData.cpp");
USEFORM("AddSpriteFrame.cpp", AddFrameWnd);
USEFORM("About.cpp", AboutWnd);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TMainWnd), &MainWnd);
                 Application->CreateForm(__classid(TAddFrameWnd), &AddFrameWnd);
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
