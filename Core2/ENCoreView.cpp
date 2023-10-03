//---------------------------------------------------------------------------
#include "ENCoreView.h"
#include "ENCoreScript.h"
#include "ENCoreMain.h"
#include "ENCoreLoader.h"
//---------------------------------------------------------------------------
ENCamera *ENCoreView::camera=NULL;

ENbool ENCoreView::Init()
{
 //Loader message
 ENCoreLoader::AddMsg("Initialize engine core view...");
 //Register script camera
 camera=(ENCamera*)ENCoreScript::GetScriptVar("view");
 if(!camera)
 {
  ENCoreMain::ShowError("Cannot init script variable view!!!");
  return false;
 }
 //Init camera values
 camera->Pos=ENVector(0,0,0);
 camera->Angle=ENVector(0,0,0);
 camera->Arc=60.0;
 camera->Range=10000.0;
 //Finished
 return true;
}

void ENCoreView::TransformProj(ENfloat aspect)
{
 if(camera)
   gluPerspective(camera->Arc,aspect,1.0f,camera->Range);
}

void ENCoreView::RotateModel()
{
 if(camera)
 {
  glRotatef(camera->Angle.x,1,0,0);
  glRotatef(camera->Angle.y+180,0,1,0);
  glRotatef(camera->Angle.z,0,0,1);
 }
}

void ENCoreView::TransformModel()
{
 if(camera)
 {
  RotateModel();
  glTranslatef(-camera->Pos.x,-camera->Pos.y,-camera->Pos.z);
 }
}
