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

void ENShaderGenericSunBump::SetSun(GLenum index,ENMapBase::ENMapSun *sun)
{
 glPushMatrix();
 glLoadIdentity();
 ENVector ldir=ENVecDir(1.0f,sun->dir);
 glLightfv(index,GL_SPOT_DIRECTION,(ENfloat*)&ldir);
 ENColor color;
 memcpy(&color,sun->SunColor,sizeof(ENColor));
 ENfloat tmpvec[4]={color.r/255.0f,color.g/255.0f,color.b/255.0f,1};
 glLightfv(index,GL_AMBIENT,tmpvec);
 glPopMatrix();
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
 ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
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
 ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
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
 //if(shaderMap==-1||BumpTex==-1||NormalTex==-1) return false;
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
 ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
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
 //if(ModelMatrix==-1||RotMatrix==-1||shaderMap==-1||
 //   BumpTex==-1||NormalTex==-1) return false;
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
 ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
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
 ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
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
//////////////////////////////////////////////////////////
/// Shaders for sprite data
//////////////////////////////////////////////////////////
ENShaderSprite::ENShaderSprite()
               :ENShaderClass("..\\Core\\Shaders\\Vertex\\SpriteShader.vert",
                              "..\\Core\\Shaders\\Fragment\\SpriteShader.frag")
{
}

ENbool ENShaderSprite::InitShader()
{
 //Init 
 if(!ENShaderClass::InitShader()) return false;
 //Get uniforms
 FrameMap1=ENGLExtension::GetUniformHandle(ProgramHandle,"Frame1");
 FrameMap2=ENGLExtension::GetUniformHandle(ProgramHandle,"Frame2");
 //Check uniforms
 if(FrameMap1==-1||FrameMap2==-1) return false;
 //Finished
 return true;
}

void ENShaderSprite::Activate()
{
 //Activate
 ENShaderClass::Activate();
 //Set parameters
 ENGLExtension::Uniform(FrameMap1,0);
 ENGLExtension::Uniform(FrameMap2,1);
}
///////////////////////////////////////////////////////////
/// Engine core shader class
///////////////////////////////////////////////////////////
//General shader
ENShaderGenericSun            *ENCoreShaders::GenericSun=NULL;
ENShaderGenericSunBump        *ENCoreShaders::GenericSunBump=NULL;
//Map shaders
ENShaderGenericLightMapSpot   *ENCoreShaders::GenericLightMapSpot=NULL;
ENShaderGenericLightMapOmni   *ENCoreShaders::GenericLightMapOmni=NULL;
ENShaderGenericBumpMapSpot    *ENCoreShaders::GenericBumpMapSpot=NULL;
ENShaderGenericBumpMapOmni    *ENCoreShaders::GenericBumpMapOmni=NULL;
//Model shaders
ENShaderGenericLightModelSpot *ENCoreShaders::GenericLightModelSpot=NULL;
ENShaderGenericLightModelOmni *ENCoreShaders::GenericLightModelOmni=NULL;
ENShaderGenericBumpModelOmni  *ENCoreShaders::GenericBumpModelOmni=NULL;
ENShaderGenericBumpModelSpot  *ENCoreShaders::GenericBumpModelSpot=NULL;
//Sprite shaders
ENShaderSprite                *ENCoreShaders::SpriteShader=NULL;

ENCoreShaders::ENShaderType  ENCoreShaders::CurrentShader=ENCoreShaders::ENNOSHADER;

ENbool ENCoreShaders::Init()
{
 //Loader message
 ENCoreLoader::AddMsg("Initialize engine core shaders...");
 //Create general shaders
 GenericSun=new ENShaderGenericSun();
 GenericSunBump=new ENShaderGenericSunBump();
 //Create map shaders
 GenericLightMapSpot=new ENShaderGenericLightMapSpot();
 GenericLightMapOmni=new ENShaderGenericLightMapOmni();
 GenericBumpMapSpot=new ENShaderGenericBumpMapSpot();
 GenericBumpMapOmni=new ENShaderGenericBumpMapOmni();
 //Create model shaders
 GenericLightModelSpot=new ENShaderGenericLightModelSpot();
 GenericLightModelOmni=new ENShaderGenericLightModelOmni();
 GenericBumpModelOmni=new ENShaderGenericBumpModelOmni();
 GenericBumpModelSpot=new ENShaderGenericBumpModelSpot();
 //Create sprite shaders
 SpriteShader=new ENShaderSprite();
 //Init base bump mapping shader
 ENBaseBumpShader::Init();
 //Init shaders
 if(!GenericSun->InitShader())
 {
  ENCoreMain::ShowError("Cannot init generic sun shader!!!");
  return false;
 }
 if(!GenericSunBump->InitShader())
 {
  ENCoreMain::ShowError("Cannot init generic sun bump shader!!!");
  return false;
 }
 if(!GenericLightMapSpot->InitShader())
 {
  ENCoreMain::ShowError("Cannot init generic spotlight map shader!!!");
  return false;
 }
 if(!GenericLightMapOmni->InitShader())
 {
  ENCoreMain::ShowError("Cannot init generic omnilight map shader!!!");
  return false;
 }
 if(!GenericBumpMapSpot->InitShader())
 {
  ENCoreMain::ShowError("Cannot init generic spotlight bump mapping shader!!!");
  return false;
 }
 if(!GenericBumpMapOmni->InitShader())
 {
  ENCoreMain::ShowError("Cannot init generic omnilight bump mapping shader!!!");
  return false;
 }
 if(!GenericLightModelSpot->InitShader())
 {
  ENCoreMain::ShowError("Cannot init generic spotlight model shader!!!");
  return false;
 }
 if(!GenericLightModelOmni->InitShader())
 {
  ENCoreMain::ShowError("Cannot init generic omnilight model shader!!!");
  return false;
 }
 if(!GenericBumpModelOmni->InitShader())
 {
  ENCoreMain::ShowError("Cannot init generic omni bumplight model shader!!!");
  return false;
 }
 if(!GenericBumpModelSpot->InitShader())
 {
  ENCoreMain::ShowError("Cannot init generic spot bumplight model shader!!!");
  return false;
 }
 if(!SpriteShader->InitShader())
 {
  ENCoreMain::ShowError("Cannot init sprite shader!!!");
  return false;
 }
 //Finished
 return true;
}

void ENCoreShaders::Free()
{
 ENBaseBumpShader::Free();
 if(GenericSun)            delete GenericSun;
 if(GenericSunBump)        delete GenericSunBump;
 if(GenericLightMapSpot)   delete GenericLightMapSpot;
 if(GenericLightMapOmni)   delete GenericLightMapOmni;
 if(GenericBumpMapSpot)    delete GenericBumpMapSpot;
 if(GenericBumpMapOmni)    delete GenericBumpMapOmni;
 if(GenericLightModelSpot) delete GenericLightModelSpot;
 if(GenericLightModelOmni) delete GenericLightModelOmni;
 if(GenericBumpModelOmni)  delete GenericBumpModelOmni;
 if(GenericBumpModelSpot)  delete GenericBumpModelOmni;
 if(SpriteShader)          delete SpriteShader;
}

void ENCoreShaders::SetGenericSun()
{
 if(CurrentShader!=ENGENERICSUN)
 {
  ResetShader();
  GenericSun->Activate();
  CurrentShader=ENGENERICSUN;
 }
}

void ENCoreShaders::SetGenericSunBump()
{
 if(CurrentShader!=ENGENERICSUNBUMP)
 {
  ResetShader();
  GenericSunBump->Activate();
  CurrentShader=ENGENERICSUNBUMP;
 }
}

void ENCoreShaders::SetGenericLightMapSpot()
{
 if(CurrentShader!=ENGENERICSPOT)
 {
  ResetShader();
  GenericLightMapSpot->Activate();
  CurrentShader=ENGENERICSPOT;
 }
}

void ENCoreShaders::SetGenericLightMapOmni()
{
 if(CurrentShader!=ENGENERICOMNI)
 {
  ResetShader();
  GenericLightMapOmni->Activate();
  CurrentShader=ENGENERICOMNI;
 }
}

void ENCoreShaders::SetGenericBumpMapSpot()
{
 if(CurrentShader!=ENGENERICBUMPSPOT)
 {
  ResetShader();
  GenericBumpMapSpot->Activate();
  CurrentShader=ENGENERICBUMPSPOT;
 }
}

void ENCoreShaders::SetGenericBumpMapOmni()
{
 if(CurrentShader!=ENGENERICBUMPOMNI)
 {
  ResetShader();
  GenericBumpMapOmni->Activate();
  CurrentShader=ENGENERICBUMPOMNI;
 }
}

void ENCoreShaders::SetGenericLightModelSpot()
{
 if(CurrentShader!=ENGENERICSPOTMODEL)
 {
  ResetShader();
  GenericLightModelSpot->Activate();
  CurrentShader=ENGENERICSPOTMODEL;
 }
}

void ENCoreShaders::SetGenericLightModelOmni()
{
 if(CurrentShader!=ENGENERICOMNIMODEL)
 {
  ResetShader();
  GenericLightModelOmni->Activate();
  CurrentShader=ENGENERICOMNIMODEL;
 }
}

void ENCoreShaders::SetGenericBumpModelOmni()
{
 if(CurrentShader!=ENGENERICBUMPOMNIMODEL)
 {
  ResetShader();
  GenericBumpModelOmni->Activate();
  CurrentShader=ENGENERICBUMPOMNIMODEL;
 }
}

void ENCoreShaders::SetGenericBumpModelSpot()
{
 if(CurrentShader!=ENGENERICBUMPSPOTMODEL)
 {
  ResetShader();
  GenericBumpModelSpot->Activate();
  CurrentShader=ENGENERICBUMPSPOTMODEL;
 }
}

void ENCoreShaders::SetSpriteShader()
{
 if(CurrentShader!=ENSPRITESHADER)
 {
  ResetShader();
  SpriteShader->Activate();
  CurrentShader=ENSPRITESHADER;
 }
}

void ENCoreShaders::ResetShader()
{
 switch(CurrentShader)
 {
  case ENGENERICSUN:
    GenericSun->Deactivate();
  break;
  case ENGENERICSUNBUMP:
    GenericSunBump->Deactivate();
  break;
  case ENGENERICSPOT:
    GenericLightMapSpot->Deactivate();
  break;
  case ENGENERICOMNI:
    GenericLightMapOmni->Deactivate();
  break;
  case ENGENERICBUMPSPOT:
    GenericBumpMapSpot->Deactivate();
  break;
  case ENGENERICBUMPOMNI:
    GenericBumpMapOmni->Deactivate();
  break;
  case ENGENERICSPOTMODEL:
    GenericLightModelSpot->Deactivate();
  break;
  case ENGENERICOMNIMODEL:
    GenericLightModelOmni->Deactivate();
  break;
  case ENGENERICBUMPOMNIMODEL:
    GenericBumpModelOmni->Deactivate();
  break;
  case ENGENERICBUMPSPOTMODEL:
    GenericBumpModelSpot->Deactivate();
  break;
  case ENSPRITESHADER:
    SpriteShader->Deactivate();
  break;
 }
 CurrentShader=ENNOSHADER;
}

void ENCoreShaders::SetMatrix(ENMatrix4X4 *mat,ENMatrix4X4 *rmat)
{
 switch(CurrentShader)
 {
  case ENGENERICSPOTMODEL:
    GenericLightModelSpot->SetModelMatrix(mat);
  break;
  case ENGENERICOMNIMODEL:
    GenericLightModelOmni->SetModelMatrix(mat);
  break;
  case ENGENERICBUMPOMNIMODEL:
    GenericBumpModelOmni->SetModelMatrix(mat,rmat);
  break;
  case ENGENERICBUMPSPOTMODEL:
    GenericBumpModelSpot->SetModelMatrix(mat,rmat);
  break;
 }
}
