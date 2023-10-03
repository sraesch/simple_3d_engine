//---------------------------------------------------------------------------
#include "ENCoreDepthBuffer.h"
#include "ENCoreMain.h"
#include "ENCoreGL.h"
//---------------------------------------------------------------------------
const ENuint ENCoreDepthBuffer::Size=128;
ENuint       ENCoreDepthBuffer::TexID=0;
HDC          ENCoreDepthBuffer::dc=NULL;
HGLRC        ENCoreDepthBuffer::rc=NULL;
HWND         ENCoreDepthBuffer::hwnd=NULL;
HPBUFFERARB  ENCoreDepthBuffer::buffer=NULL;

ENbool ENCoreDepthBuffer::Init(HDC vdc,HGLRC vrc,HWND hwnd)
{
 //Generate and set texture ID
 glGenTextures(1,&TexID);
 glBindTexture(GL_TEXTURE_2D,TexID);
 //Generate depth texture
 glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32,Size,Size,0,GL_DEPTH_COMPONENT,GL_UNSIGNED_BYTE,NULL);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
 glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
 //Init
 if(!InitBuffer(vdc,vrc,hwnd))
 {
  ENCoreMain::ShowError("Cannot initialize second depth buffer!!!");
  return false;
 }
 else
   return true;
}

void ENCoreDepthBuffer::Free()
{
 //Delete depth texture
 glDeleteTextures(1,&TexID);
 //Delete rc
 if(rc)
 {
  wglMakeCurrent(dc,rc);
  wglDeleteContext(rc);
  ENGLExtension::ReleasePbufferDC(buffer,dc);
  ENGLExtension::DestroyPbuffer(buffer);
 }
 //Delete dc
 if(dc)
   ReleaseDC(hwnd,dc);
}


void ENCoreDepthBuffer::SetShadow()
{
 glBindTexture(GL_TEXTURE_2D,TexID);
}

ENbool ENCoreDepthBuffer::InitBuffer(HDC vdc,HGLRC vrc,HWND hwnd)
{
 //Variables
 ENint  pixelformat,w,h;
 ENuint count;
 //Set hwnd
 ENCoreDepthBuffer::hwnd=hwnd;
 //Attribute list
 ENint attrib[] =
        	{
	        WGL_SUPPORT_OPENGL_ARB, TRUE,       // P-buffer will be used with OpenGL
		WGL_DRAW_TO_PBUFFER_ARB, TRUE,      // Enable render to p-buffer
		WGL_DEPTH_BITS_ARB, 16,             // At least 16 bits for depth buffer
		WGL_DOUBLE_BUFFER_ARB, FALSE,       // We don't require double buffering
		0                                   // Zero terminates the list
	};
 //Choose pixel format
 ENGLExtension::ChoosePixelFormat(vdc,(const ENint*)attrib,NULL,1,&pixelformat,&count);
 if(!count) return false;
 //Create context
 buffer=ENGLExtension::CreateGLBuffer(vdc,pixelformat,Size,Size,NULL);
 dc=ENGLExtension::GetPbufferDC(buffer);
 rc=wglCreateContext(dc);
 if(!buffer) return false;
 //Check
 ENGLExtension::QueryPbuffer(buffer,WGL_PBUFFER_WIDTH_ARB,&w);
 ENGLExtension::QueryPbuffer(buffer,WGL_PBUFFER_HEIGHT_ARB,&h);
 if((ENuint)w!=Size||(ENuint)h!=Size) return false;
 //Share list
 if(!wglShareLists(vrc,rc)) return false;
 //Config
 Config();
 //Finished
 return true;
}

void ENCoreDepthBuffer::Begin()
{
 //Set new context
 if(!wglMakeCurrent(dc,rc))
 {
  ENCoreMain::ShowError("Cannot activate second depth buffer!!!");
  ENCoreMain::StopApplication();
 }  
 //Clear
 glClear( GL_DEPTH_BUFFER_BIT );
}

void ENCoreDepthBuffer::End()
{
 glBindTexture(GL_TEXTURE_2D,TexID);
 glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,Size,Size);
 ENCoreGL::Activate();
}

void ENCoreDepthBuffer::Config()
{
 //Set context
 wglMakeCurrent(dc,rc);
 //Config
 glEnable(GL_DEPTH_TEST);
 glClearDepth(1.0);
 glColorMask(0,0,0,0);
 glViewport(0,0,Size,Size);
 glEnable(GL_POLYGON_OFFSET_FILL);
 glPolygonOffset(1.1f,4.0f);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glMatrixMode(GL_MODELVIEW);
}


