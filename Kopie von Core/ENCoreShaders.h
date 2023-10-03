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
   ENbool InitShader();
   void   Activate();
   void   Deactivate();
   static void SetSun(GLenum index,ENMapBase::ENMapSun *sun);
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
   ENbool InitShader();
   void   Activate();
 private:
   ENint shaderMap;
};

class ENShaderGenericLightMapOmni : public ENShaderClass
{
 public:
   ENShaderGenericLightMapOmni();
   ENbool InitShader();
   void   Activate();
 private:
   ENint shaderMap;
};

class ENShaderGenericBumpMapSpot : public ENShaderClass
{
 public:
   ENShaderGenericBumpMapSpot();
   ENbool InitShader();
   void   Activate();
   void   Deactivate();
 private:
   ENint  shaderMap,BumpTex,NormalTex;
};

class ENShaderGenericBumpMapOmni : public ENShaderClass
{
 public:
   ENShaderGenericBumpMapOmni();
   ENbool InitShader();
   void   Activate();
   void   Deactivate();
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
   ENbool InitShader();
   void   Activate();
   void   SetModelMatrix(ENMatrix4X4 *mat);
 private:
   ENint shaderMap,ModelMatrix;
};

class ENShaderGenericLightModelOmni : public ENShaderClass
{
 public:
   ENShaderGenericLightModelOmni();
   ENbool InitShader();
   void   Activate();
   void   SetModelMatrix(ENMatrix4X4 *mat);
 private:
   ENint shaderMap,ModelMatrix;
};

class ENShaderGenericBumpModelOmni : public ENShaderClass
{
 public:
   ENShaderGenericBumpModelOmni();
   ENbool InitShader();
   void   Activate();
   void   Deactivate();
   void   SetModelMatrix(ENMatrix4X4 *mat,ENMatrix4X4 *rmat);
 private:
   ENint  shaderMap,BumpTex,NormalTex,ModelMatrix,RotMatrix;
};

class ENShaderGenericBumpModelSpot : public ENShaderClass
{
 public:
   ENShaderGenericBumpModelSpot();
   ENbool InitShader();
   void   Activate();
   void   Deactivate();
   void   SetModelMatrix(ENMatrix4X4 *mat,ENMatrix4X4 *rmat);
 private:
   ENint  shaderMap,BumpTex,NormalTex,ModelMatrix,RotMatrix;
};
//////////////////////////////////////////////////////////
/// Shaders for sprite data
//////////////////////////////////////////////////////////
class ENShaderSprite : public ENShaderClass
{
 public:
   ENShaderSprite();
   ENbool InitShader();
   void   Activate();
 private:
   ENint  FrameMap1,FrameMap2;
};
//////////////////////////////////////////////////////////////
/// Static shaders class
//////////////////////////////////////////////////////////////
class ENCoreShaders
{
 public:
   static ENbool Init();
   static void   Free();
   static void   SetGenericSun();
   static void   SetGenericSunBump();
   static void   SetGenericLightMapSpot();
   static void   SetGenericLightMapOmni();
   static void   SetGenericBumpMapSpot();
   static void   SetGenericBumpMapOmni();
   static void   SetGenericLightModelSpot();
   static void   SetGenericLightModelOmni();
   static void   SetGenericBumpModelOmni();
   static void   SetGenericBumpModelSpot();
   static void   SetSpriteShader();
   static void   ResetShader();
   static void   SetMatrix(ENMatrix4X4 *mat,ENMatrix4X4 *rmat);
 private:
   enum  ENShaderType {ENNOSHADER = -1,
                       ENGENERICSUN = 0,ENGENERICSUNBUMP = 1,
                       ENGENERICSPOT = 2,ENGENERICOMNI = 3,
                       ENGENERICBUMPSPOT = 4,ENGENERICBUMPOMNI = 5,
                       ENGENERICSPOTMODEL = 6,ENGENERICOMNIMODEL = 7,
                       ENGENERICBUMPSPOTMODEL = 8,ENGENERICBUMPOMNIMODEL = 9,
                       ENSPRITESHADER = 10};

   static ENShaderType CurrentShader;
   //General shader
   static ENShaderGenericSun     *GenericSun;
   static ENShaderGenericSunBump *GenericSunBump;
   //Map shaders
   static ENShaderGenericLightMapSpot *GenericLightMapSpot;
   static ENShaderGenericLightMapOmni *GenericLightMapOmni;
   static ENShaderGenericBumpMapSpot  *GenericBumpMapSpot;
   static ENShaderGenericBumpMapOmni  *GenericBumpMapOmni;
   //Model shaders
   static ENShaderGenericLightModelSpot *GenericLightModelSpot;
   static ENShaderGenericLightModelOmni *GenericLightModelOmni;
   static ENShaderGenericBumpModelOmni  *GenericBumpModelOmni;
   static ENShaderGenericBumpModelSpot  *GenericBumpModelSpot;
   //Sprite shaders
   static ENShaderSprite                *SpriteShader;
};
#endif
