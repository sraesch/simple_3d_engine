//---------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "EngineMedia.h"

//---------------------------------------------------------------------------
ENASCIIMesh::ENASCIIMesh()
{
 NumFaces=0;
 NumVertexes=0;
 Faces=NULL;
 Vertexes=0;
}

ENASCIIMesh::~ENASCIIMesh()
{
 ENFreeMemory(Faces);
 ENFreeMemory(Vertexes);
}

void ENASCIIMesh::Clear()
{
 ENFreeMemory(Faces);
 ENFreeMemory(Vertexes);
 NumFaces=0;
 NumVertexes=0;
 Faces=NULL;
 Vertexes=0;
}

ENbool ENASCIIMesh::Write(char *FileName)
{
 //Variables
 ENFile handle;
 char Buffer[256];
 //Open file
 handle=ENOpenFile(FileName,"wb");
 if(handle==NULL)
   return false;
 //Write Header
 sprintf(Buffer,"ASCII MESH \n"); WriteText(handle,Buffer);
 sprintf(Buffer,"VERTEXES:%i \n", NumVertexes); WriteText(handle,Buffer);
 sprintf(Buffer,"TRIANGLES:%i \n", NumFaces); WriteText(handle,Buffer);
 //Write Vertexes
 for(ENuint a=0;a<NumVertexes;a++)
 {
  sprintf(Buffer,"VERTEX:%f|%f|%f|\n",Vertexes[a].v[0],Vertexes[a].v[1],Vertexes[a].v[2]);
  WriteText(handle,Buffer);
 }
 //Write Faces
 for(ENuint b=0;b<NumFaces;b++)
 {
  sprintf(Buffer,"TRIANGLE:%i|%i|%i|\n",Faces[b].Indices[0],Faces[b].Indices[1],
          Faces[b].Indices[2]);
  WriteText(handle,Buffer);
 }

 ENCloseFile(handle);
 return true;
}

ENbool ENASCIIMesh::Load(char *FileName)
{
 //Vars
 ENFile handle;
 char *Buffer,*Back;
 ENuint len;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(handle==NULL)
   return false;
 //Read File into buffer
 len=ENFileSize(handle);
 Buffer=(char*)ENAllocMemory(len*sizeof(char)+1);
 Back=Buffer;
 ENReadFile(Buffer,sizeof(char),len,handle);
 ENCloseFile(handle);
 Buffer[len]=0;
 //----Read number of vertexes----
 Buffer=strstr(Buffer,"VERTEXES:");
 if(Buffer==NULL) return false;
 Buffer+=9;
 NumVertexes=ExtractInteger(Buffer);
 //----Read number of triangles----
 Buffer=strstr(Buffer,"TRIANGLES:");
 if(Buffer==NULL) return false;
 Buffer+=10;
 NumFaces=ExtractInteger(Buffer);
 //----Alloc space----
 Faces=(ENMeshFace*)ENAllocMemory(sizeof(ENMeshFace)*NumFaces);
 Vertexes=(ENVector*)ENAllocMemory(sizeof(ENVector)*NumVertexes);
 //----Extract vertexes----
 for(ENuint a=0;a<NumVertexes;a++)
 {
  Buffer=strstr(Buffer,"VERTEX:");
  if(Buffer==NULL) return false;
  Buffer+=7;
  Buffer=ExtractVertex(Buffer,Vertexes[a]);
 }
 //----Extract triangles----
 for(ENuint b=0;b<NumFaces;b++)
 {
  Buffer=strstr(Buffer,"TRIANGLE:");
  if(Buffer==NULL) return false;
  Buffer+=9;
  Buffer=ExtractTriangle(Buffer,Faces[b]);
 }
 //Finished
 ENFreeMemory(Back);

 return true;
}

void ENASCIIMesh::WriteText(ENFile handle,char *text)
{
 ENWriteFile(text,1,strlen(text),handle);
}

ENint ENASCIIMesh::ExtractInteger(char *text)
{
 char t;
 ENint p=-1;
 size_t len=strlen(text);
 int res;

 for(ENuint a=0;a<len;a++)
   if(text[a]<'0'||text[a]>'9')
   {
    p=a;
    t=text[a];
    text[p]=0;
    break;
   }

 res=atoi(text);
 if(p!=-1)
  text[p]=t;

 return res;
}

char *ENASCIIMesh::ExtractVertex(char *text,ENVector &Vertex)
{
 char t;
 ENuint a;
 ENint p=-1;
 size_t len=strlen(text);
 //Extract X
 for(a=0;a<len;a++)
   if((text[a]<'0'||text[a]>'9')&&(text[a]!='.')&(text[a]!='-'))
   {
    p=a;
    t=text[a];
    text[p]=0;
    break;
   }
 Vertex.v[0]=(ENfloat)atof(text);
 if(p!=-1)
  text[p]=t;
 //Extract Y
 p=-1;
 text=strstr(text,"|")+1;
 for(a=0;a<len;a++)
   if((text[a]<'0'||text[a]>'9')&&(text[a]!='.')&(text[a]!='-'))
   {
    p=a;
    t=text[a];
    text[p]=0;
    break;
   }
 Vertex.v[1]=(ENfloat)atof(text);
 if(p!=-1)
  text[p]=t;
 //Extract Z
 p=-1;
 text=strstr(text,"|")+1;
 for(a=0;a<len;a++)
   if((text[a]<'0'||text[a]>'9')&&(text[a]!='.')&(text[a]!='-'))
   {
    p=a;
    t=text[a];
    text[p]=0;
    break;
   }
 Vertex.v[2]=(ENfloat)atof(text);
 if(p!=-1)
  text[p]=t;

 return text;
}

char *ENASCIIMesh::ExtractTriangle(char *text,ENASCIIMesh::ENMeshFace &face)
{
 //Extract A
 face.Indices[0]=ExtractInteger(text);
 //Extract B
 text=strstr(text,"|")+1;
 face.Indices[1]=ExtractInteger(text);
 //Extract C
 text=strstr(text,"|")+1;
 face.Indices[2]=ExtractInteger(text);

 return text;
}

ENuint ENASCIIMesh::GetNumVertexes()
{
 return NumVertexes;
}

ENuint ENASCIIMesh::GetNumFaces()
{
 return NumFaces;
}

void   ENASCIIMesh::GetVertexes(ENVector *Verts)
{
 memcpy(Verts,Vertexes,sizeof(ENVector)*NumVertexes);
}

void   ENASCIIMesh::GetFaces(ENASCIIMesh::ENMeshFace *vFaces)
{
 memcpy(vFaces,Faces,sizeof(ENMeshFace)*NumFaces);
}

void   ENASCIIMesh::AddVertexes(ENVector *Verts,ENuint num)
{
 Vertexes=(ENVector*)ENAddElements((ENubyte*)Vertexes,sizeof(ENVector),
                    NumVertexes,(ENubyte*)Verts,num);
}

void   ENASCIIMesh::AddFaces(ENASCIIMesh::ENMeshFace *vFaces,ENuint num)
{
 Faces=(ENMeshFace*)ENAddElements((ENubyte*)Faces,sizeof(ENMeshFace),
                    NumFaces,(ENubyte*)vFaces,num);
}

