#include <iostream.h>
#include "ENScriptParser.h"

const char _LEGAL_IDENTIFIER[]="abcdefghijklmnopqrstuvwxyz_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.";
const char ENScriptParser::OPERATORS[]="+-*/=!|&<>%";

ENScriptParser::ENScriptParser()
{ 
 this->Source=NULL; 
 Pointer=NULL;
 this->TextLength=0;
 curFileName[0]=0;
}

ENScriptParser::~ENScriptParser()
{
 ENFreeMemory(Source);
}

ENbool ENScriptParser::SetSource(char *Src)
{
 curFileName[0]=0;
 TextLength=strlen(Src);
 if(!DeleteComments(Src)) return false;
 Pointer=Source;

 return true;
}

ENbool ENScriptParser::LoadSource(char *FileName)
{
 //VARS
 ENFile handle;
 char   *Src; 
 //Open File
 handle=ENOpenFile(FileName,"rb");
 if(!handle) return false;
 //Save current file name
 strcpy(curFileName,FileName);
 //Alloc space
 TextLength=ENFileSize(handle);
 Src=(char*)ENAllocMemory(TextLength+1);
 //Read out text
 ENReadFile(Src,1,TextLength,handle);
 Src[TextLength]=0;
 //Close file
 ENCloseFile(handle);
 //Delete Comments
 if(!DeleteComments(Src)) return false;
 Pointer=Source; 
 //Finished
 ENFreeMemory(Src); 
 return true;
}

ENbool ENScriptParser::DeleteComments(char *Src)
{
 //Vars
 ENuint a,cc=0,len2=0,b; 
 //Check number of comment signs
 for(a=0;a<TextLength;a++)
   if(Src[a]=='#')
     cc++;
   else
     if(cc%2==0||Src[a]==_RETURN_)
       len2++;

 if(cc%2!=0) return false;
 //Now realloc space for new source
 Source=(char*)ENReallocMemory(Source,sizeof(char)*(len2+1)); 
 //Copy text without comments
 cc=0; b=0;
 for(a=0;a<TextLength;a++)
 {
  if(Src[a]=='#')
    cc++;
  else
    if(cc%2==0||Src[a]==_RETURN_)
    {
     Source[b]=Src[a];
     b++;
    }
 }
 //Set result
 Source[len2]=0;
 TextLength=len2;
 return true;
}

void ENScriptParser::ResetPointer()
{
 Pointer=Source;
}

ENuint ENScriptParser::CountChars(char Chr)
{
 char *tmp=Source;
 ENuint res=0;
 while(tmp<Pointer)
 {
  if(*tmp==Chr) res++;
  tmp++;
 }

 return res;
}

ENbool ENScriptParser::JumpOverCommand()
{ 
 char *res=strchr(Pointer,';');
 if(!res) return false;
 Pointer=res+1;
 return true;
}

ENbool ENScriptParser::GetString(char *str)
{
 //Vars
 ENuint len;
 char *start;
 char *er[2];
 char *end;
 //Check for beginning
 if(*Pointer!='"') return false;
 Pointer++;
 start=Pointer;
 //Get return's
 er[0]=strchr(Pointer,_RETURN_);
 er[1]=strchr(Pointer,_RETURN2_);
 //Get end
 end=strchr(Pointer,'"');
 if(!end) return false;
 //Check return's with end
 if(er[0]&&er[0]<end) return false;
 if(er[1]&&er[1]<end) return false;
 //Copy string
 if(str)
 {
  len=end-start;
  memcpy(str,start,len);
  str[len]=0;
 }
 //Finished
 Pointer=end+1;
 return true;
}

void ENScriptParser::JumpOverSpace()
{
 while(*Pointer==_SPACE_||*Pointer==_RETURN_||*Pointer==_RETURN2_)
   Pointer++;
}

