#include "EngineMedia.h"

ENMapBuilder::ENMapBuilder(ENMap *map)
{
 //Set map obj
 mapsrc=map;
 //Init values
 reportfunc=NULL;
}

ENMapBuilder::~ENMapBuilder()
{
}

void ENMapBuilder::SetReportFunc(ENREPORTFUNC rpfunc)
{
 reportfunc=rpfunc;
}

ENbool ENMapBuilder::Build(char *FileName)
{
 //Variables
 ENFile handle;
 ENuint neededtime;
 //Save current ticks
 neededtime=GetTickCount();
 //Opening file
 handle=ENOpenFile(FileName,"wb");
 if(!handle) return false;
 //Header
 ReportMsg("Write header...",0,0);
 WriteHeader(handle);
 //Body data
 WriteBody(handle);
 //Write portals 
 WritePortals(handle);
 //Write textures
 WriteTextures(handle);
 //Write bsp
 WriteBsp(handle);
 //Close file
 ENCloseFile(handle);
 //Finishing message
 char msgBuffer[80];
 neededtime=GetTickCount()-neededtime;
 sprintf(msgBuffer,"Finished in %f seconds!!!",(ENfloat)neededtime/1000);
 ReportMsg(msgBuffer,0,0);

 return true;
}

void ENMapBuilder::ReportMsg(char *Msg,ENuint pos,ENuint max)
{
 if(reportfunc)
   reportfunc(Msg,pos,max);
}

void ENMapBuilder::WriteHeader(ENFile handle)
{
 //Variables
 ENMapBase::ENMapHeader headersrc;
 ENMapBuildHeader headerdst;
 //Get header data
 headersrc=mapsrc->GetHeader();
 //Set header data
 headerdst.NumFaces=headersrc.NumFaces;
 headerdst.NumPortals=0;//Portal calculation is later
 headerdst.NumTexs=headersrc.NumTextures;
 headerdst.NumLights=headersrc.NumLights;
 headerdst.NumObjects=headersrc.NumObjects;
 headerdst.NumSounds=headersrc.NumSounds;
 strcpy(headerdst.Package,headersrc.PackageFile);
 //Write header
 ENWriteFile(&headerdst,sizeof(ENMapBuildHeader),1,handle);
}

void  ENMapBuilder::WriteBody(ENFile handle)
{
 //Variables
 ENuint i;
 ENCamera cam;
 ENMapBase::ENMapHeader header;
 //Get header
 header=mapsrc->GetHeader();
 //Sun
 ENMapBase::ENMapSun sun;
 mapsrc->GetSun(&sun);
 ENWriteFile(&sun,sizeof(ENMapBase::ENMapSun),1,handle);
 //Skybox
 for(i=0;i<6;i++)
 {
  ENPicture *spic=new ENPicture();
  mapsrc->GetSkyBox(i,spic);
  ReportMsg("Write Skybox...",i+1,6);
  spic->SaveToHandle(handle);
  delete spic;
 }
 //Camera
 cam=mapsrc->GetCamera();
 ENWriteFile(&cam,sizeof(ENCamera),1,handle);
 //Lights
 for(i=0;i<header.NumLights;i++)
 {
  ENLight light=mapsrc->GetLight(i);
  ReportMsg("Write Lights...",i+1,header.NumLights);
  ENWriteFile(&light,sizeof(ENLight),1,handle);
 }
 //Sounds
 for(i=0;i<header.NumSounds;i++)
 {
  ENSoundSrc sound=mapsrc->GetSound(i);
  ReportMsg("Write Sounds...",i+1,header.NumSounds);
  ENWriteFile(&sound,sizeof(ENSoundSrc),1,handle);
 }
 //Objects
 for(i=0;i<header.NumObjects;i++)
 {
  ENMapBase::ENMapObject obj=mapsrc->GetObject(i);
  ReportMsg("Write Objects...",i+1,header.NumObjects);
  ENWriteFile(&obj,sizeof(ENMapBase::ENMapObject),1,handle);
 }
}

