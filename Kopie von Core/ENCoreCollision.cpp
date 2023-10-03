//---------------------------------------------------------------------------
#include <math.h>
#include "ENCoreCollision.h"
#include "ENCoreMap.h"
#include "ENCoreObjects.h"
#include "ENCoreProcesses.h"
//---------------------------------------------------------------------------
ENVector      ENCoreCollision::ReflectVec;
ENVector      ENCoreCollision::SurfaceVec;
ENVector      ENCoreCollision::DirectionVec;
const ENfloat ENCoreCollision::DEFAULT_STEP_SIZE=2.0f;

#define EN_TRACE_PASSABLE   1
#define EN_TRACE_OBJECTS    2
#define EN_TRACE_MAP        4
#define EN_TRACE_FIRST      8

ENbool ENCoreCollision::SphereSphere(ENVector p1,ENVector p2,ENfloat r1,ENfloat r2)
{
 //Variables
 ENVector A;
 ENfloat d2;
 //Vector from sphere 1 to sphere 2
 A=p1-p2;
 //Get max quadratic distance
 d2=r1+r2;
 d2*=d2;
 //Compare
 return (ENDot(A,A)<=d2);
}

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

ENbool ENCoreCollision::SpherePoint(ENCOLSPHEREHULL *sphere1,ENVector point)
{
 ENVector diff=sphere1->pos-point;
 return (ENDot(diff,diff)<(sphere1->range*sphere1->range));
}

ENbool ENCoreCollision::SphereBoxRaw(ENCOLSPHEREHULL *sphere,ENVector vBox[])
{
 if(sphere->pos>(vBox[0]-sphere->range))
   return (sphere->pos<(vBox[1]+sphere->range));
 else
   return false;   
}

ENbool ENCoreCollision::SphereBox(ENCOLSPHEREHULL *sphere,ENVector vBox[],ENVector *SurfaceVec)
{
 //Variables
 ENVector vecs[4];
 //Check raw
 if(!SphereBoxRaw(sphere,vBox))
   return false;
 else
 {
  //Check if sphere is inside
  if(sphere->pos>=vBox[0]&&sphere->pos<=vBox[1])
  {
   if(SurfaceVec) *SurfaceVec=ENNormalize(sphere->pos-(vBox[1]+vBox[0])/2.0f);
   return true;
  }
  //Check front plane
  vecs[0]=ENVector(vBox[0].v[0],vBox[1].v[1],vBox[0].v[2]);
  vecs[1]=ENVector(vBox[1].v[0],vBox[1].v[1],vBox[0].v[2]);
  vecs[2]=ENVector(vBox[1].v[0],vBox[0].v[1],vBox[0].v[2]);
  vecs[3]=ENVector(vBox[0].v[0],vBox[0].v[1],vBox[0].v[2]);
  if(ENSpherePolygonCollision(vecs,sphere->pos,4,sphere->range))
  {
   if(SurfaceVec) *SurfaceVec=ENVector(0,0,-1.0f);
   return true;
  }
  //Check back plane
  vecs[0].v[2]=vBox[1].v[2];
  vecs[1].v[2]=vBox[1].v[2];
  vecs[2].v[2]=vBox[1].v[2];
  vecs[3].v[2]=vBox[1].v[2];
  if(ENSpherePolygonCollision(vecs,sphere->pos,4,sphere->range))
  {
   if(SurfaceVec) *SurfaceVec=ENVector(0,0,1.0f);
   return true;
  }
  //Check left plane
  vecs[0]=ENVector(vBox[0].v[0],vBox[1].v[1],vBox[0].v[2]);
  vecs[1]=ENVector(vBox[0].v[0],vBox[1].v[1],vBox[1].v[2]);
  vecs[2]=ENVector(vBox[0].v[0],vBox[0].v[1],vBox[1].v[2]);
  vecs[3]=ENVector(vBox[0].v[0],vBox[0].v[1],vBox[0].v[2]);
  if(ENSpherePolygonCollision(vecs,sphere->pos,4,sphere->range))
  {
   if(SurfaceVec) *SurfaceVec=ENVector(-1.0f,0,0);
   return true;
  }
  //Check right plane
  vecs[0].v[0]=vBox[1].v[0];
  vecs[1].v[0]=vBox[1].v[0];
  vecs[2].v[0]=vBox[1].v[0];
  vecs[3].v[0]=vBox[1].v[0];
  if(ENSpherePolygonCollision(vecs,sphere->pos,4,sphere->range))
  {
   if(SurfaceVec) *SurfaceVec=ENVector(1.0f,0,0);
   return true;
  }
  //Check bottom plane
  vecs[0]=ENVector(vBox[0].v[0],vBox[0].v[1],vBox[0].v[2]);
  vecs[1]=ENVector(vBox[1].v[0],vBox[0].v[1],vBox[0].v[2]);
  vecs[2]=ENVector(vBox[1].v[0],vBox[0].v[1],vBox[1].v[2]);
  vecs[3]=ENVector(vBox[0].v[0],vBox[0].v[1],vBox[1].v[2]);
  if(ENSpherePolygonCollision(vecs,sphere->pos,4,sphere->range))
  {
   if(SurfaceVec) *SurfaceVec=ENVector(0,-1.0f,0);
   return true;
  }
  //Check top plane
  vecs[0].v[1]=vBox[1].v[1];
  vecs[1].v[1]=vBox[1].v[1];
  vecs[2].v[1]=vBox[1].v[1];
  vecs[3].v[1]=vBox[1].v[1];
  if(ENSpherePolygonCollision(vecs,sphere->pos,4,sphere->range))
  {
   if(SurfaceVec) *SurfaceVec=ENVector(0,1.0f,0);
   return true;
  }
  //Nothing found
  return false;
 }
}

