//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "PackerData.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma comment(lib,"EngineSystem.lib")

ENPacker   *ENPackerData::packer=NULL;
ENbool      ENPackerData::MustSave=false;
AnsiString  ENPackerData::CurrentFile;

void ENPackerData::Init()
{
 MustSave=false;
 CurrentFile="";
 ENSoundAPI::Init();
 packer=new ENPacker();
}

void ENPackerData::Free()
{
 delete packer;
}

void ENPackerData::New()
{
 MustSave=false;
 CurrentFile="";
 packer->Clear();
}

ENPackerBase::ENPackerHeader ENPackerData::GetHeader()
{
 return packer->GetHeader();
}

ENPacker::ENPackageHeader ENPackerData::GetPackageHeader(ENuint ind)
{
 ENPacker::ENPackageHeader ph;
 packer->GetPackageHeader(ind,ph);
 return ph;
}

void ENPackerData::AddPackage(AnsiString FName,AnsiString Name)
{
 char FileName[MAX_PATH];
 char PName[33];
 //Get strings
 strcpy(FileName,FName.c_str());
 strcpy(PName,Name.c_str());
 //Add package
 if(!packer->AddPackage(FileName,PName))
   ShowMessage("Cannot add package!!!");
 else
   MustSave=true;
}

void ENPackerData::DeletePackage(AnsiString Name)
{
 ENint ind=packer->SearchPackage(Name.c_str());
 if(ind>=0)
 {
  AnsiString msg="Do you really want to delete "+Name+"?";
  ENint btn=Application->MessageBox(msg.c_str(),"Delete...",MB_YESNO);
  if(btn==IDYES)
  {
   MustSave=true;
   packer->DelPackage(ind);
  }
 }
}

void ENPackerData::RenamePackage(AnsiString Name,AnsiString NewName)
{
 ENint ind=packer->SearchPackage(Name.c_str());
 if(ind>=0)
 {
  MustSave=true;
  if(!packer->RenamePackage(ind,NewName.c_str()))
    ShowMessage("Name already exists!!!");
 }
 else
   ShowMessage("Cannot find package "+Name+"!!!");
}

void ENPackerData::ExportPackage(ENuint ind,AnsiString FileName)
{
 ENPacker::ENPackageHeader h;
 h=GetPackageHeader(ind);
 ENubyte *Data=new ENubyte[h.Size];
 packer->GetPackageData(ind,Data);
 //Try to save
 ENFile handle;
 handle=ENOpenFile(FileName.c_str(),"wb");
 if(!handle)
 {
  ShowMessage("Cannot save "+FileName+"!!!");
  delete[] Data;
 }
 else
 {
  ENWriteFile(Data,1,h.Size,handle);
  ENCloseFile(handle);
 }
}

ENint ENPackerData::SearchPackage(AnsiString Name)
{
 return packer->SearchPackage(Name.c_str());
}

void ENPackerData::Save(AnsiString FileName)
{
 if(!packer->Save(FileName.c_str()))
   ShowMessage("Cannot save as "+FileName+"!!!");
 else
 {
  CurrentFile=FileName;
  MustSave=false;
 }
}

void ENPackerData::Save()
{
 if(!packer->Save(CurrentFile.c_str()))
   ShowMessage("Cannot save as "+CurrentFile+"!!!");
 else
   MustSave=false;
}

void ENPackerData::Load(AnsiString FileName)
{
 if(!packer->Load(FileName.c_str()))
   ShowMessage("Cannot load "+FileName+"!!!");
 else
 {
  CurrentFile=FileName;
  MustSave=false;
 }
}

ENbool ENPackerData::GetCanSave()
{
 return (CurrentFile.Length()>0);
}

ENbool ENPackerData::GetMustSave()
{
 return MustSave;
}