void ENMapBuilder::CalculateFaceNormals(ENVector *Normals)
{
 //Variables
 ENuint i;
 ENVector verts[3];
 ENMapBase::ENMapFace face;
 ENMapBase::ENMapHeader header;
 //Get header
 header=mapsrc->GetHeader();
 //Calculate normals
 for(i=0;i<header.NumFaces;i++)
 {
  //Get face
  face=mapsrc->GetFace(i);
  //Get vertexes of the face
  verts[0]=mapsrc->GetVertex(face.indices[0]);
  verts[1]=mapsrc->GetVertex(face.indices[1]);
  verts[2]=mapsrc->GetVertex(face.indices[2]);
  //Calcualte normal of current face
  Normals[i]=ENNormal(verts)*(-1.0f);
  //Update report message
  ReportMsg("Calculate face normals...",i+1,header.NumFaces);
 }
}

void ENMapBuilder::CalculateVertNormals(ENVector *Normals)
{
 //Variables
 ENVector *fnormals=NULL;
 ENVector  vnormal;
 ENuint a,b;
 ENuint NumConnections;
 ENMapBase::ENMapFace face;
 ENMapBase::ENMapHeader header;
 //Get header
 header=mapsrc->GetHeader();
 //Calculate face normals
 fnormals=new ENVector[header.NumFaces];
 CalculateFaceNormals(fnormals);
 //Calculate vertex-normals
 for(a=0;a<header.NumVertexes;a++)
 {
  NumConnections=0;
  vnormal=ENVector(0,0,0);
  for(b=0;b<header.NumFaces;b++)
  {
   face=mapsrc->GetFace(b);
   if(face.indices[0]==a||face.indices[1]==a||face.indices[2]==a)
   {
    vnormal=vnormal+fnormals[b];
    NumConnections++;
   }
  }
  //Build vertex-normal
  if(NumConnections)
    vnormal=vnormal/NumConnections;
  //Add vertex normal
  Normals[a]=vnormal;
  //Report message
  ReportMsg("Calculate vertex normals...",a+1,header.NumVertexes);
 }
 //Finished
 delete[] fnormals;
}

void ENMapBuilder::WritePortals(ENFile handle)
{
 //Variables
 ENuint filepos;
 ENuint NumberPortals=0;
 ENMapBase::ENMapHeader header=mapsrc->GetHeader();
 ENVector *vertnormals=new ENVector[header.NumVertexes];
 ENbool *FreeFaces=new ENbool[header.NumFaces];
 //Calculate vertex normals
 CalculateVertNormals(vertnormals);
 //Init free faces array
 memset(FreeFaces,255,sizeof(ENbool)*header.NumFaces);
 //Write portals
 for(ENuint f=0;f<header.NumFaces;f++)
   if(FreeFaces[f])
   {
    ReportMsg("Write portals...",f+1,header.NumFaces);
    WritePortal(FreeFaces,vertnormals,f,handle);
    NumberPortals++;
   }
 //Write number of portals to the header
 filepos=ENTellFile(handle);
 ENSeekFile(handle,sizeof(ENuint),SEEK_SET);
 ENWriteFile(&NumberPortals,sizeof(ENuint),1,handle);
 ENSeekFile(handle,filepos,SEEK_SET);
 //Release data
 delete[] FreeFaces;
 delete[] vertnormals;
}

void ENMapBuilder::WritePortal(ENbool *FreeFaces,ENVector *vertnormals,ENuint ind,ENFile handle)
{
 //Variables
 ENuint   NumFaces,NumVerts;
 ENVector min,max;
 ENArray *portalverts=new ENArray(sizeof(ENVector));
 ENArray *portalnormals=new ENArray(sizeof(ENVector));
 ENArray *portalfaces=new ENArray(sizeof(ENMapBase::ENMapFace));
 //Find all faces
 FindPortalFaces(FreeFaces,portalfaces,ind);
 //Find all verts
 FindPortalVerts(portalfaces,portalverts,portalnormals,vertnormals);
 //Build box
 BuildBox(portalverts,min,max);
 //Write portal
 NumFaces=portalfaces->GetNum();
 NumVerts=portalverts->GetNum();
 ENWriteFile(&NumFaces,sizeof(ENuint),1,handle);
 ENWriteFile(&NumVerts,sizeof(ENuint),1,handle);
 portalfaces->SaveToHandleRaw(handle);
 portalverts->SaveToHandleRaw(handle);
 portalnormals->SaveToHandleRaw(handle);
 ENWriteFile(&min,sizeof(ENVector),1,handle);
 ENWriteFile(&max,sizeof(ENVector),1,handle);
 //Release
 delete portalfaces;
 delete portalverts;
 delete portalnormals;
}

