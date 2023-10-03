//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ENScriptInitDll.h"
#include "ENScriptBuilder.h"
#include "ENOperatorFuncs.h"
#include "ENSysFuncs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma comment (lib,"EngineMedia.lib")

ENPacker *ENScriptInitDll::packer=NULL;

ENbool ENScriptInitDll::Init()
{
 //Load package
 if(!LoadPackage()) return false;
 //Load types and extern source
 char *tsrc=LoadSource("Types");
 char *esrc=LoadSource("Extern");
 delete packer;
 //Check sources
 if(tsrc==NULL||esrc==NULL)
 {
  ENFreeMemory(tsrc);
  ENFreeMemory(esrc);
  ShowError("Cannot load types or extern!!!");
  return false;
 }
 //Init types
 if(!ENScriptTypeClass::SetTypes(tsrc)) return false;
 //Init operators
 if(!ENScriptOperators::Init()) return false;
 //Init extern
 if(!ENScriptExtern::LoadExtern(esrc)) return false;
 //Init system functions
 if(!ENScriptSysFuncs::Init()) return false;
 //Finished
 ENFreeMemory(tsrc);
 ENFreeMemory(esrc);
 return true;
}

ENbool ENScriptInitDll::LoadPackage()
{
 //Get Packagefile
 AnsiString PackageFile=ExtractFileDir(Application->ExeName);
 PackageFile+="\\System.enginepack";
 //Load package
 packer=new ENPacker();
 if(!packer->Load(PackageFile.c_str()))
 {
  delete packer;
  ShowError("Cannot load package System.enginepack");
  return false;
 }
 else
   return true;
}

char *ENScriptInitDll::LoadSource(char *Name)
{
 //Variables
 ENPacker::ENPackageHeader header;
 ENint ind;
 //Search textfile
 ind=packer->SearchPackage(Name);
 if(ind<0) return NULL;
 //Get header
 packer->GetPackageHeader(ind,header);
 //Load source
 char *Source=(char*)ENAllocMemory(header.Size+1);
 packer->GetPackageData(ind,(ENubyte*)Source);
 Source[header.Size]=0;
 //Finished
 return Source;
}

void ENScriptInitDll::ShowError(char *Msg)
{
 MessageBox(NULL,Msg,"Error EngineScript",MB_TASKMODAL | MB_TOPMOST | MB_ICONERROR);
}