ENbool ENCoreCollision::SphereLine(ENVector vLine[],ENVector pos,ENfloat range)
{
 ENVector vec=ENClosestPointOnLine(vLine[0],vLine[1],pos);
 return (ENDot(pos,vec)<=range*range);
}

ENbool ENCoreCollision::SphereLine(ENVector vLine[],ENCOLSPHEREHULL *sphere)
{
 ENVector vec=ENClosestPointOnLine(vLine[0],vLine[1],sphere->pos);
 ENVector p=vec-sphere->pos;
 return (ENDot(p,p)<=sphere->range*sphere->range);
}

ENbool ENCoreCollision::SphereLine(ENVector vLine[],ENVector pos,ENfloat range,ENVector &vec)
{
 vec=ENClosestPointOnLine(vLine[0],vLine[1],pos);
 ENVector p=vec-pos;
 return (ENDot(p,p)<=range*range);
}

ENbool ENCoreCollision::SpherePortal(ENCOLSPHEREHULL *sphere,ENMapBuildLoader::ENPortal *portal)
{
 ENVector vBox[2]={portal->min,portal->max};
 return SphereBoxRaw(sphere,vBox);
}

void ENCoreCollision::Trace(ENVector A,ENVector B,ENint options,ENint func)
{
 //Variables
 ENbool EnableFunc,OnlyFirst,EnablePassable,EnableMap,EnableObjects;
 ENuint nump,t,p;
 ENfloat neardist,tempdist;
 ENMapBuildLoader::ENPortal *portal;
 ENMapBase::ENMapFace       *face;
 ENVector vBox[2],fistvec,tempvec;
 ENVector vLine[2]={A,B};
 ENScriptObject *me=ENCoreObjectProcess::GetScriptMe(),*you=NULL;
 //Init values
 neardist=-1.0f;
 EnableFunc=(func>=0);
 OnlyFirst=(options&EN_TRACE_FIRST);
 EnableMap=(options&EN_TRACE_MAP);
 EnableObjects=(options&EN_TRACE_OBJECTS);
 EnablePassable=(options&EN_TRACE_PASSABLE);
 nump=ENCoreMap::GetNumPortals();
 //Check map
 if(EnableMap)
 for(p=0;p<nump;p++)
 {
  //Get portal
  portal=ENCoreMap::GetPortal(p);
  //Fill box data
  vBox[0]=portal->min;
  vBox[1]=portal->max;
  //Check if portal and line intersect
  if(ENBoxLineIntersect2(vBox,vLine))
    for(t=0;t<portal->NumFaces;t++)
    {
     //Get face data
     face=&portal->faces[t];
     ENVector faced[3]={portal->vertexes[face->indices[0]],
                        portal->vertexes[face->indices[1]],
	                portal->vertexes[face->indices[2]]};
     //Check if face and line intersect
     if(!face->props.Passable||EnablePassable)
       if(ENIntersectedPolygon(faced,vLine,3,tempvec))
       {
        if(OnlyFirst)
        {//Update first collision data
         //Calculate distance to collision point
         tempdist=ENDistance(A,tempvec);
         //Check if new collision is nearer
         if(tempdist<=neardist||neardist<0.0f)
         {
          neardist=tempdist;
          fistvec=tempvec;
         }
        }
        else//Run trace event if a function is enabled
          if(EnableFunc)
            ENCoreProcessManager::AddTraceTrigger(func,me,NULL,tempvec,false);
       }
    }
 }
 //Check object
 if(EnableObjects)
 {
  you=ENCoreObjects::Trace(vLine,func,OnlyFirst,EnablePassable,me,tempvec);
  if(OnlyFirst)
  {
   tempdist=ENDistance(tempvec,A);
   if(tempdist<=neardist||neardist<0.0f)
   {
    neardist=tempdist;
    fistvec=tempvec;
   }
    else
      you=NULL;
  }      
 }
 //Check for first contact
 if(OnlyFirst&&EnableFunc&&neardist>=0.0f)
   ENCoreProcessManager::AddTraceTrigger(func,me,you,fistvec,false);
}

