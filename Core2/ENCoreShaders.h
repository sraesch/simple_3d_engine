//---------------------------------------------------------------------------
#ifndef ENCoreShadersH
#define ENCoreShadersH
//---------------------------------------------------------------------------
#include "EngineMedia.h"

//////////////////////////////////////////////////////////
/// Base shaders
//////////////////////////////////////////////////////////
class ENBaseBumpShader
{
 public:
   static void   Init();
   static void   Free();
   static void   Set();
 protected:
   static ENuint CubeNormalMap;
};
//////////////////////////////////////////////////////////
/// General shaders
//////////////////////////////////////////////////////////
class ENShaderGenericSun : public ENShaderClass
{
 public:
   ENShaderGenericSun();
   static void SetSun(GLenum index,ENMapBase::ENMapSun *sun);
};

class ENShaderGenericSunBump : public ENShaderClass
{
 public:
   ENShaderGenericSunBump();
   virtual ENbool InitShader();
   virtual void   Activate();
   virtual void   Deactivate();
 private:
   ENint  BumpTex,NormalTex;
};
//////////////////////////////////////////////////////////
/// Shaders for map data
//////////////////////////////////////////////////////////

class ENShaderGenericLightMapSpot : public ENShaderClass
{
 public:
   ENShaderGenericLightMapSpot();
   virtual ENbool InitShader();
   virtual void   Activate();
 private:
   ENint shaderMap;
};

class ENShaderGenericLightMapOmni : public ENShaderClass
{
 public:
   ENShaderGenericLightMapOmni();
   virtual ENbool InitShader();
   virtual void   Activate();
 private:
   ENint shaderMap;
};

class ENShaderGenericBumpMapSpot : public ENShaderClass
{
 public:
   ENShaderGenericBumpMapSpot();
   virtual ENbool InitShader();
   virtual void   Activate();
   virtual void   Deactivate();
 private:
   ENint  shaderMap,BumpTex,NormalTex;
};

class ENShaderGenericBumpMapOmni : public ENShaderClass
{
 public:
   ENShaderGenericBumpMapOmni();
   virtual ENbool InitShader();
   virtual void   Activate();
   virtual void   Deactivate();
 private:
   ENint  shaderMap,BumpTex,NormalTex;
};

//////////////////////////////////////////////////////////
/// Shaders for model data
//////////////////////////////////////////////////////////
class ENShaderGenericLightModelSpot : public ENShaderClass
{
 public:
   ENShaderGenericLightModelSpot();
   virtual ENbool InitShader();
   virtual void   Activate();
   void   SetModelMatrix(ENMatrix4X4 *mat);
 private:
   ENint shaderMap,ModelMatrix;
};

class ENShaderGenericLightModelOmni : public ENShaderClass
{
 public:
   ENShaderGenericLightModelOmni();
   virtual ENbool InitShader();
   virtual void   Activate();
   void   SetModelMatrix(ENMatrix4X4 *mat);
 private:
   ENint shaderMap,ModelMatrix;
};

class ENShaderGenericBumpModelOmni : public ENShaderClass
{
 public:
   ENShaderGenericBumpModelOmni();
   virtual ENbool InitShader();
   virtual void   Activate();
   virtual void   Deactivate();
   void   SetModelMatrix(ENMatrix4X4 *mat,ENMatrix4X4 *rmat);
 private:
   ENint  shaderMap,BumpTex,NormalTex,ModelMatrix,RotMatrix;
};

class ENShaderGenericBumpModelSpot : public ENShaderClass
{
 public:
   ENShaderGenericBumpModelSpot();
   virtual ENbool InitShader();
   virtual void   Activate();
   virtual void   Deactivate();
   void   SetModelMatrix(ENMatrix4X4 *mat,ENMatrix4X4 *rmat);
 private:
   ENint  shaderMap,BumpTex,NormalTex,ModelMatrix,RotMatrix;
};
//////////////////////////////////////////////////////////////
/// Static shaders class
//////////////////////////////////////////////////////////////
#define EN_NUM_SHADERS 10
enum  ENShaderType {ENNOSHADER = -1,
                    ENGENERICSUN = 0,ENGENERICSUNBUMP = 1,
                    ENGENERICSPOT = 2,ENGENERICOMNI = 3,
                    ENGENERICBUMPSPOT = 4,ENGENERICBUMPOMNI = 5,
                    ENGENERICSPOTMODEL = 6,ENGENERICOMNIMODEL = 7,
                    ENGENERICBUMPSPOTMODEL = 8,ENGENERICBUMPOMNIMODEL = 9};

class ENCoreShaders
{
 public:
   static ENbool Init();
   static void   Free();
   static ENbool AddShader(ENShaderClass *shader,char *Name,ENShaderType type);
   static void   SetShader(ENShaderType type);
   static void   ResetShader();
   static void   SetMatrix(ENMatrix4X4 *mat,ENMatrix4X4 *rmat);
 private:
   static ENShaderType CurrentShader;
   static ENbool       ShaderCompiled;
   //Shaders
   static ENShaderClass *shaders[EN_NUM_SHADERS];
};
#endif
