//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ENDrawClass.h"
#include "ControlViews.h"

#include "ModelData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
ENDrawClass::ENDrawOptions ENDrawClass::options;
ENDrawClass::ENDrawClassTool ENDrawClass::tool=ENDrawClass::ENDRAWMOVECAM;
ENDrawClass::ENDrawClassMode ENDrawClass::mode=ENDrawClass::ENDRAWTEX;
char                         ENDrawClass::Direction=3;
TStatusPanel                 *ENDrawClass::VertexState=NULL;

ENDrawClass::ENDrawClass(TForm *form,ENDrawClass::ENDrawClassTypes type)
{
 //Set type and form data
 this->form=form;
 this->type=type;
 //Init variables
 MoveCam=false;
 RotateCam=false;
 ZoomCam=false;
 Selection=false;
 MoveObj=false;
 ScaleObj=false;
 RotateObj=false;
 AddFaceStruct.state=0;
 //Init OpenGL class
 globj=new ENGLClass(form->Handle,true,true);
 if(!globj->Init())
   ShowMessage("Cannot initialize OpenGL in window "+form->Caption+"!!!");

 glEnable(GL_DEPTH_TEST);
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_CULL_FACE);
 glCullFace(GL_BACK);
 //Init modelview matrix
 ConfigStdMatrix();
}

ENDrawClass::~ENDrawClass()
{
 delete globj;
}

void ENDrawClass::SetOptions(ENDrawClass::ENDrawOptions opt)
{
 options=opt;
 ENPacker *packer=new ENPacker();
 AnsiString Path=ExtractFileDir(Application->ExeName);
 AnsiString FName=Path+"\\System.enginepack";
 if(packer->Load(FName.c_str()))
 {
  packer->DelPackage("Modeleditor");
  packer->AddPackage((ENubyte*)&options,sizeof(ENDrawOptions),ENPackerBase::ENUNKNOWN,"Modeleditor");
  if(!packer->Save(FName.c_str()))
    ShowMessage("Cannot write "+FName+"!!!");

  delete packer;    
 }
 else
 {
  delete packer;
  ShowMessage("Cannot load "+FName+"!!!");
 }
}

ENDrawClass::ENDrawOptions ENDrawClass::GetOptions()
{
 return options;
}

void ENDrawClass::InitOptions()
{
 AnsiString Path=ExtractFileDir(Application->ExeName);
 AnsiString FName=Path+"\\System.enginepack";
 ENPacker *packer=new ENPacker();
 if(packer->Load(FName.c_str()))
 {
  ENint ind=packer->SearchPackage("Modeleditor");
  if(ind>=0)
  {
   packer->GetPackageData(ind,(ENubyte*)&options);
   delete packer;
  }
  else
  {
   delete packer;
   ShowMessage("Cannot find Mapeditor Config in package "+FName+"!!!");
   SetDefaultOptions();
  }
 }
 else
 {
  delete packer;
  ShowMessage("Cannot load "+FName+"!!!");
  SetDefaultOptions();
 }
}

void ENDrawClass::SetDefaultOptions()
{
 options.GridColor=ENVector(1.0f,1.0f,1.0f);
 options.VertexColor=ENVector(1.0f,0.0f,0.0f);
 options.SVertexColor=ENVector(1.0f,1.0f,0.0f);
 options.FaceColor=ENVector(0.5f,0.5f,0.5f);
 options.SFaceColor=ENVector(0.0f,0.0f,1.0f);
 options.ToolColor=ENVector(0.0f,1.0f,0.0f);
 options.BackColor=ENVector(0.0f,0.0f,0.0f);
 options.GridSize=16.0f;
 options.NumGridLines=10;
 options.Cullfacing=true;
 options.EnableDrawVertexes=true;
 options.EnableDrawFaces=true;
 options.EnableDrawOrigin=true;
 options.EnableDrawGrid=true;
}

