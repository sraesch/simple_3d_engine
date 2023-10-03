//---------------------------------------------------------------------------
#include "ProjectClass.h"
//---------------------------------------------------------------------------
ENbool ENProjectClass::MustSave=false;
TList  *ENProjectClass::ProjectFiles=new TList();
char   ENProjectClass::ProjectFile[MAX_PATH]={0};
///////////////////////////////////////////////////////
/// Project file
///////////////////////////////////////////////////////
void ENProjectFile::GetFileType(AnsiString FileName,FileType &type)
{
 //Get file extension
 AnsiString FileExtension=ExtractFileExt(FileName);
 FileExtension=FileExtension.LowerCase();
 //Analyse file extension
 type=ENUNKNOWN;
 if(FileExtension==".ens")              type=ENSCRIPT;
 if(FileExtension==".enbuildmap")       type=ENMAP;
 if(FileExtension==".enginepack")       type=ENPACKAGE;
}
///////////////////////////////////////////////////////
/// Project class
///////////////////////////////////////////////////////
void ENProjectClass::Free()
{
 for(ENuint a=0;a<GetNumFiles();a++)
   delete ((ENProjectFile*)ProjectFiles->Items[a]);

 delete ProjectFiles;
}

ENuint ENProjectClass::GetNumFiles()
{
 return ProjectFiles->Count;
}

ENProjectFile ENProjectClass::GetProjectFile(ENuint ind)
{
 return *((ENProjectFile*)ProjectFiles->Items[ind]);
}

void ENProjectClass::AddFile(AnsiString FileName)
{
 if(FileExists(FileName))
 {
  ENProjectFile *pfile=new ENProjectFile;
  strcpy(pfile->FileName,FileName.c_str());
  pfile->GetFileType(FileName,pfile->type);
  ProjectFiles->Add(pfile);
 }
}
