#include "EngineMedia.h"
#include "EngineSound.h"
///////////////////////////////////////////////////////////////////////
///Engine Packer Base class
///////////////////////////////////////////////////////////////////////
ENbool ENPackerBase::PackType(char *FileName,ENPackerBase::ENPackageType &type)
{
 //Preset result
 type=ENUNKNOWN;
 //Check if file exists
 ENFile handle=ENOpenFile(FileName,"rb");
 if(!handle) return false;
 ENCloseFile(handle);
 //Font
 if(ENFont::IsFont(FileName))
 {
  type=ENFONT;
  return true;
 }
 //Model
 if(ENModel::IsModel(FileName))
 {
  type=ENMODEL;
  return true;
 }
 //Sprite
 if(ENSprite::IsSprite(FileName))
 {
  type=ENSPRITE;
  return true;
 }
 //Picture
 if(ENPicture::IsPicture(FileName))
 {
  type=ENPICTURE;
  return true;
 }
 //Sound
 if(ENSound::IsSound(FileName))
 {
  type=ENSOUND;
  return true;
 }
 //Finished
 return true;
}

void ENPackerBase::GeneratePackerID(ENPackerID &id)
{
 randomize();
 for(ENuint a=0;a<10;a++)
    id.ID[a]=(ENuint)rand();
}

ENbool ENPackerBase::CompareIDs(ENPackerID *id1,ENPackerID *id2)
{
 for(ENuint a=0;a<10;a++)
    if(id1->ID[a]!=id2->ID[a])
      return false;

 return true;
}
///////////////////////////////////////////////////////////////////////
///Engine Packer class
///////////////////////////////////////////////////////////////////////
ENPacker::ENPacker()
{
 Packages=new ENArray(sizeof(ENPackage));
}

ENPacker::~ENPacker()
{
 Clear();
 delete Packages;
}

void ENPacker::Clear()
{
 //Delete package data
 for(ENuint a=0;a<Packages->GetNum();a++)
 {
  ENPackage pack;
  Packages->GetElement(a,(ENubyte*)&pack);
  ENFreeMemory(pack.data);
 }
 //Init values
 Packages->Clear();
}

ENint ENPacker::SearchPackage(char *Name)
{
 for(ENuint a=0;a<Packages->GetNum();a++)
 {
  ENPackage pack;
  Packages->GetElement(a,(ENubyte*)&pack);
   if(strcmpi(Name,pack.header.Name)==0)
     return a;
 }

 return -1;
}

ENbool ENPacker::AddPackage(char *FileName,char *Name)
{
 //Variables
 ENPackage pack;
 ENPicture *pic;
 ENSound   *snd;
 //Check if name is unique
 if(SearchPackage(Name)!=-1) return false;
 strcpy(pack.header.Name,Name);
 //Get type of file
 if(!PackType(FileName,pack.header.Type)) return false;
 //Get data
 switch(pack.header.Type)
 {
  case ENPICTURE:
    pic=new ENPicture();
    if(!pic->Load(FileName)) return false;
    pack.header.Size=pic->GetNumBytes();
    pack.data=(ENubyte*)ENAllocMemory(pack.header.Size);
    pic->GetBytes(pack.data);
    delete pic;
  break;
  case ENSOUND:
    snd=new ENSound();
    if(!snd->LoadWAV(FileName)) return false;
    pack.header.Size=snd->GetNumBytes();
    pack.data=(ENubyte*)ENAllocMemory(pack.header.Size);
    snd->GetBytes(pack.data);
    delete snd;
  break;
  default:
    if(!AddDataRaw(pack,FileName))
      return false;
 }
 //Get file position
 pack.header.Pos=PackagesSize();
 //Add package
 Packages->AddElement((ENubyte*)&pack);
 //Finished
 return true;
}

