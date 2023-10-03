#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include "EngineMedia.h"

//////////////////////////////////////////////////////////
///Eninge material not loaded
//////////////////////////////////////////////////////////
ENMaterial::ENMaterial()
{
 //Init header
 Header.Baseheight=0;
 Header.Basewidth=0;
 Header.Bumpheight=0;
 Header.Bumpwidth=0;
 Header.Lumiwidth=0;
 Header.Lumiheight=0;
 Header.EnableBump=false;
 Header.EnableAlpha=false;
 Header.EnableLuminance=false;
 //Init ID
 strcpy(Header.ID,"EM01");
 //Init body
 BaseImg=NULL;
 BumpData=NULL;
 AlphaImg=NULL;
 LumiData=NULL;
}

ENMaterial::ENMaterial(ENMaterial *mat)
{
 //Get header
 mat->GetHeader(Header);
 //Alloc space
 BaseImg=(ENubyte*)ENAllocMemory(Header.Basewidth*Header.Baseheight*4);

 if(Header.EnableAlpha)
   AlphaImg=(ENubyte*)ENAllocMemory(Header.Basewidth*Header.Baseheight*4);
 else
   AlphaImg=NULL;

 if(Header.EnableBump)
   BumpData=(ENubyte*)ENAllocMemory(Header.Bumpwidth*Header.Bumpheight*3);
 else
   BumpData=NULL;

 if(Header.EnableLuminance)
   LumiData=(ENubyte*)ENAllocMemory(Header.Lumiwidth*Header.Lumiheight*3);
 else
   LumiData=NULL;
 //Get base
 ENPicture *base=new ENPicture();
 mat->GetBase(base);
 base->GetData(BaseImg);
 delete base;
 //Get alpha
 if(Header.EnableAlpha)
 {
  ENPicture *alpha=new ENPicture();
  mat->GetAlpha(alpha);
  alpha->GetData(AlphaImg);
  delete alpha;
 }
 //Get bump
 if(Header.EnableBump)
 {
  ENPicture *bump=new ENPicture();
  mat->GetBump(bump);
  bump->GetData(BumpData);
  delete bump;
 }
 //Get luminance
 if(Header.EnableLuminance)
 {
  ENPicture *lumi=new ENPicture();
  mat->GetLumi(lumi);
  lumi->GetData(LumiData);
  delete lumi;
 }
}

ENMaterial::~ENMaterial()
{
 ENFreeMemory(BaseImg);

 if(Header.EnableBump)
   ENFreeMemory(BumpData);

 if(Header.EnableAlpha)
   ENFreeMemory(AlphaImg);

 if(Header.EnableLuminance)
   ENFreeMemory(LumiData);
}

void ENMaterial::Set(ENMaterial *mat)
{
 //Clear
 ENFreeMemory(BaseImg);

 if(Header.EnableBump)
   ENFreeMemory(BumpData);

 if(Header.EnableAlpha)
   ENFreeMemory(AlphaImg);

 if(Header.EnableLuminance)
   ENFreeMemory(LumiData);
 //Get header
 mat->GetHeader(Header);
 //Alloc space
 BaseImg=(ENubyte*)ENAllocMemory(Header.Basewidth*Header.Baseheight*4);

 if(Header.EnableAlpha)
   AlphaImg=(ENubyte*)ENAllocMemory(Header.Basewidth*Header.Baseheight*4);
 else
   AlphaImg=NULL;

 if(Header.EnableBump)
   BumpData=(ENubyte*)ENAllocMemory(Header.Bumpwidth*Header.Bumpheight*3);
 else
   AlphaImg=NULL;

 if(Header.EnableLuminance)
   LumiData=(ENubyte*)ENAllocMemory(Header.Lumiwidth*Header.Lumiheight*3);
 else
   LumiData=NULL;
 //Get base
 ENPicture *base=new ENPicture();
 mat->GetBase(base);
 base->GetData(BaseImg);
 delete base;
 //Get alpha
 if(Header.EnableAlpha)
 {
  ENPicture *alpha=new ENPicture();
  mat->GetAlpha(alpha);
  alpha->GetData(AlphaImg);
  delete alpha;
 }
 //Get bump
 if(Header.EnableBump)
 {
  ENPicture *bump=new ENPicture();
  mat->GetBump(bump);
  bump->GetData(BumpData);
  delete bump;
 }
 //Get luminance
 if(Header.EnableLuminance)
 {
  ENPicture *lumi=new ENPicture();
  mat->GetLumi(lumi);
  lumi->GetData(LumiData);
  delete lumi;
 }
}

