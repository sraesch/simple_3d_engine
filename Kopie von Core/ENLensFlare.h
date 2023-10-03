//---------------------------------------------------------------------------
#ifndef ENLensFlareH
#define ENLensFlareH
//---------------------------------------------------------------------------
#include "ENCorePacker.h"

struct ENLensFlareRing
{
 ENPictureLoaded *pic;
 ENVector2        size;
};

struct ENLensFlareType
{
 char                   Name[1024];
 ENuint                 NumRings;
 ENVector2              Size;
 ENLensFlareRing        *Rings;
};

struct ENLensFlareObj
{
 char             Name[1024];
 ENVector         pos;
 ENint            r,g,b;
 ENuint           id;
};

class ENLensFlare
{
 public:
   static void  Free();
   static void  Clear();
   static void  AddRing(char *LensType,char *picsrc,ENVector2 size);
   static ENint CreateLens(char *LensType);
   static void  SetLensPos(ENuint id,ENVector pos);
   static void  SetLensColor(ENuint id,ENint r,ENint g,ENint b);
   static void  DeleteLens(ENuint id);
   static void  Draw();
 protected:
   static void     DrawLensFlare(ENLensFlareType *lt,ENVector2 pos,ENVector2 wsize);
   static ENint    SearchLensFlareType(char *Name);
   static ENuint   LensIDCounter;
   static ENArray *LensTypes;
   static ENArray *LensFlares;
};

#endif
 