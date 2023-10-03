//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AboutWndUnit.h"
#include "MainWndUnit.h"
#include "ControlViews.h"
#include "ModelData.h"
#include "OptionWndUnit.h"
#include "ModelStatUnit.h"
#include "OptimizeUnit.h"
#include "SkinEditorUnit.h"
#include "Import3DSInterface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma comment(lib,"EngineSystem.lib")
#pragma comment(lib,"EngineMedia.lib")
TMainWnd *MainWnd;
//---------------------------------------------------------------------------
__fastcall TMainWnd::TMainWnd(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::Cascade1Click(TObject *Sender)
{
 ENViewControl::Cascade();  
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::Exit1Click(TObject *Sender)
{
 SaveQuestion();
 Close();        
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::FormShow(TObject *Sender)
{
 ENViewControl::Init(this,PopupMenu1);
 //If open command
 if(ParamCount())
 {
  ENModelData::Open(ParamStr(1).c_str());
  ENViewControl::DrawAll();
  UpdateScrollFrames();
  UpdateGroupList();
 }
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::Tile1Click(TObject *Sender)
{
 ENViewControl::Tile();
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::Showall1Click(TObject *Sender)
{
 ENViewControl::ShowAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::MoveCamera1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWMOVECAM);
 UpdateToolStat();      
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::RotateCamera1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWROTATECAM);
 UpdateToolStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::ZoomCamera1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWZOOM);
 UpdateToolStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormCreate(TObject *Sender)
{
 ENModelData::Init();
 ENDrawClass::InitOptions();
 ToolStat=MainWndStat->Panels->Items[0];
 FrameStat=MainWndStat->Panels->Items[1];
 VertexStat=MainWndStat->Panels->Items[2];
 ENDrawClass::SetVertState(VertexStat);
 BtnDir->Glyph->LoadFromResourceName((ENuint)HInstance,"DIRALL");
 //Call updates
 UpdateToolStat();
 UpdateScrollFrames();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormDestroy(TObject *Sender)
{
 ENModelData::Free();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Vertex1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWADDVERTEX);
 UpdateToolStat();
}
//---------------------------------------------------------------------------


void __fastcall TMainWnd::New1Click(TObject *Sender)
{
 SaveQuestion();
 ENModelData::New();
 ENViewControl::SelAllViewsDefault();
 ENViewControl::DrawAll();

 UpdateScrollFrames();
 UpdateGroupList();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Selectall1Click(TObject *Sender)
{
 ENModelData::SelectAll();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Invertselection1Click(TObject *Sender)
{
 ENModelData::InvertSelected();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Select1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWSELECT);
 UpdateToolStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Selectnothing1Click(TObject *Sender)
{
 ENModelData::SelectNone();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::About1Click(TObject *Sender)
{
 AboutWnd->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Face1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWADDFACE);
 UpdateToolStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Save1Click(TObject *Sender)
{
 if(!ENModelData::Save())
   SaveAs1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Open1Click(TObject *Sender)
{
 SaveQuestion();
 if(OpenDialog1->Execute())
 {
  ENModelData::Open(OpenDialog1->FileName.c_str());
  ENViewControl::DrawAll();
 }

 UpdateScrollFrames();
 UpdateGroupList();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Moveobject1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWMOVEOBJ);
 UpdateToolStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Rotateobject1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWROTATEOBJ);
 UpdateToolStat();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Scaleobject1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWSCALEOBJ);
 UpdateToolStat();        
}
//---------------------------------------------------------------------------



void __fastcall TMainWnd::OnViewMode(TObject *Sender)
{
 Wireframe1->Checked=false;
 Solid1->Checked=false;
 Textured1->Checked=false;
 TMenuItem *mitem=(TMenuItem*)Sender;
 switch(mitem->Tag)
 {
  case 0:
    Wireframe1->Checked=true;
    ENDrawClass::SetViewMode(ENDrawClass::ENDRAWWIRE);
  break;
  case 1:
    Solid1->Checked=true;
    ENDrawClass::SetViewMode(ENDrawClass::ENDRAWSOLID);
  break;
  case 2:
    Textured1->Checked=true;
    ENDrawClass::SetViewMode(ENDrawClass::ENDRAWTEX);    
  break;
 }

 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------



void __fastcall TMainWnd::Options1Click(TObject *Sender)
{
 OptionWnd->ShowModal();
 ENViewControl::DrawAll();        
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::SaveAs1Click(TObject *Sender)
{
 if(SaveDialog1->Execute())
 {
  AnsiString FName=SaveDialog1->FileName;
  AnsiString ext=ExtractFileExt(FName);
  ext.LowerCase();
  if(ext!=".emo") FName+=".emo";
  ENModelData::Save(FName.c_str());
 }
}                                      
//---------------------------------------------------------------------------
void TMainWnd::SaveQuestion()
{
 if(ENModelData::HasToSave())
 {
  ENint res=Application->MessageBox("Do you want to save???","Save...",
                                    MB_YESNO);
  if(res==IDYES)
    Save1Click(NULL);
 }    
}


void __fastcall TMainWnd::Cube1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWADDCUBE);
 UpdateToolStat();
}
//---------------------------------------------------------------------------
AnsiString TMainWnd::ToolStat2Text(ENDrawClass::ENDrawClassTool ctool)
{
 switch(ctool)
 {
  case ENDrawClass::ENDRAWMOVECAM:
    return "Move Camera";
  case ENDrawClass::ENDRAWROTATECAM:
    return "Rotate Camera";
  case ENDrawClass::ENDRAWZOOM:
    return "Zoom Camera";
  case ENDrawClass::ENDRAWMOVEOBJ:
    return "Move Object";
  case ENDrawClass::ENDRAWROTATEOBJ:
    return "Rotate Object";
  case ENDrawClass::ENDRAWSCALEOBJ:
    return "Scale Object";
  case ENDrawClass::ENDRAWADDVERTEX:
    return "Add Vertex";
  case ENDrawClass::ENDRAWADDFACE:
    return "Add Face";
  case ENDrawClass::ENDRAWADDCUBE:
    return "Add Cube";
  case ENDrawClass::ENDRAWADDCONE:
    return "Add Cone";
  case ENDrawClass::ENDRAWADDCYLINDER:
    return "Add Cylinder";
  case ENDrawClass::ENDRAWADDSPHERE:
    return "Add Sphere";
  case ENDrawClass::ENDRAWSELECT:
    return "Select";
 }

 return "";
}

void TMainWnd::UpdateToolStat()
{
 ToolStat->Text=ToolStat2Text(ENDrawClass::GetToolMode());
}

void TMainWnd::UpdateFrameStat()
{
 FrameStat->Text="Frame: "+IntToStr(ENModelData::GetCurrentFrame());
}

void __fastcall TMainWnd::Addframe1Click(TObject *Sender)
{
 ENModelData::AddFrame();
 UpdateScrollFrames();
}
//---------------------------------------------------------------------------
void TMainWnd::UpdateScrollFrames()
{
 ENModel::ENModelHeader header=ENModelData::GetHeader();
 ScrollFrames->Min=0;
 ScrollFrames->Max=header.NumFrames-1;
 ScrollFrames->Position=ENModelData::GetCurrentFrame();
 UpdateFrameStat();
}
void __fastcall TMainWnd::ScrollFramesChange(TObject *Sender)
{
 ENModelData::SetCurrentFrame(ScrollFrames->Position);
 ENViewControl::DrawAll();

 UpdateFrameStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Cone1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWADDCONE);
 UpdateToolStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Cylinder1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWADDCYLINDER);
 UpdateToolStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Sphere1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWADDSPHERE);
 UpdateToolStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Groupmanager1Click(TObject *Sender)
{
 GroupPnl->Visible=!GroupPnl->Visible;
 Groupmanager1->Checked=GroupPnl->Visible;        
}
//---------------------------------------------------------------------------
void TMainWnd::UpdateGroupList()
{
 ENModel::ENModelHeader header=ENModelData::GetHeader();
 GroupList->Clear();
 for(ENuint a=0;a<header.NumGroups;a++)
  GroupList->Items->Add(ENModelData::GetGroupName(a));
}

void __fastcall TMainWnd::CreateGroupBtnClick(TObject *Sender)
{
 char GroupIdent[33];
 strcpy(GroupIdent,GroupName->Text.c_str());
 ENModelData::CreateGroup(GroupIdent);
 UpdateGroupList();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::RenameGroupBtnClick(TObject *Sender)
{
 char GroupIdent[33];
 if(GroupList->ItemIndex>=0)
 {
  strcpy(GroupIdent,GroupName->Text.c_str());
  ENModelData::RenameGroup(GroupList->ItemIndex,GroupIdent);
  UpdateGroupList();
 }
 else
   ShowMessage("You must select a group!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::DeleteGroupBtnClick(TObject *Sender)
{
 if(GroupList->ItemIndex>=0)
 {
  ENModelData::DeleteGroup(GroupList->ItemIndex);
  UpdateGroupList();
 }
 else
   ShowMessage("You must select a group!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Modelstatus1Click(TObject *Sender)
{
 ModelStatDlg->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::SelectGroupBtnClick(TObject *Sender)
{
 if(GroupList->ItemIndex>=0)
 {
  ENModelData::SelectNone();
  ENModelData::SelectGroup(GroupList->ItemIndex);
  ENViewControl::DrawAll();
 }
 else
   ShowMessage("You must select a group!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::AddGeometryBtnClick(TObject *Sender)
{
 if(GroupList->ItemIndex>=0)
   ENModelData::AddGeometryToGroup(GroupList->ItemIndex);
 else
   ShowMessage("You must select a group!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::DeleteFaces1Click(TObject *Sender)
{
 ENModelData::DeleteSelectedFaces();
 ENViewControl::DrawAll();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::DeleteVertexes1Click(TObject *Sender)
{
 ENModelData::DeleteSelectedVertexes();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Selectconnected1Click(TObject *Sender)
{
 ENModelData::SelectConnected();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FlipNormals1Click(TObject *Sender)
{
 ENModelData::FlipNormals();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Optimize1Click(TObject *Sender)
{
 OptimizeDlg->ShowModal();
 ENViewControl::DrawAll();        
}
//---------------------------------------------------------------------------


void __fastcall TMainWnd::SkinEditor1Click(TObject *Sender)
{
 SkinEdt->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Deleteframe1Click(TObject *Sender)
{
 ENModelData::DeleteFrame();
 UpdateScrollFrames();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------



void __fastcall TMainWnd::Import3DS1Click(TObject *Sender)
{
 SaveQuestion();
 Import3DSDlg->ShowModal();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::ExportEAM1Click(TObject *Sender)
{
 if(SaveDialog2->Execute())
 {
  AnsiString FName=SaveDialog2->FileName;
  AnsiString ext=ExtractFileExt(FName);
  ext.LowerCase();
  if(ext!=".eam") FName+=".eam";
  ENModelData::ExportToASCII(FName.c_str());
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::ImportEAM1Click(TObject *Sender)
{
 SaveQuestion();
 ENModelData::New();
 if(OpenDialog2->Execute())
   ENModelData::ImportFromASCII(OpenDialog2->FileName.c_str());

 ENViewControl::DrawAll();
 UpdateScrollFrames();
 UpdateGroupList();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::BtnDirClick(TObject *Sender)
{
 BtnDir->Tag=(BtnDir->Tag+1)%3;
 switch(BtnDir->Tag)
 {
  case 0:
    BtnDir->Glyph->LoadFromResourceName((ENuint)HInstance,"DIRHOZ");
  break;
  case 1:
    BtnDir->Glyph->LoadFromResourceName((ENuint)HInstance,"DIRVER");
  break;
  case 2:
    BtnDir->Glyph->LoadFromResourceName((ENuint)HInstance,"DIRALL");
  break;
 };
 ENDrawClass::SetDirMode((char)(BtnDir->Tag+1));
}
//---------------------------------------------------------------------------



