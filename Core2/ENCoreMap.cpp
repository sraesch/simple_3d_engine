//---------------------------------------------------------------------------
#include "ENCoreMap.h"
#include "ENCoreMain.h"
#include "ENCoreLoader.h"
#include "ENCoreCollision.h"
#include "ENCoreLight.h"
#include "ENCoreObjects.h"
#include "ENCoreShaders.h"
#include "ENCorePacker.h"
#include "ENCoreDepthBuffer.h"
#include "ENCoreView.h"
//---------------------------------------------------------------------------
/////////////////////////////////////////////////////////////
///Engine Core map class
/////////////////////////////////////////////////////////////
ENMapBuildLoader                       *ENCoreMap::mapobj=NULL;
ENMapBuildBase::ENMapBuildHeader        ENCoreMap::header;

void ENCoreMap::Init()
{
 //Loader message
 ENCoreLoader::AddMsg("Initialize engine core map...");
 //Create map builder loader
 mapobj=new ENMapBuildLoader();
 //Get header of map
 header=mapobj->GetHeader();
}

void ENCoreMap::Free()
{
 delete mapobj;
}

void ENCoreMap::Load(char *FileName)
{
 //Variables
 char FName[MAX_PATH];
 //Try to find File
 if(!ENCoreMain::SearchFile(FileName,FName))
 {
  ENCoreMain::ShowError("Cannot find map!!!");
  return;
 }
 //Load map
 if(!mapobj->Load(FName))
 {
  ENCoreMain::ShowError("Error loading map!!!");
  return;
 }
 //Get header of map
 header=mapobj->GetHeader();
 //Load map package
 ENCorePacker::LoadMapPackage(header.Package);
 //Update lights
 UpdateLights();
 //Update objects
 UpdateObjects();
}

ENuint ENCoreMap::GetNumPortals()
{
 return header.NumPortals;
}

ENMapBuildLoader::ENPortal *ENCoreMap::GetPortal(ENuint ind)
{
 return mapobj->GetPortal(ind);
}

ENbool ENCoreMap::isPortalVisible(ENuint ind,ENEngineFrustum *f)
{
 //Variables
 ENMapBuildBase::ENPortal *portal;
 //Get portal
 portal=mapobj->GetPortal(ind);
 //Check if portal is visible
 return ENBoxInFrustum(portal->min.x,portal->min.y,portal->min.z,
                       portal->max.x,portal->max.y,portal->max.z,
       	               f);
}

ENbool ENCoreMap::PortalLightIntersect(ENuint ind,ENLight *light)
{
 //Variables
 ENMapBuildBase::ENPortal *portal;
 //Get portal
 portal=mapobj->GetPortal(ind);
 //Check if portal intersect light
 return ENCoreCollision::SpherePortal(&light->pos,light->range,portal);
}

void ENCoreMap::DrawPortalShadow(ENuint ind)
{
 //Variables
 ENMapBuildBase::ENPortal *portal;
 //Get portal
 portal=mapobj->GetPortal(ind);
 //Set vertex pointer
 glVertexPointer(3,GL_FLOAT,0,portal->vertexes);
 //Draw data
 glBegin(GL_TRIANGLES);
 for(ENuint f=0;f<portal->NumFaces;f++)
 {
  ENMapBase::ENMapFace *face=&portal->faces[f];
  if(!face->props.Invisible&&face->props.Shadow)
  {
   glArrayElement(face->indices[0]);
   glArrayElement(face->indices[1]);
   glArrayElement(face->indices[2]);
  }   
 }
 glEnd();
}

