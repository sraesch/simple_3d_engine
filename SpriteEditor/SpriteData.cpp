//---------------------------------------------------------------------------
#include "SpriteData.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

ENSprite *ENSpriteData::SpriteData;
char      ENSpriteData::CurrentFile[MAX_PATH];
ENuint    ENSpriteData::CurrentFrame;
ENuint    ENSpriteData::NumIDs=0;
ENuint   *ENSpriteData::TexIDs=NULL;
ENuint    ENSpriteData::BackGrdID;

void ENSpriteData::Init()
{
 SpriteData=new ENSprite();
 CurrentFile[0]=0;
 CurrentFrame=0;
 NumIDs=0;
 TexIDs=NULL;
 //Load default
 glGenTextures(1,&BackGrdID);
 glBindTexture(GL_TEXTURE_2D,BackGrdID);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 ENubyte *Data=new ENubyte[64*64*3];
 for(ENuint y=0;y<64;y++)
   for(ENuint x=0;x<64;x++)
     memset(Data+(y*64+x)*3,((x/8+y/8)%2)*127+128,3);

 glEnable(GL_TEXTURE_2D);
 glTexImage2D(GL_TEXTURE_2D,0,3,64,64,
              0,GL_RGB,GL_UNSIGNED_BYTE,Data);

 delete Data;
}

void ENSpriteData::Free()
{
 glDeleteTextures(1,&BackGrdID);
 FreeTexIDs();
 delete SpriteData;
}

void ENSpriteData::Clear()
{
 FreeTexIDs();
 delete SpriteData;
 SpriteData=new ENSprite();
 CurrentFile[0]=0;
 CurrentFrame=0;
}

void ENSpriteData::Load(char *FileName)
{
 if(SpriteData->Load(FileName))
 {
  InitTexIDs();
  CurrentFrame=0;
  strcpy(CurrentFile,FileName);
 }
 else
   ShowMessage("Cannot load file!!!");
}

void ENSpriteData::Draw(ENuint mode,ENuint w,ENuint h)
{
 if(SpriteData->GetNumFrames())
   switch(mode)
   {
    case SPRITE_DRAW_NORMAL:
      glBindTexture(GL_TEXTURE_2D,TexIDs[CurrentFrame*2]);
      DrawQuad(w,h);
    break;
    case SPRITE_DRAW_TRANS:
      glEnable(GL_ALPHA_TEST);
      glAlphaFunc(GL_GREATER,0.5f);
      glBindTexture(GL_TEXTURE_2D,BackGrdID);
      DrawQuad(w,h);
      glBindTexture(GL_TEXTURE_2D,TexIDs[CurrentFrame*2]);
      DrawQuad(w,h);
      glDisable(GL_ALPHA_TEST);      
    break;
    case SPRITE_DRAW_ALPHA:
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glBindTexture(GL_TEXTURE_2D,BackGrdID);
      DrawQuad(w,h);
      glBindTexture(GL_TEXTURE_2D,TexIDs[CurrentFrame*2+1]);
      DrawQuad(w,h);
      glDisable(GL_BLEND);
    break;
   }
   else
   {
    glBindTexture(GL_TEXTURE_2D,BackGrdID);
    DrawQuad(w,h);
   }
}

ENbool ENSpriteData::AddFrame(char *BaseFile,char *AlphaFile,
                            ENubyte red,ENubyte green,ENubyte blue)
{
 char ErrorMsg[80];
 //Load base picture
 ENPicture *base=new ENPicture();
 if(!base->Load(BaseFile))
 {
  base->GetLastErrorStr(ErrorMsg);
  ShowMessage(ErrorMsg);
  return false;
 }
 //Load alpha picture
 ENPicture *alpha=new ENPicture();
 if(!alpha->Load(AlphaFile))
 {
  alpha->GetLastErrorStr(ErrorMsg);
  ShowMessage(ErrorMsg);
  return false;
 }
 if(alpha->GetBpp()<=3)
   alpha->SetStdAlpha();
 //Add new frame
 if(!SpriteData->AddFrame(base,alpha,red,green,blue))
 {
  ShowMessage("Pictures are incompatible!!!");
  return false;
 }
 delete base,alpha;
 //Add new tex id's
 NewTexID();
 
 return true;
}

