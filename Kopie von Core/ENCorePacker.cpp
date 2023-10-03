//---------------------------------------------------------------------------
#include "ENCorePacker.h"
#include "ENCoreMain.h"
#include "ENCoreLoader.h"
//---------------------------------------------------------------------------
ENPackerLoaded *ENCorePacker::stdpacker=NULL;
ENPackerLoaded *ENCorePacker::mappacker=NULL;

void ENCorePacker::Init()
{
 //Loader message
 ENCoreLoader::AddMsg("Initialize engine core package manager...");
 //Init packer
 stdpacker=new ENPackerLoaded();
 mappacker=new ENPackerLoaded();
}

void  ENCorePacker::Free()
{
 delete stdpacker;
 delete mappacker;
}

ENbool ENCorePacker::LoadStdPackage(char *FileName)
{
 //Variables
 char FName[MAX_PATH];
 //Check for filename
 if((*FileName)==0) return true;
 //Loader message
 ENCoreLoader::AddMsg("Load standard package...");
 //Try to find File
 if(!ENCoreMain::SearchFile(FileName,FName))
 {
  ENCoreMain::ShowError("Cannot find standard package!!!");
  return false;
 }
 //Try to load file
 if(!stdpacker->Load(FName))
 {
  ENCoreMain::ShowError("Cannot load standard package!!!");
  return false;
 }
 //Try to load all data of file
 if(!stdpacker->LoadAll())
 {
  ENCoreMain::ShowError("Cannot load data of standard package!!!");
  return false;
 }
 //Finished
 return true;
}

void ENCorePacker::LoadMapPackage(char *FileName)
{
 //Variables
 char FName[MAX_PATH];
 //Check for filename
 if((*FileName)==0) return;
 //Try to find File
 if(!ENCoreMain::SearchFile(FileName,FName))
 {
  ENCoreMain::ShowError("Cannot find map package!!!");
  return;
 }
 //Try to load file
 if(!mappacker->Load(FName))
 {
  ENCoreMain::ShowError("Cannot load map package!!!");
  return;
 }
}

void *ENCorePacker::GetSource(char *Name,ENPackerBase::ENPackageType type)
{
 void *res=stdpacker->GetSource(Name,type);
 if(!res)
   res=mappacker->GetSource(Name,type);
 //Finished
 return res;
}
