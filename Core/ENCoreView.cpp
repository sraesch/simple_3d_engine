//---------------------------------------------------------------------------
#include "ENCoreView.h"
#include "ENCoreScript.h"
#include "ENCoreMain.h"
#include "ENCoreLoader.h"                   
#include "ENCoreWindow.h"
#include "ENCoreSound.h"
//---------------------------------------------------------------------------
ENCamera *ENCoreView::camera=NULL;
ENdouble  ENCoreView::ModelMat[16];
ENdouble  ENCoreView::ProjMat[16];
ENint     ENCoreView::Viewport[4];

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

void ENCoreView::TransformProj()
{
 //Variables
 ENuint  Width,Height;
 ENdouble aspect;
 //Transform if camera is defined
 if(camera)
 {
  ENCoreWindow::GetWindowSize(Width,Height);
  glViewport(0,0,Width,Height);
  aspect=(ENdouble)Width/Height;
  gluPerspective(camera->Arc,aspect,1.0f,camera->Range);
  glGetIntegerv(GL_VIEWPORT,Viewport);
  glGetDoublev(GL_PROJECTION_MATRIX,ProjMat);
 }
}

void ENCoreView::RotateModel()
{
 if(camera)
 {
  glRotatef(camera->Angle.v[0],1,0,0);
  glRotatef(camera->Angle.v[1]+180,0,1,0);
  glRotatef(camera->Angle.v[2],0,0,1);
 }
}

ENVector ENCoreView::From3DTo2D(ENVector vec)
{
 //Variables
 ENdouble res[3];
 ENVector resvec;
 //Transform vec
 gluProject((ENdouble)vec.v[0],(ENdouble)vec.v[1],(ENdouble)vec.v[2],
            ModelMat,ProjMat,Viewport,&res[0],&res[1],&res[2]);
 //Make vector
 resvec=ENVector((ENfloat)res[0],(ENfloat)Viewport[3]-(ENfloat)res[1],
                 ENDistance(camera->Pos,vec));
 //Finished
 return resvec;
}

ENVector ENCoreView::From3DTo2D2(ENVector vec)
{
 //Variables
 ENdouble res[3];
 ENVector resvec;
 //Transform vec
 gluProject((ENdouble)vec.v[0],(ENdouble)vec.v[1],(ENdouble)vec.v[2],
            ModelMat,ProjMat,Viewport,&res[0],&res[1],&res[2]);
 //Make vector
 resvec=ENVector((ENfloat)res[0],(ENfloat)Viewport[3]-(ENfloat)res[1],
                 (ENfloat)res[2]);
 //Finished
 return resvec;
}

ENVector ENCoreView::From3DTo2D3(ENVector vec)
{
 //Variables
 ENdouble res[3];
 ENVector resvec;
 ENdouble Model[16];
 //Get model matrix
 glGetDoublev(GL_MODELVIEW_MATRIX,Model);
 //Transform vec
 gluProject((ENdouble)vec.v[0],(ENdouble)vec.v[1],(ENdouble)vec.v[2],
            Model,ProjMat,Viewport,&res[0],&res[1],&res[2]);
 //Make vector
 resvec=ENVector((ENfloat)res[0],(ENfloat)res[1],(ENfloat)res[2]);
 //Finished
 return resvec;
}

ENVector ENCoreView::From2DTo3D(ENVector vec)
{
 //Variables
 ENdouble res[3];
 ENVector vdir;
 //Transform vec
 gluUnProject((ENdouble)vec.v[0],(ENdouble)Viewport[3]-(ENdouble)vec.v[1],0.8,
            ModelMat,ProjMat,Viewport,&res[0],&res[1],&res[2]);
 //Make vector
 vdir=ENVector((ENfloat)res[0],(ENfloat)res[1],(ENfloat)res[2])-camera->Pos;
 //Finished
 return ENNormalize(vdir)*vec.v[2]+camera->Pos;
}

void ENCoreView::SetCamera(ENCamera cam)
{
 *camera=cam;
}

void ENCoreView::UpdateListener()
{
 ENVector vdir=ENVecDir(1.0f,ENVector2(camera->Angle.v[0],camera->Angle.v[1]));
 ENCoreSound::SetListener(camera->Pos,vdir);
}

ENVector ENCoreView::GetCamPos()
{
 return camera->Pos;
}

void ENCoreView::TransformModel()
{
 if(camera)
 {
  RotateModel();
  glTranslatef(-camera->Pos.v[0],-camera->Pos.v[1],-camera->Pos.v[2]);
  glGetDoublev(GL_MODELVIEW_MATRIX,ModelMat);
 }
}
