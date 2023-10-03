//---------------------------------------------------------------------------
#include "ENCoreShaders.h"
#include "ENCoreLoader.h"
#include "ENCoreMain.h"
//---------------------------------------------------------------------------
///////////////////////////////////////////////////////////
/// Engine base bumpmapping shader
///////////////////////////////////////////////////////////
ENuint ENBaseBumpShader::CubeNormalMap=0;

void ENBaseBumpShader::Init()
{
 glGenTextures(1,&CubeNormalMap);
 glBindTexture(GL_TEXTURE_CUBE_MAP,CubeNormalMap);
 ENGLClass::GenNormalizationCubeMap();
}

void ENBaseBumpShader::Free()
{
 glDeleteTextures(1,&CubeNormalMap);
}

void ENBaseBumpShader::Set()
{
 glBindTexture(GL_TEXTURE_CUBE_MAP,CubeNormalMap);
}
///////////////////////////////////////////////////////////
/// Engine generic sun class
///////////////////////////////////////////////////////////
ENShaderGenericSun::ENShaderGenericSun()
                   :ENShaderClass("..\\Core\\Shaders\\Vertex\\GenericSun.vert",
                                  "..\\Core\\Shaders\\Fragment\\GenericSun.frag")
{
}

void ENShaderGenericSun::SetSun(GLenum index,ENMapBase::ENMapSun *sun)
{
 ENVector ldir=ENVecDir(1.0f,sun->dir);
 glLightfv(index,GL_SPOT_DIRECTION,(ENfloat*)&ldir);
 ENColor color;
 memcpy(&color,sun->SunColor,sizeof(ENColor));
 ENfloat tmpvec[4]={color.r/255.0f,color.g/255.0f,color.b/255.0f,1};
 glLightfv(index,GL_AMBIENT,tmpvec);
}

///////////////////////////////////////////////////////////
/// Engine generic sun bump mapping class
///////////////////////////////////////////////////////////
ENShaderGenericSunBump::ENShaderGenericSunBump()
                       :ENShaderClass("..\\Core\\Shaders\\Vertex\\GenericSunBump.vert",
                                      "..\\Core\\Shaders\\Fragment\\GenericSunBump.frag")
{
}

ENbool ENShaderGenericSunBump::InitShader()
{
 //Init 
 if(!ENShaderClass::InitShader()) return false;
 //Get uniforms
 BumpTex  =ENGLExtension::GetUniformHandle(ProgramHandle,"BumpTex");
 NormalTex=ENGLExtension::GetUniformHandle(ProgramHandle,"NormalTex");
 //Check uniforms
 if(BumpTex==-1||NormalTex==-1) return false;
 //Finished
 return true;
}

void ENShaderGenericSunBump::Activate()
{
 //Activate
 ENShaderClass::Activate();
 //Set parameters
 ENGLExtension::ActiveTexture(GL_TEXTURE1_ARB);
 glEnable(GL_TEXTURE_CUBE_MAP);
 ENBaseBumpShader::Set();
 ENGLExtension::Uniform(BumpTex,0);
 ENGLExtension::Uniform(NormalTex,1);
}

void ENShaderGenericSunBump::Deactivate()
{
 ENShaderClass::Deactivate();
 ENGLExtension::ActiveTexture(GL_TEXTURE1_ARB);
 glDisable(GL_TEXTURE_CUBE_MAP);
 ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
}
///////////////////////////////////////////////////////////
/// Engine generic spot lighting(map) class
///////////////////////////////////////////////////////////
ENShaderGenericLightMapSpot::ENShaderGenericLightMapSpot()
                           :ENShaderClass("..\\Core\\Shaders\\Vertex\\GenericLightMap.vert",
                                         "..\\Core\\Shaders\\Fragment\\GenericLightSpot.frag")
{
}

ENbool ENShaderGenericLightMapSpot::InitShader()
{
 //Init 
 if(!ENShaderClass::InitShader()) return false;
 //Get uniforms
 shaderMap=ENGLExtension::GetUniformHandle(ProgramHandle,"ShadowMap");
 //Check uniforms
 if(shaderMap==-1) return false;
 //Finished
 return true;
}

