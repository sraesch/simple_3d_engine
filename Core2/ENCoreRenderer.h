//---------------------------------------------------------------------------
#ifndef ENCoreRendererH
#define ENCoreRendererH
//---------------------------------------------------------------------------
#include "ENCoreMap.h"

class ENCoreRenderer
{
 public:
   static void Render();
   static void Free();
 private:
   static ENuint                NumPortals;
   static ENubyte               *PriPortalBuffer;
   static ENubyte               *SndPortalBuffer;
   static ENEngineFrustum       Frust;

   static void Update();
   static void DrawGroundData();
   static void DrawTexData();
   static void DrawLightData();
   static void DrawSunData();

   static void DrawSpotLight(ENLight *light);
   static void DrawOmniLight(ENLight *light);
   static void DrawShadowSpot(ENLight *light,ENMatrix4X4 &lmat);
   static void DrawShadowOmni(ENLight *light,ENuint ind,ENMatrix4X4 &lmat);
   static void DrawPortalLight(ENuint ind,ENbool omni);
};

#endif
