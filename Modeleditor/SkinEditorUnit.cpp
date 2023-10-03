//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SkinEditorUnit.h"
#include "ModelData.h"
#include "SkinManagerUnit.h"
#include "ENDrawClass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSkinEdt *SkinEdt;
//---------------------------------------------------------------------------
__fastcall TSkinEdt::TSkinEdt(TComponent* Owner)
        : TForm(Owner)
{
 globj=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TSkinEdt::FormShow(TObject *Sender)
{
 Draw();
 FormResize(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TSkinEdt::FormCreate(TObject *Sender)
{
 CurrentTool=ENSKINTOOLSELECT;
 Selection=false;
 MoveCoord=false;
 RotateCoord=false;
 ScaleCoord=false;
 globj=new ENGLClass(Handle,true,true);
 if(!globj->Init())
   ShowMessage("Cannot initialize OpenGL!!!");
}
//---------------------------------------------------------------------------

void __fastcall TSkinEdt::FormDestroy(TObject *Sender)
{
 delete globj;
}
//---------------------------------------------------------------------------

void TSkinEdt::Draw()
{
 globj->Start();
 glDisable(GL_DEPTH_TEST);
 Transform();
 DrawSkin();
 DrawSkinFaces();
 DrawSkinVertexes();
 if(Selection)
   DrawSelectTool();
 glEnable(GL_DEPTH_TEST);
 globj->End();
}

void TSkinEdt::DrawSkin()
{
 glEnable(GL_TEXTURE_2D);
 ENModelData::ActivateCurrentSkin();
 glBegin(GL_QUADS);
 glColor3f(1,1,1);
 glTexCoord2f(0,0);
 glVertex2f(0,ClientHeight);
 glTexCoord2f(1,0);
 glVertex2f(ClientWidth,ClientHeight);
 glTexCoord2f(1,1);
 glVertex2f(ClientWidth,0);
 glTexCoord2f(0,1);
 glVertex2f(0,0);
 glEnd();
 glDisable(GL_TEXTURE_2D);
}

void TSkinEdt::DrawSkinVertexes()
{
 ENModel::ENModelHeader header=ENModelData::GetHeader();
 ENDrawClass::ENDrawOptions options=ENDrawClass::GetOptions();
 glPointSize(3.0f);
 glBegin(GL_POINTS);
 for(ENuint a=0;a<header.NumFaces;a++)
 {
  ENModel::ENModelFace face=ENModelData::GetFace(a);
  //Vertex 1
  if(ENModelData::IsCoordSelected(a*3))
    glColor3fv((ENfloat*)&options.SVertexColor);
  else
    glColor3fv((ENfloat*)&options.VertexColor);
  glVertex2f(face.Coord[0].v[0]*ClientWidth,ClientHeight-face.Coord[0].v[1]*ClientHeight);
  //Vertex 2
  if(ENModelData::IsCoordSelected(a*3+1))
    glColor3fv((ENfloat*)&options.SVertexColor);
  else
    glColor3fv((ENfloat*)&options.VertexColor);
  glVertex2f(face.Coord[1].v[0]*ClientWidth,ClientHeight-face.Coord[1].v[1]*ClientHeight);
  //Vertex 3
  if(ENModelData::IsCoordSelected(a*3+2))
    glColor3fv((ENfloat*)&options.SVertexColor);
  else
    glColor3fv((ENfloat*)&options.VertexColor);
  glVertex2f(face.Coord[2].v[0]*ClientWidth,ClientHeight-face.Coord[2].v[1]*ClientHeight);
 }
 glEnd();
}

void TSkinEdt::DrawSkinFaces()
{
 ENModel::ENModelHeader header=ENModelData::GetHeader();
 ENDrawClass::ENDrawOptions options=ENDrawClass::GetOptions();
 glBegin(GL_LINES);
 glColor3fv((ENfloat*)&options.FaceColor);
 for(ENuint a=0;a<header.NumFaces;a++)
 {
  ENModel::ENModelFace face=ENModelData::GetFace(a);
  glVertex2f(face.Coord[0].v[0]*ClientWidth,ClientHeight-face.Coord[0].v[1]*ClientHeight);
  glVertex2f(face.Coord[1].v[0]*ClientWidth,ClientHeight-face.Coord[1].v[1]*ClientHeight);

  glVertex2f(face.Coord[1].v[0]*ClientWidth,ClientHeight-face.Coord[1].v[1]*ClientHeight);
  glVertex2f(face.Coord[2].v[0]*ClientWidth,ClientHeight-face.Coord[2].v[1]*ClientHeight);

  glVertex2f(face.Coord[2].v[0]*ClientWidth,ClientHeight-face.Coord[2].v[1]*ClientHeight);
  glVertex2f(face.Coord[0].v[0]*ClientWidth,ClientHeight-face.Coord[0].v[1]*ClientHeight);
 }
 glEnd();
}

void TSkinEdt::DrawSkinGrid()
{
 ENModel::ENModelHeader header=ENModelData::GetHeader();
 ENDrawClass::ENDrawOptions options=ENDrawClass::GetOptions();
 glBegin(GL_LINES);
 ENDrawClass::GetOptions();
 glColor3fv((ENfloat*)&options.FaceColor);
 for(ENuint a=0;a<header.NumFaces;a++)
 {
  ENModel::ENModelFace face=ENModelData::GetFace(a);
  glVertex2f(face.Coord[0].v[0]*ClientWidth,face.Coord[0].v[0]*ClientHeight);
  glVertex2f(face.Coord[1].v[0]*ClientWidth,face.Coord[1].v[0]*ClientHeight);

  glVertex2f(face.Coord[1].v[0]*ClientWidth,face.Coord[1].v[0]*ClientHeight);
  glVertex2f(face.Coord[2].v[0]*ClientWidth,face.Coord[2].v[0]*ClientHeight);

  glVertex2f(face.Coord[2].v[0]*ClientWidth,face.Coord[2].v[0]*ClientHeight);
  glVertex2f(face.Coord[0].v[0]*ClientWidth,face.Coord[0].v[0]*ClientHeight);
 }
 glEnd();
}

void __fastcall TSkinEdt::FormResize(TObject *Sender)
{
 globj->ConfigMatrix(0.0f,0.0f,ClientWidth,ClientHeight);
 mspec.ENGetMatrixSpec();
 Draw();
}
//---------------------------------------------------------------------------

void __fastcall TSkinEdt::FormPaint(TObject *Sender)
{
 Draw();
}
//---------------------------------------------------------------------------
void TSkinEdt::Transform()
{
 glMatrixMode(GL_PROJECTION);
 glLoadMatrixd(mspec.projMatrix);
 glMatrixMode(GL_MODELVIEW);
 glLoadMatrixd(mspec.modelMatrix);
}
void __fastcall TSkinEdt::SkinManager1Click(TObject *Sender)
{
 SkinManagerDlg->ShowModal();        
}
//---------------------------------------------------------------------------

void __fastcall TSkinEdt::Top1Click(TObject *Sender)
{
 ENModelData::MakeSkinGridTop();
 Draw();
}
//---------------------------------------------------------------------------

void __fastcall TSkinEdt::Front1Click(TObject *Sender)
{
 ENModelData::MakeSkinGridFront();
 Draw();
}
//---------------------------------------------------------------------------

void __fastcall TSkinEdt::Side1Click(TObject *Sender)
{
 ENModelData::MakeSkinGridSide();
 Draw();
}
//---------------------------------------------------------------------------


void __fastcall TSkinEdt::Cube1Click(TObject *Sender)
{
 ENModelData::MakeSkinGridCube();
 Draw();
}
//---------------------------------------------------------------------------

void __fastcall TSkinEdt::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 ENVector2 temp[2];
 if(Button!=mbLeft) return;
 Current=ENVector2(X,ClientHeight-Y);
 switch(CurrentTool)
 {
  case ENSKINTOOLSELECT:
    if(!Shift.Contains(ssShift))
      ENModelData::SelectNoneCoord();
    SelectFrom=ENVector2(X,ClientHeight-Y);
    SelectTo=SelectFrom;
    Selection=true;
  break;
  case ENSKINTOOLMOVE:
    MoveCoord=true;
  break;
  case ENSKINTOOLSCALE:
    ScaleCoord=true;
    ENModelData::BoxSelectedCoord(temp[0],temp[1]);
    CenterVec=(temp[0]+temp[1])/2;
  break;
  case ENSKINTOOLROT:
    RotateCoord=true;
    ENModelData::BoxSelectedCoord(temp[0],temp[1]);
    CenterVec=(temp[0]+temp[1])/2;
  break;
 }
}
//---------------------------------------------------------------------------

void __fastcall TSkinEdt::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
 if(Button!=mbLeft) return;
 MoveCoord=false;
 RotateCoord=false;
 ScaleCoord=false;
 if(Selection)
 {
  Selection=false;
  SelectFrom.v[0]/=(ENfloat)ClientWidth;
  SelectFrom.v[1]/=(ENfloat)ClientHeight;
  SelectTo.v[0]/=(ENfloat)ClientWidth;
  SelectTo.v[1]/=(ENfloat)ClientHeight;
  SelectFrom.v[1]=1.0f-SelectFrom.v[1];
  SelectTo.v[1]=1.0f-SelectTo.v[1];
  ENModelData::SelectCoord(SelectFrom,SelectTo);
  Draw();
 }
}
//---------------------------------------------------------------------------

void __fastcall TSkinEdt::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
 VecDir=ENVector2(X,ClientHeight-Y)-Current;
 VecDir.v[0]/=(ENfloat)ClientWidth;
 VecDir.v[1]/=(ENfloat)ClientHeight;
 VecDir.v[1]=-VecDir.v[1];
 Current=ENVector2(X,ClientHeight-Y);
 if(Selection)
 {
  SelectTo=ENVector2(X,ClientHeight-Y);
  Draw();
 }
 if(MoveCoord)
 {
  ENModelData::TranslateCoord(VecDir,true);
  Draw();
 }
 if(ScaleCoord)
 {
  ENModelData::ScaleCoord(CenterVec,VecDir,true);
  Draw();
 }
 if(RotateCoord)
 {
  ENModelData::RotateCoord(CenterVec,VecDir.v[0]*50,true);
  Draw();                      
 }
}

