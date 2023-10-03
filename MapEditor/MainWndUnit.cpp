//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MainWndUnit.h"
#include "MapData.h"
#include "MapSources.h"
#include "ControlViews.h"
#include "OptionWndUnit.h"
#include "AboutWndUnit.h"
#include "MapStatUnit.h"
#include "FaceDlgUnit.h"
#include "BuildDlgUnit.h"
#include "ImportQuake3Dlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainWnd *MainWnd;
//---------------------------------------------------------------------------
__fastcall TMainWnd::TMainWnd(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::FormCreate(TObject *Sender)
{
 AnsiString FileExt;
 FileExt=ENPicture::GetImageExtStr();
 FileExt+="|Engine material(*.emat)|*.emat";
 OpenMaterialDlg->Filter=FileExt;

 ENMapData::Init();
 ENMapSources::Init();
 ENDrawClass::InitOptions();
 ToolStat=MainWndStat->Panels->Items[0];
 BtnDir->Glyph->LoadFromResourceName((ENuint)HInstance,"DIRALL");
 //Call updates
 UpdateToolStat();
 InitPreviewImage();
}
//---------------------------------------------------------------------------
void TMainWnd::InitPreviewImage()
{
 TexPrevImg->Picture->Bitmap->Width=1;
 TexPrevImg->Picture->Bitmap->Height=1;
 TexPrevImg->Picture->Bitmap->Canvas->Pixels[0][0]=clWhite;
}

void __fastcall TMainWnd::FormDestroy(TObject *Sender)
{
 SaveQuestion();
 ENMapData::Free();
 ENMapSources::Free();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormShow(TObject *Sender)
{
 ENViewControl::Init(this,PopupMenu1);
 //If open command
 if(ParamCount())
 {
  ENMapData::Open(ParamStr(1).c_str());
  ENViewControl::DrawAll();
  InitPreviewImage();
  UpdateGroupList();
  UpdateTextureList();
 }
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
  case ENDrawClass::ENDRAWMOVETEX:
    return "Move Texture";
  case ENDrawClass::ENDRAWROTATETEX:
    return "Rotate Texture";
  case ENDrawClass::ENDRAWSCALETEX:
    return "Scale Texture";
  case ENDrawClass::ENDRAWADDVERTEX:
    return "Add Vertex";
  case ENDrawClass::ENDRAWADDFACE:
    return "Add Face";
  case ENDrawClass::ENDRAWADDLIGHT:
    return "Add Light";
  case ENDrawClass::ENDRAWADDSOUND:
    return "Add Sound";    
  case ENDrawClass::ENDRAWADDOBJECT:
    return "Add Object";
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

void TMainWnd::UpdateGroupList()
{
 ENMapBase::ENMapHeader header=ENMapData::GetHeader();
 GroupList->Clear();
 for(ENuint a=0;a<header.NumGroups;a++)
  GroupList->Items->Add(ENMapData::GetGroupName(a));
}

void TMainWnd::UpdateTextureList()
{
 ENMapBase::ENMapHeader header=ENMapData::GetHeader();
 TextureList->Clear();
 for(ENuint a=0;a<header.NumTextures;a++)
  TextureList->Items->Add(ENMapData::GetTextureName(a));
}

void __fastcall TMainWnd::CreateGroupBtnClick(TObject *Sender)
{
 char GroupIdent[33];
 strcpy(GroupIdent,GroupName->Text.c_str());
 ENMapData::CreateGroup(GroupIdent);
 UpdateGroupList();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::RenameGroupBtnClick(TObject *Sender)
{
 char GroupIdent[33];
 if(GroupList->ItemIndex>=0)
 {
  strcpy(GroupIdent,GroupName->Text.c_str());
  ENMapData::RenameGroup(GroupList->ItemIndex,GroupIdent);
  UpdateGroupList();
 }
 else
   ShowMessage("You must select a group!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::SelectGroupBtnClick(TObject *Sender)
{
 if(GroupList->ItemIndex>=0)
 {
  ENMapData::SelectNone();
  ENMapData::SelectGroup(GroupList->ItemIndex);
  ENViewControl::DrawAll();
 }
 else
   ShowMessage("You must select a group!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::DeleteGroupBtnClick(TObject *Sender)
{
 if(GroupList->ItemIndex>=0)
 {
  ENMapData::DeleteGroup(GroupList->ItemIndex);
  UpdateGroupList();
 }
 else
   ShowMessage("You must select a group!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::AddGeometryBtnClick(TObject *Sender)
{
 if(GroupList->ItemIndex>=0)
   ENMapData::AddGeometryToGroup(GroupList->ItemIndex);
 else
   ShowMessage("You must select a group!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Select1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWSELECT);
 UpdateToolStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Selectall1Click(TObject *Sender)
{
 ENMapData::SelectAll();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Invertselection1Click(TObject *Sender)
{
 ENMapData::InvertSelected();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Selectnothing1Click(TObject *Sender)
{
 ENMapData::SelectNone();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Selectconnected1Click(TObject *Sender)
{
 ENMapData::SelectConnected();
 ENViewControl::DrawAll();
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

void __fastcall TMainWnd::Vertex1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWADDVERTEX);
 UpdateToolStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Wireframe1Click(TObject *Sender)
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

void __fastcall TMainWnd::Exit1Click(TObject *Sender)
{
 Close();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Cascade1Click(TObject *Sender)
{
 ENViewControl::Cascade();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Tile1Click(TObject *Sender)
{
 ENViewControl::Tile();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::manager1Click(TObject *Sender)
{
 ManagerPnl->Visible=!ManagerPnl->Visible;
 manager1->Checked=ManagerPnl->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Options1Click(TObject *Sender)
{
 OptionWnd->ShowModal();
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

void __fastcall TMainWnd::Cube1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWADDCUBE);
 UpdateToolStat();
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

void __fastcall TMainWnd::DeleteFaces1Click(TObject *Sender)
{
 ENMapData::DeleteSelectedFaces();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::DeleteVertexes1Click(TObject *Sender)
{
 ENMapData::DeleteSelected();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FlipNormals1Click(TObject *Sender)
{
 ENMapData::FlipNormals();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::New1Click(TObject *Sender)
{
 SaveQuestion();
 ENMapData::New();
 ENViewControl::SelAllViewsDefault();
 ENViewControl::DrawAll();

 UpdateGroupList();
 UpdateTextureList();
 InitPreviewImage();
}
//---------------------------------------------------------------------------
void TMainWnd::SaveQuestion()
{
 if(ENMapData::HasToSave())
 {
  ENint res=Application->MessageBox("Do you want to save???","Save...",
                                    MB_YESNO);
  if(res==IDYES)
    Save1Click(NULL);
 }    
}

void __fastcall TMainWnd::SaveAs1Click(TObject *Sender)
{
 if(SaveDialog1->Execute())
 {
  AnsiString FName=SaveDialog1->FileName;
  AnsiString ext=ExtractFileExt(FName);
  ext.LowerCase();
  if(ext!=".enmap") FName+=".enmap";
  ENMapData::Save(FName.c_str());
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Save1Click(TObject *Sender)
{
 if(!ENMapData::Save())
   SaveAs1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Open1Click(TObject *Sender)
{
 SaveQuestion();
 if(OpenDialog1->Execute())
 {
  ENMapData::Open(OpenDialog1->FileName.c_str());
  ENViewControl::DrawAll();
 }

 InitPreviewImage();
 UpdateGroupList();
 UpdateTextureList();
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

void __fastcall TMainWnd::CreateTextureBtnClick(TObject *Sender)
{
 if(OpenMaterialDlg->Execute())
 {
  char TexIdent[33];
  strcpy(TexIdent,TextureName->Text.c_str());
  ENMapData::CreateTexture(TexIdent,OpenMaterialDlg->FileName.c_str());
  UpdateTextureList();
  if(TextureList->Items->Count)
  {
   TextureList->ItemIndex=TextureList->Items->Count-1;
   TextureListClick(Sender);
  }
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::ImportEAM1Click(TObject *Sender)
{
 SaveQuestion();
 ENMapData::New();
 if(OpenDialog2->Execute())
   ENMapData::ImportFromASCII(OpenDialog2->FileName.c_str());

 ENViewControl::DrawAll();
 UpdateTextureList();
 UpdateGroupList();
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
  ENMapData::ExportToASCII(FName.c_str());
 }
}
//---------------------------------------------------------------------------


void __fastcall TMainWnd::Mapstatus1Click(TObject *Sender)
{
 MapStatDlg->ShowModal();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::RenameTextureBtnClick(TObject *Sender)
{
 char TexIdent[33];
 if(TextureList->ItemIndex>=0)
 {
  strcpy(TexIdent,TextureName->Text.c_str());
  ENMapData::RenameTexture(TextureList->ItemIndex,TexIdent);
  UpdateTextureList();
 }
 else
   ShowMessage("You must select a texture!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::DeleteTextureBtnClick(TObject *Sender)
{
 if(TextureList->ItemIndex>=0)
 {
  ENMapData::DeleteTexture(TextureList->ItemIndex);
  UpdateTextureList();
  ENViewControl::DrawAll();
 }
 else
   ShowMessage("You must select a texture!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::AssignTextureBtnClick(TObject *Sender)
{
 if(TextureList->ItemIndex>=0)
 {
  ENMapData::AssignTexture(TextureList->ItemIndex);
  ENViewControl::DrawAll();
 }
 else
   ShowMessage("You must select a texture!!!");
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::TextureListClick(TObject *Sender)
{
 if(TextureList->ItemIndex>=0)
 {
  ENMapData::GetTexturePreview(TexPrevImg->Picture->Bitmap,TextureList->ItemIndex);
 }
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::Light1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWADDLIGHT);
 UpdateToolStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::EditLight1Click(TObject *Sender)
{
 ENMapData::EditLight();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Editfacepropeties1Click(TObject *Sender)
{
 FaceDlg->ShowModal();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Showall1Click(TObject *Sender)
{
 ENViewControl::ShowAll();        
}
//---------------------------------------------------------------------------



void __fastcall TMainWnd::MoveTexture1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWMOVETEX);
 UpdateToolStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::RotateTexture1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWROTATETEX);
 UpdateToolStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::ScaleTexture1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWSCALETEX);
 UpdateToolStat();
}
//---------------------------------------------------------------------------


void __fastcall TMainWnd::Object2Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWADDOBJECT);
 UpdateToolStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Editobject1Click(TObject *Sender)
{
 ENMapData::EditObject();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Build1Click(TObject *Sender)
{
 if(BuildDialog->Execute())
 {
  AnsiString FileName=BuildDialog->FileName;
  AnsiString ext=ExtractFileExt(FileName);
  ext.LowerCase();
  if(ext!=".enbuildmap") FileName+=".enbuildmap";
  BuildDlg->Build(FileName);
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::TextureListMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
 //Variables
 char TexNfo[80];
 TPoint Pos;
 //Check for texture
 Pos=TPoint(X,Y);
 ENint index=TextureList->ItemAtPos(Pos,true);
 //If texture found, so update hint
 if(index>=0)
 {
  ENMapData::GetTextureInfo((ENuint)index,TexNfo);
  TextureList->Hint=TexNfo;
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::ImportQuake3map1Click(TObject *Sender)
{
 ImportQuake3->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Sounds1Click(TObject *Sender)
{
 ENDrawClass::SetToolMode(ENDrawClass::ENDRAWADDSOUND);
 UpdateToolStat();
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Editsound1Click(TObject *Sender)
{
 ENMapData::EditSound();
 ENViewControl::DrawAll();
}
//---------------------------------------------------------------------------

