#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "EngineMedia.h"

///////////////////////////////////////////////////////////////////////
///Engine Font
///////////////////////////////////////////////////////////////////////
ENFont::ENFont()
{
 strcpy(header.ID,"EF01");
 memset(header.AvailableLetter,0,sizeof(ENbool)*256*2);
}

ENFont::~ENFont()
{
 Clear();
}

void ENFont::Clear()
{
 for(ENuint a=0;a<256;a++)
 {
  if(header.AvailableLetter[a*2])
    delete Letter[0][a];
  if(header.AvailableLetter[a*2+1])
    delete Letter[1][a];
 }
 strcpy(header.ID,"EF01");
 memset(header.AvailableLetter,0,sizeof(ENbool)*256*2);
}

ENbool ENFont::CheckFontHeader(ENFontHeader fh)
{
 if(fh.ID[4]) return false;
 if(strcmp(fh.ID,"EF01")!=0) return false;
 return true;
}

ENbool ENFont::Load(char *FileName)
{
 //Variables
 ENFile handle;
 //Open
 handle=ENOpenFile(FileName,"rb");
 if(handle==NULL) return false;
 //Read
 ENbool res=LoadFromHandle(handle);
 //Close
 ENCloseFile(handle);
 return res;
}

ENbool ENFont::LoadFromHandle(ENFile handle)
{
 //Variables
 ENPicture *pic;
 ENFontHeader h;
 //Read header
 ENReadFile(&h,sizeof(ENFontHeader),1,handle);
 //Check header
 if(!CheckFontHeader(h))
   return false;
 //Clear
 Clear();
 //Set new header
 header=h;
 //Load data
 for(ENuint a=0;a<256;a++)
 {
  if(header.AvailableLetter[a*2])
  {
   pic=new ENPicture();
   pic->LoadFromHandle(handle);
   Letter[0][a]=pic;
  }
  if(header.AvailableLetter[a*2+1])
  {
   pic=new ENPicture();
   pic->LoadFromHandle(handle);
   Letter[1][a]=pic;
  }
 }
 //Finished
 return true;
}

ENbool ENFont::Write(char *FileName)
{
 //Variables
 ENPicture *pic;
 ENFile handle;
 //Open
 handle=ENOpenFile(FileName,"wb");
 if(handle==NULL) return false;
 //Write header
 ENWriteFile(&header,sizeof(ENFontHeader),1,handle);
 //Write data
 for(ENuint a=0;a<256;a++)
 {
  if(header.AvailableLetter[a*2])
  {
   pic=Letter[0][a];
   pic->SaveToHandle(handle);
  }
  if(header.AvailableLetter[a*2+1])
  {
   pic=Letter[1][a];
   pic->SaveToHandle(handle);
  }
 }
 //Finished
 ENCloseFile(handle);
 return true;
}

ENFont::ENFontHeader ENFont::GetHeader()
{
 return header;
}

ENbool ENFont::GetLetter(ENuint ind,ENbool base,ENPicture *res)
{
 ind%=256;
 ENuint w=0;
 if(!base) w=1;
 if(header.AvailableLetter[ind*2+w])
 {
  res->Set(Letter[w][ind]);
  return true;
 }
 else
   return false;
}

void ENFont::SetLetter(ENuint ind,ENbool base,ENPicture *res)
{
 ind%=256;
 ENuint w=0;
 if(!base) w=1;
 //If letter already exists, delete letter
 if(header.AvailableLetter[ind*2+w])
   delete Letter[w][ind];
 //Set data
 Letter[w][ind]=new ENPicture(res);
 header.AvailableLetter[ind*2+w]=true;
}

ENbool ENFont::DelLetter(ENuint ind,ENbool base)
{
 ind%=256;
 ENuint w=0;
 if(!base) w=1;
 if(header.AvailableLetter[ind*2+w])
 {
  delete Letter[w][ind];
  header.AvailableLetter[ind*2+w]=false;
  return true;
 }
 else
   return false;
}

ENbool ENFont::IsFont(char *FileName)
{
 //Open
 ENFile handle=ENOpenFile(FileName,"rb");
 if(handle==NULL) return false;
 //Read
 ENFontHeader h;
 ENReadFile(&h,sizeof(ENFontHeader),1,handle);
 ENCloseFile(handle);
 //Check
 return CheckFontHeader(h);
}
///////////////////////////////////////////////////////////////////////
///Engine Font loaded
///////////////////////////////////////////////////////////////////////
ENFontLoaded::ENFontLoaded(ENFont *data)
{
 //Variables
 ENPicture *pic;
 //Set header
 header=data->GetHeader();
 //Init letters
 for(ENuint a=0;a<256;a++)
 {
  //Init base letter
  if(header.AvailableLetter[a*2])
  {
   glGenTextures(1,&Letters[a].Normal);
   //Get picture
   pic=new ENPicture();
   data->GetLetter(a,true,pic);
   //Init letter
   glBindTexture(GL_TEXTURE_2D,Letters[a].Normal);
   SetGLFontFlags();
   pic->InitGLTexNoAlpha();
   delete pic;
  }
  //Init transparent letter
  if(header.AvailableLetter[a*2])
  {
   glGenTextures(1,&Letters[a].Transparent);
   //Get picture
   pic=new ENPicture();
   data->GetLetter(a,true,pic);
   //Init letter
   glBindTexture(GL_TEXTURE_2D,Letters[a].Transparent);
   SetGLFontFlags();
   pic->InitGLTex();
   delete pic;
  }
  //Init alpha letter
  if(header.AvailableLetter[a*2+1])
  {
   glGenTextures(1,&Letters[a].Alpha);
   //Get picture
   pic=new ENPicture();
   data->GetLetter(a,false,pic);
   //Init letter
   glBindTexture(GL_TEXTURE_2D,Letters[a].Alpha);
   SetGLFontFlags();
   pic->InitGLTex();
   delete pic;
  }
 }
}

ENFontLoaded::~ENFontLoaded()
{
 //Delete letters
 for(ENuint a=0;a<256;a++)
 {
  if(header.AvailableLetter[a*2])
  {
   glDeleteTextures(1,&Letters[a].Normal);
   glDeleteTextures(1,&Letters[a].Transparent);
  }
  if(header.AvailableLetter[a*2+1])
    glDeleteTextures(1,&Letters[a].Alpha);
 }
}

ENbool ENFontLoaded::IsLetterAvailable(ENuint index,ENbool base)
{
 if(base)
   return header.AvailableLetter[index*2];
 else
   return header.AvailableLetter[index*2+1]; 
}

void ENFontLoaded::SetLetter(ENuint ind,ENuint mode)
{
 ind%=256;
 mode%=3;
 switch(mode)
 {
  case 0:
    if(IsLetterAvailable(ind,true))
      glBindTexture(GL_TEXTURE_2D,Letters[ind].Normal);
    else
      glBindTexture(GL_TEXTURE_2D,0);
  break;
  case 1:
    if(IsLetterAvailable(ind,true))
      glBindTexture(GL_TEXTURE_2D,Letters[ind].Transparent);
    else
      glBindTexture(GL_TEXTURE_2D,0);
  break;
  case 2:
    if(IsLetterAvailable(ind,false))
      glBindTexture(GL_TEXTURE_2D,Letters[ind].Alpha);
    else
      glBindTexture(GL_TEXTURE_2D,0);
  break;
 }
}

void ENFontLoaded::SetGLFontFlags()
{
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