void ENDrawClass::ConfigStdMatrix()
{
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 switch(type)
 {
  case ENDRAWTOP:
    glTranslatef(form->ClientWidth/2,form->ClientHeight/2,0);
    glRotatef(90.0f,1,0,0);
  break;
  case ENDRAWFRONT:
    glTranslatef(form->ClientWidth/2,form->ClientHeight/2,0);
  break;
  case ENDRAWSIDE:
    glTranslatef(form->ClientWidth/2,form->ClientHeight/2,0);
    glRotatef(90.0f,0,1,0);
  break;
  case ENDRAW3D:
    glTranslatef(0.0f,0.0f,-140.0f);
    glRotatef( 45.0f,1.0f,0.0f,0.0f);
    glRotatef(-45.0f,0.0f,1.0f,0.0f);
  break;
 }
 glGetDoublev(GL_MODELVIEW_MATRIX,Matrices.modelMatrix);
}

void ENDrawClass::ConfigProjMatrix()
{
 if(type==ENDRAW3D)
 {
  globj->Set();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0,0,form->ClientWidth,form->ClientHeight);
  ENfloat aspect=(ENfloat)form->ClientWidth/form->ClientHeight;
  gluPerspective(60, aspect,1.0, 10000.0f);
  glMatrixMode(GL_MODELVIEW);
 }
 else
   globj->ConfigMatrix(0,0,form->ClientWidth,form->ClientHeight);

 glGetDoublev(GL_PROJECTION_MATRIX,Matrices.projMatrix);
 glGetIntegerv(GL_VIEWPORT,Matrices.viewport);
}

void ENDrawClass::DrawFaces()
{
 //Variables
 ENModel::ENModelHeader header;
 ENModel::ENModelFace   face;
 ENuint a;
 //Get model header
 header=ENModelData::GetHeader();
 //Init
 glEnableClientState(GL_VERTEX_ARRAY);
 ENModelData::SetGLVertexes();
 //Set cullfacing on/off
 if(options.Cullfacing)
   glEnable(GL_CULL_FACE);
 else
   glDisable(GL_CULL_FACE);
 //Set face modus
 if(mode==ENDRAWWIRE)
   glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
 else
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

 if(mode==ENDRAWTEX)
 {
  glEnable(GL_TEXTURE_2D);
  ENModelData::ActivateCurrentSkin();
 }
 //Transform modelview matrix
 glPushMatrix();
 MoveMatrixOutOfDepth();
 //Draw selected triangles
 glBegin(GL_TRIANGLES);
 glColor3fv((ENfloat*)&options.SFaceColor);
 for(a=0;a<header.NumFaces;a++)
   if(ENModelData::IsFaceSelected(a))
   {
    face=ENModelData::GetFace(a);
    glTexCoord2fv((ENfloat*)&face.Coord[0]);
    glArrayElement(face.Indices[0]);
    glTexCoord2fv((ENfloat*)&face.Coord[1]);
    glArrayElement(face.Indices[1]);
    glTexCoord2fv((ENfloat*)&face.Coord[2]);
    glArrayElement(face.Indices[2]);
 }
 //Draw not selected triangles
 glColor3fv((ENfloat*)&options.FaceColor);
 for(a=0;a<header.NumFaces;a++)
   if(!ENModelData::IsFaceSelected(a))
   {
    face=ENModelData::GetFace(a);
    glTexCoord2fv((ENfloat*)&face.Coord[0]);
    glArrayElement(face.Indices[0]);
    glTexCoord2fv((ENfloat*)&face.Coord[1]);
    glArrayElement(face.Indices[1]);
    glTexCoord2fv((ENfloat*)&face.Coord[2]);
    glArrayElement(face.Indices[2]);
 }
 glEnd();
 //Reset
 glPopMatrix();
 glDisable(GL_TEXTURE_2D);
 glDisable(GL_CULL_FACE);
 glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
 glDisableClientState(GL_VERTEX_ARRAY);
}

