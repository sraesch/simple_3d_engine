//---------------------------------------------------------------------------
#include "ENCoreAlphaBuffer.h"
#include "ENCoreObjects.h"
//---------------------------------------------------------------------------
ENuint					ENCoreAlphaBuffer::BufferSize=0;
ENuint					ENCoreAlphaBuffer::Current=0;
ENCoreAlphaBuffer::ENAlphaObj           *ENCoreAlphaBuffer::AlphaObjs=NULL;

void ENCoreAlphaBuffer::AddAlphaObj(ENALPHATYPES type,ENfloat dist,ENint src)
{
 //Check current size of alpha Buffer
 if((Current+1)>=BufferSize)
 {
  BufferSize+=0xF;
  AlphaObjs=(ENAlphaObj*)ENReallocMemory(AlphaObjs,sizeof(ENAlphaObj)*BufferSize);
 }
 //Add alpha object
 AlphaObjs[Current].type=type;
 AlphaObjs[Current].dist=dist;
 AlphaObjs[Current].src=src;
 Current++;
}

void ENCoreAlphaBuffer::Free()
{
 if(BufferSize)
   ENFreeMemory(AlphaObjs);
}

inline void ENCoreAlphaBuffer::ExchAlphaObj(ENAlphaObj &a,ENAlphaObj &b)
{
 ENAlphaObj Temp=a;
 a=b;
 b=Temp;
}

ENint ENCoreAlphaBuffer::DevideArray(ENint min,ENint max)
{
 ENint b=min;
 ENAlphaObj PVE=AlphaObjs[max];
 for(ENint a=min;a<=max;a++)
   if(AlphaObjs[a].dist>=PVE.dist)
   {
    ExchAlphaObj(AlphaObjs[b],AlphaObjs[a]);
    b++;
   }
 return b-1;
}

void ENCoreAlphaBuffer::QSortBuffer(ENint min,ENint max)
{
 if(min<max)
 {
  ENint index=DevideArray(min,max);
  QSortBuffer(min,index-1);
  QSortBuffer(index+1,max);
 }
}

void ENCoreAlphaBuffer::DrawBuffer()
{
 //Variables
 ENAlphaSprite *spr;
 //Check for alpha data
 if(Current)
 {
  //Sort alpha objects
  QSortBuffer(0,Current-1);
  //Init alpha transparency
  glEnable(GL_ALPHA_TEST);
  glEnable(GL_BLEND);
  glAlphaFunc(GL_GREATER,0.0f);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //Disable depth mask
  glDepthMask(GL_FALSE);
  //Draw alpha buffer
  for(ENuint a=0;a<Current;a++)
	switch(AlphaObjs[a].type)
  {
   case ENALPHASPRITE:
     spr=(ENAlphaSprite*)AlphaObjs[a].src;
     ((ENCoreObject*)spr->sprite)->DrawSpriteAlpha(spr->isSun,&spr->SunColor);
     delete spr;
   break;
  };
  //Reset
  Current=0;
  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glDepthMask(GL_TRUE);
 }
}

