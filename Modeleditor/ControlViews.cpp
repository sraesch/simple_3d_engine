//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ControlViews.h"
#include "TopWndUnit.h"
#include "FrontWndUnit.h"
#include "SideWndUnit.h"
#include "View3DWndUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
TForm           *ENViewControl::Parent=NULL;

void ENViewControl::Init(TForm *Parent,TPopupMenu *menu)
{
 ENViewControl::Parent=Parent;
 Parent->Tile();
 //Config standard view
 TopWnd->StdView();
 FrontWnd->StdView();
 SideWnd->StdView();
 View3DWnd->StdView();
 //Set pop up menu
 TopWnd->PopupMenu=menu;
 FrontWnd->PopupMenu=menu;
 SideWnd->PopupMenu=menu;
 View3DWnd->PopupMenu=menu;
 //Set captions
 TopWnd->Caption="Top view";
 FrontWnd->Caption="Front view";
 SideWnd->Caption="Side view";
 View3DWnd->Caption="3D view";
}

void ENViewControl::Cascade()
{
 Parent->Cascade();
}

void ENViewControl::Tile()
{
 Parent->Tile();
}

void ENViewControl::ShowAll()
{
 TopWnd->WindowState=wsNormal;
 FrontWnd->WindowState=wsNormal;
 SideWnd->WindowState=wsNormal;
 View3DWnd->WindowState=wsNormal;
}

void ENViewControl::DrawAll()
{
 TopWnd->Draw();
 FrontWnd->Draw();
 SideWnd->Draw();
 View3DWnd->Draw();
}

void ENViewControl::SelAllViewsDefault()
{
 TopWnd->StdView();
 FrontWnd->StdView();
 SideWnd->StdView();
 View3DWnd->StdView();
}