void ENDrawClass::DrawVertexes()
{
 //Variables
 ENModel::ENModelHeader header;
 //Get model header
 header=ENModelData::GetHeader();
 //Init
 glDisable(GL_DEPTH_TEST);
 glEnableClientState(GL_VERTEX_ARRAY);
 ENModelData::SetGLVertexes();
 //Transform modelview matrix
 glPushMatrix();
 MoveMatrixOutOfDepth();
 //Draw
 glPointSize(3.0f);
 glBegin(GL_POINTS);
 for(ENuint a=0;a<header.NumVertexes;a++)
 {
  if(ENModelData::IsVertSelected(a))
    glColor3fv((ENfloat*)&options.SVertexColor);
  else
    glColor3fv((ENfloat*)&options.VertexColor);

  glArrayElement(a);
 }

 glEnd();
 //Reset
 glPopMatrix();
 glDisableClientState(GL_VERTEX_ARRAY);
 glEnable(GL_DEPTH_TEST);
}

void ENDrawClass::DrawGrid()
{
 glBegin(GL_LINES);
 glColor3fv((ENfloat*)&options.GridColor);
 for(ENint x=-options.NumGridLines;x<=options.NumGridLines;x++)
 {
  glVertex3f(-options.NumGridLines*options.GridSize,0,x*options.GridSize);
  glVertex3f( options.NumGridLines*options.GridSize,0,x*options.GridSize);

  glVertex3f(x*options.GridSize,0,-options.NumGridLines*options.GridSize);
  glVertex3f(x*options.GridSize,0, options.NumGridLines*options.GridSize);
 }
 glEnd();
}

void ENDrawClass::DrawOrigin()
{
 glDisable(GL_DEPTH_TEST);
 glBegin(GL_LINES);

 glColor3f(1,0,0);
 glVertex3f(0.0f,0.0f,0.0f);
 glVertex3f(10.0f,0.0f,0.0f);

 glColor3f(0,1,0);
 glVertex3f(0.0f,0.0f,0.0f);
 glVertex3f(0.0f,10.0f,0.0f);

 glColor3f(0,0,1);
 glVertex3f(0.0f,0.0f,0.0f);
 glVertex3f(0.0f,0.0f,10.0f);

 glEnd();
 glEnable(GL_DEPTH_TEST);
}

void ENDrawClass::BeginDraw2D()
{
 glDisable(GL_DEPTH_TEST);
 glPushMatrix();
 glLoadIdentity();
 glMatrixMode(GL_PROJECTION);
 glPushMatrix();
 glLoadIdentity();
 gluOrtho2D(0,form->ClientWidth,form->ClientHeight,0);
}

void ENDrawClass::EndDraw2D()
{
 glPopMatrix();
 glMatrixMode(GL_MODELVIEW);
 glPopMatrix();
 glEnable(GL_DEPTH_TEST);
}

void ENDrawClass::DrawSelection()
{
 glBegin(GL_LINES);
 glColor3fv((ENfloat*)&options.ToolColor);
 glVertex3f(SelectFrom.v[0],SelectFrom.v[1],0.1f);
 glVertex3f(SelectTo.v[0],SelectFrom.v[1],0.1f);

 glVertex3f(SelectTo.v[0],SelectFrom.v[1],0.1f);
 glVertex3f(SelectTo.v[0],SelectTo.v[1],0.1f);

 glVertex3f(SelectFrom.v[0],SelectFrom.v[1],0.1f);
 glVertex3f(SelectFrom.v[0],SelectTo.v[1],0.1f);

 glVertex3f(SelectFrom.v[0],SelectTo.v[1],0.1f);
 glVertex3f(SelectTo.v[0],SelectTo.v[1],0.1f);

 glEnd();
}