void ENMaterial::SetBaseData(ENPicture *src,ENubyte red,ENubyte green,ENubyte blue)
{
 //VARS
 ENPicture *tmppic;
 //Init temp picture
 tmppic=new ENPicture(src);
 tmppic->SetTransparent(red,green,blue);
 //Free alpha data if they doesn't fit
 if(Header.Basewidth!=tmppic->GetWidth()||
    Header.Baseheight!=tmppic->GetHeight())
 {
  Header.EnableAlpha=false;
  ENFreeMemory(AlphaImg);
  AlphaImg=NULL;
 }
 //Realloc for the base data
 BaseImg=(ENubyte*)ENReallocMemory(BaseImg,
              4*tmppic->GetWidth()*tmppic->GetHeight());
 //Change header
 Header.Basewidth=tmppic->GetWidth();
 Header.Baseheight=tmppic->GetHeight();
 //Copy new data
 tmppic->GetData(BaseImg);
 //Delete temp picture
 delete tmppic;
}

ENbool ENMaterial::SetAlphaData(ENPicture *src)
{
 //VARS
 ENuint x,y;
 //Check size
 if(Header.Basewidth!=src->GetWidth()||
    Header.Baseheight!=src->GetHeight())
    return false;
 //Realloc for the base data
 Header.EnableAlpha=true;
 AlphaImg=(ENubyte*)ENReallocMemory(AlphaImg,4*src->GetWidth()*src->GetHeight());
 //Insert new data
 for(x=0;x<src->GetWidth();x++)
   for(y=0;y<src->GetHeight();y++)
   {
    //Copy base data
    memcpy(AlphaImg+(y*src->GetWidth()+x)*4,
           BaseImg+(y*src->GetWidth()+x)*4,3);
    //Second: Get alpha color
    AlphaImg[(y*src->GetWidth()+x)*4+3]=src->GetAlpha(x,y);
   }

 return true;
}

void ENMaterial::SetBumpData(ENPicture *src)
{
 //Realloc for the bump data
 BumpData=(ENubyte*)ENReallocMemory(BumpData,src->GetWidth()*src->GetHeight()*3);
 //Change header
 Header.Bumpwidth=src->GetWidth();
 Header.Bumpheight=src->GetHeight();
 Header.EnableBump=true;
 //Copy new data
 for(ENuint x=0;x<src->GetWidth();x++)
   for(ENuint y=0;y<src->GetHeight();y++)
   {
    ENuint c=src->GetPixel(x,y);
    //Set pixel
    memcpy(BumpData+(y*src->GetWidth()+x)*3,&c,3);
   }
}

void ENMaterial::SetLumiData(ENPicture *src)
{
 //Realloc for the luminance data
 LumiData=(ENubyte*)ENReallocMemory(LumiData,src->GetWidth()*src->GetHeight()*3);
 //Change header
 Header.Lumiwidth=src->GetWidth();
 Header.Lumiheight=src->GetHeight();
 Header.EnableLuminance=true;
 //Copy new data
 for(ENuint x=0;x<src->GetWidth();x++)
   for(ENuint y=0;y<src->GetHeight();y++)
   {
    ENuint c=src->GetPixel(x,y);
    //Set pixel
    memcpy(LumiData+(y*src->GetWidth()+x)*3,&c,3);
   } 
}