void TSkinEdt::DrawSelectTool()
{
 ENDrawClass::ENDrawOptions options=ENDrawClass::GetOptions();
 glColor3fv((ENfloat*)&options.ToolColor);
 glBegin(GL_LINES);
 glVertex2f(SelectFrom.v[0],SelectFrom.v[1]);
 glVertex2f(SelectTo.v[0],SelectFrom.v[1]);

 glVertex2f(SelectTo.v[0],SelectFrom.v[1]);
 glVertex2f(SelectTo.v[0],SelectTo.v[1]);

 glVertex2f(SelectTo.v[0],SelectTo.v[1]);
 glVertex2f(SelectFrom.v[0],SelectTo.v[1]);

 glVertex2f(SelectFrom.v[0],SelectTo.v[1]);
 glVertex2f(SelectFrom.v[0],SelectFrom.v[1]);
 glEnd();
}
//---------------------------------------------------------------------------

void __fastcall TSkinEdt::Selection1Click(TObject *Sender)
{
 CurrentTool=ENSKINTOOLSELECT;
}
//---------------------------------------------------------------------------

void __fastcall TSkinEdt::Selectall1Click(TObject *Sender)
{
 ENModelData::SelectAllCoord();
 Draw();
}
//---------------------------------------------------------------------------

void __fastcall TSkinEdt::Selectnone1Click(TObject *Sender)
{
 ENModelData::SelectNoneCoord();
 Draw();
}
//---------------------------------------------------------------------------


void __fastcall TSkinEdt::Move1Click(TObject *Sender)
{
 CurrentTool=ENSKINTOOLMOVE;        
}
//---------------------------------------------------------------------------

void __fastcall TSkinEdt::Rotate1Click(TObject *Sender)
{
 CurrentTool=ENSKINTOOLROT;
}
//---------------------------------------------------------------------------

void __fastcall TSkinEdt::Scale1Click(TObject *Sender)
{
 CurrentTool=ENSKINTOOLSCALE;
}
//---------------------------------------------------------------------------