void ENMapBuilder::BuildBox(ENArray *verts,ENVector &min,ENVector &max)
{
 //Variables
 ENVector vertex;
 //Init min and max
 verts->GetElement(0,(ENubyte*)&min);
 verts->GetElement(0,(ENubyte*)&max);
 //build box
 for(ENuint a=0;a<verts->GetNum();a++)
 {
  //Get vertex
  verts->GetElement(a,(ENubyte*)&vertex);
  //Min Vector
  if(vertex.v[0]<min.v[0]) min.v[0]=vertex.v[0];
  if(vertex.v[1]<min.v[1]) min.v[1]=vertex.v[1];
  if(vertex.v[2]<min.v[2]) min.v[2]=vertex.v[2];
  //Max Vector
  if(vertex.v[0]>max.v[0]) max.v[0]=vertex.v[0];
  if(vertex.v[1]>max.v[1]) max.v[1]=vertex.v[1];
  if(vertex.v[2]>max.v[2]) max.v[2]=vertex.v[2];
 }
}

void ENMapBuilder::FindPortalFaces(ENbool *FreeFaces,ENArray *pfaces,ENuint ind)
{
 //Variables
 ENMapBase::ENMapFace   face,sndface;
 ENMapBase::ENMapHeader header=mapsrc->GetHeader();
 ENVector               trivecs[3];
 //Get current face
 face=mapsrc->GetFace(ind);
 //Add face to list
 trivecs[0]=mapsrc->GetVertex(face.indices[0]);
 trivecs[1]=mapsrc->GetVertex(face.indices[1]);
 trivecs[2]=mapsrc->GetVertex(face.indices[2]);
 pfaces->AddElement((ENubyte*)&face);
 //Not free anymore
 FreeFaces[ind]=false;
 //Look for other faces
 for(ENuint a=ind+1;a<header.NumFaces;a++)
   if(FreeFaces[a])
  {
   sndface=mapsrc->GetFace(a);
   if(sndface.indices[0]==face.indices[0]||
      sndface.indices[0]==face.indices[1]||
      sndface.indices[0]==face.indices[2]||
      sndface.indices[1]==face.indices[0]||
      sndface.indices[1]==face.indices[1]||
      sndface.indices[1]==face.indices[2]||
      sndface.indices[2]==face.indices[0]||
      sndface.indices[2]==face.indices[1]||
      sndface.indices[2]==face.indices[2])
        FindPortalFaces(FreeFaces,pfaces,a);
  }
}

void ENMapBuilder::FindPortalVerts(ENArray *pfaces,ENArray *pverts,ENArray *pnormals,ENVector *vertnormals)
{
 //Variables
 ENuint order;
 ENVector vertex;
 ENMapBase::ENMapFace face;
 ENArray *FaceOrder=new ENArray(sizeof(ENuint));
 //Collect all faces
 for(ENuint a=0;a<pfaces->GetNum();a++)
 {
  //Get current face
  pfaces->GetElement(a,(ENubyte*)&face);
  //Check for new vertexes
  for(ENuint b=0;b<3;b++)
  {
   ENint pos=GetFacePos(face.indices[b],FaceOrder);
   if(pos==-1)
   {
    vertex=mapsrc->GetVertex(face.indices[b]);
    order=face.indices[b];

    pos=(ENuint)FaceOrder->GetNum();

    pverts->AddElement((ENubyte*)&vertex);
    pnormals->AddElement((ENubyte*)&vertnormals[face.indices[b]]);
    FaceOrder->AddElement((ENubyte*)&order);
   }
   face.indices[b]=(ENuint)pos;
  }
  pfaces->SetElement(a,(ENubyte*)&face);
 }
 //Release
 delete FaceOrder;
}

ENint ENMapBuilder::GetFacePos(ENuint ind,ENArray *FaceOrder)
{
 for(ENuint a=0;a<FaceOrder->GetNum();a++)
 {
  ENuint ce;
  FaceOrder->GetElement(a,(ENubyte*)&ce);
  if(ce==ind) return a;
 }

 return -1;
}

void ENMapBuilder::WriteTextures(ENFile handle)
{
 //Variables
 ENMapBase::ENMapHeader header;
 //Get header
 header=mapsrc->GetHeader();
 //Copy textures
 for(ENuint a=0;a<header.NumTextures;a++)
 {
  ENMaterial *mat=new ENMaterial();
  mapsrc->GetTextureData(mat,a);
  mat->SaveToHandle(handle);
  delete mat;
  ReportMsg("Write textures...",a+1,header.NumTextures);
 }
}

