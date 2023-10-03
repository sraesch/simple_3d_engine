#include <iostream.h>
#include "ENScriptLinker.h"

///////////////////////////ERROR's///////////////////////////////////
#define   EN_LINKER_ERROR0    "Cannot link!!!"
#define   EN_LINKER_ERROR1    "Cannot load file!!!"
#define   EN_LINKER_ERROR2    "Redudant linking!!!"
#define   EN_LINKER_ERROR3    "Incorrect linker command!!!"
/////////////////////////////////////////////////////////////////////

ENScriptLinker::ENScriptLinker()
{ 
 NumFiles=0;
 Files=NULL;
 parser=NULL; 
 Links.NumLinks=0;
 Links.Links=NULL;
 ErrorMsg[0]=0;
}

ENScriptLinker::~ENScriptLinker()
{
 ENFreeMemory(Files);
 if(parser) delete parser;
 DeleteLinks(&Links);
}

ENbool ENScriptLinker::Link(char *FileName,char *Path)
{ 
 //Init
 ErrorMsg[0]=0;
 if(parser) delete parser;
 parser=new ENScriptParser();
 DeleteLinks(&Links);
 //Set path
 strcpy(this->Path.str,Path);
 //Pharse all files
 Links.NumLinks=0;
 Links.Links=NULL; 
 Links.Parent=NULL;
 //Set file name for link
 strcpy(Links.FName.str,FileName);
 //Pharse file
 if(!PharseFile(&Links)) {AddError(EN_LINKER_ERROR0);return false;}
 //Finished
 delete parser;
 parser=NULL;
 
 return true;
}

ENuint ENScriptLinker::GetNum()
{
 return NumFiles;
}

ENTEXTSTR ENScriptLinker::GetFile(ENuint ind)
{
 return Files[ind%NumFiles];
}

void ENScriptLinker::GetError(ENuint len,char *dst)
{
 ENuint slen=strlen(ErrorMsg);
 if(slen>len-1)
 {
  memcpy(dst,ErrorMsg,len-1);
  ErrorMsg[len]=0;
 }
 else
   strcpy(dst,ErrorMsg);
}

ENbool ENScriptLinker::PharseFile(ENLinks *Link)
{
 //VARS
 char      LName[MAX_PATH];
 ENTEXTSTR TmpInclude; 
 //Create file name for loading
 sprintf(LName,"%s\\%s",Path.str,Link->FName.str);
 //Load source
 if(!parser->LoadSource(LName)) {AddError(EN_LINKER_ERROR1);return false;}
 //Get all includes
 if(!GetInclude(TmpInclude.str)) return false;
 while(TmpInclude.str[0]!=0)
 {
  Link->NumLinks++;
  Link->Links=(ENLinks*)ENReallocMemory(Link->Links,sizeof(ENLinks)*Link->NumLinks);
  Link->Links[Link->NumLinks-1].NumLinks=0;
  Link->Links[Link->NumLinks-1].Links=NULL;
  Link->Links[Link->NumLinks-1].Parent=Link;
  strcpy(Link->Links[Link->NumLinks-1].FName.str,TmpInclude.str);
  //Check for redundance
  if(IsRedundant(&Link->Links[Link->NumLinks-1])) {AddError(EN_LINKER_ERROR2);return false;}

  if(!GetInclude(TmpInclude.str)) return false;
 }
 //Process all includes
 for(ENuint a=0;a<Link->NumLinks;a++)
   if(!PharseFile(&Link->Links[a])) return false;

 //Add name to list
 AddFile(Link->FName.str);
 //Finished
 return true;
}

ENbool ENScriptLinker::GetInclude(char *inc)
{ 
 while(parser->FindNext("include",true))
 {  
  if(parser->CountChars('"')%2==0)//Found include
  {
   //Jump over space
   parser->JumpOverSpace();
   //If end
   if(parser->PointerEnd()) {AddError(EN_LINKER_ERROR3);return false;}
   //Get string argument
   if(!parser->GetString(inc)) {AddError(EN_LINKER_ERROR3);return false;}
   
   return true;
  }
 } 

 inc[0]=0;
 return true;
}

ENbool ENScriptLinker::IsRedundant(ENLinks *link)
{
 char RefName[MAX_PATH];
 strcpy(RefName,link->FName.str);
 link=link->Parent;

 while(link)
 {
  if(strcmpi(link->FName.str,RefName)==0) return true;
  link=link->Parent;
 }

 return false;
}

void ENScriptLinker::AddFile(char *fname)
{
 //Check if name already exists
 for(ENuint a=0;a<NumFiles;a++)
   if(strcmpi(fname,Files[a].str)==0)
	 return;
 //Add new file name
 NumFiles++;
 Files=(ENTEXTSTR*)ENReallocMemory(Files,sizeof(ENTEXTSTR)*NumFiles);
 strcpy(Files[NumFiles-1].str,fname);
}

void ENScriptLinker::DeleteLinks(ENLinks *lnks)
{
 for(ENuint a=0;a<lnks->NumLinks;a++)
   DeleteLinks(&lnks->Links[a]);

 ENFreeMemory(lnks->Links);
 lnks->NumLinks=0;
 lnks->Links=NULL;
}

void ENScriptLinker::AddError(char *Error)
{
 char Buffer[80];
 sprintf(Buffer,"Linker Error: %s\n",Error);
 strcat(ErrorMsg,Buffer); 
}