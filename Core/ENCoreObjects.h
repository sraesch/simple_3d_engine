//---------------------------------------------------------------------------
#ifndef ENCoreObjectsH
#define ENCoreObjectsH
//---------------------------------------------------------------------------
#include "EngineMedia.h"
#include "EngineScript.h"

#define EN_CORE_HULL_SPHERE  0
#define EN_CORE_HULL_BOX     1
#define EN_CORE_HULL_POLYGON 2

class ENCoreObject
{
 public:
   enum ENObjType {ENMODELOBJ,ENSPRITEOBJ};

   ENCoreObject(void *src,ENObjType type,ENMapBase::ENMapObject *conf,ENint addr);
   ~ENCoreObject();
   void   Process();
   void   CalculateVisible();
   inline ENbool isVisible();
   inline ENbool isShadow();
   inline ENbool isAlpha();
   void   DrawGround();
   ENbool DrawRaw();
   void   DrawTex();
   void   DrawSunData();
   void   DrawLightData(ENbool omni);
   void   DrawSprite(ENbool isSun,ENVector *SunColor,ENbool enablealpha);
   void   DrawSpriteAlpha(ENbool isSun,ENVector *SunColor);
   void   GetSphereHull(ENVector &pos,ENfloat &range);
   void   GetBoxHull(ENVector vBox[]);
   ENbool SphereCollision(ENVector pos,ENfloat range,ENVector &SurfaceVec);
   ENbool LineCollision(ENVector vLine[],ENVector &pos,ENbool ignorePassable);
   ENScriptObject *GetScriptObject();
   ENVector GetVertPos(ENuint ind);
   ENfloat ZDist();
 protected:
   void                         *src;
   ENObjType                    type;
   ENScriptObject               data;
   ENVector                     min,max;
   ENVector                     *Vertexes;
   ENVector                     *Normals;
   ENfloat                      LastFrame;
 private:
   ENbool                       Visible;
   ENMatrix4X4                  ObjMat;
   ENMatrix3X3                  RotMat;
   ENuint                       ProcessID;

   void CalculateSpriteColor(ENbool isSun,ENVector *SunColor);
   void InitData(ENMapBase::ENMapObject *conf);
};

class ENCoreObjects
{
 public:
   static void Init();
   static void Free();
   static void Clear();
   static void Idle();
   static void UpdateObjects(ENuint num,ENMapBase::ENMapObject *mobjs);
   static ENScriptObject *CreateObject(char *Name,ENint func);
   static void DrawGroundData();
   static ENbool DrawShadowData();
   static void DrawLightData(ENbool omni);
   static void DrawSunData();
   static void DrawTexData();
   static void DrawSprites();

   static ENuint GetNum();
   static void   DeleteObject(ENCoreObject *obj);
   static ENbool SphereCollision(ENVector pos,ENfloat range,void *src);
   static ENScriptObject *Trace(ENVector vLine[],ENint func,ENbool OnlyFirst,
                                ENbool EnablePassable,ENScriptObject *me,ENVector &pos);
 private:
   static void *GetObjData(char *SrcName,ENCoreObject::ENObjType &type);

   static const ENuint AllocP;
   static const ENuint FreeP;
   static ENArrayBasic *objects;
};

#endif