void ENCoreMap::DrawPortalGnd(ENuint ind)
{
 //Variables
 ENMapBuildBase::ENPortal *portal;
 //Get portal
 portal=mapobj->GetPortal(ind);
 //Init
 glVertexPointer(3,GL_FLOAT,0,portal->vertexes);
 //Draw data
 glBegin(GL_TRIANGLES);
 for(ENuint f=0;f<portal->NumFaces;f++)
 {
  ENMapBase::ENMapFace *face=&portal->faces[f];
  if(!face->props.Invisible)
  {//Check if face is visible
   if(mapobj->SupportMaterialType(face->texind,ENMapBuildLoader::ENMATLUMI))
   {
    glColor3f(1,1,1);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    mapobj->SetMaterial(face->texind,ENMapBuildLoader::ENMATLUMI);
    glBegin(GL_TRIANGLES);
    //Vertex 1
    glTexCoord2fv((ENfloat*)&face->texcoord[0]);
    glArrayElement(face->indices[0]);
    //Vertex 2
    glTexCoord2fv((ENfloat*)&face->texcoord[1]);
    glArrayElement(face->indices[1]);
    //Vertex 3
    glTexCoord2fv((ENfloat*)&face->texcoord[2]);
    glArrayElement(face->indices[2]);
   }
   else
   {
    if(face->props.Lighting)
      glColor3f(0,0,0);
    else
      glColor3f(1,1,1);

    glArrayElement(face->indices[0]);
    glArrayElement(face->indices[1]);
    glArrayElement(face->indices[2]);
   }     
  }
 }
 glEnd();
}

void ENCoreMap::DrawPortalTex(ENuint ind)
{
 //Variables
 ENMapBuildBase::ENPortal *portal;
 //Get portal
 portal=mapobj->GetPortal(ind);
 //Set vertex pointer
 glVertexPointer(3,GL_FLOAT,0,portal->vertexes);
 //Draw texture faces
 for(ENuint f=0;f<portal->NumFaces;f++)
   DrawTexFace(portal->faces+f);
}

void ENCoreMap::DrawSkyBox()
{
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 glPushMatrix();
 glLoadIdentity();
 ENCoreView::RotateModel();
 glColor3f(1,1,1);
 //FRONT
 if(mapobj->SetSkyBox(0))
 {
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex3f(-10, 10,-10);
  glTexCoord2f(1,0);
  glVertex3f( 10, 10,-10);
  glTexCoord2f(1,1);
  glVertex3f( 10,-10,-10);
  glTexCoord2f(0,1);
  glVertex3f(-10,-10,-10);
  glEnd();
 }
 //BACK
 if(mapobj->SetSkyBox(1))
 {
  glBegin(GL_QUADS);
  glTexCoord2f(1,0);
  glVertex3f(-10, 10, 10);
  glTexCoord2f(0,0);
  glVertex3f( 10, 10, 10);
  glTexCoord2f(0,1);
  glVertex3f( 10,-10, 10);
  glTexCoord2f(1,1);
  glVertex3f(-10,-10, 10);
  glEnd();
 }
 //LEFT
 if(mapobj->SetSkyBox(2))
 {
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex3f(-10, 10, 10);
  glTexCoord2f(1,0);
  glVertex3f(-10, 10,-10);
  glTexCoord2f(1,1);
  glVertex3f(-10,-10,-10);
  glTexCoord2f(0,1);
  glVertex3f(-10,-10, 10);
  glEnd();
 }
 //RIGHT
 if(mapobj->SetSkyBox(3))
 {
  glBegin(GL_QUADS);
  glTexCoord2f(1,0);
  glVertex3f( 10, 10, 10);
  glTexCoord2f(0,0);
  glVertex3f( 10, 10,-10);
  glTexCoord2f(0,1);
  glVertex3f( 10,-10,-10);
  glTexCoord2f(1,1);
  glVertex3f( 10,-10, 10);
  glEnd();
 }
 //TOP
 if(mapobj->SetSkyBox(4))
 {
  glBegin(GL_QUADS);
  glTexCoord2f(0,1);
  glVertex3f(-10, 10, 10);
  glTexCoord2f(0,0);
  glVertex3f( 10, 10, 10);
  glTexCoord2f(1,0);
  glVertex3f( 10, 10,-10);
  glTexCoord2f(1,1);
  glVertex3f(-10, 10,-10);
  glEnd();
 }
 //BOTTOM
 if(mapobj->SetSkyBox(5))
 {
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex3f(-10,-10, 10);
  glTexCoord2f(0,1);
  glVertex3f( 10,-10, 10);
  glTexCoord2f(1,1);
  glVertex3f( 10,-10,-10);
  glTexCoord2f(1,0);
  glVertex3f(-10,-10,-10);
  glEnd();
 }
 glPopMatrix();
 glEnable(GL_CULL_FACE);
 glEnable(GL_DEPTH_TEST);
}

