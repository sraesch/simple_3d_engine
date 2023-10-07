#include "EngineSystem.h"
/////////////////////////////////////////////////
///ENGLClass
/////////////////////////////////////////////////
ENuint ENGLClass::NumGLObjs=0;
HGLRC  ENGLClass::GlobalRC=NULL;

ENGLClass::ENGLClass(void *dst,ENbool OwnDC,ENbool UseGlobalRC)
{
 this->UseGlobalRC=UseGlobalRC;
 this->OwnDC=OwnDC;
 if(UseGlobalRC) NumGLObjs++;
 if(OwnDC)
   dc=GetDC(dst);
 else
   dc=dst;
   
 rc=NULL;
}

ENGLClass::~ENGLClass()
{
 wglMakeCurrent(dc,NULL);
 if(!UseGlobalRC)
   wglDeleteContext(rc);
 else
 {
  NumGLObjs--;
  if(!NumGLObjs)
  {
   wglDeleteContext(GlobalRC);
   GlobalRC=NULL;
  }
 }  
}

bool ENGLClass::Init()
{
 //Setup pixel format
 if(!SetupPixelFormat()) return false;
 if(!UseGlobalRC)
 {
  //Create OpenGL Context
  rc = wglCreateContext(dc);
  if(rc==NULL) return false;
  //Make current
  if(!wglMakeCurrent(dc,rc)) return false;
 }
 else
   if(!GlobalRC)
   {
    GlobalRC = wglCreateContext(dc);
    if(GlobalRC==NULL) return false;
    //Make current
    if(!wglMakeCurrent(dc,GlobalRC)) return false;
   }

 return true;
}

