#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "EngineMedia.h"

///////////////////////////////////////////////////////////////////////
///Engine Sprite
///////////////////////////////////////////////////////////////////////
ENSprite::ENSprite()
{
 //Init data
 frames=new ENArray(sizeof(ENSpriteFrame));
}

ENSprite::~ENSprite()
{
 delete frames;
}

void ENSprite::Clear()
{
 for(ENuint a=0;a<frames->GetNum();a++)
 {
  ENSpriteFrame frame;
  frames->GetElement(a,(ENubyte*)&frame);
  delete frame.Normal;
  delete frame.Alpha;
 }
 frames->Clear();
}

ENbool ENSprite::AddFrame(ENPicture *Base,ENPicture *Alpha,
                          ENubyte red,ENubyte green,ENubyte blue)
{ 
 //VARS
 ENPicture *A,*B;
 ENuint     x,y;
 //Check sizes
 if(Base->GetWidth()!=Alpha->GetWidth()||
    Base->GetHeight()!=Alpha->GetHeight()) return false;
 //Init pictures
 B=new ENPicture(Base);
 A=new ENPicture(Base);
 A->SetStdAlpha();
 //Make base transparent
 B->SetTransparent(red,green,blue);
 //Create alpha bitmap
 for(x=0;x<Base->GetWidth();x++)
   for(y=0;y<Base->GetHeight();y++)
     A->SetAlpha(x,y,Alpha->GetAlpha(x,y));
 //Now add new frame
 ENSpriteFrame frame;
 frame.Normal=B;
 frame.Alpha=A;
 frames->AddElement((ENubyte*)&frame);
 //Finished
 return true;
}

void ENSprite::DeleteFrame(ENuint frame)
{
 if(frame>=frames->GetNum()) return;

 ENSpriteFrame fr;
 frames->GetElement(frame,(ENubyte*)&fr);
 delete fr.Normal;
 delete fr.Alpha;
 frames->DelElement(frame);
}

ENbool ENSprite::Load(char *FileName)
{
 //Variables
 ENFile handle;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle) return false;
 //Read
 ENbool res=LoadFromHandle(handle);
 //Finished
 ENCloseFile(handle);
 return res;
}

ENbool ENSprite::LoadFromHandle(ENFile handle)
{
 //Variables
 ENSpriteHeader h;
 //Read header
 ENReadFile(&h,sizeof(ENSpriteHeader),1,handle);
 if(strcmp(h.ID,"ES01")!=0)
   return false;
 else
   Clear();
 //Read data
 for(ENuint a=0;a<h.NumFrames;a++)
 {
  ENSpriteFrame frame;
  //Alloc space for the two pictures
  frame.Alpha=new ENPicture();
  frame.Normal=new ENPicture();
  //Now read base data
  frame.Normal->LoadFromHandle(handle);
  //Now read alpha data
  frame.Alpha->LoadFromHandle(handle);
  //Add frame
  frames->AddElement((ENubyte*)&frame);
 }
 //Finished
 return true;
}

ENbool ENSprite::Save(char *FileName)
{
 //Variables
 ENFile handle;
 ENSpriteHeader header;
 //Open file
 handle=ENOpenFile(FileName,"wb");
 if(!handle) return false;
 //Write header
 GetHeader(header);
 ENWriteFile(&header,sizeof(ENSpriteHeader),1,handle);
 //Write data
 for(ENuint a=0;a<header.NumFrames;a++)
 {
  ENSpriteFrame frame;
  frames->GetElement(a,(ENubyte*)&frame);
  //Now write base data
  frame.Normal->SaveToHandle(handle);
  //Now write alpha data
  frame.Alpha->SaveToHandle(handle);
 }
 //Finished
 ENCloseFile(handle);
 return true;
}

ENuint ENSprite::GetNumFrames()
{
 return frames->GetNum();
}

void ENSprite::GetBase(ENPicture *pic,ENuint frame)
{
 if(frame<frames->GetNum())
 {
  ENSpriteFrame fr;
  frames->GetElement(frame,(ENubyte*)&fr);
  pic->Set(fr.Normal);
 }
}

void ENSprite::GetAlpha(ENPicture *pic,ENuint frame)
{
 if(frame<frames->GetNum())
 {
  ENSpriteFrame fr;
  frames->GetElement(frame,(ENubyte*)&fr);
  pic->Set(fr.Alpha);
 }
}

void ENSprite::GetHeader(ENSpriteHeader &header)
{
 strcpy(header.ID,"ES01");
 header.NumFrames=frames->GetNum();
}

ENbool ENSprite::IsSprite(char *FileName)
{
 //Open
 ENFile handle=ENOpenFile(FileName,"rb");
 if(handle==NULL) return false;
 //Read
 ENSpriteHeader h;
 ENReadFile(&h,sizeof(ENSpriteHeader),1,handle);
 ENCloseFile(handle);
 //Check
 
 return (strcmp(h.ID,"ES01")==0);
}
///////////////////////////////////////////////////////////////////////
///Engine Sprite loaded
///////////////////////////////////////////////////////////////////////
ENSpriteLoaded::ENSpriteLoaded(ENSprite *data)
{
 //Init values
 curframe=0;
 nxtframe=0;
 framefact=0.0f;
 //Get number of frames
 NumFrames=data->GetNumFrames();
 //Alloc space for frames
 frames=(ENSpriteFrameLoaded*)ENAllocMemory(sizeof(ENSpriteFrameLoaded)*NumFrames);
 //Initialize all frames
 for(ENuint a=0;a<NumFrames;a++)
 {
  //Generate textures
  glGenTextures(1,&frames[a].Normal);
  glGenTextures(1,&frames[a].Alpha);    
  //Get base data
  ENPicture *base=new ENPicture();
  data->GetBase(base,a);
  //Get alpha data
  ENPicture *alpha=new ENPicture();
  data->GetAlpha(alpha,a);
  //Init sprite frame
  glBindTexture(GL_TEXTURE_2D,frames[a].Normal);
  SetGLTexFlags();
  base->InitGLTex();
  glBindTexture(GL_TEXTURE_2D,frames[a].Alpha);
  SetGLTexFlags();
  alpha->InitGLTex();
  //Delete base and alpha picture
  delete base;
  delete alpha;
 }
}