ENbool ENScriptParser::GetIdentifier(char *res)
{  
 for(ENuint a=0;a<EN_MAX_NAME_LENGTH;a++)
   if(!strchr(_LEGAL_IDENTIFIER,Pointer[a]))//Found an charakter which isn't an identifier
   {    
	res[a]=0;
	Pointer+=a;
	return true;
   }
   else
	res[a]=Pointer[a];

 //Name is too long
 return false;
}
   

ENbool ENScriptParser::GetText(char *res)
{  
 for(ENuint a=0;a<EN_MAX_NAME_LENGTH;a++)
   if(Pointer[a]==_SPACE_||Pointer[a]==_RETURN_||Pointer[a]==_RETURN2_||
	  Pointer[a]==0)//Found an charakter which isn't an identifier
   {    
	res[a]=0;
	Pointer+=a;
	return true;
   }
   else
	res[a]=Pointer[a];

 //Name is too long
 return false;
}

ENbool ENScriptParser::GetExpression(ENScriptRange &range)
{
 //Vars
 ENuint num=1; 
 //Set start
 range.start=Pointer;
 //Get expression
 while(num>0)
 {
  if(*Pointer=='(') num++;
  if(*Pointer==')') num--;
  if(*Pointer==0) return false;    
  Pointer++;
 }
 //End
 range.end=Pointer-1;

 return true;
}

ENbool ENScriptParser::GetOperator(char *Op)
{
 //VARS 
 ENuint a=0;
 //GO
 while(strchr(OPERATORS,*Pointer))
 {
  Op[a]=*Pointer;
  Pointer++;
  a++;
  if(a>EN_MAX_OPERATOR_LENGTH) 
    return false;
 }

 Op[a]=0;

 return true;
}

ENbool ENScriptParser::ValidName(char *Name,char *ExtCh)
{
 //VARS
 ENuint a,len=strlen(Name);
 //Check length
 if(!len) return false;
 //Check first charakter
 if(*Name>=0&&*Name<=9) return false;
 //Check charakters
 for(a=0;a<len;a++)
   if(!strchr(_LEGAL_IDENTIFIER,Name[a]))
   {
	if(!ExtCh) return false;
	if(!strchr(ExtCh,Name[a])) return false;
   }

 return true;
}

ENbool ENScriptParser::FindNext(char *Name,ENbool word)
{
 char name2[EN_MAX_NAME_LENGTH+1];
 char *res;
 if(!word)
 {
  res=strstr(Pointer,Name);
  if(!res) return false;
  Pointer=res+strlen(Name);
  return true;
 }
 else
 {
  while(!PointerEnd())
  {
   JumpOverSpace();
   if(GetText(name2))
	 if(strcmpi(name2,Name)==0)
 	   return true;
  }
  return false;
 }
}

char ENScriptParser::GetCurrentChar()
{
 if(!Pointer)
   return 0;
 else
   return *Pointer;
}

void ENScriptParser::IncPointer(ENint num)
{
 if(Pointer)
 {
  Pointer+=num; 
  if(Pointer<Source) Pointer=Source;
  if(Pointer>Source+TextLength) Pointer=Source+TextLength;
 }
}

ENbool ENScriptParser::GetBody(ENScriptRange &range)
{
 //Vars
 ENuint numInst;
 //Check for first bracket
 if(*Pointer!='{') 
   return false;
 else
   Pointer++;
 range.start=Pointer;
 numInst=1;
 //Find the end 
 while(*Pointer)
 {
  if(*Pointer=='{') numInst++;
  if(*Pointer=='}') numInst--;
  if(!numInst)
  {
   range.end=Pointer;
   Pointer++;
   return true;
  }
  Pointer++;
 }
 //No end where found
 return false;
}

ENbool ENScriptParser::PointerEnd()
{ 
 return ((Source+TextLength)<=Pointer);
}

void ENScriptParser::GetOperatorString(char *str)
{
 strcpy(str,OPERATORS);
}
