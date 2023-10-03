//---------------------------------------------------------------------------
#ifndef ENCoreRendererH
#define ENCoreRendererH
//---------------------------------------------------------------------------
#include "ENCoreMap.h"

class ENCoreRenderer
{
 public:
   static void Init();
   static void Free();
   static void Render();
   static void Set();
 private:
   static ENuint                DefShadow;
   static ENuint                NumPortals;
   static ENubyte               *PriPortalBuffer;
   static ENubyte               *SndPortalBuffer;
   static ENEngineFrustum       Frust;
   
   static void Update();
   static void DrawGroundData();
   static void DrawTexData();
   static void DrawLightData();
   static void DrawSunData();

   static void   DrawSpotLight(ENLight *light);
   static void   DrawOmniLight(ENLight *light);
   static ENbool DrawShadowSpot(ENLight *light,ENMatrix4X4 &lmat);
   static ENbool DrawShadowOmni(ENLight *light,ENuint ind,ENMatrix4X4 &lmat);
   static void   DrawPortalLight(ENuint ind,ENbool omni);
};

#endif