void ENDrawClass::DrawAddFaceTool()
{
 ENVector vert;
 glColor3fv((ENfloat*)&options.ToolColor);
 glBegin(GL_LINES);
 switch(AddFaceStruct.state)
 {
  case 2:
    vert=ENModelData::GetVertex(AddFaceStruct.Ind[1]);
    vert=ENModelData::From3DTo2D(vert,Matrices);
    glVertex3f(vert.v[0],form->ClientHeight-vert.v[1],0.1f);
    vert=ENModelData::GetVertex(AddFaceStruct.Ind[0]);
    vert=ENModelData::From3DTo2D(vert,Matrices);
    glVertex3f(vert.v[0],form->ClientHeight-vert.v[1],0.1f);
    vert=ENModelData::GetVertex(AddFaceStruct.Ind[1]);
    vert=ENModelData::From3DTo2D(vert,Matrices);
    glVertex3f(vert.v[0],form->ClientHeight-vert.v[1],0.1f);
    glVertex3f(Save2DPos.v[0],Save2DPos.v[1],0.1f);
  case 1:
    vert=ENModelData::GetVertex(AddFaceStruct.Ind[0]);
    vert=ENModelData::From3DTo2D(vert,Matrices);
    glVertex3f(vert.v[0],form->ClientHeight-vert.v[1],0.1f);
    glVertex3f(Save2DPos.v[0],Save2DPos.v[1],0.1f);
  break;
 }
 glEnd(); 
}

void ENDrawClass::Draw()
{
 globj->Set();
 glClearColor(options.BackColor.v[0],options.BackColor.v[1],options.BackColor.v[2],1.0f);
 globj->Start();
 Transform();
 if(options.EnableDrawGrid)
   DrawGrid();
 if(options.EnableDrawOrigin)
   DrawOrigin();
 if(options.EnableDrawFaces)
   DrawFaces();
 if(options.EnableDrawVertexes)
   DrawVertexes();
 //2D elements
 BeginDraw2D();
 if(Selection)
   DrawSelection();
 if(tool==ENDRAWADDFACE)
   DrawAddFaceTool();
 globj->End();

 EndDraw2D();
}

void ENDrawClass::OnMouseDown(ENint x,ENint y,ENbool eShift)
{
 ENint Vertind;
 Save2DPos=ENVector2((ENfloat)x,(ENfloat)y);
 Save2DPos=DirectionRestriction(Save2DPos);
 switch(tool)
 {
  case ENDRAWMOVECAM:
    if(!RotateCam&&!ZoomCam)
      MoveCam=true;
  break;
  case ENDRAWROTATECAM:
    if(!MoveCam&&!ZoomCam&&type==ENDRAW3D)
      RotateCam=true;
  break;
  case ENDRAWZOOM:
    if(!MoveCam&&!RotateCam)
      ZoomCam=true;
  break;
  case ENDRAWMOVEOBJ:
    MoveObj=true;
    TempModVec=ENModelData::From2DTo3D(
                ENVector2(Save2DPos.v[0],form->ClientHeight-Save2DPos.v[1]),
                0.8,Matrices);
  break;
  case ENDRAWSCALEOBJ:
    ScaleObj=true;
    CenterVec=ENModelData::Center(true);
    TempModVec=ENModelData::From2DTo3D(
                ENVector2(Save2DPos.v[0],form->ClientHeight-Save2DPos.v[1]),
                0.8,Matrices);
  break;
  case ENDRAWROTATEOBJ:
    RotateObj=true;
    CenterVec=ENModelData::Center(true);
    TempModVec=ENModelData::From2DTo3D(
                ENVector2(Save2DPos.v[0],form->ClientHeight-Save2DPos.v[1]),
                0.8,Matrices);
  break;
  case ENDRAWADDVERTEX:
    globj->Set();
    ENModelData::AddVertex(ENVector2(x,form->ClientHeight-y),type,Matrices);
    ENViewControl::DrawAll();
  break;
  case ENDRAWADDCUBE:
    globj->Set();
    ENModelData::AddPrefab(ENModelData::ENPREFABCUBE,ENVector2(x,form->ClientHeight-y),type,Matrices);
    ENViewControl::DrawAll();
  break;
  case ENDRAWADDCONE:
    globj->Set();
    ENModelData::AddPrefab(ENModelData::ENPREFABCONE,ENVector2(x,form->ClientHeight-y),type,Matrices);
    ENViewControl::DrawAll();
  break;
  case ENDRAWADDCYLINDER:
    globj->Set();
    ENModelData::AddPrefab(ENModelData::ENPREFABCYLINDER,ENVector2(x,form->ClientHeight-y),type,Matrices);
    ENViewControl::DrawAll();
  break;
  case ENDRAWADDSPHERE:
    globj->Set();
    ENModelData::AddPrefab(ENModelData::ENPREFABSPHERE,ENVector2(x,form->ClientHeight-y),type,Matrices);
    ENViewControl::DrawAll();
  break;
  case ENDRAWADDFACE:
    Vertind=ENModelData::SelectOneVert(ENVector2((ENfloat)x,
                         (ENfloat)(form->ClientHeight-y)),type,Matrices);
    if(Vertind==-1)
      AddFaceStruct.state=0;
    else
      switch(AddFaceStruct.state)
      {
       case 0:
         AddFaceStruct.Ind[0]=Vertind;
         AddFaceStruct.state++;
       break;
       case 1:
         AddFaceStruct.Ind[1]=Vertind;
         AddFaceStruct.state++;
       break;
       case 2:
         AddFaceStruct.Ind[2]=Vertind;
         AddFaceStruct.state=0;
         ENModelData::AddFace(AddFaceStruct.Ind);
         ENViewControl::DrawAll();
       break;
      }
  break;
  case ENDRAWSELECT:
    if(!eShift)
      ENModelData::SelectNone();

    SelectFrom=ENVector2((ENfloat)x,(ENfloat)y);
    SelectTo=SelectFrom;
    Selection=true;
  break;
 }
}

