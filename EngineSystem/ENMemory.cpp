#include "EngineSystem.h"
#include <stdio.h>

void* ENAllocMemory(ENuint size)
{
 return malloc(size);
}

void *ENReallocMemory(void *addr,ENuint size)
{
 return realloc(addr,size);
}

void ENFreeMemory(void *addr)
{
 free(addr);
}

ENFile ENOpenFile(char *filename,char *mode)
{
 return fopen(filename,mode); 
}

ENFile ENOpenFile(const char *filename, const char *mode)
{
 return fopen(filename,mode);
}

ENint ENSeekFile(ENFile stream,ENint offset,ENint whence)
{
 return fseek(stream,offset,whence);
}

ENint  ENTellFile(ENFile stream)
{
 return ftell(stream);
}

ENuint ENReadFile(void *ptr,ENuint size,ENuint n,ENFile stream)
{
 return fread(ptr,size,n,stream);
}

ENuint ENWriteFile(void *ptr,ENuint size,ENuint n,ENFile stream)
{
 return fwrite(ptr,size,n,stream);
}

ENint ENCloseFile(ENFile stream)
{
 return fclose(stream);
}

ENubyte *ENAddElement(ENubyte *ptr,ENuint esize,ENuint &num,ENubyte *element)
{
 num++;
 ptr=(ENubyte*)ENReallocMemory(ptr,esize*num);
 memcpy(ptr+(num-1)*esize,element,esize);
 return ptr;
}

ENubyte *ENAddElements(ENubyte *ptr,ENuint esize,ENuint &num,ENubyte *elements,
                       ENuint numelements)
{
 num+=numelements;
 ptr=(ENubyte*)ENReallocMemory(ptr,esize*num);
 memcpy(ptr+(num-numelements)*esize,elements,esize*numelements);
 return ptr;
}

ENubyte *ENDelElement(ENubyte *ptr,ENuint esize,ENuint &num,ENuint ind)
{
 memmove(ptr+esize*ind,ptr+esize*(ind+1),(num-ind-1)*esize);
 num--;
 ptr=(ENubyte*)ENReallocMemory(ptr,esize*num);
 return ptr;
}

ENint ENFileSize(ENFile handle)
{
 int sb,res;
 sb=ENTellFile(handle);
 ENSeekFile(handle,0,SEEK_END);
 res=ENTellFile(handle);
 ENSeekFile(handle,sb,SEEK_SET);
 return res;
}

ENbool ENFileExists(char *FileName)
{
 OFSTRUCT of;
 return OpenFile(FileName,&of,OF_EXIST | OF_READ)!=-1;
}

void ENRemoveFileName(char *fn)
{
 ENint temp=-1,len;
 len=strlen(fn);
 for(ENint a=0;a<len;a++)
   if(fn[a]=='\\'||fn[a]=='/') temp=a;

 if(temp!=-1) fn[temp+1]=0;
}

void ENRemoveFilePath(char *fn)
{
 ENint temp=-1,len;
 len=strlen(fn);
 for(ENint a=0;a<len;a++)
   if(fn[a]=='\\'||fn[a]=='/') temp=a;

 if(temp!=-1) 
   strcpy(fn,fn+temp+1);
}

void ENReadStream(ENubyte *stream,ENuint &_c,void *dst,ENuint size,ENuint num)
{
 memcpy(dst,stream+_c,size*num);
 _c+=size*num;
}
////////////////////////////////////////////////////
/// Engine Class ENArrayBasic
////////////////////////////////////////////////////
ENArrayBasic::ENArrayBasic(ENuint ElementSize,ENuint allocp,ENuint freep)
{
 this->ElementSize=ElementSize;
 Data=NULL;
 AllocStoragePack=allocp;
 FreeStoragePack=freep;
 NumElements=0;
 NumStorage=0;
}

ENArrayBasic::~ENArrayBasic()
{
 Clear();
}

void ENArrayBasic::Clear()
{
 ENFreeMemory(Data);
 Data=NULL;
 NumElements=0;
 NumStorage=0;
}

void ENArrayBasic::AddElements(ENubyte *elements,ENuint num)
{
 //Add number of new elements
 NumElements+=num;
 //Check if we have enough space
 if(NumElements>NumStorage)
 {
  NumStorage=NumElements/AllocStoragePack+1;
  NumStorage*=AllocStoragePack;
  Data=(ENubyte*)ENReallocMemory(Data,NumStorage*ElementSize);
 }
 //Add new elements
 memcpy(Data+(NumElements-num)*ElementSize,elements,ElementSize*num);
}

