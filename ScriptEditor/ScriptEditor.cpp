//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("ScriptEditor.res");
USEFORM("MainWndUnit.cpp", MainWnd);
USEFORM("ScriptWndUnit.cpp", ScriptWnd);
USEUNIT("ScriptFormContainer.cpp");
USEFORM("..\EngineScript\CompileDlgUnit.cpp", CompileDlg);
USEFORM("..\EngineScript\AboutDlgUnit.cpp", AboutDlg);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TMainWnd), &MainWnd);
                 Application->CreateForm(__classid(TCompileDlg), &CompileDlg);
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
