//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#pragma comment(lib,"EngineSystem.lib")
#pragma comment(lib,"EngineMedia.lib")
USEFORM("MainWndUnit.cpp", MainWnd);
USEFORM("AddConeUnit.cpp", AddConeDlg);
USEFORM("AddCubeUnit.cpp", AddCubeDlg);
USEFORM("AddCylinderUnit.cpp", AddCylinderDlg);
USEFORM("AddSphereUnit.cpp", AddSphereDlg);
USEFORM("FrontWndUnit.cpp", FrontWnd);
USEFORM("SideWndUnit.cpp", SideWnd);
USEFORM("TopWndUnit.cpp", TopWnd);
USEFORM("View3DWndUnit.cpp", View3DWnd);
USEFORM("OptionWndUnit.cpp", OptionWnd);
USEFORM("AboutWndUnit.cpp", AboutWnd);
USEFORM("MapStatUnit.cpp", MapStatDlg);
USEFORM("LightDlgUnit.cpp", LightDlg);
USEFORM("FaceDlgUnit.cpp", FaceDlg);
USEFORM("ObjectDlgUnit.cpp", ObjectDlg);
USEFORM("BuildDlgUnit.cpp", BuildDlg);
USEFORM("SkyDlgUnit.cpp", SkyDlg);
USEFORM("CameraDlgUnit.cpp", CameraDlg);
USEFORM("ImportQuake3Dlg.cpp", ImportQuake3);
USEFORM("SoundDlgUnit.cpp", SoundDlg);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TMainWnd), &MainWnd);
                 Application->CreateForm(__classid(TAddConeDlg), &AddConeDlg);
                 Application->CreateForm(__classid(TAddCubeDlg), &AddCubeDlg);
                 Application->CreateForm(__classid(TAddCylinderDlg), &AddCylinderDlg);
                 Application->CreateForm(__classid(TAddSphereDlg), &AddSphereDlg);
                 Application->CreateForm(__classid(TFrontWnd), &FrontWnd);
                 Application->CreateForm(__classid(TSideWnd), &SideWnd);
                 Application->CreateForm(__classid(TTopWnd), &TopWnd);
                 Application->CreateForm(__classid(TView3DWnd), &View3DWnd);
                 Application->CreateForm(__classid(TOptionWnd), &OptionWnd);
                 Application->CreateForm(__classid(TAboutWnd), &AboutWnd);
                 Application->CreateForm(__classid(TMapStatDlg), &MapStatDlg);
                 Application->CreateForm(__classid(TLightDlg), &LightDlg);
                 Application->CreateForm(__classid(TFaceDlg), &FaceDlg);
                 Application->CreateForm(__classid(TObjectDlg), &ObjectDlg);
                 Application->CreateForm(__classid(TBuildDlg), &BuildDlg);
                 Application->CreateForm(__classid(TSkyDlg), &SkyDlg);
                 Application->CreateForm(__classid(TCameraDlg), &CameraDlg);
                 Application->CreateForm(__classid(TImportQuake3), &ImportQuake3);
                 Application->CreateForm(__classid(TSoundDlg), &SoundDlg);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