void ENMapBuilder::WriteBsp(ENFile handle)
{
 //Variables
 ENBSPPolygon           polygon;
 ENMapBase::ENMapHeader header;
 ENBSPBuilder  *bspbuilder=new ENBSPBuilder(reportfunc);
 //Get header
 header=mapsrc->GetHeader();
 //Get polygones
 for(ENuint a=0;a<header.NumFaces;a++)
 {
  ENMapBase::ENMapFace face=mapsrc->GetFace(a);
  polygon.num=3;
  polygon.verts=(ENVector*)ENAllocMemory(sizeof(ENVector)*3);
  polygon.verts[0]=mapsrc->GetVertex(face.indices[0]);
  polygon.verts[1]=mapsrc->GetVertex(face.indices[1]);
  polygon.verts[2]=mapsrc->GetVertex(face.indices[2]);
  bspbuilder->AddPolygon(polygon);
 }
 //Build bsp
 ENBSPTree *tree=bspbuilder->BuildTree();
 //Save tree
 tree->WriteBsp(handle);
 //Delete tree
 delete tree;
}
///////////////////////////////////////////////////////////////////////
///Engine builded map loader
///////////////////////////////////////////////////////////////////////
ENMapBuildLoader::ENMapBuildLoader()
{
 //Init map header
 memset(&header,0,sizeof(header));
 //Init sky box
 for(ENuint a=0;a<6;a++) SkyBox[a]=NULL;
 //Init sun
 Sun.Enable=false;
 //Init data
 materials=NULL;
 lights=NULL;
 sounds=NULL;
 objects=NULL;
 portals=NULL;
 bsptree=NULL;
}

ENMapBuildLoader::~ENMapBuildLoader()
{
 Clear();
}

void ENMapBuildLoader::Clear()
{
 //Variables
 ENuint a;
 //Delete materials
 for(a=0;a<header.NumTexs;a++)
   delete materials[a];

 ENFreeMemory(materials);
 //Delete skybox
 for(ENuint a=0;a<6;a++)
   if(SkyBox[a])
   {
    delete SkyBox[a];
    SkyBox[a]=NULL;
   }
 //Init sun
 Sun.Enable=false;
 //Delete portals
 for(a=0;a<header.NumPortals;a++)
 {
  ENFreeMemory(portals[a].faces);
  ENFreeMemory(portals[a].vertexes);
  ENFreeMemory(portals[a].normals);
 }

 ENFreeMemory(portals);
 //Delete others
 ENFreeMemory(lights);
 ENFreeMemory(sounds);
 ENFreeMemory(objects);
 //Delete bsp tree
 delete bsptree;
 //Init map header
 memset(&header,0,sizeof(header));
 //Init data
 materials=NULL;
 lights=NULL;
 sounds=NULL;
 objects=NULL;
 portals=NULL;
 bsptree=NULL;
}

