//---------------------------------------------------------------------------
#include <vcl.h>
#include "Quake3Loader.h"
//---------------------------------------------------------------------------
Quake3Loader::Quake3Loader()
{
 NumVerts=0;
 NumFaces=0;
 NumTex=0;
 NumLightmaps=0;

 Verts=NULL;
 Faces=NULL;
 Texture=NULL;
 Lightmaps=NULL;
}

Quake3Loader::~Quake3Loader()
{
 Clear();
}

void Quake3Loader::Clear()
{
 ENFreeMemory(Verts);
 ENFreeMemory(Faces);
 ENFreeMemory(Texture);
 ENFreeMemory(Lightmaps);
 //Init values
 NumVerts=0;
 NumFaces=0;
 NumTex=0;
 NumLightmaps=0;

 Verts=NULL;
 Faces=NULL;
 Texture=NULL;
 Lightmaps=NULL;
}

ENbool Quake3Loader::Load(char *FileName)
{
 //Variables
 Quake3Header qheader={0};
 Quake3Lump   qlumps[kMaxLumps]={0};
 ENFile handle;
 //Open File
 handle=ENOpenFile(FileName,"rb");
 if(!handle)
 {
  ShowMessage("Cannot open file "+AnsiString(FileName)+" !!!");
  return false;
 }
 //Read header and lump data
 ENReadFile(&qheader,1,sizeof(Quake3Header),handle);
 ENReadFile(qlumps,kMaxLumps,sizeof(Quake3Lump),handle);
 //Check header
 if(!CheckHeader(&qheader))
 {
  ENCloseFile(handle);
  ShowMessage("Invalid file!!!");
  return false;
 }
 else
 Clear();
 //Alloc space
 AllocSpace(qlumps);
 //Read vertexes
 ReadVerts(qlumps,handle);
 //Read faces
 ENSeekFile(handle,qlumps[kFaces].offset,SEEK_SET);
 ENReadFile(Faces,NumFaces,sizeof(Quake3Face),handle);
 //Read textures
 ENSeekFile(handle,qlumps[kTextures].offset,SEEK_SET);
 ENReadFile(Texture,NumTex,sizeof(Quake3Texture),handle);
 for(ENuint a=0;a<NumTex;a++)
   CheckTexture(Texture+a);
 //Finished
 ENCloseFile(handle);
 return true;
}

void Quake3Loader::CheckTexture(Quake3Texture *tex)
{
 char *s=tex->strName;
 while((s=strchr(s,'/'))!=NULL)
   *s='\\';
}

ENuint Quake3Loader::GetNumVerts()
{
 return NumVerts;
}

ENuint Quake3Loader::GetNumFaces()
{
 return NumFaces;
}

ENuint Quake3Loader::GetNumTextures()
{
 return NumTex;
}

Quake3Vertex Quake3Loader::GetVertex(ENuint ind)
{
 return Verts[ind];
}

Quake3Face   Quake3Loader::GetFace(ENuint ind)
{
 return Faces[ind];
}

Quake3Texture Quake3Loader::GetTexture(ENuint ind)
{
 return Texture[ind];
}

ENbool Quake3Loader::CheckHeader(Quake3Header *qheader)
{
 //Variables
 char ref[5];
 //Check string ID
 memcpy(ref,qheader->strID,4);
 ref[4]=0;
 if(strcmp(ref,"IBSP")!=0) return false;
 //Check numeric ID
 if(qheader->version!=0x2e) return false;

 return true;
}

void Quake3Loader::AllocSpace(Quake3Lump *qlumps)
{
 //Vertexes
 NumVerts=qlumps[kVertices].length/sizeof(Quake3Vertex);
 Verts=(Quake3Vertex*)ENAllocMemory(sizeof(Quake3Vertex)*NumVerts);
 //Faces
 NumFaces=qlumps[kFaces].length/sizeof(Quake3Face);
 Faces=(Quake3Face*)ENAllocMemory(sizeof(Quake3Face)*NumFaces);
 //Textures
 NumTex=qlumps[kTextures].length/sizeof(Quake3Texture);
 Texture=(Quake3Texture*)ENAllocMemory(sizeof(Quake3Texture)*NumTex);
 //Lightmaps
 NumLightmaps=qlumps[kLightmaps].length/sizeof(Quake3Lightmap);
 Lightmaps=(Quake3Lightmap*)ENAllocMemory(sizeof(Quake3Lightmap)*NumLightmaps);
}

void Quake3Loader::ReadVerts(Quake3Lump *qlumps,ENFile handle)
{
 //Seek
 ENSeekFile(handle,qlumps[kVertices].offset,SEEK_SET);
 //Process and convert vertexes
 for(ENuint i=0;i<NumVerts;i++)
 {
  //Read vertex
  ENReadFile(&Verts[i],1,sizeof(Quake3Vertex),handle);
  //Swap the y and z values, and negate the new z so Y is up
  ENfloat temp= Verts[i].vPosition.v[0];
		Verts[i].vPosition.v[1] = Verts[i].vPosition.v[2];
		Verts[i].vPosition.v[2] = -temp;
  //Negate the V texture coordinate because it is upside down otherwise...
  Verts[i].vTextureCoord.v[1]*=-1.0f;
 }
}
