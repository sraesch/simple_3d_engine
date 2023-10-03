#include <math.h>
#include "EngineMedia.h"
///////////////////////////////////////////////////////////
/// Engine shader base class
///////////////////////////////////////////////////////////
ENShaderClass::ENShaderClass(char *VertShader,char *FragShader)
{
 //Set resource
 strcpy(resource.VertShader,VertShader);
 strcpy(resource.FragShader,FragShader);
 resource.UsePackage=false;
 resource.packer=NULL;
 //Init variables
 ProgramHandle=NULL;
 Active=false;
}

ENShaderClass::ENShaderClass(char *VertShader,char *FragShader,ENPacker *packer)
{
 //Set resource
 strcpy(resource.VertShader,VertShader);
 strcpy(resource.FragShader,FragShader);
 resource.UsePackage=true;
 resource.packer=packer;
 //Init variables
 ProgramHandle=NULL;
 Active=false;
}

ENShaderClass::~ENShaderClass()
{
 if(ProgramHandle) ENGLExtension::DeleteObject(ProgramHandle);
}

ENbool ENShaderClass::InitShader()
{
 //VARS
 char *shaderData;
 //Create program handle
 ProgramHandle = ENGLExtension::CreateProgram();
 //Vertex shader
 shaderData = ReadSource(resource.VertShader);
 if(!shaderData) return false;
 if(!AddShader(shaderData,GL_VERTEX_SHADER_ARB))
   return false;

 ENFreeMemory(shaderData);
 //Fragment shader
 shaderData = ReadSource(resource.FragShader);
 if(!shaderData) return false;
 if(!AddShader(shaderData,GL_FRAGMENT_SHADER_ARB))
   return false;

 ENFreeMemory(shaderData); 
 //Finished
 return true;
}

void ENShaderClass::Activate()
{
 ENGLExtension::UseProgram(ProgramHandle);
 Active=true;
}

void ENShaderClass::Deactivate()
{
 ENGLExtension::UseProgram(0);
 Active=false;
}

void ENShaderClass::ENprintInfoLog(ENhandle object)
{
    int maxLength = 0;
    ENGLExtension::GetObjectParameter(object,GL_OBJECT_INFO_LOG_LENGTH_ARB,&maxLength);

    char *infoLog = new char[maxLength];
    ENGLExtension::GetInfoLog(object,maxLength,maxLength,infoLog);

    ENFile handle=fopen("CompilerLog.txt","wb");
    fprintf(handle,"%s\n", infoLog);
    fclose(handle);
}

ENbool ENShaderClass::AddShader(const char *shaderSource,GLenum shaderType)
{
 //VARS
 ENhandle object;
 ENint compiled,linked;
 //Check data
 if(!shaderSource) return false;
 //Create shader object
 object=ENGLExtension::CreateShader(shaderType);
 //Set source
 ENGLExtension::SetShaderSource(object,shaderSource);
 //Compile shader object
 ENGLExtension::CompileShader(object);
 //Check if shader is compiled
 compiled=0;
 ENGLExtension::GetObjectParameter(object,GL_OBJECT_COMPILE_STATUS_ARB,&compiled);
 if (!compiled)
 {
  ENprintInfoLog(object);
  return false;
 }
 //Attach shader to program
 ENGLExtension::AttachObject(ProgramHandle,object);
 //Delete object
 ENGLExtension::DeleteObject(object);
 //Link program
 ENGLExtension::LinkProgram(ProgramHandle);
 //Check if program is linked
 linked=false;
 ENGLExtension::GetObjectParameter(ProgramHandle,GL_OBJECT_LINK_STATUS_ARB,&linked);
 if(!linked)
 {
  ENprintInfoLog(ProgramHandle);
  return false;
 }
 //Finished
 return true;
}

char  *ENShaderClass::ReadSource(char *Name)
{
 if(!resource.UsePackage)
   return ReadSourceFile(Name);
 else
   return ReadSourcePack(Name);
}

char *ENShaderClass::ReadSourceFile(char *FileName)
{
 //Variables
 char  *Buffer;
 ENint  len;
 ENFile handle;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle) return NULL;
 //Get length from file
 len=ENFileSize(handle);
 //Read out file
 Buffer=(char*)ENAllocMemory(sizeof(char)*(len+1));
 ENReadFile(Buffer,1,len,handle);
 Buffer[len]=0;
 //Close file and return buffer
 ENCloseFile(handle);
 return Buffer;
}

char *ENShaderClass::ReadSourcePack(char *Name)
{
 //Variables
 char  *Buffer;
 ENint ind;
 ENPackerBase::ENPackageHeader h;
 //Search package
 ind=resource.packer->SearchPackage(Name);
 if(ind<0) return NULL;
 //Get package header
 resource.packer->GetPackageHeader(ind,h);
 //Read out
 Buffer=(char*)ENAllocMemory(sizeof(char)*(h.Size+1));
 resource.packer->GetPackageData(ind,(ENubyte*)Buffer);
 Buffer[h.Size]=0;
 //Finished
 return Buffer;
}