ENbool ENCoreCollision::SpherePortal(ENVector *pos,ENfloat range,
                                     ENMapBuildLoader::ENPortal *portal)
{
 ENCOLSPHEREHULL sphere;
 sphere.Init(*pos,range);
 return SpherePortal(&sphere,portal);
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
       {
        ComputeReflectVector(ENNormal(faced));
        return true;
       }
    }
 }

 return false;
}

ENVector ENCoreCollision::ComputeReflectVector(ENVector vNormal)
{
 vNormal=ENNormalize(vNormal);
 SurfaceVec=vNormal;
 DirectionVec=ENNormalize(DirectionVec);
 ENfloat cosa=ENDot(vNormal,DirectionVec)*2;
 ReflectVec=DirectionVec-vNormal*cosa;
 //Finished
 return ENNormalize(ReflectVec);
}

ENbool ENCoreCollision::SphereObjects(ENCOLSPHEREHULL *sphere,void *src)
{
 return ENCoreObjects::SphereCollision(sphere->pos,sphere->range,src);
}

ENbool ENCoreCollision::Sphere(ENCOLSPHEREHULL *sphere,void *src)
{
 //Check map
 if(SphereMap(sphere))
 {
  if(src)//Add collision event
  {
   ENCoreObject *obj=(ENCoreObject*)src;
   ENScriptObject *me=obj->GetScriptObject();
   ENCoreProcessManager::AddCollisionTrigger(me->Func,me,NULL,ReflectVec,SurfaceVec);
  }
  return true;
 }
 //Check objects
 if(SphereObjects(sphere,src)) return true;
 //Finished
 return false;
}

ENbool ENCoreCollision::MoveSphere(ENCOLSPHEREHULL *sphere,ENVector2 Angles,ENfloat range,void *src)
{
 return MoveSphere(sphere,ENVecDir(range,Angles),src);
}

ENbool ENCoreCollision::MoveSphere(ENCOLSPHEREHULL *sphere,ENVector dir,void *src)
{
 //Variables
 ENfloat range=ENLength(dir);
 ENfloat sts;
 //Set step size
 sts=sphere->range/2.0f;
 if(sts>DEFAULT_STEP_SIZE) sts=DEFAULT_STEP_SIZE;
 //Set direction vector
 DirectionVec=dir;
 //Create steps
 ENint  numst=(ENint)ceil(fabs(range/sts));
 if(numst==0) return false;
 //Step vector
 ENVector sv=dir/numst;
 //Step by step
 for(ENint a=0;a<numst;a++)
 {
  sphere->pos=sphere->pos+sv;
  if(ENCoreCollision::Sphere(sphere,src))
  {
   sphere->pos=sphere->pos-sv;
   return true;
  }
 }
 //Return position
 return false;
}