ENbool ENPacker::AddPackage(ENubyte *Data,ENuint size,
                            ENPackerBase::ENPackageType Type,char *Name)
{
 //Variables
 ENPackage pack;
 //Check if name is unique
 if(SearchPackage(Name)!=-1) return false;
 //Set package header
 strcpy(pack.header.Name,Name);
 pack.header.Type=Type;
 pack.header.Size=size;
 //Get file position
 pack.header.Pos=PackagesSize();
 //Set data
 pack.data=(ENubyte*)ENAllocMemory(pack.header.Size);
 memcpy(pack.data,Data,size);
 //Add package
 Packages->AddElement((ENubyte*)&pack);
 //Finished
 return true;
}

void ENPacker::DelPackage(ENuint ind)
{
 if(Packages->GetNum())
 {
  //Get package
  ENPackage pack;
  Packages->GetElement(ind,(ENubyte*)&pack);
  //Release Package data
  ENFreeMemory(pack.data);
  //Update other packages
  for(ENuint a=ind+1;a<Packages->GetNum();a++)
  {
   ENPackage p;
   Packages->GetElement(ind,(ENubyte*)&p);
   p.header.Pos-=pack.header.Size;
   Packages->SetElement(ind,(ENubyte*)&p);
  }
  //Delete package
  Packages->DelElement(ind);
 }
}

void ENPacker::DelPackage(char *Name)
{
 //Search package
 ENint ind=SearchPackage(Name);
 //Delete package
 if(ind>=0) DelPackage((ENuint)ind);
}

ENbool ENPacker::RenamePackage(ENuint ind,char *Name)
{
 //Check if name already exists
 if(SearchPackage(Name)>=0) return false;
 //Rename package
 if(Packages->GetNum())
 {
  ENPackage pack;
  Packages->GetElement(ind,(ENubyte*)&pack);
  strcpy(pack.header.Name,Name);
  Packages->SetElement(ind,(ENubyte*)&pack);
 }
 //Finished
 return true;
}

ENuint ENPacker::PackagesSize()
{
 ENuint res=0;
 //Calculate size
 for(ENuint a=0;a<Packages->GetNum();a++)
 {
  ENPackage pack;
  Packages->GetElement(a,(ENubyte*)&pack);
  res+=pack.header.Size;
 }
 //Finished
 return res;  
}

void ENPacker::GetPackageHeader(ENuint ind,ENPackageHeader &h)
{
 if(Packages->GetNum())
 {
  ENPackage pack;
  Packages->GetElement(ind,(ENubyte*)&pack);
  h=pack.header;
 }
}

void ENPacker::GetPackageData(ENuint ind,ENubyte *data)
{
 if(Packages->GetNum())
 {
  ENPackage pack;
  Packages->GetElement(ind,(ENubyte*)&pack);
  memcpy(data,pack.data,pack.header.Size);
 }
}

ENPackerBase::ENPackerHeader ENPacker::GetHeader()
{
 ENPackerHeader header;
 strcpy(header.PACKID,"EP01");
 header.NumPackages=Packages->GetNum();
 return header;
}

ENbool ENPacker::Save(char *FileName)
{
 //Variables
 ENFile handle;
 ENPackerID id;
 ENPackerHeader header;
 ENPackageHeader pheader;
 ENuint a;
 //Open file
 handle=ENOpenFile(FileName,"wb");
 if(!handle) return false;
 //Write ID
 GeneratePackerID(id);
 ENWriteFile(&id,sizeof(ENPackerID),1,handle);
 //Write header
 header=GetHeader();
 ENWriteFile(&header,sizeof(ENPackerHeader),1,handle);
 //Write package headers
 for(a=0;a<Packages->GetNum();a++)
 {
  GetPackageHeader(a,pheader);
  ENWriteFile(&pheader,sizeof(ENPackageHeader),1,handle);
 }
 //Write package data
 for(a=0;a<Packages->GetNum();a++)
 {
  GetPackageHeader(a,pheader);
  ENubyte *pdata=new ENubyte[pheader.Size];
  GetPackageData(a,pdata);
  ENWriteFile(pdata,1,pheader.Size,handle);
  delete[] pdata;
 }  
 //Finished
 ENCloseFile(handle);
 return true;   
}

