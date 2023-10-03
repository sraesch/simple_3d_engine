//---------------------------------------------------------------------------
#ifndef ENCoreLightH
#define ENCoreLightH
//---------------------------------------------------------------------------
#include "EngineMedia.h"
#include "EngineScript.h"

class ENCoreLight
{
 public:
   struct ENDynamicLight
   {
    ENLight *light;
    ENuint   ID;
    ENbool   Visible;
   };

   static void Free();
   static void CheckVisibility(ENEngineFrustum *vfrust);
   static ENCoreLight::ENDynamicLight CreateLight();
   static ENint                       SearchLight(ENuint ID);
   static void                        DeleteLight(ENuint ID);
   static void                        SetLight(ENScriptLight *light);
   static ENCoreLight::ENDynamicLight GetLight(ENuint ID);
   static void                        GetLight(ENuint index,ENLight &light);
   static ENbool                      IsLightVisible(ENuint index);
   static ENuint                      GetNumLights();
   static void                        UpdateLights(ENuint num,ENLight *mlights);
 private:
   static const ENuint 	LIGHTBUFFERSTEP;
   static ENuint          BufferSize;
   static ENuint          NumLights;
   static ENuint          LightIDCount;
   static ENDynamicLight *Lights;

   static void            CheckLightBuffer();
};

#endif
