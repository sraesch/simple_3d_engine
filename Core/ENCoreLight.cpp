//---------------------------------------------------------------------------
#include "ENCoreLight.h"
//---------------------------------------------------------------------------
const ENuint 	                ENCoreLight::LIGHTBUFFERSTEP=0x5;
ENuint		                ENCoreLight::BufferSize=0;
ENuint		                ENCoreLight::NumLights=0;
ENuint                          ENCoreLight::LightIDCount=0;
ENCoreLight::ENDynamicLight    *ENCoreLight::Lights=NULL;

void ENCoreLight::Free()
{
 ENFreeMemory(Lights);
 Lights=NULL;
 BufferSize=0;
 NumLights=0;
 LightIDCount=0;
}

void ENCoreLight::CheckVisibility(ENEngineFrustum *vfrust)
{
 //Calculate visibility of lights
 for(ENuint l=0;l<NumLights;l++)
 {
  ENVector pos=Lights[l].light->pos;
  ENfloat  range=Lights[l].light->range;
  Lights[l].Visible=ENSphereInFrustum(pos.v[0],pos.v[1],pos.v[2],range,vfrust);
 }
}

ENCoreLight::ENDynamicLight ENCoreLight::CreateLight()
{
 CheckLightBuffer();
 Lights[NumLights].light=(ENLight*)ENAllocMemory(sizeof(ENLight));
 Lights[NumLights].ID=LightIDCount;
 *Lights[NumLights].light=ENLight(ENColor(255,255,255,255),ENVector(0,0,0),
                                 100,ENVector2(0,0),180);
 LightIDCount++;
 NumLights++;

 return Lights[NumLights-1];
}

ENint ENCoreLight::SearchLight(ENuint ID)
{
 for(ENuint a=0;a<NumLights;a++)
   if(Lights[a].ID==ID)
	 return a;

 return -1;
}

void ENCoreLight::DeleteLight(ENuint ID)
{
 //Variables
 ENint          index;
 ENDynamicLight light;
 //Search index
 index=SearchLight(ID);
 if(index==-1) return;
 //Get light
 light=Lights[index];
 //Copy
 memmove(Lights+index,Lights+index+1,(NumLights-index-1)*sizeof(ENDynamicLight));
 NumLights--;
 //Delete light
 ENFreeMemory(light.light);
}

void ENCoreLight::SetLight(ENScriptLight *light)
{
 //Get index
 ENint index=SearchLight(light->ID);
 if(index==-1) return;
 //Set light
 Lights[index].ID=light->ID;
 Lights[index].light->color.r=(ENubyte)light->r;
 Lights[index].light->color.g=(ENubyte)light->g;
 Lights[index].light->color.b=(ENubyte)light->b;
 Lights[index].light->pos=light->pos;
 Lights[index].light->dir=light->dir;
 Lights[index].light->range=light->range;
 Lights[index].light->spotcut=light->spotcutoff;
}

ENCoreLight::ENDynamicLight ENCoreLight::GetLight(ENuint ID)
{
 //Get index
 ENint index=SearchLight(ID);
 if(index==-1) return CreateLight();
 //Set light
 return Lights[index];
}

void ENCoreLight::GetLight(ENuint index,ENLight &light)
{
 light=*Lights[index].light;
}

ENbool ENCoreLight::IsLightVisible(ENuint index)
{
 return Lights[index].Visible;
}

ENuint ENCoreLight::GetNumLights()
{
 return NumLights;
}

void ENCoreLight::UpdateLights(ENuint num,ENLight *mlights)
{
 //Release
 Free();
 //Set new lights
 LightIDCount=num;
 BufferSize=num;
 NumLights=num;
 Lights=(ENDynamicLight*)ENAllocMemory(sizeof(ENDynamicLight)*BufferSize);
 for(ENuint l=0;l<NumLights;l++)
 {
 Lights[l].light=(ENLight*)ENAllocMemory(sizeof(ENLight));
 Lights[l].ID=l;
 *Lights[l].light=mlights[l];

 }
}

void ENCoreLight::CheckLightBuffer()
{
 if(NumLights+1>=BufferSize)
 {
  BufferSize+=LIGHTBUFFERSTEP;
  Lights=(ENDynamicLight*)ENReallocMemory(Lights,
	                 sizeof(ENDynamicLight)*BufferSize);
 }
}
