//---------------------------------------------------------------------------
#include "ENCoreGL.h"
#include "ENCoreMain.h"
#include "ENCoreView.h"
#include "ENCoreLoader.h"
#include "ENCoreDepthBuffer.h"
//---------------------------------------------------------------------------
ENGLClass *ENCoreGL::gldevice=NULL;

ENbool ENCoreGL::Init(HWND WndHandle)
{
 //Loader message
 ENCoreLoader::AddMsg("Initialize engine core OpenGL...");
 //Create device
 gldevice=new ENGLClass(WndHandle,true,false);
 if(!gldevice->Init())
 {
  ENCoreMain::ShowError("Cannot initialize OpenGL device!!!");
  return false;
 }
 else
 {
  ConfigGL();
  //Init OpenGL extensions
  if(!ENGLExtension::Load())
  {
   ENCoreMain::ShowError("Cannot load OpenGL Extensions!!!");
   return false;
  }
  //Init second depth buffer
  if(!ENCoreDepthBuffer::Init(gldevice->GetObjDC(),gldevice->GetObjRC(),WndHandle))
    return false;
  else
    return true;
 }
}

void ENCoreGL::Free()
{
 ENCoreDepthBuffer::Free();
 delete gldevice;
}

void ENCoreGL::BeginDraw()
{
 gldevice->Start();
 glPushMatrix();
 TransformMatrices();
}

void ENCoreGL::EndDraw()
{
 glPopMatrix();
 gldevice->End();
}

void ENCoreGL::Activate()
{
 gldevice->Set();
}

void ENCoreGL::ConfigGL()
{
 glEnable(GL_AUTO_NORMAL);
 glEnable(GL_DEPTH_TEST); 
 glDisable(GL_BLEND);
 glDisable(GL_ALPHA_TEST);
 glEnable(GL_TEXTURE_2D);
 glEnable(GL_CULL_FACE);
 glCullFace(GL_BACK);
 glClearDepth(1.0);
 glClearColor(0,0,0,1);
}

void ENCoreGL::TransformMatrices()
{
 //Transform projection matrix
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 ENCoreView::TransformProj();
 //Initialize modelview matrix
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 ENCoreView::TransformModel();
}