ENbool ENPacker::Load(char *FileName)
{
 //Variables
 ENFile handle;
 ENPackerHeader h;
 ENPackageHeader *pheaders;
 ENuint a;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle) return false;
 //Seek over ID
 ENSeekFile(handle,sizeof(ENPackerID),SEEK_CUR);
 //Read header
 ENReadFile(&h,sizeof(ENPackerHeader),1,handle);
 if(strcmp(h.PACKID,"EP01")!=0)
 {
  ENCloseFile(handle);
  return false;
 }
 else
   Clear();
 //Read package headers
 pheaders=new ENPackageHeader[h.NumPackages];
 for(a=0;a<h.NumPackages;a++)
   ENReadFile(pheaders+a,sizeof(ENPackageHeader),1,handle);
 //Read package data
 for(a=0;a<h.NumPackages;a++)
 {
  ENPackage package;
  package.header=pheaders[a];
  package.data=(ENubyte*)ENAllocMemory(pheaders[a].Size);
  ENReadFile(package.data,1,pheaders[a].Size,handle);
  Packages->AddElement((ENubyte*)&package);
 }
 //Finished
 delete[] pheaders;
 ENCloseFile(handle);
 return true;   
}

ENbool ENPacker::AddDataRaw(ENPackage &pack,char *FileName)
{
 //Variables
 ENFile handle;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle) return false;
 //Get size of file
 pack.header.Size=ENFileSize(handle);
 //Read out data
 pack.data=(ENubyte*)ENAllocMemory(pack.header.Size);
 ENReadFile(pack.data,1,pack.header.Size,handle);
 //Finished
 ENCloseFile(handle);
 return true;
}
///////////////////////////////////////////////////////////////////////
///Engine Packer loaded
///////////////////////////////////////////////////////////////////////
ENPackerLoaded::ENPackerLoaded()
{
 //Init values
 Loaded=false;
 memset(&id,0,sizeof(ENPackerID));
 *PackageFile=0;
 NumPackages=0;
 Packages=NULL;
 DataStartPos=0;
}

ENPackerLoaded::~ENPackerLoaded()
{
 Clear();
}

void ENPackerLoaded::Clear()
{
 //Release package data
 for(ENuint a=0;a<NumPackages;a++)
   DeletePackage(Packages+a);
 //Release package headers
 ENFreeMemory(Packages);
 //Init values
 Loaded=false;
 memset(&id,0,sizeof(ENPackerID));
 *PackageFile=0;
 NumPackages=0;
 Packages=NULL;
 DataStartPos=0;
}

ENbool ENPackerLoaded::Load(char *FileName)
{
 //Variables
 ENFile handle;
 ENPackerID ident;
 ENPackerHeader h;
 ENuint a;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle) return false;
 //Read and check ID's
 ENReadFile(&ident,sizeof(ENPackerID),1,handle);
 if(ENPackerBase::CompareIDs(&id,&ident))
 {
  ENCloseFile(handle);
  return true;
 }
 else
   id=ident;
 //Read header
 ENReadFile(&h,sizeof(ENPackerHeader),1,handle);
 if(strcmp(h.PACKID,"EP01")!=0)
 {
  ENCloseFile(handle);
  return false;
 }
 else
 {
  Clear();
  NumPackages=h.NumPackages;
 }
 //Read package headers
 Packages=(ENPackageLoaded*)ENAllocMemory(sizeof(ENPackageLoaded)*NumPackages);
 for(a=0;a<NumPackages;a++)
 {
  ENReadFile(&Packages[a].header,sizeof(ENPackageHeader),1,handle);
  Packages[a].Loaded=false;
  if(!a)
    Packages[a].header.Pos=0;
  else
    Packages[a].header.Pos=Packages[a-1].header.Pos+Packages[a-1].header.Size;
 }
 //Set data
 DataStartPos=ENTellFile(handle);
 Loaded=true;
 strcpy(PackageFile,FileName);
 //Finished
 ENCloseFile(handle);
 return true;
}