void ENMaterial::FreeBumpData()
{
 if(Header.EnableBump)
 {
  ENFreeMemory(BumpData);
  BumpData=NULL;
  Header.EnableBump=false;
 }
}

void ENMaterial::FreeLumiData()
{
 if(Header.EnableLuminance)
 {
  ENFreeMemory(LumiData);
  LumiData=NULL;
  Header.EnableLuminance=false;
 }
}

ENbool ENMaterial::Save(char *FileName)
{
 //Vars
 ENFile handle;
 //Open file
 handle=ENOpenFile(FileName,"wb");
 if(handle==NULL) return false;
 //Write header
 ENWriteFile(&Header,1,sizeof(ENMaterialHeader),handle);
 //Write body data
 ENWriteFile(BaseImg,4*sizeof(ENubyte),Header.Basewidth*Header.Baseheight,handle);
 if(Header.EnableAlpha)
   ENWriteFile(AlphaImg,4*sizeof(ENubyte),Header.Basewidth*Header.Baseheight,handle);
 if(Header.EnableBump)
   ENWriteFile(BumpData,3,Header.Bumpwidth*Header.Bumpheight,handle);
 if(Header.EnableLuminance)
   ENWriteFile(LumiData,3,Header.Lumiwidth*Header.Lumiheight,handle);
 //Finished
 ENCloseFile(handle);
 return true;
}

ENbool ENMaterial::Load(char *FileName)
{
 //Vars
 ENFile handle;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(handle==NULL) return false;
 //Read
 ENbool res=LoadFromHandle(handle);
 //Finished
 ENCloseFile(handle);
 return res;
}

void ENMaterial::SaveToHandle(ENFile handle)
{
 //Write header
 ENWriteFile(&Header,1,sizeof(ENMaterialHeader),handle);
 //Write body data
 ENWriteFile(BaseImg,4*sizeof(ENubyte),Header.Basewidth*Header.Baseheight,handle);
 if(Header.EnableAlpha)
   ENWriteFile(AlphaImg,4*sizeof(ENubyte),Header.Basewidth*Header.Baseheight,handle);
 if(Header.EnableBump)
   ENWriteFile(BumpData,3,Header.Bumpwidth*Header.Bumpheight,handle);
 if(Header.EnableLuminance)
   ENWriteFile(LumiData,3,Header.Lumiwidth*Header.Lumiheight,handle);
}

ENbool ENMaterial::LoadFromHandle(ENFile handle)
{
 ENMaterialHeader _header;
 //Read header
 ENReadFile(&_header,sizeof(ENMaterialHeader),1,handle);
 //Check header
 if(strcmp(_header.ID,"EM01")!=0)
   return false;
 else
   Header=_header;
 //Realloc
 BaseImg =(ENubyte*)ENReallocMemory(BaseImg,
	            4*Header.Basewidth*Header.Baseheight);

 if(Header.EnableAlpha)
    AlphaImg =(ENubyte*)ENReallocMemory(AlphaImg,
	            4*Header.Basewidth*Header.Baseheight);

 if(Header.EnableBump)
   BumpData=(ENubyte*)ENReallocMemory(BumpData,
	                               Header.Bumpwidth*Header.Bumpheight*3);

 if(Header.EnableLuminance)
   LumiData=(ENubyte*)ENReallocMemory(LumiData,
	                              Header.Lumiwidth*Header.Lumiheight*3);
 //Read body data
 ENReadFile(BaseImg,Header.Basewidth*Header.Baseheight,4,handle);
 if(Header.EnableAlpha)
   ENReadFile(AlphaImg,Header.Basewidth*Header.Baseheight,4,handle);

 if(Header.EnableBump)
   ENReadFile(BumpData,Header.Bumpwidth*Header.Bumpheight,3,handle);

 if(Header.EnableLuminance)
   ENReadFile(LumiData,Header.Lumiwidth*Header.Lumiheight,3,handle);

 //Finished
 return true;
}

