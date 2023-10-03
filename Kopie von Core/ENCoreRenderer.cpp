//---------------------------------------------------------------------------
#include "ENCoreRenderer.h"
#include "ENCoreLight.h"
#include "ENCoreShaders.h"
#include "ENCoreDepthBuffer.h"
#include "ENCoreObjects.h"
#include "ENCoreConfig.h"
#include "ENCoreAlphaBuffer.h"
//---------------------------------------------------------------------------
ENuint          ENCoreRenderer::NumPortals=0;
ENuint          ENCoreRenderer::DefShadow=0;
ENubyte         *ENCoreRenderer::PriPortalBuffer=NULL;
ENubyte         *ENCoreRenderer::SndPortalBuffer=NULL;
ENEngineFrustum ENCoreRenderer::Frust;

void ENCoreRenderer::Init()
{
 glGenTextures(1,&DefShadow);
 glBindTexture(GL_TEXTURE_2D,DefShadow);
 //Generate depth texture
 ENfloat smap[4]={1.0f,1.0f,1.0f,1.0f};
 glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT16,2,2,0,GL_DEPTH_COMPONENT,GL_FLOAT,smap);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
 glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_INTENSITY);
}

void ENCoreRenderer::Free()
{
 //Free texture
 glDeleteTextures(1,&DefShadow);
 //Free data
 ENFreeMemory(PriPortalBuffer);
 ENFreeMemory(SndPortalBuffer);
 //Set default
 PriPortalBuffer=NULL;
 SndPortalBuffer=NULL;
}

void ENCoreRenderer::Render()
{
 //Update data
 Update();
 //Draw sky box
 ENCoreMap::DrawSkyBox();
 //Init
 glEnableClientState(GL_VERTEX_ARRAY);
 //Draw ground data
 DrawGroundData();
 //Config depth and blend buffer
 glDepthFunc(GL_EQUAL);
 glDepthMask(GL_FALSE);
 glEnable(GL_BLEND);
 glBlendFunc(GL_ONE,GL_ONE);
 //Draw light data
 DrawLightData();
 //Draw tex data
 DrawTexData();
 //Reset
 glDepthFunc(GL_LESS);
 glDepthMask(GL_TRUE);
 glDisable(GL_BLEND);
 glDisableClientState(GL_VERTEX_ARRAY);
 //Draw sprites
 ENCoreObjects::DrawSprites();
 //Draw alpha buffer
 ENCoreAlphaBuffer::DrawBuffer();
}

void ENCoreRenderer::Update()
{
 //Variables
 ENuint nump;
 //Calculate new frustum planes
 ENCalculateFrustumPlanes(Frust);
 //Process visibility of lights
 ENCoreLight::CheckVisibility(&Frust);
 //Get current number of portals
 nump=ENCoreMap::GetNumPortals();
 //Check if data are up to date
 if(nump!=NumPortals)
 {
  NumPortals=nump;
  //Update primary portal buffer
  ENFreeMemory(PriPortalBuffer);
  PriPortalBuffer=(ENubyte*)ENAllocMemory(nump);
  //Update secondary portal buffer
  ENFreeMemory(SndPortalBuffer);
  SndPortalBuffer=(ENubyte*)ENAllocMemory(nump);
 }
}

void ENCoreRenderer::DrawGroundData()
{
 //Variables
 ENuint p;
 //Init
 glDisable(GL_TEXTURE_2D);
 //Draw portals
 for(p=0;p<NumPortals;p++)
 {
  //Check if portal is visible
  PriPortalBuffer[p]=ENCoreMap::isPortalVisible(p,&Frust);
  //If portal is visible draw ground data
  if(PriPortalBuffer[p]) ENCoreMap::DrawPortalGnd(p);
 }
 //Draw objects
 ENCoreObjects::DrawGroundData();
 //Reset
 glEnable(GL_TEXTURE_2D);
 glDisableClientState(GL_NORMAL_ARRAY);
}

void ENCoreRenderer::DrawTexData()
{
 //Variables
 ENuint p;
 //Init
 glBlendFunc(GL_ZERO,GL_SRC_COLOR);
 //Draw portals
 glColor3f(1.0f,1.0f,1.0f);
 for(p=0;p<NumPortals;p++)
   if(PriPortalBuffer[p])
     ENCoreMap::DrawPortalTex(p);
 //Draw objects
 ENCoreObjects::DrawTexData();     
}

