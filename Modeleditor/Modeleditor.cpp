//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USEFORM("MainWndUnit.cpp", MainWnd);
USEFORM("TopWndUnit.cpp", TopWnd);
USEFORM("FrontWndUnit.cpp", FrontWnd);
USEFORM("SideWndUnit.cpp", SideWnd);
USEFORM("View3DWndUnit.cpp", View3DWnd);
USEFORM("AboutWndUnit.cpp", AboutWnd);
USEFORM("AddCubeUnit.cpp", AddCubeDlg);
USEFORM("OptionWndUnit.cpp", OptionWnd);
USEFORM("AddConeUnit.cpp", AddConeDlg);
USEFORM("AddCylinderUnit.cpp", AddCylinderDlg);
USEFORM("AddSphereUnit.cpp", AddSphereDlg);
USEFORM("ModelStatUnit.cpp", ModelStatDlg);
USEFORM("OptimizeUnit.cpp", OptimizeDlg);
USEFORM("SkinEditorUnit.cpp", SkinEdt);
USEFORM("SkinManagerUnit.cpp", SkinManagerDlg);
USEFORM("Import3DSInterface.cpp", Import3DSDlg);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TMainWnd), &MainWnd);
                 Application->CreateForm(__classid(TTopWnd), &TopWnd);
                 Application->CreateForm(__classid(TFrontWnd), &FrontWnd);
                 Application->CreateForm(__classid(TSideWnd), &SideWnd);
                 Application->CreateForm(__classid(TView3DWnd), &View3DWnd);
                 Application->CreateForm(__classid(TAboutWnd), &AboutWnd);
                 Application->CreateForm(__classid(TAddCubeDlg), &AddCubeDlg);
                 Application->CreateForm(__classid(TOptionWnd), &OptionWnd);
                 Application->CreateForm(__classid(TAddConeDlg), &AddConeDlg);
                 Application->CreateForm(__classid(TAddCylinderDlg), &AddCylinderDlg);
                 Application->CreateForm(__classid(TAddSphereDlg), &AddSphereDlg);
                 Application->CreateForm(__classid(TModelStatDlg), &ModelStatDlg);
                 Application->CreateForm(__classid(TOptimizeDlg), &OptimizeDlg);
                 Application->CreateForm(__classid(TSkinEdt), &SkinEdt);
                 Application->CreateForm(__classid(TSkinManagerDlg), &SkinManagerDlg);
                 Application->CreateForm(__classid(TImport3DSDlg), &Import3DSDlg);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
