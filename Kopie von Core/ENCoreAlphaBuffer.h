//---------------------------------------------------------------------------
#ifndef ENCoreAlphaBufferH
#define ENCoreAlphaBufferH
//---------------------------------------------------------------------------
#include "EngineSystem.h"

struct ENAlphaSprite
{
 ENbool   isSun;
 ENVector SunColor;
 void    *sprite;
};

class ENCoreAlphaBuffer
{
 public:
   enum ENALPHATYPES {ENALPHASPRITE};
   static void AddAlphaObj(ENALPHATYPES type,ENfloat dist,ENint src);
   static void DrawBuffer();
   static void Free();
 protected:
   struct ENAlphaObj
   {
    ENfloat  dist;
    ENALPHATYPES type;
    ENint        src;
   };
   static ENuint      BufferSize,Current;
   static ENAlphaObj *AlphaObjs;
   static inline void ExchAlphaObj(ENAlphaObj &a,ENAlphaObj &b);
   static ENint       DevideArray(ENint min,ENint max);
   static void        QSortBuffer(ENint min,ENint max);
};

#endif
 