void ENCoreRenderer::DrawLightData()
{
 //Variables
 ENLight light;
 ENuint  l,NumLights;
 //Get number of lights
 NumLights=ENCoreLight::GetNumLights();
 //Init
 ENGLExtension::ActiveTexture(GL_TEXTURE1_ARB);
 glEnable(GL_TEXTURE_2D);
 ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
 //Draw light data
 for(l=0;l<NumLights;l++)
   if(ENCoreLight::IsLightVisible(l))
   {
    //Get and set light
    ENCoreLight::GetLight(l,light);
    light.SetGLLight(GL_LIGHT0);
    //Draw light data for current light
    if(light.isSpotLight())
      DrawSpotLight(&light);
    else
      DrawOmniLight(&light);
   }
 //Reset Matrix
 glMatrixMode(GL_TEXTURE);
 glLoadIdentity();
 glMatrixMode(GL_MODELVIEW);
 //Draw sun
 if(ENCoreMap::SetSun())
   DrawSunData();
 //Reset all
 ENGLExtension::ActiveTexture(GL_TEXTURE1_ARB);
 glDisable(GL_TEXTURE_2D);
 ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
 ENCoreShaders::ResetShader();
}

void ENCoreRenderer::DrawSunData()
{
 //Variables
 ENMapBuildBase::ENPortal *portal;
 //Draw sundata
 glEnableClientState(GL_NORMAL_ARRAY);
 for(ENuint p=0;p<NumPortals;p++)
   if(PriPortalBuffer[p])
   {
    portal=portal=ENCoreMap::GetPortal(p);
    glVertexPointer(3,GL_FLOAT,0,portal->vertexes);
    glNormalPointer(GL_FLOAT,0,portal->normals);
    for(ENuint f=0;f<portal->NumFaces;f++)
    {
     //Get current face
     ENMapBase::ENMapFace *face=&portal->faces[f];
     //Set face material
     ENCoreMap::SetSunFace(face);
     //Set face
     if(face->props.Lighting)
     {
      glBegin(GL_TRIANGLES);
      glTexCoord2fv((ENfloat*)&face->texcoord[0]);
      glArrayElement(face->indices[0]);
      glTexCoord2fv((ENfloat*)&face->texcoord[1]);
      glArrayElement(face->indices[1]);
      glTexCoord2fv((ENfloat*)&face->texcoord[2]);
      glArrayElement(face->indices[2]);
      glEnd();
     }
    }
   }
 //Draw lightdata for all objects, which intersect
 ENCoreObjects::DrawSunData();
 //Reset texture matrix
 glMatrixMode(GL_TEXTURE);
 glLoadIdentity();
 glMatrixMode(GL_MODELVIEW);
 //Reset
 glDisableClientState(GL_NORMAL_ARRAY);
}

void ENCoreRenderer::DrawSpotLight(ENLight *light)
{
 //Variables
 ENMatrix4X4 LightMatrix;
 //Draw shadow data
 if(DrawShadowSpot(light,LightMatrix))
 {
  //Set shadow matrix
  glMatrixMode(GL_TEXTURE);
  glLoadMatrixf(&LightMatrix.val[0][0]);
  glMatrixMode(GL_MODELVIEW);
  //Set shadow
  ENCoreDepthBuffer::SetShadow();
  //Draw lightdata for all portals, which intersect
  glEnableClientState(GL_NORMAL_ARRAY);
  for(ENuint p=0;p<NumPortals;p++)
  {
   if(PriPortalBuffer[p]&&SndPortalBuffer[p])
     DrawPortalLight(p,false);
  }
  //Draw lightdata for all objects, which intersect
  ENCoreObjects::DrawLightData(false);
  //Reset
  glDisableClientState(GL_NORMAL_ARRAY);
 }
}