void ENShaderGenericLightMapSpot::Activate()
{
 //Activate
 ENShaderClass::Activate();
 //Set parameters
 ENGLExtension::Uniform(shaderMap,0);
}
///////////////////////////////////////////////////////////
/// Engine generic omni lighting(map) class
///////////////////////////////////////////////////////////
ENShaderGenericLightMapOmni::ENShaderGenericLightMapOmni()
                           :ENShaderClass("..\\Core\\Shaders\\Vertex\\GenericLightMap.vert",
                                          "..\\Core\\Shaders\\Fragment\\GenericLightOmni.frag")
{
}

ENbool ENShaderGenericLightMapOmni::InitShader()
{
 //Init 
 if(!ENShaderClass::InitShader()) return false;
 //Get uniforms
 shaderMap=ENGLExtension::GetUniformHandle(ProgramHandle,"ShadowMap");
 //Check uniforms
 if(shaderMap==-1) return false;
 //Finished
 return true;
}

void ENShaderGenericLightMapOmni::Activate()
{
 //Activate
 ENShaderClass::Activate();
 //Set parameters
 ENGLExtension::Uniform(shaderMap,0);
}
///////////////////////////////////////////////////////////
/// Engine generic spot bump mapping(map) class
///////////////////////////////////////////////////////////
ENShaderGenericBumpMapSpot::ENShaderGenericBumpMapSpot()
                           :ENShaderClass("..\\Core\\Shaders\\Vertex\\GenericBumpMap.vert",
                                          "..\\Core\\Shaders\\Fragment\\GenericBumpMapSpot.frag")
{
}

ENbool ENShaderGenericBumpMapSpot::InitShader()
{
 //Init 
 if(!ENShaderClass::InitShader()) return false;
 //Get uniforms
 shaderMap=ENGLExtension::GetUniformHandle(ProgramHandle,"ShadowMap");
 BumpTex  =ENGLExtension::GetUniformHandle(ProgramHandle,"BumpTex");
 NormalTex=ENGLExtension::GetUniformHandle(ProgramHandle,"NormalTex");
 //Check uniforms
 if(shaderMap==-1||BumpTex==-1||NormalTex==-1) return false;
 //Finished
 return true;
}

void ENShaderGenericBumpMapSpot::Activate()
{
 //Activate
 ENShaderClass::Activate();
 //Set parameters
 ENGLExtension::Uniform(shaderMap,0);
 ENGLExtension::Uniform(BumpTex,1);
 ENGLExtension::Uniform(NormalTex,2);
 ENGLExtension::ActiveTexture(GL_TEXTURE2_ARB);
 glEnable(GL_TEXTURE_CUBE_MAP);
 ENBaseBumpShader::Set();
}

void ENShaderGenericBumpMapSpot::Deactivate()
{
 ENShaderClass::Deactivate();
 ENGLExtension::ActiveTexture(GL_TEXTURE2_ARB);
 glDisable(GL_TEXTURE_CUBE_MAP);
 ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
}
///////////////////////////////////////////////////////////
/// Engine generic omni bump mapping(map) class
///////////////////////////////////////////////////////////
ENShaderGenericBumpMapOmni::ENShaderGenericBumpMapOmni()
                           :ENShaderClass("..\\Core\\Shaders\\Vertex\\GenericBumpMap.vert",
                                          "..\\Core\\Shaders\\Fragment\\GenericBumpMapOmni.frag")
{
}

ENbool ENShaderGenericBumpMapOmni::InitShader()
{
 //Init
 if(!ENShaderClass::InitShader()) return false;
 //Get uniforms
 shaderMap=ENGLExtension::GetUniformHandle(ProgramHandle,"ShadowMap");
 BumpTex  =ENGLExtension::GetUniformHandle(ProgramHandle,"BumpTex");
 NormalTex=ENGLExtension::GetUniformHandle(ProgramHandle,"NormalTex");
 //Check uniforms
 if(shaderMap==-1||BumpTex==-1||NormalTex==-1) return false;
 //Finished
 return true;
}

void ENShaderGenericBumpMapOmni::Activate()
{
 //Activate
 ENShaderClass::Activate();
 //Set parameters
 ENGLExtension::Uniform(shaderMap,0);
 ENGLExtension::Uniform(BumpTex,1);
 ENGLExtension::Uniform(NormalTex,2);
 ENGLExtension::ActiveTexture(GL_TEXTURE2_ARB);
 glEnable(GL_TEXTURE_CUBE_MAP);
 ENBaseBumpShader::Set();
}