void ENDrawClass::OnMouseMove(ENint x,ENint y)
{
 ENVector tvec;
 char Axis;
 ENfloat angle;
 ENVector2 tmp2dvec;
 //Activate current OpenGL context
 globj->Set();
 glPushMatrix();
 //Calculate mousevector
 ENVector2 mousevec=ENVector2((ENfloat)x,(ENfloat)y)-Save2DPos;
 Save2DPos=ENVector2((ENfloat)x,(ENfloat)y);
 //Move Camera operations
 if(MoveCam)
 {
  glLoadIdentity();
  if(type==ENDRAW3D)
    glTranslatef(mousevec.v[0],0,-mousevec.v[1]);
  else
    glTranslatef(mousevec.v[0],-mousevec.v[1],0);

  glMultMatrixd(Matrices.modelMatrix);
  glGetDoublev(GL_MODELVIEW_MATRIX,Matrices.modelMatrix);
  Draw();
 }
 //Rotate Camera operations
 if(RotateCam)
 {
  glLoadIdentity();
  glRotatef(mousevec.v[1],1,0,0);  
  glMultMatrixd(Matrices.modelMatrix);
  glRotatef(mousevec.v[0],0,1,0);

  glGetDoublev(GL_MODELVIEW_MATRIX,Matrices.modelMatrix);
  Draw();
 }
 //Zoom Camera operations
 if(ZoomCam)
 {
  glLoadIdentity();
  mousevec=ENVector2(1,1)+ENVector2(1,1)*mousevec.v[1]*0.001;
  glScalef(mousevec.v[0],mousevec.v[1],1);
  glMultMatrixd(Matrices.modelMatrix);
  glGetDoublev(GL_MODELVIEW_MATRIX,Matrices.modelMatrix);
  Draw();
 }
 //Move object
 if(MoveObj)
 {
  tmp2dvec=DirectionRestriction(ENVector2(x,y));
  tvec=ENModelData::From2DTo3D(ENVector2(tmp2dvec.v[0],
              form->ClientHeight-tmp2dvec.v[1]),0.8,Matrices);
  ENModelData::Translate(tvec-TempModVec,true);
  TempModVec=tvec;
  ENViewControl::DrawAll();
 }
 //Scale object
 if(ScaleObj)
 {
  tmp2dvec=DirectionRestriction(ENVector2(x,y));
  tvec=ENModelData::From2DTo3D(ENVector2(tmp2dvec.v[0],
                        form->ClientHeight-tmp2dvec.v[1]),0.8,Matrices);
  ENModelData::Scale(CenterVec,tvec-TempModVec,true);
  TempModVec=tvec;
  ENViewControl::DrawAll();
 }
 //Rotate object
 if(RotateObj&&type!=ENDRAW3D)
 {
  switch(type)
  {
   case ENDRAWTOP:
     Axis='y';
   break;
   case ENDRAWFRONT:
     Axis='z';
   break;
   case ENDRAWSIDE:
     Axis='x';
   break;
  }
  tvec=ENModelData::From2DTo3D(ENVector2(x,form->ClientHeight-y),0.8,Matrices);
  angle=ENLength(mousevec);
  if(angle>0.0f)
  {
   angle=ENLength(tvec)/angle*mousevec.v[1];
   ENModelData::Rotate(CenterVec,angle/100000.0f,Axis,true);
  }
  ENViewControl::DrawAll();                      
 }
 //Process Selection
 if(Selection)
 {
  SelectTo=ENVector2((ENfloat)x,(ENfloat)y);
  Draw();
 }
 //Call draw command if add face mode
 if(tool==ENDRAWADDFACE)
   Draw();

 glPopMatrix();
}