ENbool ENCoreCollision::MoveSphereSmooth(ENCOLSPHEREHULL *sphere,ENVector2 Angles,ENfloat range,void *src)
{
 return MoveSphereSmooth(sphere,ENVecDir(range,Angles),src);
}

ENbool ENCoreCollision::MoveSphereSmooth(ENCOLSPHEREHULL *sphere,ENVector dir,void *src)
{
 //Variables
 ENfloat sts;
 ENfloat range=ENLength(dir);
 //Set direction vector
 DirectionVec=dir;
 //Set step size
 sts=sphere->range/2.0f;
 if(sts>DEFAULT_STEP_SIZE) sts=DEFAULT_STEP_SIZE;
 //Create steps
 ENint  numst=(ENint)ceil(fabs(range/sts));
 if(numst==0) return false;
 //Step vector
 ENVector sv=dir/numst;
 //Step by step
 for(ENint a=0;a<numst;a++)
 {
  sphere->pos=sphere->pos+sv;
  if(ENCoreCollision::Sphere(sphere,src))//If it doesn't work, try smoothing
  {
   sphere->pos=sphere->pos-sv;
   ENuint failed=0;
   //X Direction
   if(sv.v[0]==0.0f)
	 failed++;
   else
   {
	sphere->pos.v[0]+=sv.v[0];
	if(ENCoreCollision::Sphere(sphere,src))
	{
	 sphere->pos.v[0]-=sv.v[0];
	 failed++;
	}
   }
   //Y Direction
   if(sv.v[1]==0.0f)
	 failed++;
   else
   {
	sphere->pos.v[1]+=sv.v[1];
	if(ENCoreCollision::Sphere(sphere,src))
	{
	 sphere->pos.v[1]-=sv.v[1];
	 failed++;
	}
   }
   //Z Direction
   if(sv.v[2]==0.0f)
	 failed++;
   else
   {
	sphere->pos.v[2]+=sv.v[2];
	if(ENCoreCollision::Sphere(sphere,src))
	{
	 sphere->pos.v[2]-=sv.v[2];
	 failed++;
	}
   }
   //Check if all 3 attemps failed
   if(failed==3) return false;
  }
 }
 //Return position
 return true;
}

ENbool ENCoreCollision::MoveObject(ENScriptObject *obj,ENVector2 Angles,ENfloat range,ENbool smooth)
{
 //Variables
 ENCOLSPHEREHULL csp;
 ENVector SaveObjPos;
 ENbool ColResult;
 //Get sphere hull
 ((ENCoreObject*)obj->Addr)->GetSphereHull(csp.pos,csp.range);
 SaveObjPos=csp.pos;
 //Move sphere
 if(smooth)
   ColResult=MoveSphereSmooth(&csp,Angles,range,obj->Addr);
 else
   ColResult=MoveSphere(&csp,Angles,range,obj->Addr);
 //Update object position
 obj->Pos=obj->Pos+csp.pos-SaveObjPos;
 //Finished
 return ColResult;
}

ENbool ENCoreCollision::MoveObject(ENScriptObject *obj,ENVector dir,ENbool smooth)
{
 //Variables
 ENCOLSPHEREHULL csp;
 ENVector SaveObjPos;
 ENbool ColResult;
 //Get sphere hull
 ((ENCoreObject*)obj->Addr)->GetSphereHull(csp.pos,csp.range);
 SaveObjPos=csp.pos;
 //Move sphere
 if(smooth)
   ColResult=MoveSphereSmooth(&csp,dir,obj->Addr);
 else
   ColResult=MoveSphere(&csp,dir,obj->Addr);
 //Update object position
 obj->Pos=obj->Pos+csp.pos-SaveObjPos;
 //Finished
 return ColResult;
}