void ENSpriteData::DeleteFrame()
{
 DeleteTexID(CurrentFrame);
 SpriteData->DeleteFrame(CurrentFrame);
 if(!SpriteData->GetNumFrames())
   CurrentFrame=0;
 else
   CurrentFrame=CurrentFrame%SpriteData->GetNumFrames();
}

void ENSpriteData::SetCurrentFrame(ENuint frame)
{
 if(SpriteData->GetNumFrames())
   CurrentFrame=frame%SpriteData->GetNumFrames();
 else
   CurrentFrame=0;
}

ENuint ENSpriteData::GetCurrent()
{
 return CurrentFrame;
}

void ENSpriteData::DrawQuad(ENuint w,ENuint h)
{
 glBegin(GL_QUADS);
   glColor3f(1,1,1);
   glTexCoord2f(0,1);
   glVertex3d(0,0,0);
   glTexCoord2f(1,1);
   glVertex3d(w,0,0);
   glTexCoord2f(1,0);
   glVertex3d(w,h,0);
   glTexCoord2f(0,0);   
   glVertex3d(0,h,0);
 glEnd();
}

void ENSpriteData::FreeTexIDs()
{
 glDeleteTextures(NumIDs*2,TexIDs);
 ENFreeMemory(TexIDs);
 TexIDs=NULL;
 NumIDs=0;
}

void ENSpriteData::InitTexIDs()
{
 //Free old IDs
 FreeTexIDs();
 //Init new IDs
 NumIDs=SpriteData->GetNumFrames();
 TexIDs=(ENuint*)ENAllocMemory(sizeof(ENuint)*NumIDs*2);
 glGenTextures(NumIDs*2,TexIDs);
 //Init textures
 for(ENuint a=0;a<NumIDs;a++)
 {
  //Get base and alpha data
  ENPicture *base=new ENPicture();
  ENPicture *alpha=new ENPicture();
  SpriteData->GetBase(base,a);
  SpriteData->GetAlpha(alpha,a);
  //Set base texture
  glBindTexture(GL_TEXTURE_2D,TexIDs[a*2]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  base->InitGLTex();
  //Set alpha texture
  glBindTexture(GL_TEXTURE_2D,TexIDs[a*2+1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  alpha->InitGLTex();
  //Free
  delete base,alpha;
 }
}

void ENSpriteData::NewTexID()
{
 NumIDs=SpriteData->GetNumFrames();
 TexIDs=(ENuint*)ENReallocMemory(TexIDs,sizeof(ENuint)*SpriteData->GetNumFrames()*2);
 ENuint ind=SpriteData->GetNumFrames()-1;
 glGenTextures(2,&TexIDs[ind*2]);
 //Get base and alpha data
 ENPicture *base=new ENPicture();
 ENPicture *alpha=new ENPicture();
 SpriteData->GetBase(base,ind);
 SpriteData->GetAlpha(alpha,ind);
 //Set base texture
 glBindTexture(GL_TEXTURE_2D,TexIDs[ind*2]);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 base->InitGLTex();
 //Set alpha texture
 glBindTexture(GL_TEXTURE_2D,TexIDs[ind*2+1]);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 alpha->InitGLTex();
 //Free
 delete base,alpha;
}

void ENSpriteData::DeleteTexID(ENuint ind)
{
 glDeleteTextures(2,&TexIDs[ind*2]);
 memmove(&TexIDs[ind*2],&TexIDs[(ind+1)*2],(NumIDs-ind-1)*2);
 TexIDs=(ENuint*)ENReallocMemory(TexIDs,(NumIDs-1)*2*sizeof(ENuint));
 NumIDs--;
}