void ENShaderGenericBumpMapOmni::Deactivate()
{
 ENShaderClass::Deactivate();
 ENGLExtension::ActiveTexture(GL_TEXTURE2_ARB);
 glDisable(GL_TEXTURE_CUBE_MAP);
 ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
}
///////////////////////////////////////////////////////////
/// Engine generic spot lighting(model) class
///////////////////////////////////////////////////////////
ENShaderGenericLightModelSpot::ENShaderGenericLightModelSpot()
                           :ENShaderClass("..\\Core\\Shaders\\Vertex\\GenericLightModel.vert",
                                          "..\\Core\\Shaders\\Fragment\\GenericLightSpot.frag")
{
}

ENbool ENShaderGenericLightModelSpot::InitShader()
{
 //Init 
 if(!ENShaderClass::InitShader()) return false;
 //Get uniforms
 shaderMap=ENGLExtension::GetUniformHandle(ProgramHandle,"ShadowMap");
 ModelMatrix=ENGLExtension::GetUniformHandle(ProgramHandle,"ModelMatrix");
 //Check uniforms
 if(shaderMap==-1||ModelMatrix==-1) return false;
 //Finished
 return true;
}

void ENShaderGenericLightModelSpot::Activate()
{
 //Activate
 ENShaderClass::Activate();
 //Set parameters
 ENGLExtension::Uniform(shaderMap,0);
}

void ENShaderGenericLightModelSpot::SetModelMatrix(ENMatrix4X4 *mat)
{
 if(Active)
   ENGLExtension::Uniform(ModelMatrix,false,mat);
}
///////////////////////////////////////////////////////////
/// Engine generic omni lighting(model) class
///////////////////////////////////////////////////////////
ENShaderGenericLightModelOmni::ENShaderGenericLightModelOmni()
                           :ENShaderClass("..\\Core\\Shaders\\Vertex\\GenericLightModel.vert",
                                          "..\\Core\\Shaders\\Fragment\\GenericLightOmni.frag")
{
}

ENbool ENShaderGenericLightModelOmni::InitShader()
{
 //Init 
 if(!ENShaderClass::InitShader()) return false;
 //Get uniforms
 shaderMap=ENGLExtension::GetUniformHandle(ProgramHandle,"ShadowMap");
 ModelMatrix=ENGLExtension::GetUniformHandle(ProgramHandle,"ModelMatrix");
 //Check uniforms
 if(shaderMap==-1||ModelMatrix==-1) return false;
 //Finished
 return true;
}

void ENShaderGenericLightModelOmni::Activate()
{
 //Activate
 ENShaderClass::Activate();
 //Set parameters
 ENGLExtension::Uniform(shaderMap,0);
}

void ENShaderGenericLightModelOmni::SetModelMatrix(ENMatrix4X4 *mat)
{
 if(Active)
   ENGLExtension::Uniform(ModelMatrix,false,mat);
}
///////////////////////////////////////////////////////////
/// Engine generic omni bump mapping(model) class
///////////////////////////////////////////////////////////
ENShaderGenericBumpModelOmni::ENShaderGenericBumpModelOmni()
                           :ENShaderClass("..\\Core\\Shaders\\Vertex\\GenericBumpModel.vert",
                                          "..\\Core\\Shaders\\Fragment\\GenericBumpMapOmni.frag")
{
}

ENbool ENShaderGenericBumpModelOmni::InitShader()
{
 //Init 
 if(!ENShaderClass::InitShader()) return false;
 //Get uniforms
 ModelMatrix=ENGLExtension::GetUniformHandle(ProgramHandle,"ModelMatrix");
 RotMatrix=ENGLExtension::GetUniformHandle(ProgramHandle,"RotMatrix");

 shaderMap=ENGLExtension::GetUniformHandle(ProgramHandle,"ShadowMap");
 BumpTex=ENGLExtension::GetUniformHandle(ProgramHandle,"BumpTex");
 NormalTex=ENGLExtension::GetUniformHandle(ProgramHandle,"NormalTex");
 //Check uniforms
 if(ModelMatrix==-1||RotMatrix==-1||shaderMap==-1||
    BumpTex==-1||NormalTex==-1) return false;
 //Finished
 return true;
}

