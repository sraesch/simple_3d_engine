//---------------------------------------------------------------------------
#include "ENCoreConfig.h"
//---------------------------------------------------------------------------
#include "ENCoreMain.h"
GLenum ENCoreConfig::ShadowPrecision=GL_DEPTH_COMPONENT;
ENint  ENCoreConfig::ShadowSize=128;

ENbool ENCoreConfig::Load()
{
 //Variables
 ENFile handle;
 char FName[MAX_PATH];
 //Open file
 if(!ENCoreMain::SearchFile("CoreConfig.txt",FName))
 {
  ENCoreMain::ShowError("Cannot find CoreConfig.txt!!!");
  return false;
 }
 handle=ENOpenFile(FName,"rb");
 if(!handle)
 {
  ENCoreMain::ShowError("Cannot read CoreConfig.txt!!!");
  return false;
 }
 //Read config
 if(!InterpConfig(handle))
 {
  ENCoreMain::ShowError("Error in CoreConfig.txt!!!");
  return false;
 }
 //Finished
 return true;
}

void ENCoreConfig::GetShadowData(GLenum &precision,ENint &size)
{
 precision=ShadowPrecision;
 size=ShadowSize;
}

ENbool ENCoreConfig::InterpConfig(ENFile handle)
{
 //Variables
 ENuint len;
 char   *FileBuffer;
 //Read file
 len=ENFileSize(handle);
 FileBuffer=new char[len+1];
 ENReadFile(FileBuffer,len,1,handle);
 FileBuffer[len]=0;
 //Close file
 ENCloseFile(handle);
 //Convert to lower character
 StrToLower(FileBuffer);
 //Read shadow data
 if(!ReadShadowData(FileBuffer)) return false;
 //Finished
 delete[] FileBuffer;
 return true;
}

ENbool ENCoreConfig::ReadShadowData(char FileBuffer[])
{
 //Variables
 ENint shp;
 //Read shadow precision
 if(!ReadVal(FileBuffer,"shadow_bits",shp)) return false;
 switch(shp)
 {
  case 16:
    ShadowPrecision=GL_DEPTH_COMPONENT16;
  break;
  case 24:
    ShadowPrecision=GL_DEPTH_COMPONENT24;
  break;
  case 32:
    ShadowPrecision=GL_DEPTH_COMPONENT32;
  break;
 };
 //Read shadow size
 if(!ReadVal(FileBuffer,"shadow_size",ShadowSize)) return false;
 //Finished
 return true;
}

void   ENCoreConfig::StrToLower(char FileBuffer[])
{
 while(*FileBuffer)
 {
  if(*FileBuffer>='A'&&*FileBuffer<='Z') *FileBuffer-='A'-'a';
  FileBuffer++;
 }
}

ENbool ENCoreConfig::ReadVal(char FileBuffer[],const char *Name,ENint &val)
{
 //Variables
 char Buffer[80];
 char *pos,*end;
 ENint len;
 //Find variable
 pos=strstr(FileBuffer,Name);
 if(!pos) return false;
 //Jump over variable
 pos+=strlen(Name);
 //Jump to number
 while(*pos==13||*pos==10||*pos==32)
   pos++;

 end=pos;
 //Numbers
 while(*end>='0'&&*end<='9')
   end++;
 //Check length
 len=end-pos;
 if(!len||len>79) return false;
 //Get string
 memcpy(Buffer,pos,len);
 Buffer[len]=0;
 //Convert string
 val=atoi(Buffer);
 return true;
}



