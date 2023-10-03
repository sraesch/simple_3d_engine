//---------------------------------------------------------------------------
#include "MaterialData.h"
//---------------------------------------------------------------------------
ENMaterial *ENMaterialData::mat;
ENuint      ENMaterialData::IDs[4];
ENuint      ENMaterialData::BackGrdID;

void ENMaterialData::Init()
{
 mat=new ENMaterial();
 glGenTextures(4,IDs);
 glGenTextures(1,&BackGrdID);
 //Load default
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

void ENMaterialData::Free()
{
 glDeleteTextures(4,IDs);
 glDeleteTextures(1,&BackGrdID);
 delete mat;
}

void ENMaterialData::Clear()
{
 delete mat;
 mat=new ENMaterial();
}

void ENMaterialData::Draw(ENuint mode,ENuint w,ENuint h)
{
 ENMaterial::ENMaterialHeader header;
 mat->GetHeader(header);
 DrawBack(w,h);
 switch(mode)
 {
  case EN_MATERIALDRAW_BASE:
    if((header.Basewidth*header.Baseheight))
    {
     glBindTexture(GL_TEXTURE_2D,IDs[0]);
     DrawQuad(w,h);
    }
  break;
  case EN_MATERIALDRAW_TRANS:
    if((header.Basewidth*header.Baseheight))
    {
     glEnable(GL_ALPHA_TEST);
     glAlphaFunc(GL_GREATER,0.5f);
     glBindTexture(GL_TEXTURE_2D,IDs[0]);
     DrawQuad(w,h);
     glDisable(GL_ALPHA_TEST);
    }
  break;
  case EN_MATERIALDRAW_ALPHA:
    if((header.Basewidth*header.Baseheight)&&header.EnableAlpha)
    {
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     glBindTexture(GL_TEXTURE_2D,IDs[1]);
     DrawQuad(w,h);
     glDisable(GL_BLEND);
    }
  break;
  case EN_MATERIALDRAW_BUMP:
    if((header.Bumpwidth*header.Bumpheight)&&header.EnableBump)
    {
     glBindTexture(GL_TEXTURE_2D,IDs[2]);
     DrawQuad(w,h);
    }
  break;
  case EN_MATERIALDRAW_LUMI:
    if((header.Lumiwidth*header.Lumiheight)&&header.EnableLuminance)
    {
     glBindTexture(GL_TEXTURE_2D,IDs[3]);
     DrawQuad(w,h);
    }
  break;
 };
}

void ENMaterialData::DrawQuad(ENuint w,ENuint h)
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

void ENMaterialData::DrawBack(ENuint w,ENuint h)
{
 glBindTexture(GL_TEXTURE_2D,BackGrdID);
 DrawQuad(w,h);
}

void ENMaterialData::SetTexture(ENuint which)
{
 ENPicture *pic=new ENPicture();
 switch(which)
 {
  case 0:
    mat->GetBase(pic);
  break;
  case 1:
    mat->GetAlpha(pic);
  break;
  case 2:
    mat->GetBump(pic);  
  break;
  case 3:
    mat->GetLumi(pic);
  break;
 }
 pic->Power2();
 glBindTexture(GL_TEXTURE_2D,IDs[which]);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 pic->InitGLTex();
 delete pic;
}
