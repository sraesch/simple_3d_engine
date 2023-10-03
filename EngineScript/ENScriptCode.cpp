#include "ENScriptCode.h"

ENScriptCode::ENScriptCode()
{
 _ENConst=NULL;
 Paths=NULL;
 Texts=NULL;
 Panels=NULL;
 Actions=NULL; 
 Commands=NULL;
 Package[0]=0;
 _ENSizeConst=0;
 NumPaths=0;
 MainFunc=-1;
 LoadFunc=-1;
 NumTexts=0;
 NumPanels=0;
 NumActions=0;
 NumCommands=0;
 _ENStackPointer=0;
}

ENScriptCode::~ENScriptCode()
{
 ClearAll();
}

void ENScriptCode::ClearAll()
{
 ENFreeMemory(_ENConst);
 ENFreeMemory(Paths);
 ENFreeMemory(Texts);
 ENFreeMemory(Panels);
 ENFreeMemory(Actions);
 ENFreeMemory(Commands);
 _ENConst=NULL;
 Paths=NULL;
 Texts=NULL;
 Panels=NULL;
 Actions=NULL; 
 Commands=NULL;
 Package[0]=0;
 _ENSizeConst=0;
 NumPaths=0;
 MainFunc=-1;
 LoadFunc=-1;
 NumTexts=0;
 NumPanels=0;
 NumActions=0;
 NumCommands=0;
 _ENStackPointer=0;
}

ENbool ENScriptCode::LoadFromFile(char *FileName)
{
 //Vars
 FILE *handle;
 ENuint a;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(handle==NULL)
   return false;
 //Clear all
 ClearAll();
 //-------Read constant values
 ENReadFile(&_ENSizeConst,sizeof(ENuint),1,handle);
 _ENConst=(ENubyte*)ENAllocMemory(_ENSizeConst);
 ENReadFile(_ENConst,sizeof(ENubyte),_ENSizeConst,handle);
 //-------Read path's
 ENReadFile(&NumPaths,sizeof(ENuint),1,handle);  
 Paths=(ENPATH*)ENAllocMemory(sizeof(ENPATH)*NumPaths);
 ENReadFile(Paths,sizeof(ENPATH),NumPaths,handle);
 //-------Read jumps
 ENReadFile(&MainFunc,sizeof(ENint),1,handle);
 ENReadFile(&LoadFunc,sizeof(ENint),1,handle);
 //-------Read text's
 ENReadFile(&NumTexts,sizeof(ENuint),1,handle);
 Texts=(ENint*)ENAllocMemory(sizeof(ENint)*NumTexts);
 ENReadFile(Texts,sizeof(ENint),NumTexts,handle);
 for(a=0;a<NumTexts;a++)
   Texts[a]=(ENint)(ENStack+Texts[a]);
 //-------Read panel's
 ENReadFile(&NumPanels,sizeof(ENuint),1,handle);
 Panels=(ENint*)ENAllocMemory(sizeof(ENint)*NumPanels);
 ENReadFile(Panels,sizeof(ENint),NumPanels,handle);
 for(a=0;a<NumPanels;a++)
   Panels[a]=(ENint)(ENStack+Panels[a]);
 //-------Read action's
 ENReadFile(&NumActions,sizeof(ENuint),1,handle);
 Actions=(ENACTIONREC*)ENAllocMemory(sizeof(ENACTIONREC)*NumActions);
 ENReadFile(Actions,sizeof(ENACTIONREC),NumActions,handle);
 //-------Read package
 ENReadFile(Package,sizeof(char),256,handle);
 //-------Read main part
 ENReadFile(&NumCommands,sizeof(ENuint),1,handle);
 Commands=(ENSCRIPTCOMMAND*)ENAllocMemory
	          (sizeof(ENSCRIPTCOMMAND)*NumCommands);
 ENReadFile(Commands,sizeof(ENSCRIPTCOMMAND),
	         NumCommands,handle);
 //Close file
 ENCloseFile(handle);
 return true;
}

ENbool ENScriptCode::SaveToFile(char *FileName)
{
 //Vars
 ENFile handle;
 //Open file
 handle=ENOpenFile(FileName,"wb");
 if(handle==NULL) return false;
 //-------Write constant values
 ENWriteFile(&_ENSizeConst,sizeof(ENuint),1,handle); 
 ENWriteFile(_ENConst,sizeof(ENubyte),_ENSizeConst,handle);
 //-------Write path's
 ENWriteFile(&NumPaths,sizeof(ENuint),1,handle);  
 ENWriteFile(Paths,sizeof(ENPATH),NumPaths,handle);
 //-------Write jumps
 ENWriteFile(&MainFunc,sizeof(ENint),1,handle);
 ENWriteFile(&LoadFunc,sizeof(ENint),1,handle);
 //-------Write text's
 ENWriteFile(&NumTexts,sizeof(ENuint),1,handle);  
 ENWriteFile(Texts,sizeof(ENint),NumTexts,handle);
 //-------Write panel's
 ENWriteFile(&NumPanels,sizeof(ENuint),1,handle);  
 ENWriteFile(Panels,sizeof(ENint),NumPanels,handle);
 //-------Write action's
 ENWriteFile(&NumActions,sizeof(ENuint),1,handle);
 ENWriteFile(Actions,sizeof(ENACTIONREC),NumActions,handle);
 //-------Read package
 ENWriteFile(Package,sizeof(char),256,handle);
 //-------Write code
 ENWriteFile(&NumCommands,sizeof(ENuint),1,handle);
 ENWriteFile(Commands,sizeof(ENSCRIPTCOMMAND),
	         NumCommands,handle);
 //Close file
 ENCloseFile(handle);
 return true;
}