void ENShaderGenericBumpModelOmni::Activate()
{
 //Activate
 ENShaderClass::Activate();
 //Set parameters
 ENGLExtension::Uniform(shaderMap,0);
 ENGLExtension::Uniform(BumpTex,1);
 ENGLExtension::Uniform(NormalTex,2);
 ENGLExtension::ActiveTexture(GL_TEXTURE2_ARB);
 glEnable(GL_TEXTURE_CUBE_MAP);
 ENBaseBumpShader::Set();
}

void ENShaderGenericBumpModelOmni::Deactivate()
{
 ENShaderClass::Deactivate();
 ENGLExtension::ActiveTexture(GL_TEXTURE2_ARB);
 glDisable(GL_TEXTURE_CUBE_MAP);
 ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
}

void ENShaderGenericBumpModelOmni::SetModelMatrix(ENMatrix4X4 *mat,ENMatrix4X4 *rmat)
{
 if(Active)
 {
  ENGLExtension::Uniform(ModelMatrix,false,mat);
  //Set rotation Matrix
  ENMatrix3X3 crmat;
  memcpy(crmat.val[0],rmat->val[0],sizeof(ENfloat)*3);
  memcpy(crmat.val[1],rmat->val[1],sizeof(ENfloat)*3);
  memcpy(crmat.val[2],rmat->val[2],sizeof(ENfloat)*3);
  ENGLExtension::Uniform(RotMatrix,false,&crmat);
 }
}
///////////////////////////////////////////////////////////
/// Engine generic spot bump mapping(model) class
///////////////////////////////////////////////////////////
ENShaderGenericBumpModelSpot::ENShaderGenericBumpModelSpot()
                           :ENShaderClass("..\\Core\\Shaders\\Vertex\\GenericBumpModel.vert",
                                          "..\\Core\\Shaders\\Fragment\\GenericBumpMapSpot.frag")
{
}

ENbool ENShaderGenericBumpModelSpot::InitShader()
{
 //Init 
 if(!ENShaderClass::InitShader()) return false;
 //Get uniforms
 ModelMatrix=ENGLExtension::GetUniformHandle(ProgramHandle,"ModelMatrix");
 RotMatrix=ENGLExtension::GetUniformHandle(ProgramHandle,"RotMatrix");

 shaderMap=ENGLExtension::GetUniformHandle(ProgramHandle,"ShadowMap");
 BumpTex=ENGLExtension::GetUniformHandle(ProgramHandle,"BumpTex");
 NormalTex=ENGLExtension::GetUniformHandle(ProgramHandle,"NormalTex");
 //Check uniforms
 if(ModelMatrix==-1||RotMatrix==-1||shaderMap==-1||
    BumpTex==-1||NormalTex==-1) return false;
 //Finished
 return true;
}

void ENShaderGenericBumpModelSpot::Activate()
{
 //Activate
 ENShaderClass::Activate();
 //Set parameters
 ENGLExtension::Uniform(shaderMap,0);
 ENGLExtension::Uniform(BumpTex,1);
 ENGLExtension::Uniform(NormalTex,2);
 ENGLExtension::ActiveTexture(GL_TEXTURE2_ARB);
 glEnable(GL_TEXTURE_CUBE_MAP);
 ENBaseBumpShader::Set();
}

void ENShaderGenericBumpModelSpot::Deactivate()
{
 ENShaderClass::Deactivate();
 ENGLExtension::ActiveTexture(GL_TEXTURE2_ARB);
 glDisable(GL_TEXTURE_CUBE_MAP);
 ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
}

void ENShaderGenericBumpModelSpot::SetModelMatrix(ENMatrix4X4 *mat,ENMatrix4X4 *rmat)
{
 if(Active)
 {
  ENGLExtension::Uniform(ModelMatrix,false,mat);
  //Set rotation Matrix
  ENMatrix3X3 crmat;
  memcpy(crmat.val[0],rmat->val[0],sizeof(ENfloat)*3);
  memcpy(crmat.val[1],rmat->val[1],sizeof(ENfloat)*3);
  memcpy(crmat.val[2],rmat->val[2],sizeof(ENfloat)*3);
  ENGLExtension::Uniform(RotMatrix,false,&crmat);
 }
}
///////////////////////////////////////////////////////////
/// Engine core shader class
///////////////////////////////////////////////////////////
//Shaders
ENShaderClass *ENCoreShaders::shaders[EN_NUM_SHADERS]={NULL};
ENShaderType   ENCoreShaders::CurrentShader=ENNOSHADER;
ENbool         ENCoreShaders::ShaderCompiled=false;

