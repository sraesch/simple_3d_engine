//---------------------------------------------------------------------------
#ifndef ENCoreMapH
#define ENCoreMapH
//---------------------------------------------------------------------------
#include "EngineMedia.h"
/////////////////////////////////////////////////////////////
///Engine Core map class
/////////////////////////////////////////////////////////////
class ENCoreMap
{
 public:
   static void                          Init();
   static void                          Free();
   static void                          Load(char *FileName);
   static ENuint                        GetNumPortals();
   static ENMapBuildLoader::ENPortal   *GetPortal(ENuint ind);
   static ENbool                        isPortalVisible(ENuint ind,ENEngineFrustum *f);
   static ENbool                        PortalLightIntersect(ENuint ind,ENLight *light);
   static void                          DrawPortalShadow(ENuint ind);
   static void                          DrawPortalGnd(ENuint ind);
   static void                          DrawPortalTex(ENuint ind);
   static void                          DrawSkyBox();
   static void                          SetLightFace(ENMapBase::ENMapFace *face,
                                                     ENbool omni);
   static void                          SetSunFace(ENMapBase::ENMapFace *face);
   static ENbool                        SetSun();
   static ENbool                        isSun(ENubyte suncolor[]);
 private:
   static ENMapBuildBase::ENMapBuildHeader header;
   static ENMapBuildLoader *mapobj;

   static void DrawTexFace(ENMapBase::ENMapFace *face);
   static void UpdateLights();
   static void UpdateSounds();
   static void UpdateObjects();   
};

#endif