void ENScriptCode::AddText(ENint txt)
{
 NumTexts++;
 Texts=(ENint*)ENReallocMemory(Texts,sizeof(ENint)*NumTexts);
 Texts[NumTexts-1]=txt;
}

ENuint ENScriptCode::AddConst(ENubyte *cons,ENuint size)
{
 ENuint res=_ENSizeConst;
 _ENSizeConst+=size; 
 _ENConst=(ENubyte*)ENReallocMemory(_ENConst,_ENSizeConst);
 memcpy(_ENConst+_ENSizeConst-size,cons,size);
 return res;
}

void ENScriptCode::AddPath(ENPATH path)
{
 NumPaths++;
 Paths=(ENPATH*)ENReallocMemory(Paths,sizeof(ENPATH)*NumPaths);
 Paths[NumPaths-1]=path;
}

void ENScriptCode::SetMainFunc(ENint pos)
{
 MainFunc=pos;
}

void ENScriptCode::SetLoadFunc(ENint pos)
{
 LoadFunc=pos;
}

void ENScriptCode::SetPackage(char *fname)
{
 strcpy(Package,fname);
}

void ENScriptCode::AddPanel(ENint panel)
{
 NumPanels++;
 Panels=(ENint*)ENReallocMemory(Panels,sizeof(ENint)*NumPanels);
 Panels[NumPanels-1]=panel;
}

void ENScriptCode::AddAction(ENACTIONREC action)
{
 NumActions++;
 Actions=(ENACTIONREC*)ENReallocMemory(Actions,sizeof(ENACTIONREC)*NumActions);
 Actions[NumActions-1]=action;
}

void ENScriptCode::AddCommand(ENSCRIPTCOMID id,ENint a,ENint b)
{ 
 ENSCRIPTCOMMAND com;
 //Init command
 com.a=a;
 com.b=b;
 com.comid=id;
 //Add command 
 NumCommands++; 
 Commands=(ENSCRIPTCOMMAND*)ENReallocMemory(Commands,sizeof(ENSCRIPTCOMMAND)*NumCommands); 
 Commands[NumCommands-1]=com;
}

void ENScriptCode::SetCommandParamA(ENint ind,ENint a)
{
 Commands[ind].a=a;
}

void ENScriptCode::SetStackPointer(ENint pos)
{
 _ENStackPointer=pos;
}

void ENScriptCode::GetPackage(char *dst)
{
 strcpy(dst,Package);
}

ENuint ENScriptCode::GetNumCommands()
{
 return NumCommands;
}

ENuint ENScriptCode::GetNumPaths()
{
 return NumPaths;
}

ENuint ENScriptCode::GetNumTexts()
{
 return NumTexts;
}

ENuint ENScriptCode::GetNumPanels()
{
 return NumPanels;
}

ENuint ENScriptCode::GetTextAddr(ENuint ind)
{ 
 return Texts[ind];
}

ENuint ENScriptCode::GetPanelAddr(ENuint ind)
{
 return Panels[ind];
}

ENint ENScriptCode::GetActionPos(char *name)
{
 for(ENuint a=0;a<NumActions;a++)
   if(strcmpi(Actions[a].Name,name)==0)
	 return Actions[a].Pos;

 return -1;
}

void ENScriptCode::GetPath(ENuint ind,ENPATH &path)
{
 if(NumPaths)
   strcpy(path.Path,Paths[ind%NumPaths].Path);
 else
   path.Path[0]=0;
}

ENuint ENScriptCode::GetStackPointer()
{
 return _ENStackPointer;
}

ENint ENScriptCode::GetStack()
{
 return (ENint)ENStack;
}

ENScriptCode::ENSCRIPTCOMMAND ENScriptCode::GetCommand(ENuint ind)
{
 return Commands[ind];
}

ENint ENScriptCode::GetConstAddr()
{
 return (ENint)_ENConst;
}

ENint ENScriptCode::GetNumActions()
{
 return NumActions;
}

ENTEXTSTR ENScriptCode::GetActionName(ENuint ind)
{
 ENTEXTSTR res;
 strcpy(res.str,Actions[ind].Name);
 return res;
}

ENint ENScriptCode::GetMainFunc()
{
 return MainFunc;
}

ENint ENScriptCode::GetLoadFunc()
{
 return LoadFunc;
}