void ENMaterial::GetHeader(ENMaterial::ENMaterialHeader &h)
{
 h=Header;
}

void ENMaterial::GetBase(ENPicture *dst)
{
 dst->Set(4,Header.Basewidth,Header.Baseheight,BaseImg);
}

void ENMaterial::GetAlpha(ENPicture *dst)
{
 if(Header.EnableAlpha)
   dst->Set(4,Header.Basewidth,Header.Baseheight,AlphaImg);
}

void ENMaterial::GetBump(ENPicture *dst)
{
 if(Header.EnableBump)
   dst->Set(3,Header.Bumpwidth,Header.Bumpheight,BumpData);
}

void ENMaterial::GetLumi(ENPicture *dst)
{
 if(Header.EnableLuminance)
   dst->Set(3,Header.Lumiwidth,Header.Lumiheight,LumiData);
}
//////////////////////////////////////////////////////////
///Eninge material loaded
//////////////////////////////////////////////////////////
ENMaterialLoaded::ENMaterialLoaded(ENMaterial *mat,ENbool mipmap)
{
 //Set header data
 ENMaterial::ENMaterialHeader header;
 mat->GetHeader(header);
 EnableBump=header.EnableBump;
 EnableAlpha=header.EnableAlpha;
 EnableLuminance=header.EnableLuminance;
 //Init base
 ENPicture *base=new ENPicture();
 mat->GetBase(base);
 glGenTextures(1,&BaseID);
 glBindTexture(GL_TEXTURE_2D,BaseID);
 InitTex(base,mipmap);
 delete base;
 //Init alpha
 if(EnableAlpha)
 {
  ENPicture *alpha=new ENPicture();
  mat->GetAlpha(alpha);
  glGenTextures(1,&AlphaID);
  glBindTexture(GL_TEXTURE_2D,AlphaID);
  InitTex(alpha,mipmap);
  delete alpha;
 }
 //Init bump
 if(EnableBump)
 {
  ENPicture *bump=new ENPicture();
  mat->GetBump(bump);
  glGenTextures(1,&BumpID);
  glBindTexture(GL_TEXTURE_2D,BumpID);
  InitTex(bump,mipmap);
  delete bump;
 }
 //Init luminance
 if(EnableLuminance)
 {
  ENPicture *lumi=new ENPicture();
  mat->GetLumi(lumi);
  glGenTextures(1,&LumiID);
  glBindTexture(GL_TEXTURE_2D,LumiID);
  InitTex(lumi,mipmap);
  delete lumi;
 }
}

ENMaterialLoaded::~ENMaterialLoaded()
{
 glDeleteTextures(1,&BaseID);
 if(EnableAlpha)
   glDeleteTextures(1,&AlphaID);
 if(EnableBump)
   glDeleteTextures(1,&BumpID);
 if(EnableLuminance)
   glDeleteTextures(1,&LumiID);   
}

void ENMaterialLoaded::InitTex(ENPicture *pic,ENbool mipmap)
{
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 if(mipmap)
 {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  pic->InitGLMipMap();
 }
 else
 {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  pic->InitGLTex();
 }
}

void ENMaterialLoaded::SetBase()
{
 glBindTexture(GL_TEXTURE_2D,BaseID);
}

void ENMaterialLoaded::SetAlpha()
{
 if(EnableAlpha)
   glBindTexture(GL_TEXTURE_2D,AlphaID);
}

void ENMaterialLoaded::SetBump()
{
 if(EnableBump)
   glBindTexture(GL_TEXTURE_2D,BumpID);
}

void ENMaterialLoaded::SetLuminance()
{
 if(EnableLuminance)
   glBindTexture(GL_TEXTURE_2D,LumiID);
}

ENbool ENMaterialLoaded::isAlphaSupported()
{
 return EnableAlpha;
}

ENbool ENMaterialLoaded::isBumpSupported()
{
 return EnableBump;
}

ENbool ENMaterialLoaded::isLuminanceSupported()
{
 return EnableLuminance;
}