ENSpriteLoaded::~ENSpriteLoaded()
{
 //Delete all frames
 for(ENuint a=0;a<NumFrames;a++)
 {
  glDeleteTextures(1,&frames[a].Normal);
  glDeleteTextures(1,&frames[a].Alpha);
 }
 //Delete frames
 ENFreeMemory(frames);
}

void ENSpriteLoaded::SetCurrentFrame(ENfloat frame)
{
 curframe=((ENuint)floor(frame))%NumFrames;
 nxtframe=(curframe+1)%NumFrames;
 framefact=(ENfloat)fmod(frame,1.0);
}

void ENSpriteLoaded::DrawRaw()
{
 //Variables
 ENbool isCullfacing;
 //Init
 isCullfacing=glIsEnabled(GL_CULL_FACE);
 glDisable(GL_CULL_FACE);
 //Draw
 glBegin(GL_QUADS);
 glVertex3f(-10.0f, 10.0f,0.0f);
 glVertex3f(-10.0f,-10.0f,0.0f);
 glVertex3f( 10.0f,-10.0f,0.0f);
 glVertex3f( 10.0f, 10.0f,0.0f);
 glEnd();
 //Reset
 if(isCullfacing) glEnable(GL_CULL_FACE);
}

void ENSpriteLoaded::DrawLightData()
{
 //Variables
 ENbool isCullfacing;
 //Init
 isCullfacing=glIsEnabled(GL_CULL_FACE);
 glDisable(GL_CULL_FACE);
 //Draw
 glBegin(GL_QUADS);
 glNormal3f(0.0f,0.0f,1.0f);
 glVertex3f(-10.0f, 10.0f,0.0f);
 glVertex3f(-10.0f,-10.0f,0.0f);
 glVertex3f( 10.0f,-10.0f,0.0f);
 glVertex3f( 10.0f, 10.0f,0.0f);
 glEnd();
 //Reset
 if(isCullfacing) glEnable(GL_CULL_FACE);
}

void ENSpriteLoaded::Draw()
{
 //Variables
 ENbool isCullfacing;
 //Init
 isCullfacing=glIsEnabled(GL_CULL_FACE);
 glDisable(GL_CULL_FACE);
 glBindTexture(GL_TEXTURE_2D,frames[curframe].Normal);
 //Draw
 glBegin(GL_QUADS);
 glTexCoord2f(0.0f,0.0f);
 glVertex3f(-10.0f, 10.0f,0.0f);
 glTexCoord2f(0.0f,1.0f);
 glVertex3f(-10.0f,-10.0f,0.0f);
 glTexCoord2f(1.0f,1.0f);
 glVertex3f( 10.0f,-10.0f,0.0f);
 glTexCoord2f(1.0f,0.0f);
 glVertex3f( 10.0f, 10.0f,0.0f);
 glEnd();
 //Reset
 if(isCullfacing) glEnable(GL_CULL_FACE);
}

void ENSpriteLoaded::DrawInterp(ENbool alpha)
{
 //Variables
 ENbool isCullfacing;
 //Init
 isCullfacing=glIsEnabled(GL_CULL_FACE);
 glDisable(GL_CULL_FACE);
 ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
 if(alpha)
   glBindTexture(GL_TEXTURE_2D,frames[curframe].Alpha);
 else
   glBindTexture(GL_TEXTURE_2D,frames[curframe].Normal);

 ENGLExtension::ActiveTexture(GL_TEXTURE1_ARB);
 if(alpha)
   glBindTexture(GL_TEXTURE_2D,frames[nxtframe].Alpha);
 else
   glBindTexture(GL_TEXTURE_2D,frames[nxtframe].Normal);
 //Set frame factor
 ENGLExtension::TexCoord(GL_TEXTURE1_ARB,framefact,framefact);
 //Draw
 glBegin(GL_QUADS);
 glTexCoord2f(0.0f,0.0f);
 glVertex3f(-10.0f, 10.0f,0.0f);
 glTexCoord2f(0.0f,1.0f);
 glVertex3f(-10.0f,-10.0f,0.0f);
 glTexCoord2f(1.0f,1.0f);
 glVertex3f( 10.0f,-10.0f,0.0f);
 glTexCoord2f(1.0f,0.0f);
 glVertex3f( 10.0f, 10.0f,0.0f);
 glEnd();
 //Reset
 if(isCullfacing) glEnable(GL_CULL_FACE);
}

void ENSpriteLoaded::GetBox(ENVector &min,ENVector &max)
{
 min=ENVector(-10.0f,-10.0f,-1.0f);
 max=ENVector( 10.0f, 10.0f, 1.0f);
}

void ENSpriteLoaded::SetGLTexFlags()
{
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