void ENArrayBasic::AddElement(ENubyte *element)
{
 AddElements(element,1);
}

ENbool ENArrayBasic::DelElement(ENuint ind)
{
 if(NumElements)
 {
  //Be sure for access violation
  ind%=NumElements;
  //Delete element
  memmove(Data+ElementSize*ind,Data+ElementSize*(ind+1),
         (NumElements-ind-1)*ElementSize);
  NumElements--;
  //Compare storage and number of elements
  if(NumStorage>NumElements+FreeStoragePack)
  {
   NumStorage=NumElements/AllocStoragePack+1;
   NumStorage*=AllocStoragePack;
   Data=(ENubyte*)ENReallocMemory(Data,NumStorage*ElementSize);
  }
  return true;
 }
 else
   return false;
}

void   ENArrayBasic::LoadFromHandle(ENFile handle)
{
 //Variables
 ENuint num;
 //Read
 ENReadFile(&num,sizeof(ENuint),1,handle);
 //Alloc space
 ENubyte *Buffer=new ENubyte[num*ElementSize];
 //Read data
 ENReadFile(Buffer,ElementSize,num,handle);
 //Add data
 AddElements(Buffer,num);
 //Release buffer
 delete Buffer;
}

void   ENArrayBasic::SaveToHandle(ENFile handle)
{
 ENWriteFile(&NumElements,sizeof(ENuint),1,handle);
 ENWriteFile(Data,ElementSize,NumElements,handle);
}

void   ENArrayBasic::SaveToHandleRaw(ENFile handle)
{
 ENWriteFile(Data,ElementSize,NumElements,handle);
}

ENuint ENArrayBasic::GetNum()
{
 return NumElements;
}

ENuint ENArrayBasic::GetElementSize()
{
 return ElementSize;
}

ENbool ENArrayBasic::GetElement(ENuint ind,ENubyte *element)
{
 if(NumElements)
 {
  ind%=NumElements;
  memcpy(element,Data+ElementSize*ind,ElementSize);
  return true;
 }
 else
   return false;
}

void ENArrayBasic::GetElements(ENubyte *elements)
{
 memcpy(elements,Data,NumElements*ElementSize);
}

ENbool ENArrayBasic::SetElement(ENuint ind,ENubyte *element)
{
 if(NumElements)
 {
  ind%=NumElements;
  memcpy(Data+ElementSize*ind,element,ElementSize);
  return true;
 }
 else
   return false;
}
////////////////////////////////////////////////////
/// Engine Class ENArray
////////////////////////////////////////////////////
ENArray::ENArray(ENuint ElementSize)
       : ENArrayBasic(ElementSize,24,32)
{
}

ENArray::ENArray(ENArray *arrayobj)
       : ENArrayBasic(0,24,32)
{
 ElementSize=arrayobj->GetElementSize();
 NumElements=arrayobj->GetNum();
 NumStorage=NumElements;
 Data=(ENubyte*)ENAllocMemory(ElementSize*NumStorage);
 arrayobj->GetElements(Data);
}
////////////////////////////////////////////////////
/// Engine Class ENBooleanArray
////////////////////////////////////////////////////

ENBooleanArray::ENBooleanArray()
              : ENArray(sizeof(ENubyte))
{
}

void ENBooleanArray::SetElement(ENubyte stat,ENuint index)
{
 if(NumElements)
   Data[index%NumElements]=stat;
}

void ENBooleanArray::SetElements(ENubyte stat,ENuint ind1,ENuint ind2)
{
 if(ind1<NumElements&&ind2<NumElements&&ind1<=ind2)
   memset(Data,stat,ind2-ind1+1);
}

void ENBooleanArray::AddElement(ENubyte stat)
{
 ENArray::AddElement(&stat);
}

void ENBooleanArray::AddElements(ENubyte stat,ENuint num)
{
 ENubyte *data=new ENubyte[num];
 memset(data,stat,num);
 ENArray::AddElements(data,num);
 delete[] data;
}

ENubyte ENBooleanArray::GetElement(ENuint ind)
{
 return Data[ind];
}
//////////////////////////////////////////////////////////////////////////
//// Engine Class memory stream
//////////////////////////////////////////////////////////////////////////
ENMemoryStream::ENMemoryStream(ENubyte *addr)
{
 this->addr=addr;
 pos=0;
}

void ENMemoryStream::Read(void *dst,ENuint size)
{
 memcpy(dst,addr+pos,size);
 pos+=size;
}

void ENMemoryStream::Seek(ENint diff)
{
 pos+=diff;
}