void ENCoreMap::SetLightFace(ENMapBase::ENMapFace *face,ENbool omni)
{
 if(omni)
 {//Omni directional lighting shader's
  if(mapobj->SupportMaterialType(face->texind,ENMapBuildLoader::ENMATBUMP))
  {
   ENGLExtension::ActiveTexture(GL_TEXTURE1_ARB);
   mapobj->SetMaterial(face->texind,ENMapBuildLoader::ENMATBUMP);
   ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
   ENCoreShaders::SetShader(ENGENERICBUMPOMNI);
  }
  else
    ENCoreShaders::SetShader(ENGENERICOMNI);
 }
 else
 {//Spot directional lighting shader's
  if(mapobj->SupportMaterialType(face->texind,ENMapBuildLoader::ENMATBUMP))
  {
   ENGLExtension::ActiveTexture(GL_TEXTURE1_ARB);
   mapobj->SetMaterial(face->texind,ENMapBuildLoader::ENMATBUMP);
   ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
   ENCoreShaders::SetShader(ENGENERICBUMPSPOT);
  }
  else
    ENCoreShaders::SetShader(ENGENERICSPOT);
 }
}

void ENCoreMap::SetSunFace(ENMapBase::ENMapFace *face)
{
 if(mapobj->SupportMaterialType(face->texind,ENMapBuildLoader::ENMATBUMP))
 {
  mapobj->SetMaterial(face->texind,ENMapBuildLoader::ENMATBUMP);
  ENCoreShaders::SetShader(ENGENERICSUNBUMP);
 }
 else
   ENCoreShaders::SetShader(ENGENERICSUN);
}

ENbool ENCoreMap::SetSun(GLenum index)
{
 ENMapBase::ENMapSun sun;
 mapobj->GetSun(&sun);
 if(sun.Enable)
 {
  ENShaderGenericSun::SetSun(index,&sun);
  return true;
 }
 else
   return false;
}

void ENCoreMap::DrawTexFace(ENMapBase::ENMapFace *face)
{
 //Set base
 if(face->texind>=0)
   mapobj->SetMaterial(face->texind,ENMapBuildLoader::ENMATBASE);
 else
   glBindTexture(GL_TEXTURE_2D,0);

 //Draw face
 glBegin(GL_TRIANGLES);
 //Vertex 1
 glTexCoord2fv((ENfloat*)&face->texcoord[0]);
 glArrayElement(face->indices[0]);
 //Vertex 2
 glTexCoord2fv((ENfloat*)&face->texcoord[1]);
 glArrayElement(face->indices[1]);
 //Vertex 3
 glTexCoord2fv((ENfloat*)&face->texcoord[2]);
 glArrayElement(face->indices[2]);

 glEnd();
}

void ENCoreMap::UpdateLights()
{
 //Alloc space for lights
 ENLight *maplights=new ENLight[header.NumLights];
 //Get lights
 mapobj->GetLights(maplights);
 //Update lights
 ENCoreLight::UpdateLights(header.NumLights,maplights);
 //Release lights
 delete[] maplights;
}

void ENCoreMap::UpdateObjects()
{
 //Alloc space for objects
 ENMapBase::ENMapObject *mapobjs=new ENMapBase::ENMapObject[header.NumObjects];
 //Get objects
 mapobj->GetObjects(mapobjs);
 //Update objects
 ENCoreObjects::UpdateObjects(header.NumObjects,mapobjs);
 //Release objects
 delete[] mapobjs;
}