void ENCoreRenderer::DrawOmniLight(ENLight *light)
{
 //Variables
 ENMatrix4X4 LightMatrix;
 //Draw all shadows
 for(ENuint s=0;s<6;s++)
   if(DrawShadowOmni(light,s,LightMatrix))
   {
    //Set shadow matrix
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(&LightMatrix.val[0][0]);
    glMatrixMode(GL_MODELVIEW);
    //Set shadow
    ENCoreDepthBuffer::SetShadow();
    //Draw lightdata for all portals, which intersect
    glEnableClientState(GL_NORMAL_ARRAY);
    for(ENuint p=0;p<NumPortals;p++)
      if(PriPortalBuffer[p]&&SndPortalBuffer[p])
        DrawPortalLight(p,true);
    //Draw lightdata for all objects, which intersect
    ENCoreObjects::DrawLightData(true);
    //Reset
    glDisableClientState(GL_NORMAL_ARRAY);
   }
}

ENbool ENCoreRenderer::DrawShadowSpot(ENLight *light,ENMatrix4X4 &lmat)
{
 //Variables
 ENEngineFrustum vfrust;
 ENbool res=false;
 //Begin depth buffer
 ENCoreDepthBuffer::Begin();
 //Set light matrix
 light->BuildLightMatrix(&lmat);
 //Calcualte frustum
 ENCalculateFrustumPlanes(vfrust,&lmat);
 //Set data
 glEnableClientState(GL_VERTEX_ARRAY);
 //Draw portals
 for(ENuint p=0;p<NumPortals;p++)
 {
  //Update secondary portal buffer
  SndPortalBuffer[p]=ENCoreMap::isPortalVisible(p,&vfrust);
  //Check if portal intersects light
  if(SndPortalBuffer[p])
  {
   res=true;
   ENCoreMap::DrawPortalShadow(p);
  }
 }
 //Draw objects
 if(ENCoreObjects::DrawShadowData())  res=true;
 //Release vertex array
 glDisableClientState(GL_VERTEX_ARRAY);
 //Finished depth buffer
 ENCoreDepthBuffer::End(res);
 return res;
}

ENbool ENCoreRenderer::DrawShadowOmni(ENLight *light,ENuint ind,ENMatrix4X4 &lmat)
{
 //Variables
 ENEngineFrustum vfrust;
 ENbool res=false;
 //Begin depth buffer
 ENCoreDepthBuffer::Begin();
 //Set light matrix
 light->BuildOmniLightMatrix(&lmat,ind);
 //Calcualte frustum
 ENCalculateFrustumPlanes(vfrust,&lmat);
 //Set data
 glEnableClientState(GL_VERTEX_ARRAY);
 //Draw portals
 for(ENuint p=0;p<NumPortals;p++)
 {
  //Update secondary portal buffer
  SndPortalBuffer[p]=ENCoreMap::isPortalVisible(p,&vfrust);
  //Check if portal intersects light
  if(SndPortalBuffer[p])
  {
   res=true;
   ENCoreMap::DrawPortalShadow(p);
  }
 }
 //Draw objects
 if(ENCoreObjects::DrawShadowData()) res=true;
 //Release vertex array
 glDisableClientState(GL_VERTEX_ARRAY);
 //Finished depth buffer
 ENCoreDepthBuffer::End(res);
 return res;
}

void ENCoreRenderer::DrawPortalLight(ENuint ind,ENbool omni)
{
 //Variables
 ENMapBuildBase::ENPortal *portal;
 //Get portal
 portal=ENCoreMap::GetPortal(ind);
 //Set data
 glVertexPointer(3,GL_FLOAT,0,portal->vertexes);
 glNormalPointer(GL_FLOAT,0,portal->normals);
 //Draw faces
 for(ENuint f=0;f<portal->NumFaces;f++)
 {
  //Get current face
  ENMapBase::ENMapFace *face=&portal->faces[f];
  //Set face material
  ENCoreMap::SetLightFace(face,omni);
  //Set face
  if(face->props.Lighting)
  {
   glBegin(GL_TRIANGLES);
   glTexCoord2fv((ENfloat*)&face->texcoord[0]);
   glArrayElement(face->indices[0]);
   glTexCoord2fv((ENfloat*)&face->texcoord[1]);
   glArrayElement(face->indices[1]);
   glTexCoord2fv((ENfloat*)&face->texcoord[2]);
   glArrayElement(face->indices[2]);
   glEnd();
  }   
 }
}