ENbool ENPackerLoaded::LoadAll()
{
 ENbool res=true;
 for(ENuint a=0;a<NumPackages;a++)
  if(!LoadPackage(Packages+a))
    res=false;

 return res;    
}

void *ENPackerLoaded::GetSource(char *Name,ENPackageType type)
{
 ENint ind=SearchPackage(Name);
 if(ind>=0)
 {
  ENPackageLoaded *package=&Packages[ind];
  if(package->header.Type!=type) return NULL;
    if(!package->Loaded)
      if(!LoadPackage(package))
        return NULL;

  return package->Data;
 }
 else
   return NULL;
}

ENPackerBase::ENPackageType ENPackerLoaded::GetType(ENuint ind)
{
 if(NumPackages)
   return Packages[ind%NumPackages].header.Type;
 else
   return ENUNKNOWN;
}

ENint ENPackerLoaded::SearchPackage(char *Name)
{
 for(ENuint a=0;a<NumPackages;a++)
  if(strcmpi(Name,Packages[a].header.Name)==0)
    return (ENint)a;

 return -1;
}

ENuint ENPackerLoaded::GetNum()
{
 return NumPackages;
}

void ENPackerLoaded::GetName(ENuint ind,char *Name)
{
 if(NumPackages)
 {
  ind%=NumPackages;
  strcpy(Name,Packages[ind].header.Name);
 }
}

void ENPackerLoaded::DeletePackage(ENPackageLoaded *package)
{
 if(package->Loaded)
   switch(package->header.Type)
   {
    case ENPICTURE:
      delete (ENPictureLoaded*)package->Data;
    break;
    case ENFONT:
      delete (ENFontLoaded*)package->Data;
    break;
    case ENMODEL:
      delete (ENModelLoaded*)package->Data;
    break;
    case ENSPRITE:
      delete (ENSpriteLoaded*)package->Data;
    break;
    case ENSOUND:
      delete (ENSoundLoaded*)package->Data;
    break;
 }
}

ENbool ENPackerLoaded::LoadPackage(ENPackageLoaded *package)
{
 //Variables
 ENPicture *pic;
 ENSound   *snd;
 ENFont    *font;
 ENSprite  *sprite;
 ENModel   *model;

 //Load package
 if(!package->Loaded)
 {
  ENFile handle=PrepareToLoadPackage(package);
  if(!handle) return false;
  switch(package->header.Type)
  {
   case ENPICTURE:
     pic=new ENPicture();
     if(!pic->LoadFromHandle(handle)) return false;
     pic->Power2();
     package->Data=new ENPictureLoaded(pic);
     delete pic;
   break;
   case ENSOUND:
     snd=new ENSound();
     if(!snd->LoadFromHandle(handle)) return false;
     package->Data=new ENSoundLoaded(snd);
     delete snd;
   break;
   case ENFONT:
     font=new ENFont();
     if(!font->LoadFromHandle(handle)) return false;
     package->Data=new ENFontLoaded(font);
     delete font;
   break;
   case ENMODEL:
     model=new ENModel();
     if(!model->LoadFromHandle(handle)) return false;
     package->Data=new ENModelLoaded(model);
     delete model;
   break;
   case ENSPRITE:
     sprite=new ENSprite();
     if(!sprite->LoadFromHandle(handle)) return false;
     package->Data=new ENSpriteLoaded(sprite);
     delete sprite;
   break;
  }
  ENCloseFile(handle);
  package->Loaded=true;
 }
 return true;
}

ENFile ENPackerLoaded::PrepareToLoadPackage(ENPackageLoaded *package)
{
 if(!package->Loaded)
 {
  ENFile handle;
  //Open package file
  handle=ENOpenFile(PackageFile,"rb");
  if(!handle) return NULL;
  //Seek
  ENSeekFile(handle,DataStartPos,SEEK_SET);
  ENSeekFile(handle,package->header.Pos,SEEK_CUR);
  //Finished
  return handle;
 }
 else
   return NULL;
}