void ENDrawClass::OnMouseUp()
{
 MoveCam=false;
 ZoomCam=false;
 RotateCam=false;
 MoveObj=false;
 RotateObj=false;
 ScaleObj=false;
 if(Selection)
 {
  SelectFrom.v[1]=form->ClientHeight-SelectFrom.v[1];
  SelectTo.v[1]=form->ClientHeight-SelectTo.v[1];
  ENModelData::Select(SelectFrom,SelectTo,type,Matrices);
  Selection=false;
  ENViewControl::DrawAll();
  ENint VertInd=ENModelData::GetSelectedVertInd();
  if(VertInd>=0)
    VertexState->Text=AnsiString("Current vertex: ")+VertInd;
  else
    VertexState->Text="Current vertex: ";
 }
}

void ENDrawClass::SetToolMode(ENDrawClass::ENDrawClassTool tool)
{
 ENDrawClass::tool=tool;
}

ENDrawClass::ENDrawClassTool ENDrawClass::GetToolMode()
{
 return tool;
}

void ENDrawClass::SetViewMode(ENDrawClass::ENDrawClassMode mode)
{
 ENDrawClass::mode=mode;
}

void ENDrawClass::SetDirMode(char Direction)
{
 ENDrawClass::Direction=Direction;
}

void ENDrawClass::SetVertState(TStatusPanel *VertState)
{
 VertexState=VertState;
}

void ENDrawClass::Transform()
{
 glMatrixMode(GL_PROJECTION);
 glLoadMatrixd(Matrices.projMatrix);
 glViewport(Matrices.viewport[0],Matrices.viewport[1],
            Matrices.viewport[2],Matrices.viewport[3]);
 glMatrixMode(GL_MODELVIEW);
 glLoadMatrixd(Matrices.modelMatrix);

}

void ENDrawClass::MoveMatrixOutOfDepth()
{
 ENfloat val=0.0f;
 switch(type)
 {
  case ENDRAWTOP:
    val=ENModelData::GetMaxVal('y');
    if(val>0.0f) glTranslatef(0,-val,0);
  break;
  case ENDRAWFRONT:
    val=ENModelData::GetMaxVal('z');
    if(val>0.0f) glTranslatef(0,0,-val);
  break;
  case ENDRAWSIDE:
    val=ENModelData::GetMinVal('x');
    if(val<0.0f) glTranslatef(-val,0,0);
  break;
 }
}

ENVector2 ENDrawClass::DirectionRestriction(ENVector2 vec)
{
 if((Direction&1)!=1)
   vec.v[0]=0.0f;
 if((Direction&2)!=2)
   vec.v[1]=0.0f;

 return vec;
}