ENbool ENMapBuildLoader::Load(char *FileName)
{
 //Variables
 ENFile handle;
 ENuint a;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle) return false;
 //Clear map
 Clear();
 //Read header
 ENReadFile(&header,sizeof(ENMapBuildHeader),1,handle);
 //Alloc space
 materials=(ENMaterialLoaded**)ENAllocMemory(sizeof(ENMaterialLoaded*)*header.NumTexs);
 lights=(ENLight*)ENAllocMemory(sizeof(ENLight)*header.NumLights);
 sounds=(ENSoundSrc*)ENAllocMemory(sizeof(ENSoundSrc)*header.NumSounds);
 objects=(ENMapBase::ENMapObject*)ENAllocMemory(sizeof(ENMapBase::ENMapObject)*header.NumObjects);
 portals=(ENPortal*)ENAllocMemory(sizeof(ENPortal)*header.NumPortals);
 //Read sun
 ENReadFile(&Sun,sizeof(ENMapBase::ENMapSun),1,handle);
 //Read sky box
 for(a=0;a<6;a++)
 {
  ENPicture *pic=new ENPicture();
  pic->LoadFromHandle(handle);
  SkyBox[a]=new ENPictureLoaded(pic);
  delete pic;
 }
 //Read camera
 ENReadFile(&CameraObj,sizeof(ENCamera),1,handle);
 //Read body
 ENReadFile(lights,sizeof(ENLight),header.NumLights,handle);
 ENReadFile(sounds,sizeof(ENSoundSrc),header.NumSounds,handle);
 ENReadFile(objects,sizeof(ENMapBase::ENMapObject),header.NumObjects,handle);
 //Read portals
 for(a=0;a<header.NumPortals;a++)
 {
  ENReadFile(&portals[a].NumFaces,sizeof(ENuint),1,handle);
  ENReadFile(&portals[a].NumVerts,sizeof(ENuint),1,handle);
  portals[a].faces=(ENMapBase::ENMapFace*)ENAllocMemory(sizeof(ENMapBase::ENMapFace)*portals[a].NumFaces);
  portals[a].vertexes=(ENVector*)ENAllocMemory(sizeof(ENVector)*portals[a].NumVerts);
  portals[a].normals=(ENVector*)ENAllocMemory(sizeof(ENVector)*portals[a].NumVerts);
  ENReadFile(portals[a].faces,sizeof(ENMapBase::ENMapFace),portals[a].NumFaces,handle);
  ENReadFile(portals[a].vertexes,sizeof(ENVector),portals[a].NumVerts,handle);
  ENReadFile(portals[a].normals,sizeof(ENVector),portals[a].NumVerts,handle);
  ENReadFile(&portals[a].min,sizeof(ENVector),1,handle);
  ENReadFile(&portals[a].max,sizeof(ENVector),1,handle);
 }
 //Read textures
 for(a=0;a<header.NumTexs;a++)
 {
  ENMaterial *mat=new ENMaterial();
  if(!mat->LoadFromHandle(handle)) return false;
  materials[a]=new ENMaterialLoaded(mat,true);
  delete mat;
 }
 //Read bsp tree
 bsptree=new ENBSPTree();
 bsptree->LoadBsp(handle);
 //Finished
 ENCloseFile(handle);
 return true;
}

ENMapBuildBase::ENMapBuildHeader ENMapBuildLoader::GetHeader()
{
 return header;
}

ENMapBuildLoader::ENPortal *ENMapBuildLoader::GetPortal(ENuint ind)
{
 return &portals[ind];
}

void ENMapBuildLoader::SetMaterial(ENuint ind,ENMATTYPE type)
{
 ENMaterialLoaded *mat=materials[ind];
 switch(type)
 {
  case ENMATBASE:
    mat->SetBase();
  break;
  case ENMATALPHA:
    mat->SetAlpha();
  break;
  case ENMATBUMP:
    mat->SetBump();
  break;
  case ENMATLUMI:
    mat->SetLuminance();
  break;
 }
}

void ENMapBuildLoader::GetSun(ENMapBase::ENMapSun *sun)
{
 *sun=Sun;
}

ENbool ENMapBuildLoader::SupportMaterialType(ENuint ind,ENMATTYPE type)
{
 ENMaterialLoaded *mat=materials[ind];
 switch(type)
 {
  case ENMATBASE:
    return true;
  case ENMATALPHA:
    return mat->isAlphaSupported();
  case ENMATBUMP:
    return mat->isBumpSupported();
  case ENMATLUMI:
    return mat->isLuminanceSupported();
  default:
    return false;
 }
}

ENbool ENMapBuildLoader::SetSkyBox(ENuint ind)
{
 ind%=6;
 if(SkyBox[ind])
 {
  SkyBox[ind]->Set(0);
  return true;
 }
 else
   return false;
}

ENCamera ENMapBuildLoader::GetCamera()
{
 return CameraObj;
}

ENBSPTree *ENMapBuildLoader::GetBspTree()
{
 return bsptree;
}

void ENMapBuildLoader::GetLights(ENLight *lightsdst)
{
 memcpy(lightsdst,lights,sizeof(ENLight)*header.NumLights);
}

void ENMapBuildLoader::GetSounds(ENSoundSrc *soundsdst)
{
 memcpy(soundsdst,sounds,sizeof(ENSoundSrc)*header.NumSounds);
}

void ENMapBuildLoader::GetObjects(ENMapBase::ENMapObject *objectsdst)
{
 memcpy(objectsdst,objects,sizeof(ENMapBase::ENMapObject)*header.NumObjects);
}