void ENGLClass::Start()
{
 this->Set();
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ENGLClass::End()
{
 glFinish();
 SwapBuffers(dc);
}

void ENGLClass::ConfigMatrix(FLOAT minx,FLOAT miny,FLOAT maxx,FLOAT maxy)
{
 this->Set();
 //Config
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(minx,maxx,miny,maxy,-1,10000);
 glViewport(minx,miny,maxx-minx,maxy-miny);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}

void ENGLClass::Set()
{
 if(!UseGlobalRC)
   wglMakeCurrent(dc,rc);
 else
   wglMakeCurrent(dc,GlobalRC);
}

HDC   ENGLClass::GetObjDC()
{
 return dc;
}

HGLRC ENGLClass::GetObjRC()
{
 if(UseGlobalRC)
   return GlobalRC;
 else
   return rc;
}

bool ENGLClass::SetupPixelFormat()
{
   int nPixelFormat;

   PIXELFORMATDESCRIPTOR pfd = {
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW |
	PFD_SUPPORT_OPENGL |
	PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	24,
	0, 0, 0, 0, 0, 0,
	0,            
	0,
        0,
	0, 0, 0, 0,   
	32,
	0,                    
	0,                    
	PFD_MAIN_PLANE,       
	0,
	0, 0, 0               
};
   nPixelFormat = ChoosePixelFormat(dc, &pfd);
   if(!nPixelFormat)
     return false;

   if(!SetPixelFormat(dc, nPixelFormat, &pfd))
     return false;

 return true;
}

void ENGLClass::FormatVector(ENVector &vec)
{
 vec=ENNormalize(vec)*0.5f+ENVector(0.5f,0.5f,0.5f);
}

void ENGLClass::GenNormalizationCubeMap()
{
 //VARS
 ENubyte *data;
 ENuint   bytePtr,j,i,size=128;
 ENfloat  offset,halfSize;
 ENVector temp;
 //Init buffer
 data=(ENubyte*)ENAllocMemory(size*size*3);
 //Set values
 offset=0.5f;
 halfSize=size*0.5f; 
 //Positive X
 bytePtr=0;
 for(j=0; j<size; j++)
   for(i=0; i<size; i++)
   {
    temp.v[0] = halfSize;
    temp.v[1] = (j+offset-halfSize);
    temp.v[2] = -(i+offset-halfSize);

    FormatVector(temp);

    data[bytePtr] = (ENubyte)(temp.v[0] * 255.0f);
    data[bytePtr+1] = (ENubyte)(temp.v[1] * 255.0f);
    data[bytePtr+2] = (ENubyte)(temp.v[2] * 255.0f);

    bytePtr+=3;
   }
 glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			 0,GL_RGB8,size,size,0,GL_RGB,GL_UNSIGNED_BYTE,data);
 //Negative X
 bytePtr=0;
 for(j=0; j<size; j++)
   for(i=0; i<size; i++)
   {
    temp.v[0] = -halfSize;
    temp.v[1] = (j+offset-halfSize);
    temp.v[2] = (i+offset-halfSize);

    FormatVector(temp);

    data[bytePtr] = (ENubyte)(temp.v[0] * 255.0f);
    data[bytePtr+1] = (ENubyte)(temp.v[1] * 255.0f);
    data[bytePtr+2] = (ENubyte)(temp.v[2] * 255.0f);

    bytePtr+=3;
   }
 glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
			0, GL_RGB8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
 //Positive Y
 bytePtr=0;
 for(j=0; j<size; j++)
   for(i=0; i<size; i++)
   {
    temp.v[0] = i+offset-halfSize;
    temp.v[1] = -halfSize;
    temp.v[2] = j+offset-halfSize;

    FormatVector(temp);

    data[bytePtr] = (ENubyte)(temp.v[0] * 255.0f);
    data[bytePtr+1] = (ENubyte)(temp.v[1] * 255.0f);
    data[bytePtr+2] = (ENubyte)(temp.v[2] * 255.0f);

    bytePtr+=3;
   }
 glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
			0, GL_RGB8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
 //Negative Y
 bytePtr=0;
 for(j=0; j<size; j++)
   for(i=0; i<size; i++)
   {
    temp.v[0] = i+offset-halfSize;
    temp.v[1] = halfSize;
    temp.v[2] = -(j+offset-halfSize);

    FormatVector(temp);

    data[bytePtr] = (ENubyte)(temp.v[0] * 255.0f);
    data[bytePtr+1] = (ENubyte)(temp.v[1] * 255.0f);
    data[bytePtr+2] = (ENubyte)(temp.v[2] * 255.0f);

    bytePtr+=3;
   }
 glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			0, GL_RGB8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

 //Positive Z
 bytePtr=0;
 for(j=0; j<size; j++)
   for(i=0; i<size; i++)
   {
    temp.v[0] = i+offset-halfSize;
    temp.v[1] = (j+offset-halfSize);
    temp.v[2] = halfSize;

    FormatVector(temp);

    data[bytePtr] = (ENubyte)(temp.v[0] * 255.0f);
    data[bytePtr+1] = (ENubyte)(temp.v[1] * 255.0f);
    data[bytePtr+2] = (ENubyte)(temp.v[2] * 255.0f);

    bytePtr+=3;
   }
 glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
 			0, GL_RGB8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

 //Negative Z
 bytePtr=0;
 for(j=0; j<size; j++)
   for(i=0; i<size; i++)
   {
    temp.v[0] = -(i+offset-halfSize);
    temp.v[1] = (j+offset-halfSize);
    temp.v[2] = -halfSize;

    FormatVector(temp);

    data[bytePtr] = (ENubyte)(temp.v[0] * 255.0f);
    data[bytePtr+1] = (ENubyte)(temp.v[1] * 255.0f);
    data[bytePtr+2] = (ENubyte)(temp.v[2] * 255.0f);

    bytePtr+=3;
   }
 glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
			0, GL_RGB8, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
 //Free buffer
 ENFreeMemory(data);
 //Set texture parameter's
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
/////////////////////////////////////////////////
///ENGLExtension
/////////////////////////////////////////////////
ENbool ENGLExtension::Loaded=false;
//WGL render buffer
PFNWGLCREATEPBUFFERARBPROC              ENGLExtension::wglCreatePbufferARB=NULL;
PFNWGLGETPBUFFERDCARBPROC               ENGLExtension::wglGetPbufferDCARB=NULL;
PFNWGLRELEASEPBUFFERDCARBPROC           ENGLExtension::wglReleasePbufferDCARB=NULL;
PFNWGLDESTROYPBUFFERARBPROC             ENGLExtension::wglDestroyPbufferARB=NULL;
PFNWGLQUERYPBUFFERARBPROC               ENGLExtension::wglQueryPbufferARB=NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC          ENGLExtension::wglChoosePixelFormatARB=NULL;
//WGL render to texture
PFNWGLBINDTEXIMAGEARBPROC               ENGLExtension::wglBindTexImageARB=NULL;
PFNWGLRELEASETEXIMAGEARBPROC            ENGLExtension::wglReleaseTexImageARB=NULL;
PFNWGLSETPBUFFERATTRIBARBPROC           ENGLExtension::wglSetPbufferAttribARB=NULL;
//Multitexturing extensions
PFNGLACTIVETEXTUREARBPROC               ENGLExtension::glActiveTextureARB=NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC         ENGLExtension::glClientActiveTextureARB=NULL;
PFNGLMULTITEXCOORD2FARBPROC	        ENGLExtension::glMultiTexCoord2fARB=NULL;
PFNGLMULTITEXCOORD2FVARBPROC	        ENGLExtension::glMultiTexCoord2fvARB=NULL;
PFNGLMULTITEXCOORD3FARBPROC	        ENGLExtension::glMultiTexCoord3fARB=NULL;
PFNGLMULTITEXCOORD3FVARBPROC	        ENGLExtension::glMultiTexCoord3fvARB=NULL;
//Occlusion queries
PFNGLGENQUERIESARBPROC                  ENGLExtension::glGenQueriesARB=NULL;
PFNGLDELETEQUERIESARBPROC               ENGLExtension::glDeleteQueriesARB=NULL;
PFNGLBEGINQUERYARBPROC                  ENGLExtension::glBeginQueryARB=NULL;
PFNGLENDQUERYARBPROC                    ENGLExtension::glEndQueryARB=NULL;
PFNGLGETQUERYOBJECTUIVARBPROC           ENGLExtension::glGetQueryObjectuivARB=NULL;
//OpenGL Shader language extensions
PFNGLCREATEPROGRAMOBJECTARBPROC         ENGLExtension::glCreateProgramObjectARB=NULL;
PFNGLDELETEOBJECTARBPROC                ENGLExtension::glDeleteObjectARB=NULL;
PFNGLCREATESHADEROBJECTARBPROC          ENGLExtension::glCreateShaderObjectARB=NULL;
PFNGLSHADERSOURCEARBPROC                ENGLExtension::glShaderSourceARB=NULL;
PFNGLCOMPILESHADERARBPROC               ENGLExtension::glCompileShaderARB=NULL;
PFNGLATTACHOBJECTARBPROC                ENGLExtension::glAttachObjectARB=NULL;
PFNGLLINKPROGRAMARBPROC                 ENGLExtension::glLinkProgramARB=NULL;
PFNGLVALIDATEPROGRAMARBPROC             ENGLExtension::glValidateProgramARB=NULL;
PFNGLUSEPROGRAMOBJECTARBPROC            ENGLExtension::glUseProgramObjectARB=NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC        ENGLExtension::glGetObjectParameterivARB=NULL;
PFNGLGETINFOLOGARBPROC                  ENGLExtension::glGetInfoLogARB=NULL;

PFNGLGETUNIFORMLOCATIONARBPROC          ENGLExtension::glGetUniformLocationARB=NULL;
PFNGLUNIFORM1IARBPROC                   ENGLExtension::glUniform1iARB=NULL;
PFNGLUNIFORM2IARBPROC                   ENGLExtension::glUniform2iARB=NULL;
PFNGLUNIFORM3IARBPROC                   ENGLExtension::glUniform3iARB=NULL;
PFNGLUNIFORM4IARBPROC                   ENGLExtension::glUniform4iARB=NULL;
PFNGLUNIFORM1FARBPROC                   ENGLExtension::glUniform1fARB=NULL;
PFNGLUNIFORM2FARBPROC                   ENGLExtension::glUniform2fARB=NULL;
PFNGLUNIFORM3FARBPROC                   ENGLExtension::glUniform3fARB=NULL;
PFNGLUNIFORM4FARBPROC                   ENGLExtension::glUniform4fARB=NULL;
PFNGLUNIFORM1IVARBPROC                  ENGLExtension::glUniform1ivARB=NULL;
PFNGLUNIFORM2IVARBPROC                  ENGLExtension::glUniform2ivARB=NULL;
PFNGLUNIFORM3IVARBPROC                  ENGLExtension::glUniform3ivARB=NULL;
PFNGLUNIFORM4IVARBPROC                  ENGLExtension::glUniform4ivARB=NULL;
PFNGLUNIFORM1FVARBPROC                  ENGLExtension::glUniform1fvARB=NULL;
PFNGLUNIFORM2FVARBPROC                  ENGLExtension::glUniform2fvARB=NULL;
PFNGLUNIFORM3FVARBPROC                  ENGLExtension::glUniform3fvARB=NULL;
PFNGLUNIFORM4FVARBPROC                  ENGLExtension::glUniform4fvARB=NULL;
PFNGLUNIFORMMATRIX2FVARBPROC            ENGLExtension::glUniformMatrix2fvARB=NULL;
PFNGLUNIFORMMATRIX3FVARBPROC            ENGLExtension::glUniformMatrix3fvARB=NULL;
PFNGLUNIFORMMATRIX4FVARBPROC            ENGLExtension::glUniformMatrix4fvARB=NULL;

PFNGLGETATTRIBLOCATIONARBPROC           ENGLExtension::glGetAttribLocationARB=NULL;
PFNGLVERTEXATTRIB2FARBPROC              ENGLExtension::glVertexAttrib2fARB=NULL;
PFNGLVERTEXATTRIB2FVARBPROC             ENGLExtension::glVertexAttrib2fvARB=NULL;
PFNGLVERTEXATTRIB3FARBPROC              ENGLExtension::glVertexAttrib3fARB=NULL;
PFNGLVERTEXATTRIB3FVARBPROC             ENGLExtension::glVertexAttrib3fvARB=NULL;
PFNGLENABLEVERTEXATTRIBARRAYARBPROC     ENGLExtension::glEnableVertexAttribArrayARB=NULL;
PFNGLDISABLEVERTEXATTRIBARRAYARBPROC    ENGLExtension::glDisableVertexAttribArrayARB=NULL;
PFNGLVERTEXATTRIBPOINTERARBPROC         ENGLExtension::glVertexAttribPointerARB=NULL;

ENbool ENGLExtension::Load()
{
 //Load wgl extensions
 if(!InitWGLExtensions()) return false;
 //Load multitexturing extensions
 if(!InitMultiTexturingExt()) return false;
 //Load occlusion queries
 if(!InitOcclusionExt()) return false;
 //Load shader extensions
 if(!InitShaderExt()) return false;
 //Finished
 Loaded=true;
 return true;
}

ENbool ENGLExtension::isLoaded()
{
 return Loaded;
}

HPBUFFERARB ENGLExtension::CreateGLBuffer(HDC dc,ENint pixelformat,ENint width,
                                     ENint height,const ENint Attrib[])
{
 return wglCreatePbufferARB(dc,pixelformat,width,height,Attrib);
}

ENbool ENGLExtension::QueryPbuffer(HPBUFFERARB hPbuffer,ENint iAttribute,
                                   ENint *piValue)
{
 return wglQueryPbufferARB(hPbuffer,iAttribute,piValue);
}

HDC    ENGLExtension::GetPbufferDC(HPBUFFERARB hPbuffer)
{
 return wglGetPbufferDCARB(hPbuffer);
}

ENint  ENGLExtension::ReleasePbufferDC(HPBUFFERARB hPbuffer, HDC hDC)
{
 return wglReleasePbufferDCARB(hPbuffer,hDC);
}

ENbool ENGLExtension::DestroyPbuffer(HPBUFFERARB hPbuffer)
{
 return wglDestroyPbufferARB(hPbuffer);
}

ENbool ENGLExtension::ChoosePixelFormat(HDC hdc,const ENint *piAttribIList,
                                        const ENfloat *pfAttribFList,ENuint nMaxFormats,
                                        ENint *piFormats,ENuint *nNumFormats)
{
 return wglChoosePixelFormatARB(hdc,piAttribIList,pfAttribFList, nMaxFormats,piFormats,nNumFormats);
}

ENbool ENGLExtension::BindTexImage(HPBUFFERARB hPbuffer,ENint iBuffer)
{
 return wglBindTexImageARB(hPbuffer,iBuffer);
}

ENbool ENGLExtension::ReleaseTexImage(HPBUFFERARB hPbuffer,ENint iBuffer)
{
 return wglReleaseTexImageARB(hPbuffer,iBuffer);
}

ENbool ENGLExtension::SetPbufferAttrib(HPBUFFERARB hPbuffer,const ENint *piAttribList)
{
 return wglSetPbufferAttribARB(hPbuffer,piAttribList);
}

void ENGLExtension::ActiveTexture(GLenum texture)
{
 glActiveTextureARB(texture);
}

void ENGLExtension::ActiveClientTexture(GLenum texture)
{
 glClientActiveTextureARB(texture);
}

void ENGLExtension::TexCoord(GLenum target,ENfloat s,ENfloat t)
{
 glMultiTexCoord2fARB(target,s,t);
}

void ENGLExtension::TexCoord(GLenum target,ENVector2 coord)
{
 glMultiTexCoord2fARB(target,coord.v[0],coord.v[1]);
}

void ENGLExtension::TexCoord(GLenum target,ENVector2 *coord)
{
 glMultiTexCoord2fvARB(target,(ENfloat*)coord);
}

void ENGLExtension::TexCoord(GLenum target,ENfloat s,ENfloat t,ENfloat u)
{
 glMultiTexCoord3fARB(target,s,t,u);
}

void ENGLExtension::TexCoord(GLenum target,ENVector coord)
{
 glMultiTexCoord3fARB(target,coord.v[0],coord.v[1],coord.v[2]);
}

void ENGLExtension::TexCoord(GLenum target,ENVector *coord)
{
 glMultiTexCoord3fvARB(target,(ENfloat*)coord);
}

void ENGLExtension::GenerateQueries(ENuint num,ENuint *ids)
{
 glGenQueriesARB(num,ids);
}

void ENGLExtension::DeleteQueries(ENuint num,ENuint *ids)
{
 glDeleteQueriesARB(num,ids);
}

void ENGLExtension::BeginQuery(GLenum target,ENuint id)
{
 glBeginQueryARB(target,id);
}

void ENGLExtension::EndQuery(GLenum target)
{
 glEndQueryARB(target);
}

void ENGLExtension::GetQueryObjectuiv(ENuint id,GLenum pname,GLuint *params)
{
 glGetQueryObjectuivARB(id,pname,params);
}

ENhandle ENGLExtension::CreateProgram()
{
 return glCreateProgramObjectARB();
}

void ENGLExtension::DeleteObject(ENhandle object)
{
 glDeleteObjectARB(object);
}

ENhandle ENGLExtension::CreateShader(GLenum shaderType)
{
 return glCreateShaderObjectARB(shaderType);
}

void ENGLExtension::SetShaderSource(ENhandle shader,const char *source)
{
 ENint len=(ENint)strlen(source);
 glShaderSourceARB(shader,1,&source,&len);
}

void ENGLExtension::CompileShader(ENhandle shader)
{
 glCompileShaderARB(shader);
}

void ENGLExtension::AttachObject(ENhandle program,ENhandle shader)
{
 glAttachObjectARB(program,shader);
}

void ENGLExtension::LinkProgram(ENhandle program)
{
 glLinkProgramARB(program);
}

void ENGLExtension::ValidateProgram(ENhandle program)
{
 glValidateProgramARB(program);
}

ENint ENGLExtension::GetUniformHandle(ENhandle program,const char *name)
{
 return glGetUniformLocationARB(program,name);
}

void ENGLExtension::UseProgram(ENhandle program)
{
 glUseProgramObjectARB(program);
}

void ENGLExtension::GetObjectParameter(ENhandle object,GLenum pname,ENint *params)
{
 glGetObjectParameterivARB(object,pname,params);
}

void ENGLExtension::GetInfoLog(ENhandle object,ENuint maxLength,ENint &len,char *infoLog)
{
 glGetInfoLogARB(object,maxLength,&len,infoLog);
}

void ENGLExtension::Uniform(ENint location,ENint v0)
{
 glUniform1iARB(location,v0);
}

void ENGLExtension::Uniform(ENint location,ENint v0,ENint v1)
{
 glUniform2iARB(location,v0,v1);
}

void ENGLExtension::Uniform(ENint location,ENint v0,ENint v1,ENint v2)
{
 glUniform3iARB(location,v0,v1,v2);
}

void ENGLExtension::Uniform(ENint location,ENint v0,ENint v1,ENint v2,ENint v3)
{
 glUniform4iARB(location,v0,v1,v2,v3);
}

void ENGLExtension::Uniform(ENint location,ENfloat v0)
{
 glUniform1fARB(location,v0);
}

void ENGLExtension::Uniform(ENint location,ENfloat v0,ENfloat v1)
{
 glUniform2fARB(location,v0,v1);
}

void ENGLExtension::Uniform(ENint location,ENfloat v0,ENfloat v1,ENfloat v2)
{
 glUniform3fARB(location,v0,v1,v2);
}

void ENGLExtension::Uniform(ENint location,ENfloat v0,ENfloat v1,ENfloat v2,ENfloat v3)
{
 glUniform4fARB(location,v0,v1,v2,v3);
}

void ENGLExtension::Uniform(ENint location,POINTERTYPE type,ENuint num,void *data)
{
 switch(type)
 {
  case EN_INT_1:
    glUniform1ivARB(location,num,(ENint*)data);
  break;
  case EN_INT_2:
    glUniform2ivARB(location,num,(ENint*)data);
  break;
  case EN_INT_3:
    glUniform3ivARB(location,num,(ENint*)data);
  break;
  case EN_INT_4:
    glUniform4ivARB(location,num,(ENint*)data);
  break;       
  case EN_FLOAT_1:
    glUniform1fvARB(location,num,(ENfloat*)data);
  break;
  case EN_FLOAT_2:
    glUniform2fvARB(location,num,(ENfloat*)data);
  break;
  case EN_FLOAT_3:
    glUniform3fvARB(location,num,(ENfloat*)data);
  break;
  case EN_FLOAT_4:
    glUniform4fvARB(location,num,(ENfloat*)data);
  break;
 }
}

void ENGLExtension::Uniform(ENint location,ENbool transpose,ENMatrix2X2 matrix)
{
 glUniformMatrix2fvARB(location,1,transpose,&matrix.val[0][0]);
}

void ENGLExtension::Uniform(ENint location,ENbool transpose,ENMatrix2X2 *matrix)
{
 glUniformMatrix2fvARB(location,1,transpose,&matrix->val[0][0]);
}

void ENGLExtension::Uniform(ENint location,ENbool transpose,ENMatrix3X3 matrix)
{
 glUniformMatrix3fvARB(location,1,transpose,&matrix.val[0][0]);
}

void ENGLExtension::Uniform(ENint location,ENbool transpose,ENMatrix3X3 *matrix)
{
 glUniformMatrix3fvARB(location,1,transpose,&matrix->val[0][0]);
}

void ENGLExtension::Uniform(ENint location,ENbool transpose,ENMatrix4X4 matrix)
{
 glUniformMatrix4fvARB(location,1,transpose,&matrix.val[0][0]);
}

void ENGLExtension::Uniform(ENint location,ENbool transpose,ENMatrix4X4 *matrix)
{
 glUniformMatrix4fvARB(location,1,transpose,&matrix->val[0][0]);
}

ENint ENGLExtension::GetAttribHandle(ENhandle program,const char *name)
{
 return glGetAttribLocationARB(program,name);
}

void ENGLExtension::Attrib(ENint location,ENVector2 vec)
{
 glVertexAttrib2fARB(location,vec.v[0],vec.v[1]);
}

void ENGLExtension::Attrib(ENint location,ENVector2 *vec)
{
 glVertexAttrib2fvARB(location,(ENfloat*)vec);
}

void ENGLExtension::Attrib(ENint location,ENVector vec)
{
 glVertexAttrib3fARB(location,vec.v[0],vec.v[1],vec.v[2]);
}

void ENGLExtension::Attrib(ENint location,ENVector *vec)
{
 glVertexAttrib3fvARB(location,(ENfloat*)vec);
}

void ENGLExtension::EnableAttribArray(ENuint location)
{
 glEnableVertexAttribArrayARB(location);
}

void ENGLExtension::DisableAttribArray(ENuint location)
{
 glDisableVertexAttribArrayARB(location);
}

void ENGLExtension::VertexAttribPointer(ENuint index,ENint size,GLenum type,
                                        ENbool normalized,ENuint stride,const void *pointer)
{
 glVertexAttribPointerARB(index,size,type,normalized,stride,pointer);
}

ENbool ENGLExtension::InitMultiTexturingExt()
{
 //glActiveTextureARB
 glActiveTextureARB=(PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
 if(!glActiveTextureARB) return false;
 //glClientActiveTextureARB
 glClientActiveTextureARB=(PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTextureARB");
 if(!glClientActiveTextureARB) return false;
 //glMultiTexCoord2fARB
 glMultiTexCoord2fARB=(PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
 if(!glMultiTexCoord2fARB) return false;
 //glMultiTexCoord2fvARB
 glMultiTexCoord2fvARB=(PFNGLMULTITEXCOORD2FVARBPROC)wglGetProcAddress("glMultiTexCoord2fvARB");
 if(!glMultiTexCoord2fvARB) return false;
 //glMultiTexCoord3fARB
 glMultiTexCoord3fARB=(PFNGLMULTITEXCOORD3FARBPROC)wglGetProcAddress("glMultiTexCoord3fARB");
 if(!glMultiTexCoord3fARB) return false;
 //glMultiTexCoord3fvARB
 glMultiTexCoord3fvARB=(PFNGLMULTITEXCOORD3FVARBPROC)wglGetProcAddress("glMultiTexCoord3fvARB");
 if(!glMultiTexCoord3fvARB) return false;
 //Finished
 return true;
}

ENbool ENGLExtension::InitOcclusionExt()
{
 //glGenQueriesARB
 glGenQueriesARB=(PFNGLGENQUERIESARBPROC)wglGetProcAddress("glGenQueriesARB");
 if(!glGenQueriesARB) return false;
 //glDeleteQueriesARB
 glDeleteQueriesARB=(PFNGLDELETEQUERIESARBPROC)wglGetProcAddress("glDeleteQueriesARB");
 if(!glDeleteQueriesARB) return false;
 //glBeginQueryARB
 glBeginQueryARB=(PFNGLBEGINQUERYARBPROC)wglGetProcAddress("glBeginQueryARB");
 if(!glBeginQueryARB) return false;
 //glEndQueryARB
 glEndQueryARB=(PFNGLENDQUERYARBPROC)wglGetProcAddress("glEndQueryARB");
 if(!glEndQueryARB) return false;
 //glGetQueryObjectuivARB
 glGetQueryObjectuivARB=(PFNGLGETQUERYOBJECTUIVARBPROC)wglGetProcAddress("glGetQueryObjectuivARB");
 if(!glGetQueryObjectuivARB) return false;
 //Finished
 return true;
}

ENbool ENGLExtension::InitShaderExt()
{
 //glCreateProgramObjectARB
 glCreateProgramObjectARB=(PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
 if(!glCreateProgramObjectARB) return false;
 //glDeleteObjectARB
 glDeleteObjectARB=(PFNGLDELETEOBJECTARBPROC)wglGetProcAddress("glDeleteObjectARB");
 if(!glDeleteObjectARB) return false;
 //glCreateShaderObjectARB
 glCreateShaderObjectARB=(PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
 if(!glCreateShaderObjectARB) return false;
 //glShaderSourceARB
 glShaderSourceARB=(PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB");
 if(!glShaderSourceARB) return false;
 //glCompileShaderARB
 glCompileShaderARB=(PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB");
 if(!glCompileShaderARB) return false;
 //glAttachObjectARB;
 glAttachObjectARB=(PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB");
 if(!glAttachObjectARB) return false;
 //glLinkProgramARB
 glLinkProgramARB=(PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
 if(!glLinkProgramARB) return false;
 //glValidateProgramARB
 glValidateProgramARB=(PFNGLVALIDATEPROGRAMARBPROC)wglGetProcAddress("glValidateProgramARB");
 if(!glValidateProgramARB) return false;
 //glUniform1iARB
 glUniform1iARB=(PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
 if(!glUniform1iARB) return false;
 //glUniform2iARB
 glUniform2iARB=(PFNGLUNIFORM2IARBPROC)wglGetProcAddress("glUniform2iARB");
 if(!glUniform2iARB) return false;
 //glUniform3iARB
 glUniform3iARB=(PFNGLUNIFORM3IARBPROC)wglGetProcAddress("glUniform3iARB");
 if(!glUniform3iARB) return false;
 //glUniform4iARB
 glUniform4iARB=(PFNGLUNIFORM4IARBPROC)wglGetProcAddress("glUniform4iARB");
 if(!glUniform4iARB) return false;
 //glUniform1fARB
 glUniform1fARB=(PFNGLUNIFORM1FARBPROC)wglGetProcAddress("glUniform1fARB");
 if(!glUniform1fARB) return false;
 //glUniform2fARB
 glUniform2fARB=(PFNGLUNIFORM2FARBPROC)wglGetProcAddress("glUniform2fARB");
 if(!glUniform2fARB) return false;
 //glUniform3fARB
 glUniform3fARB=(PFNGLUNIFORM3FARBPROC)wglGetProcAddress("glUniform3fARB");
 if(!glUniform3fARB) return false;
 //glUniform4fARB
 glUniform4fARB=(PFNGLUNIFORM4FARBPROC)wglGetProcAddress("glUniform4fARB");
 if(!glUniform4fARB) return false;
 //glUniform1ivARB
 glUniform1ivARB=(PFNGLUNIFORM1IVARBPROC)wglGetProcAddress("glUniform1iARB");
 if(!glUniform1ivARB) return false;
 //glUniform2ivARB
 glUniform2ivARB=(PFNGLUNIFORM2IVARBPROC)wglGetProcAddress("glUniform2ivARB");
 if(!glUniform2ivARB) return false;
 //glUniform3ivARB
 glUniform3ivARB=(PFNGLUNIFORM3IVARBPROC)wglGetProcAddress("glUniform3ivARB");
 if(!glUniform3ivARB) return false;
 //glUniform4ivARB
 glUniform4ivARB=(PFNGLUNIFORM4IVARBPROC)wglGetProcAddress("glUniform4ivARB");
 if(!glUniform4ivARB) return false;
 //glUniform1fvARB
 glUniform1fvARB=(PFNGLUNIFORM1FVARBPROC)wglGetProcAddress("glUniform1fvARB");
 if(!glUniform1fvARB) return false;
 //glUniform2fvARB
 glUniform2fvARB=(PFNGLUNIFORM2FVARBPROC)wglGetProcAddress("glUniform2fvARB");
 if(!glUniform2fvARB) return false;
 //glUniform3fvARB
 glUniform3fvARB=(PFNGLUNIFORM3FVARBPROC)wglGetProcAddress("glUniform3fvARB");
 if(!glUniform3fvARB) return false;
 //glUniform4fvARB
 glUniform4fvARB=(PFNGLUNIFORM4FVARBPROC)wglGetProcAddress("glUniform4fvARB");
 if(!glUniform4fvARB) return false;
 //glGetUniformLocationARB
 glGetUniformLocationARB=(PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
 if(!glGetUniformLocationARB) return false;
 //glUseProgramObjectARB
 glUseProgramObjectARB=(PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
 if(!glUseProgramObjectARB) return false;
 //glUniformMatrix2fvARB
 glUniformMatrix2fvARB=(PFNGLUNIFORMMATRIX2FVARBPROC)wglGetProcAddress("glUniformMatrix2fvARB");
 if(!glUniformMatrix2fvARB) return false;
 //glUniformMatrix3fvARB
 glUniformMatrix3fvARB=(PFNGLUNIFORMMATRIX3FVARBPROC)wglGetProcAddress("glUniformMatrix3fvARB");
 if(!glUniformMatrix3fvARB) return false;
 //glUniformMatrix4fvARB
 glUniformMatrix4fvARB=(PFNGLUNIFORMMATRIX4FVARBPROC)wglGetProcAddress("glUniformMatrix4fvARB");
 if(!glUniformMatrix4fvARB) return false;
 ///glGetObjectParameterivARB
 glGetObjectParameterivARB=(PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB");
 if(!glGetObjectParameterivARB) return false;
 //glGetInfoLogARB
 glGetInfoLogARB=(PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetInfoLogARB");
 if(!glGetInfoLogARB) return false;
 //glGetAttribLocationARB;
 glGetAttribLocationARB=(PFNGLGETATTRIBLOCATIONARBPROC)wglGetProcAddress("glGetAttribLocationARB");
 if(!glGetAttribLocationARB) return false;
 //glVertexAttrib2fARB
 glVertexAttrib2fARB=(PFNGLVERTEXATTRIB2FARBPROC)wglGetProcAddress("glVertexAttrib2fARB");
 if(!glVertexAttrib2fARB) return false;
 //glVertexAttrib2fvARB
 glVertexAttrib2fvARB=(PFNGLVERTEXATTRIB2FVARBPROC)wglGetProcAddress("glVertexAttrib2fvARB");
 if(!glVertexAttrib2fvARB) return false;
 //glVertexAttrib3fARB
 glVertexAttrib3fARB=(PFNGLVERTEXATTRIB3FARBPROC)wglGetProcAddress("glVertexAttrib3fARB");
 if(!glVertexAttrib3fARB) return false;
 //glVertexAttrib3fvARB
 glVertexAttrib3fvARB=(PFNGLVERTEXATTRIB3FVARBPROC)wglGetProcAddress("glVertexAttrib3fvARB");
 if(!glVertexAttrib3fvARB) return false;
 //glEnableVertexAttribArrayARB
 glEnableVertexAttribArrayARB=(PFNGLENABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress("glEnableVertexAttribArrayARB");
 if(!glEnableVertexAttribArrayARB) return false;
 //glDisableVertexAttribArrayARB
 glDisableVertexAttribArrayARB=(PFNGLDISABLEVERTEXATTRIBARRAYARBPROC)wglGetProcAddress("glDisableVertexAttribArrayARB");
 if(!glDisableVertexAttribArrayARB) return false;
 //glVertexAttribPointerARB
 glVertexAttribPointerARB=(PFNGLVERTEXATTRIBPOINTERARBPROC)wglGetProcAddress("glVertexAttribPointerARB");
 if(!glVertexAttribPointerARB) return false;
 //Finished
 return true;
}

ENbool ENGLExtension::InitWGLExtensions()
{
 //wglCreatePbufferARB
 wglCreatePbufferARB=(PFNWGLCREATEPBUFFERARBPROC)wglGetProcAddress("wglCreatePbufferARB");
 if(!wglCreatePbufferARB) return false;
 //wglGetPbufferDCARB
 wglGetPbufferDCARB=(PFNWGLGETPBUFFERDCARBPROC)wglGetProcAddress("wglGetPbufferDCARB");
 if(!wglGetPbufferDCARB) return false;
 //wglReleasePbufferDCARB
 wglReleasePbufferDCARB=(PFNWGLRELEASEPBUFFERDCARBPROC)wglGetProcAddress("wglReleasePbufferDCARB");
 if(!wglReleasePbufferDCARB) return false;
 //wglDestroyPbufferARB
 wglDestroyPbufferARB=(PFNWGLDESTROYPBUFFERARBPROC)wglGetProcAddress("wglDestroyPbufferARB");
 if(!wglDestroyPbufferARB) return false;
 //wglQueryPbufferARB
 wglQueryPbufferARB=(PFNWGLQUERYPBUFFERARBPROC)wglGetProcAddress("wglQueryPbufferARB");
 if(!wglQueryPbufferARB) return false;
 //wglChoosePixelFormatARB
 wglChoosePixelFormatARB=(PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
 if(!wglChoosePixelFormatARB) return false;
 //wglBindTexImageARB
 wglBindTexImageARB=(PFNWGLBINDTEXIMAGEARBPROC)wglGetProcAddress("wglBindTexImageARB");
 if(!wglBindTexImageARB) return false;
 //wglReleaseTexImageARB
 wglReleaseTexImageARB=(PFNWGLRELEASETEXIMAGEARBPROC)wglGetProcAddress("wglReleaseTexImageARB");
 if(!wglReleaseTexImageARB) return false;
 //wglSetPbufferAttribARB
 wglSetPbufferAttribARB=(PFNWGLSETPBUFFERATTRIBARBPROC)wglGetProcAddress("wglSetPbufferAttribARB");
 if(!wglSetPbufferAttribARB) return false;
 //Finished
 return true;
}
