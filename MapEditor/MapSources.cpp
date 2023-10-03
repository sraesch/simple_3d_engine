//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MapSources.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
ENPackerLoaded *ENMapSources::packer=NULL;

void ENMapSources::Init()
{
 packer=new ENPackerLoaded();
}

void ENMapSources::Free()
{
 delete packer;
}

void ENMapSources::Clear()
{
 packer->Clear();
}

void ENMapSources::Load(char *Path,char *FileName)
{
 //Variables
 AnsiString FName;
 //1. Absolute filename
 FName=FileName;
 if(FileExists(FName))
 {
  if(!packer->Load(FName.c_str()))
    ShowMessage("Cannot load file "+FName+"!!!");

  return;
 }
 //2. Search in mapfile directory
 FName=AnsiString(Path)+"\\"+AnsiString(FileName);
 if(FileExists(FName))
 {
  if(!packer->Load(FName.c_str()))
    ShowMessage("Cannot load file "+FName+"!!!");

  return;
 }
 //3. Search in exename directory
 FName=ExtractFileDir(Application->ExeName)+"\\"+AnsiString(FileName);
 if(FileExists(FName))
 {
  if(!packer->Load(FName.c_str()))
    ShowMessage("Cannot load file "+FName+"!!!");

  return;
 }
 //Not found
 ShowMessage("Cannot find package!!!");
}

ENbool ENMapSources::Draw(char *Name)
{
 //Variables
 void *data;
 //Model
 data=packer->GetSource(Name,ENPackerBase::ENMODEL);
 if(data)
 {
  ((ENModelLoaded*)data)->SetCurrentFrame(0.0f);
  ((ENModelLoaded*)data)->SetCurrentSkin(0);
  ((ENModelLoaded*)data)->Draw(NULL,NULL);
  return true;
 }
 //Sprite
 data=packer->GetSource(Name,ENPackerBase::ENSPRITE);
 if(data)
 {
  ((ENSpriteLoaded*)data)->Draw();
  return true;
 }
 //Not found
 return false;
}

void  ENMapSources::GetPacks(TListBox *lb,ENPackerBase::ENPackageType type)
{
 lb->Clear();
 for(ENuint a=0;a<packer->GetNum();a++)
   if(packer->GetType(a)==type)
   {
    char Buffer[80];
    packer->GetName(a,Buffer);
    lb->Items->Add(Buffer);
   }
}

ENint ENMapSources::GetPack(char *Name)
{
 ENint ind=packer->SearchPackage(Name);
 if(ind==-1) return -1;

 ENPackerBase::ENPackageType t=packer->GetType(ind);
 if(t!=ENPackerBase::ENMODEL&&t!=ENPackerBase::ENSPRITE)
   return -1;

 return ind;
}

ENVector ENMapSources::GetObjDim(char *Name)
{
 //Variables
 ENVector min,max;
 void *data;
 //Model
 data=packer->GetSource(Name,ENPackerBase::ENMODEL);
 if(data)
 {
  ((ENModelLoaded*)data)->GetBox(min,max);
  return max-min;
 }
 //Sprite
 data=packer->GetSource(Name,ENPackerBase::ENSPRITE);
 if(data)
 {
  ((ENSpriteLoaded*)data)->GetBox(min,max);
  return max-min;
 }
 //Not found
 return ENVector(0.0f,0.0f,0.0f);
}
