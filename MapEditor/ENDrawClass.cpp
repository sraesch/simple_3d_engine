//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>                                            
#pragma hdrstop

#include "ENDrawClass.h"
#include "ControlViews.h"
#include "MapSources.h"

#include "MapData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
ENDrawClass::ENDrawOptions ENDrawClass::options;
ENDrawClass::ENDrawClassTool ENDrawClass::tool=ENDrawClass::ENDRAWMOVECAM;
ENDrawClass::ENDrawClassMode ENDrawClass::mode=ENDrawClass::ENDRAWTEX;
char                         ENDrawClass::Direction=3;

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
 MoveTex=false;
 RotateTex=false;
 ScaleTex=false;
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
  packer->DelPackage("Mapeditor");
  packer->AddPackage((ENubyte*)&options,sizeof(ENDrawOptions),ENPackerBase::ENUNKNOWN,"Mapeditor");
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
  ENint ind=packer->SearchPackage("Mapeditor");
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
 options.SLightColor=ENVector(1.0f,0.0f,1.0f);
 options.ObjColor=ENVector(1.0f,0.0f,1.0f);
 options.SObjColor=ENVector(0.0f,0.0f,1.0f);
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
 ENMapBase::ENMapHeader header;
 ENMapBase::ENMapFace   face;
 ENuint a;
 //Get model header
 header=ENMapData::GetHeader();
 //Init
 glEnableClientState(GL_VERTEX_ARRAY);
 ENMapData::SetGLVertexes();
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
   glEnable(GL_TEXTURE_2D);
 //Transform modelview matrix
 glPushMatrix();
 MoveMatrixOutOfDepth();
 //Draw selected triangles
 glColor3fv((ENfloat*)&options.SFaceColor);
 for(a=0;a<header.NumFaces;a++)
   if(ENMapData::IsFaceSelected(a))
   {
    face=ENMapData::GetFace(a);
    if(mode==ENDRAWTEX)
      ENMapData::ActivateTexture(face.texind);

    glBegin(GL_TRIANGLES);
    glTexCoord2fv((ENfloat*)&face.texcoord[0]);
    glArrayElement(face.indices[0]);
    glTexCoord2fv((ENfloat*)&face.texcoord[1]);
    glArrayElement(face.indices[1]);
    glTexCoord2fv((ENfloat*)&face.texcoord[2]);
    glArrayElement(face.indices[2]);
    glEnd();
 }
 //Draw not selected triangles
 for(a=0;a<header.NumFaces;a++)
   if(!ENMapData::IsFaceSelected(a))
   {
    face=ENMapData::GetFace(a);
    if(mode!=ENDRAWTEX||face.texind==-1)
      glColor3fv((ENfloat*)&options.FaceColor);
    else
      glColor3f(1,1,1);

    if(mode==ENDRAWTEX)
      ENMapData::ActivateTexture(face.texind);

    glBegin(GL_TRIANGLES);
    glTexCoord2fv((ENfloat*)&face.texcoord[0]);
    glArrayElement(face.indices[0]);
    glTexCoord2fv((ENfloat*)&face.texcoord[1]);
    glArrayElement(face.indices[1]);
    glTexCoord2fv((ENfloat*)&face.texcoord[2]);
    glArrayElement(face.indices[2]);
    glEnd();
 }
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
 ENMapBase::ENMapHeader header;
 //Get model header
 header=ENMapData::GetHeader();
 //Init
 glDisable(GL_DEPTH_TEST);
 glEnableClientState(GL_VERTEX_ARRAY);
 ENMapData::SetGLVertexes();
 //Transform modelview matrix
 glPushMatrix();
 MoveMatrixOutOfDepth();
 //Draw
 glPointSize(3.0f);
 glBegin(GL_POINTS);
 for(ENuint a=0;a<header.NumVertexes;a++)
 {
  if(ENMapData::IsVertSelected(a))
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

void ENDrawClass::DrawLights()
{
 //Variables
 ENMapBase::ENMapHeader header;
 //Get model header
 header=ENMapData::GetHeader();
 //Init
 glDisable(GL_DEPTH_TEST);
 //Draw
 glPointSize(3.0f);
 for(ENuint a=0;a<header.NumLights;a++)
 {
  ENLight light=ENMapData::GetLight(a);
  if(ENMapData::IsLightSelected(a))
    glColor3fv((ENfloat*)&options.SLightColor);
  else
    glColor3ub(light.color.r,light.color.g,light.color.b);

  DrawLight(light);
 }
 //Reset
 glEnable(GL_DEPTH_TEST);
}

void ENDrawClass::DrawSounds()
{
 //Variables
 ENMapBase::ENMapHeader header;
 //Get model header
 header=ENMapData::GetHeader();
 //Init
 glDisable(GL_DEPTH_TEST);
 //Draw
 glPointSize(3.0f);
 for(ENuint a=0;a<header.NumSounds;a++)
 {
  ENSoundSrc sound=ENMapData::GetSound(a);
  if(ENMapData::IsSoundSelected(a))
    glColor3fv((ENfloat*)&options.SObjColor);
  else
    glColor3fv((ENfloat*)&options.ObjColor);

  DrawSound(sound);
 }
 //Reset
 glEnable(GL_DEPTH_TEST);
}

void ENDrawClass::DrawCamera()
{
 //Init
 glPushMatrix();
 glDisable(GL_DEPTH_TEST);
 //Get camera
 ENCamera cam=ENMapData::GetCamera();
 //Move camera out of depth
 MoveCameraOutOfDepth();
 //Transform
 glTranslatef(cam.Pos.v[0],cam.Pos.v[1],cam.Pos.v[2]);
 //Set color
 if(ENMapData::IsCameraSelected())
   glColor3fv((ENfloat*)&options.SObjColor);
 else
   glColor3fv((ENfloat*)&options.ObjColor);
 //////////////////Draw camera//////////////////
 //Draw box
 glBegin(GL_LINE_LOOP);
 glVertex3f(-8, 10,20);
 glVertex3f( 8, 10,20);
 glVertex3f( 8,-10,20);
 glVertex3f(-8,-10,20);
 glEnd();
 glBegin(GL_LINE_LOOP);
 glVertex3f(-8, 10,-20);
 glVertex3f( 8, 10,-20);
 glVertex3f( 8,-10,-20);
 glVertex3f(-8,-10,-20);
 glEnd();
 glBegin(GL_LINES);
 glVertex3f(-8, 10, 20);
 glVertex3f(-8, 10,-20);
 glVertex3f( 8, 10, 20);
 glVertex3f( 8, 10,-20);
 glVertex3f( 8,-10, 20);
 glVertex3f( 8,-10,-20);
 glVertex3f(-8,-10, 20);
 glVertex3f(-8,-10,-20);
 glEnd();
 //Draw Lens
 glBegin(GL_LINE_LOOP);
 glVertex3f(-5, 0,20);
 glVertex3f( 0, 5,20);
 glVertex3f( 5, 0,20);
 glVertex3f( 0,-5,20);
 glEnd();
 glBegin(GL_LINE_LOOP);
 glVertex3f(-5, 0,22);
 glVertex3f( 0, 5,22);
 glVertex3f( 5, 0,22);
 glVertex3f( 0,-5,22);
 glEnd();
 glBegin(GL_LINES);
 glVertex3f(-5, 0,22);
 glVertex3f(-5, 0,20);
 glVertex3f( 0, 5,22);
 glVertex3f( 0, 5,20);
 glVertex3f( 5, 0,22);
 glVertex3f( 5, 0,20);
 glVertex3f( 0,-5,22);
 glVertex3f( 0,-5,20);
 glEnd();
 //Draw Rack
 glBegin(GL_LINES);
 glVertex3f( 0,-10,0);
 glVertex3f(-8,-40,20);

 glVertex3f( 0,-10,0);
 glVertex3f(-8,-40,-20);

 glVertex3f( 0,-10,0);
 glVertex3f( 8,-40,20);

 glVertex3f( 0,-10,0);
 glVertex3f( 8,-40,-20);
 glEnd();
 ///////////////////////////////////////////////
 //Reset
 glPopMatrix();
 glEnable(GL_DEPTH_TEST);
}

void ENDrawClass::DrawLight(ENLight light)
{
 //Variables
 GLUquadricObj *lightobj=gluNewQuadric();
 //Init glu object
 gluQuadricDrawStyle(lightobj,GLU_LINE);
 //Push matrix
 glPushMatrix();
 //Move light out of depth
 MoveLightOutOfDepth(light);
 //Transform
 glTranslatef(light.pos.v[0],light.pos.v[1],light.pos.v[2]);
 //Draw light
 if(!light.isSpotLight())
   gluSphere(lightobj,light.range,10,10);
 else
 {
  glRotatef(-light.dir.v[1],0,1,0);
  glRotatef(-light.dir.v[0],1,0,0);  
  gluCylinder(lightobj,0.0f,light.range*sin(ENRadians(light.spotcut)),
              light.range,10.0f,10.0f);
 }
 //Pop old matrix
 glPopMatrix();

 gluDeleteQuadric(lightobj);
}

void ENDrawClass::DrawSound(ENSoundSrc sound)
{
 //Push matrix
 glPushMatrix();
 //Move sound out of depth
 MoveSoundOutOfDepth(sound);
 //Transform
 glTranslatef(sound.pos.v[0],sound.pos.v[1],sound.pos.v[2]);
 //Draw sound
 glBegin(GL_LINES);
 glVertex3f(-sound.range,0,0);
 glVertex3f( sound.range,0,0);

 glVertex3f(0,-sound.range,0);
 glVertex3f(0, sound.range,0);

 glVertex3f(0,0,-sound.range);
 glVertex3f(0,0, sound.range);
 glEnd();
 //Pop old matrix
 glPopMatrix();
}

void ENDrawClass::DrawObjects()
{
 //Variables
 ENMapBase::ENMapHeader header;
 //Get model header
 header=ENMapData::GetHeader();
 //Draw
 for(ENuint a=0;a<header.NumObjects;a++)
 {
  ENMapBase::ENMapObject obj=ENMapData::GetObject(a);
  DrawObject(obj,a);
 }
}

void ENDrawClass::DrawObject(ENMapBase::ENMapObject obj,ENuint objind)
{
 glPushMatrix();
 MoveObjectOutOfDepth(obj);
 glTranslatef(obj.Pos.v[0],obj.Pos.v[1],obj.Pos.v[2]);
 glRotated(obj.Angle.v[0],1,0,0);
 glRotated(obj.Angle.v[1],0,1,0);
 glRotated(obj.Angle.v[2],1,0,1);
 glScalef(obj.Scale.v[0],obj.Scale.v[1],obj.Scale.v[2]);
 ENint ind=ENMapSources::GetPack(obj.Name);
 if(ind<0)
 {
  //Set color
  if(ENMapData::IsObjectSelected(objind))
    glColor3fv((ENfloat*)&options.SObjColor);
  else
    glColor3fv((ENfloat*)&options.ObjColor);
  //Draw object
  glBegin(GL_LINES);
    glVertex3f(-10.0f,0.0f,0.0f);
    glVertex3f( 10.0f,0.0f,0.0f);

    glVertex3f(0.0f,-10.0f,0.0f);
    glVertex3f(0.0f, 10.0f,0.0f);

    glVertex3f(0.0f,0.0f,-10.0f);
    glVertex3f(0.0f,0.0f, 10.0f);
  glEnd();
 }
 else
 {
  //Set color
  if(ENMapData::IsObjectSelected(objind))
    glColor3fv((ENfloat*)&options.SObjColor);
  else
    glColor3f(1.0f,1.0f,1.0f);

   glEnable(GL_TEXTURE_2D);
   ENMapSources::Draw(obj.Name);
   glDisable(GL_TEXTURE_2D);
 }
 glPopMatrix();
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

void ENDrawClass::UpdateMousePos(ENVector2 mpos)
{
 //Variables
 char Buffer[80];
 //1. Convert mouse position to 3D space
 ENVector pos=ENMapData::From2DTo3D(mpos,0.8,Matrices);
 //2. Convert to window specification
 switch(type)
 {
  case ENDrawClass::ENDRAWTOP:
    pos.v[1]=0.0f;
    sprintf(Buffer,"Top view(%f|%f|%f)",pos.v[0],pos.v[1],pos.v[2]);
  break;
  case ENDrawClass::ENDRAWFRONT:
    pos.v[2]=0.0f;
    sprintf(Buffer,"Front view(%f|%f|%f)",pos.v[0],pos.v[1],pos.v[2]);
  break;
  case ENDrawClass::ENDRAWSIDE:
    pos.v[0]=0.0f;
    sprintf(Buffer,"Side view(%f|%f|%f)",pos.v[0],pos.v[1],pos.v[2]);
  break;
  case ENDrawClass::ENDRAW3D:
  sprintf(Buffer,"3D view(%f|%f|%f)",pos.v[0],pos.v[1],pos.v[2]);
  break;
 }
 //3.Update caption
 form->Caption=Buffer;
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
    vert=ENMapData::GetVertex(AddFaceStruct.Ind[1]);
    vert=ENMapData::From3DTo2D(vert,Matrices);
    glVertex3f(vert.v[0],form->ClientHeight-vert.v[1],0.1f);
    vert=ENMapData::GetVertex(AddFaceStruct.Ind[0]);
    vert=ENMapData::From3DTo2D(vert,Matrices);
    glVertex3f(vert.v[0],form->ClientHeight-vert.v[1],0.1f);
    vert=ENMapData::GetVertex(AddFaceStruct.Ind[1]);
    vert=ENMapData::From3DTo2D(vert,Matrices);
    glVertex3f(vert.v[0],form->ClientHeight-vert.v[1],0.1f);
    glVertex3f(Save2DPos.v[0],Save2DPos.v[1],0.1f);
  case 1:
    vert=ENMapData::GetVertex(AddFaceStruct.Ind[0]);
    vert=ENMapData::From3DTo2D(vert,Matrices);
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

 DrawObjects();
 DrawCamera();
 DrawLights();
 DrawSounds();
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
  case ENDRAWMOVETEX:
    MoveTex=true;
  break;
  case ENDRAWROTATETEX:
    RotateTex=true;
  break;
  case ENDRAWSCALETEX:
    ScaleTex=true;
  break;
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
    TempModVec=ENMapData::From2DTo3D(
                ENVector2(Save2DPos.v[0],form->ClientHeight-Save2DPos.v[1]),
                0.8,Matrices);
  break;
  case ENDRAWSCALEOBJ:
    ScaleObj=true;
    CenterVec=ENMapData::Center(true);
    TempModVec=ENMapData::From2DTo3D(
                ENVector2(Save2DPos.v[0],form->ClientHeight-Save2DPos.v[1]),
                0.8,Matrices);
  break;
  case ENDRAWROTATEOBJ:
    RotateObj=true;
    CenterVec=ENMapData::Center(true);
    TempModVec=ENMapData::From2DTo3D(
                ENVector2(Save2DPos.v[0],form->ClientHeight-Save2DPos.v[1]),
                0.8,Matrices);
  break;
  case ENDRAWADDVERTEX:
    globj->Set();
    ENMapData::AddVertex(ENVector2(x,form->ClientHeight-y),type,Matrices);
    ENViewControl::DrawAll();
  break;
  case ENDRAWADDLIGHT:
    globj->Set();
    ENMapData::AddLight(ENVector2(x,form->ClientHeight-y),type,Matrices);
    ENViewControl::DrawAll();
  break;
  case ENDRAWADDSOUND:
    globj->Set();
    ENMapData::AddSound(ENVector2(x,form->ClientHeight-y),type,Matrices);
    ENViewControl::DrawAll();
  break;
  case ENDRAWADDOBJECT:
    globj->Set();
    ENMapData::AddObject(ENVector2(x,form->ClientHeight-y),type,Matrices);
    ENViewControl::DrawAll();
  break;
  case ENDRAWADDCUBE:
    globj->Set();
    ENMapData::AddPrefab(ENMapData::ENPREFABCUBE,ENVector2(x,form->ClientHeight-y),type,Matrices);
    ENViewControl::DrawAll();
  break;
  case ENDRAWADDCONE:
    globj->Set();
    ENMapData::AddPrefab(ENMapData::ENPREFABCONE,ENVector2(x,form->ClientHeight-y),type,Matrices);
    ENViewControl::DrawAll();
  break;
  case ENDRAWADDCYLINDER:
    globj->Set();
    ENMapData::AddPrefab(ENMapData::ENPREFABCYLINDER,ENVector2(x,form->ClientHeight-y),type,Matrices);
    ENViewControl::DrawAll();
  break;
  case ENDRAWADDSPHERE:
    globj->Set();
    ENMapData::AddPrefab(ENMapData::ENPREFABSPHERE,ENVector2(x,form->ClientHeight-y),type,Matrices);
    ENViewControl::DrawAll();
  break;
  case ENDRAWADDFACE:
    Vertind=ENMapData::SelectOneVert(ENVector2((ENfloat)x,
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
         ENMapData::AddFace(AddFaceStruct.Ind);
         ENViewControl::DrawAll();
       break;
      }
  break;
  case ENDRAWSELECT:
    if(!eShift)
      ENMapData::SelectNone();

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
 //Update caption
 UpdateMousePos(ENVector2((ENfloat)x,(ENfloat)(form->ClientHeight-y)));
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
  tvec=ENMapData::From2DTo3D(ENVector2(tmp2dvec.v[0],
              form->ClientHeight-tmp2dvec.v[1]),0.8,Matrices);
  ENMapData::Translate(tvec-TempModVec,true);
  TempModVec=tvec;
  ENViewControl::DrawAll();
 }
 //Scale object
 if(ScaleObj)
 {
  tmp2dvec=DirectionRestriction(ENVector2(x,y));
  tvec=ENMapData::From2DTo3D(ENVector2(tmp2dvec.v[0],
                        form->ClientHeight-tmp2dvec.v[1]),0.8,Matrices);
  ENMapData::Scale(CenterVec,tvec-TempModVec,true);
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
  tvec=ENMapData::From2DTo3D(ENVector2(x,form->ClientHeight-y),0.8,Matrices);
  angle=ENLength(mousevec);
  if(angle>0.0f)
  {
   angle=ENLength(tvec)/angle*mousevec.v[0];
   ENMapData::Rotate(CenterVec,angle/100000.0f,Axis,true);
  }
  ENViewControl::DrawAll();                      
 }
 //Move texture
 if(MoveTex)
 {
  ENMapData::TranslateTex(mousevec/500.0f);
  ENViewControl::DrawAll();
 }
 //Scale texture
 if(ScaleTex)
 {
  ENMapData::ScaleTex(mousevec/500.0f);
  ENViewControl::DrawAll();
 }
 //Rotate texture
 if(RotateTex)
 {
  ENMapData::RotateTex(mousevec.v[0]/100.0f);
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
 MoveTex=false;
 RotateTex=false;
 ScaleTex=false;
 if(Selection)
 {
  SelectFrom.v[1]=form->ClientHeight-SelectFrom.v[1];
  SelectTo.v[1]=form->ClientHeight-SelectTo.v[1];
  ENMapData::Select(SelectFrom,SelectTo,type,Matrices);
  Selection=false;
  ENViewControl::DrawAll();
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
    val=ENMapData::GetMaxVal('y');
    if(val>0.0f) glTranslatef(0,-val,0);
  break;
  case ENDRAWFRONT:
    val=ENMapData::GetMaxVal('z');
    if(val>0.0f) glTranslatef(0,0,-val);
  break;
  case ENDRAWSIDE:
    val=ENMapData::GetMinVal('x');
    if(val<0.0f) glTranslatef(-val,0,0);
  break;
 }
}

void ENDrawClass::MoveLightOutOfDepth(ENLight light)
{
 ENfloat val=0.0f;
 switch(type)
 {
  case ENDRAWTOP:
    val=light.pos.v[1]+light.range;
    if(val>0.0f) glTranslatef(0,-val,0);
  break;
  case ENDRAWFRONT:
    val=light.pos.v[2]+light.range;
    if(val>0.0f) glTranslatef(0,0,-val);
  break;
  case ENDRAWSIDE:
    val=light.pos.v[0]-light.range;
    if(val<0.0f) glTranslatef(-val,0,0);
  break;
 }
}

void ENDrawClass::MoveSoundOutOfDepth(ENSoundSrc sound)
{
 ENfloat val=0.0f;
 switch(type)
 {
  case ENDRAWTOP:
    val=sound.pos.v[1]+sound.range;
    if(val>0.0f) glTranslatef(0,-val,0);
  break;
  case ENDRAWFRONT:
    val=sound.pos.v[2]+sound.range;
    if(val>0.0f) glTranslatef(0,0,-val);
  break;
  case ENDRAWSIDE:
    val=sound.pos.v[0]-sound.range;
    if(val<0.0f) glTranslatef(-val,0,0);
  break;
 }
}

void ENDrawClass::MoveCameraOutOfDepth()
{
 ENfloat val=0.0f;
 ENCamera cam=ENMapData::GetCamera();
 switch(type)
 {
  case ENDRAWTOP:
    val=cam.Pos.v[1]+100.0f;
    if(val>0.0f) glTranslatef(0,-val,0);
  break;
  case ENDRAWFRONT:
    val=cam.Pos.v[2]+100.0f;
    if(val>0.0f) glTranslatef(0,0,-val);
  break;
  case ENDRAWSIDE:
    val=cam.Pos.v[0]-100.0f;
    if(val<0.0f) glTranslatef(-val,0,0);
  break;
 }
}

void ENDrawClass::MoveObjectOutOfDepth(ENMapBase::ENMapObject obj)
{
 ENVector bdim;
 ENfloat val=0.0f;
 //Get box
 if(ENMapSources::GetPack(obj.Name)==-1)
   bdim=ENVector(20.0f,20.0f,20.0f);
 else
   bdim=ENMapSources::GetObjDim(obj.Name);
 //Transform bdim
 bdim=bdim*obj.Scale;
 //Move out of depth
 switch(type)
 {
  case ENDRAWTOP:
    val=obj.Pos.v[1]+bdim.v[1];
    if(val>0.0f) glTranslatef(0,-val,0);
  break;
  case ENDRAWFRONT:
    val=obj.Pos.v[2]+bdim.v[2];
    if(val>0.0f) glTranslatef(0,0,-val);
  break;
  case ENDRAWSIDE:
    val=obj.Pos.v[0]-bdim.v[0];
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