ENbool ENCoreShaders::Init()
{
 //Loader message
 ENCoreLoader::AddMsg("Initialize engine core shaders...");
 //Init result
 ShaderCompiled=true;
 //Create general shaders
 AddShader(new ENShaderGenericSun(),"Generic Sun",ENGENERICSUN);
 AddShader(new ENShaderGenericSunBump(),"Generic Sun bump mapping",ENGENERICSUNBUMP);
 //Create map shaders
 AddShader(new ENShaderGenericLightMapSpot(),"Generic light map spot",ENGENERICSPOT);
 AddShader(new ENShaderGenericLightMapOmni(),"Generic light map omni",ENGENERICOMNI);
 AddShader(new ENShaderGenericBumpMapSpot(),"Generic light bump map spot",ENGENERICBUMPSPOT);
 AddShader(new ENShaderGenericBumpMapOmni(),"Generic light bump map omni",ENGENERICBUMPOMNI);
 //Create model shaders
 AddShader(new ENShaderGenericLightModelSpot(),"Generic light model spot",ENGENERICSPOTMODEL);
 AddShader(new ENShaderGenericLightModelOmni(),"Generic light model omni",ENGENERICOMNIMODEL);
 AddShader(new ENShaderGenericBumpModelSpot(),"Generic bump light model spot",ENGENERICBUMPSPOTMODEL);
 AddShader(new ENShaderGenericBumpModelOmni(),"Generic bump light model omni",ENGENERICBUMPOMNIMODEL);
 //Init base bump mapping shader
 ENBaseBumpShader::Init();
 //Finished
 return ShaderCompiled;
}

ENbool ENCoreShaders::AddShader(ENShaderClass *shader,char *Name,ENShaderType type)
{
 if(!shader->InitShader())
 {
  char Buffer[80];
  sprintf(Buffer,"Cannot init shader %s!!!",Name);
  ENCoreMain::ShowError(Buffer);
  ShaderCompiled=false;
  return false;  
 }
 else
 {
  ENint index=(ENuint)type;
  if(index<0||index>=EN_NUM_SHADERS)
  {
   ENCoreMain::ShowError("Shader access violation!!!");
   ShaderCompiled=false;
   return false;
  }
  else
  {
   shaders[index]=shader;
   return true;
  }
 }
}

void ENCoreShaders::Free()
{
 //Free base bump mapping shader
 ENBaseBumpShader::Free();
 //Free shaders
 for(ENuint a=0;a<EN_NUM_SHADERS;a++)
   delete shaders[a];
}

void ENCoreShaders::SetShader(ENShaderType type)
{
 if(CurrentShader!=type)
 {
  ResetShader();
  shaders[(ENuint)type]->Activate();
  CurrentShader=type;
 }
}

void ENCoreShaders::ResetShader()
{
 if(CurrentShader>=0&&CurrentShader<EN_NUM_SHADERS)
   shaders[(ENuint)CurrentShader]->Deactivate();
   
 CurrentShader=ENNOSHADER;
}

void ENCoreShaders::SetMatrix(ENMatrix4X4 *mat,ENMatrix4X4 *rmat)
{
 switch(CurrentShader)
 {
  case ENGENERICSPOTMODEL:
    ((ENShaderGenericLightModelSpot*)shaders[(ENuint)CurrentShader])->SetModelMatrix(mat);
  break;
  case ENGENERICOMNIMODEL:
    ((ENShaderGenericLightModelOmni*)shaders[(ENuint)CurrentShader])->SetModelMatrix(mat);
  break;
  case ENGENERICBUMPOMNIMODEL:
    ((ENShaderGenericBumpModelOmni*)shaders[(ENuint)CurrentShader])->SetModelMatrix(mat,rmat);
  break;
  case ENGENERICBUMPSPOTMODEL:
    ((ENShaderGenericBumpModelSpot*)shaders[(ENuint)CurrentShader])->SetModelMatrix(mat,rmat);
  break;
 }
}
