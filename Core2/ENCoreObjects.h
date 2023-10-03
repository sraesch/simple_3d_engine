//---------------------------------------------------------------------------
#ifndef ENCoreObjectsH
#define ENCoreObjectsH
//---------------------------------------------------------------------------
#include "EngineMedia.h"
#include "EngineScript.h"

#define COL_HULL_POLYGON 0
#define COL_HULL_SPHERE  1

class ENCoreObject
{
 public:
   enum ENObjType {ENMODELOBJ,ENSPRITEOBJ};

   ENCoreObject(void *src,ENObjType type,ENMapBase::ENMapObject *conf,ENint addr);
   ~ENCoreObject();
   void   Process();
   void   CalculateVisible();
   inline ENbool isVisible();
   void   DrawGround();
   void   DrawRaw();
   void   DrawTex();
   void   DrawSunData();
   void   DrawLightData(ENbool omni);
   void   DrawLuminance();
 protected:
   void                         *src;
   ENObjType                    type;
   ENScriptObject               data;
   ENVector                     min,max;
   ENVector                     *Vertexes;
   ENVector                     *Normals;
 private:
   ENbool                       Visible;
   ENMatrix4X4                  ObjMat,RotMat;
   ENuint                       ProcessID;

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
   static void DrawGroundData();
   static void DrawRawData();
   static void DrawLightData(ENbool omni);
   static void DrawSunData();
   static void DrawTexData();

   static ENuint GetNum();
   static void   DeleteObject(ENCoreObject *obj);   
 private:
   static void *GetObjData(char *SrcName,ENCoreObject::ENObjType &type);

   static const ENuint AllocP;
   static const ENuint FreeP;
   static ENArrayBasic *objects;
};

#endif
