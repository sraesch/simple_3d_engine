//---------------------------------------------------------------------------
#include <math.h>
#include "ENCoreCollision.h"
#include "ENCoreMap.h"
//---------------------------------------------------------------------------
const ENfloat ENCoreCollision::STEPSIZE=2.0f;

ENbool ENCoreCollision::SphereSphere(ENCOLSPHEREHULL *sphere1,ENCOLSPHEREHULL *sphere2)
{
 //Variables
 ENVector A;
 ENfloat d2;
 //Vector from sphere 1 to sphere 2
 A=sphere1->pos-sphere2->pos;
 //Get max quadratic distance
 d2=sphere1->range+sphere2->range;
 d2*=d2;
 //Compare
 return (ENDot(A,A)<=d2);
}

ENbool ENCoreCollision::SpherePortal(ENCOLSPHEREHULL *sphere,ENMapBuildLoader::ENPortal *portal)
{
 //Check X
 if(sphere->pos.x+sphere->range<portal->min.x) return false;
 if(sphere->pos.x-sphere->range>portal->max.x) return false;
 //Check Y
 if(sphere->pos.y+sphere->range<portal->min.y) return false;
 if(sphere->pos.y-sphere->range>portal->max.y) return false;
 //Check Z
 if(sphere->pos.z+sphere->range<portal->min.z) return false;
 if(sphere->pos.z-sphere->range>portal->max.z) return false;
 //Finished
 return true;
}

ENbool ENCoreCollision::SpherePortal(ENVector *pos,ENfloat range,
                                     ENMapBuildLoader::ENPortal *portal)
{
 //Check X
 if(pos->x+range<portal->min.x) return false;
 if(pos->x-range>portal->max.x) return false;
 //Check Y
 if(pos->y+range<portal->min.y) return false;
 if(pos->y-range>portal->max.y) return false;
 //Check Z
 if(pos->z+range<portal->min.z) return false;
 if(pos->z-range>portal->max.z) return false;
 //Finished
 return true;
}

ENbool ENCoreCollision::SphereMap(ENCOLSPHEREHULL *sphere)
{
 //Variables
 ENuint nump=ENCoreMap::GetNumPortals();
 //Check all portals
 for(ENuint p=0;p<nump;p++)
 {
  ENMapBuildLoader::ENPortal *portal=ENCoreMap::GetPortal(p);
  if(SpherePortal(sphere,portal))
    for(ENuint t=0;t<portal->NumFaces;t++)
    {
     ENMapBase::ENMapFace *face=&portal->faces[t];
     ENVector faced[3]={portal->vertexes[face->indices[0]],
                        portal->vertexes[face->indices[1]],
	                portal->vertexes[face->indices[2]]};
     if(!face->props.Passable)
       if(ENSpherePolygonCollision(faced,sphere->pos,3,sphere->range))
         return true;
    }
 }
 return false;
}

ENbool ENCoreCollision::Sphere(ENCOLSPHEREHULL *sphere)
{
 //Check map
 if(SphereMap(sphere)) return true;
 //Finished
 return false;
}

ENbool ENCoreCollision::MoveSphere(ENCOLSPHEREHULL *sphere,ENVector2 Angles,ENfloat range)
{
 //Create steps
 ENint  numst=(ENint)ceil(fabs(range/STEPSIZE));
 if(numst==0) return false;
 ENfloat stepsize=(ENfloat)range/numst;
 //Step vector
 ENVector sv=ENVecDir(stepsize,Angles);
 //Step by step
 for(ENint a=0;a<numst;a++)
 {
  sphere->pos=sphere->pos+sv;
  if(ENCoreCollision::Sphere(sphere))
  {
   sphere->pos=sphere->pos-sv;
   return true;
  }
 }
 //Return position
 return false;
}


