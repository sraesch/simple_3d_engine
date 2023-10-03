//---------------------------------------------------------------------------
#ifndef MaterialDataH
#define MaterialDataH
//---------------------------------------------------------------------------
#include <vcl.h>
#include "EngineMedia.h"

#pragma comment(lib,"EngineMedia.lib")

#define EN_MATERIALDRAW_BASE  0
#define EN_MATERIALDRAW_TRANS 1
#define EN_MATERIALDRAW_ALPHA 2
#define EN_MATERIALDRAW_BUMP  3
#define EN_MATERIALDRAW_LUMI  4

class ENMaterialData
{
 public:
   static ENMaterial *mat; 
   static void Init();
   static void Free();
   static void Clear();
   static void Draw(ENuint mode,ENuint w,ENuint h);
   static void SetTexture(ENuint which);
 protected:
   static ENuint IDs[4];
   static ENuint BackGrdID;
   static void          DrawQuad(ENuint w,ENuint h);
   static void          DrawBack(ENuint w,ENuint h);
};

#endif
