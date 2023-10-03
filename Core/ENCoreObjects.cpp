//---------------------------------------------------------------------------
#include <math.h>
#include "ENCoreObjects.h"
#include "ENCorePacker.h"
#include "ENCoreMain.h"
#include "ENCoreShaders.h"
#include "ENCoreProcesses.h"
#include "ENCoreScript.h"
#include "ENCoreCollision.h"
#include "ENCoreLight.h"
#include "ENCoreMap.h"
#include "ENCoreAlphaBuffer.h"
#include "ENCoreView.h"
//---------------------------------------------------------------------------
//////////////////////////////////////////////////////////////
//// Engine core object
//////////////////////////////////////////////////////////////
ENCoreObject::ENCoreObject(void *src,ENObjType type,
                           ENMapBase::ENMapObject *conf,ENint addr)
{
 //Copy values
 this->src=src;
 this->type=type;
 //Init values
 Visible=true;
 LastFrame=-1.0f;
 //Set data
 InitData(conf);
 //Set function
 data.Func=addr;
 //If type is model
 if(type==ENMODELOBJ)
 {
  ENModelLoaded *m=(ENModelLoaded*)src;
  ENModelLoaded::ENLoadedHeader h;
  m->GetHeader(h);
  Vertexes=new ENVector[h.NumVertexes];
  Normals=new ENVector[h.NumVertexes];
 }
 else
 {
  Vertexes=NULL;
  Normals=NULL;
 }
 //Create object process
 ProcessID=ENCoreProcessManager::AddObjectProcess(addr,&data,NULL,false);
}

ENCoreObject::~ENCoreObject()
{
 //Delete process, if needed
 if(ProcessID)
   ENCoreProcessManager::FreeProcess(ProcessID);
 //Update processes
 ENCoreProcessManager::CheckProcessForObject(&data);
 //If object is a model, delete model data
 if(type==ENMODELOBJ)
 {
  if(Vertexes) delete[] Vertexes;
  if(Normals)  delete[] Normals;
 }
}

void ENCoreObject::Process()
{
 //Get box
 if(type==ENMODELOBJ)
 {
  ENModelLoaded *m=(ENModelLoaded*)src;
  m->SetCurrentFrame(data.CurrentFrame);
  if(data.CurrentFrame!=LastFrame)
  {
   m->InterpData(Vertexes,Normals);
   LastFrame=data.CurrentFrame;
  }
  m->GetBox(min,max);
 }
 else
   ((ENSpriteLoaded*)src)->GetBox(min,max);
 //Check for view flags
 ENVector2 ViewAngle;
 if(data.ViewX||data.ViewY)
   ViewAngle=ENPolar(ENCoreView::GetCamPos()-data.Pos);
 //Get model matrix
 glPushMatrix();
 glLoadIdentity();
 glTranslatef(data.Pos.v[0],data.Pos.v[1],data.Pos.v[2]);
 if(data.ViewY)
   glRotatef(-ViewAngle.v[1],0,1,0);
 else
   glRotatef(-data.Angle.v[1],0,1,0);
 if(data.ViewX)
   glRotatef( ViewAngle.v[0],1,0,0);
 else
   glRotatef( data.Angle.v[0],1,0,0);

 glRotatef( data.Angle.v[2],0,0,1);
 glScalef(data.Scale.v[0],data.Scale.v[1],data.Scale.v[2]);
 glGetFloatv(GL_MODELVIEW_MATRIX,&ObjMat.val[0][0]);
 glPopMatrix();
 //Get rotation matrix
 glPushMatrix();
 glLoadIdentity();
 if(data.ViewY)
   glRotatef( ViewAngle.v[1],0,1,0);
 else
   glRotatef( data.Angle.v[1],0,1,0);
 if(data.ViewX)
   glRotatef(-ViewAngle.v[0],1,0,0);
 else
   glRotatef(-data.Angle.v[0],1,0,0);

 glRotatef(-data.Angle.v[2],0,0,1);
 //Get rotation matrix
 ENfloat rmat[4][4];
 glGetFloatv(GL_MODELVIEW_MATRIX,&rmat[0][0]);
 memcpy(RotMat.val[0],rmat[0],3*sizeof(ENfloat));
 memcpy(RotMat.val[1],rmat[1],3*sizeof(ENfloat));
 memcpy(RotMat.val[2],rmat[2],3*sizeof(ENfloat));
 //Reset matrix
 glPopMatrix();
}

ENfloat ENCoreObject::ZDist()
{
 ENVector tmp;

 if(type==ENMODELOBJ)
 {//Model
  glPushMatrix();
  glMultMatrixf(&ObjMat.val[0][0]);
  tmp=(min+max)/2.0f;
  tmp=ENCoreView::From3DTo2D3(tmp);
  glPopMatrix();
  return tmp.v[2];
 }
 else
 {//Sprite
  tmp=data.Pos;
  tmp=ENCoreView::From3DTo2D3(tmp);
  return tmp.v[2];
 }
}

void   ENCoreObject::CalculateVisible()
{
 //Variables
 ENEngineFrustum f;
 //Check special case
 if(!data.Visible)
   Visible=false;
 else
 {
  //Calculate new frustum
  glPushMatrix();
  glMultMatrixf(&ObjMat.val[0][0]);
  ENCalculateFrustumPlanes(f);
  glPopMatrix();
  //Calculate if object is visible
  Visible=ENBoxInFrustum(min.v[0],min.v[1],min.v[2],max.v[0],max.v[1],max.v[2],&f);
 }  
}

ENbool ENCoreObject::isVisible()
{
 return Visible;
}

ENbool ENCoreObject::isShadow()
{
 return (ENbool)data.Shadow;
}

ENbool ENCoreObject::isAlpha()
{
 return (data.Mode==EN_SHOW_ALPHA || data.Alpha<255);
}

void   ENCoreObject::DrawGround()
{
 //Set ground color
 if(data.Lighting)
   glColor3f(0,0,0);
 else
   glColor3f(1,1,1);
 //Transform
 glPushMatrix();
 glMultMatrixf(&ObjMat.val[0][0]);
 //Init
 if(type==ENMODELOBJ)
   ((ENModelLoaded*)src)->DrawGround(Vertexes);
 //Reset
 glPopMatrix();
}

void   ENCoreObject::DrawTex()
{
 //Set color
 glColor3ub(data.Red,data.Green,data.Blue);
 //Transform
 glPushMatrix();
 glMultMatrixf(&ObjMat.val[0][0]);
 //Init
 if(type==ENMODELOBJ)
   ((ENModelLoaded*)src)->DrawTex(Vertexes);
 //Reset
 glPopMatrix();
}

ENbool ENCoreObject::DrawRaw()
{
 //Variables
 ENEngineFrustum f;
 //Check type
 if(type!=ENMODELOBJ) return false;
 //Transform
 glPushMatrix();
 glMultMatrixf(&ObjMat.val[0][0]);
 //Calculate frustum planes
 ENCalculateFrustumPlanes(f);
 //Check frustum
 if(!ENBoxInFrustum(min.v[0],min.v[1],min.v[2],max.v[0],max.v[1],max.v[2],&f)) return false;
 //Init
 ((ENModelLoaded*)src)->DrawRaw(Vertexes);
 //Reset
 glPopMatrix();
 //Finished
 return true;
}

void ENCoreObject::DrawLightData(ENbool omni)
{
 if(!Visible) return;
 //Transform
 glPushMatrix();
 glMultMatrixf(&ObjMat.val[0][0]);
 //Init
 if(type==ENMODELOBJ)
 {
  ENModelLoaded *m=(ENModelLoaded*)src;
  if(m->isCurSkinBump())
  {
   if(omni)
     ENCoreShaders::SetGenericBumpModelOmni();
   else
     ENCoreShaders::SetGenericBumpModelSpot();

   ENCoreShaders::SetMatrix(&ObjMat,&RotMat);
  }
  else
  {
   if(!omni)
     ENCoreShaders::SetGenericLightModelSpot();
   else
     ENCoreShaders::SetGenericLightModelOmni();

   ENCoreShaders::SetMatrix(&ObjMat,NULL);
  }
  m->DrawLightData(Vertexes,Normals,GL_TEXTURE1_ARB);
 }
 //Reset
 glPopMatrix();
}

void   ENCoreObject::CalculateSpriteColor(ENbool isSun,ENVector *SunColor)
{
 if(data.Lighting)
 {
  //Variables
  ENuint    l,NumLights;
  ENLight   light;
  ENVector4 lcolor;
  ENVector4 mcolor=ENVector4((ENfloat)data.Red/255.0f,
                           (ENfloat)data.Green/255.0f,
                           (ENfloat)data.Blue/255.0f,
                           (ENfloat)data.Alpha/255.0f);
  //Init light color
  if(isSun)
    lcolor=ENVector4(SunColor->v[0],SunColor->v[1],SunColor->v[2],1.0f);
  else
    lcolor=ENVector4(0.0f,0.0f,0.0f,1.0f);
  //Get number of lights
  NumLights=ENCoreLight::GetNumLights();
  //Calculate light color
  for(l=0;l<NumLights;l++)
    if(ENCoreLight::IsLightVisible(l))
    {
     //Get light
     ENCoreLight::GetLight(l,light);
     //Calculate lightcolor for this light
     ENVector lc=light.CalculateLightColor(data.Pos);
     //Add lightcolor
     lcolor.v[0]+=lc.v[0];
     lcolor.v[1]+=lc.v[1];
     lcolor.v[2]+=lc.v[2];
    }
  //Set light color
  if(lcolor.v[0]>1.0f) lcolor.v[0]=1.0f;
  if(lcolor.v[1]>1.0f) lcolor.v[1]=1.0f;
  if(lcolor.v[2]>1.0f) lcolor.v[2]=1.0f;
  lcolor=mcolor*lcolor;
  glColor4fv((ENfloat*)&lcolor);
 }
 else
   glColor3ub(data.Red,data.Green,data.Blue);
}

void   ENCoreObject::DrawSprite(ENbool isSun,ENVector *SunColor,
                                ENbool enablealpha)
{
 //Variables
 ENSpriteLoaded *sprite;
 //Check if object is a sprite
 if(type==ENSPRITEOBJ&&Visible)
 {
  //Transform
  glPushMatrix();
  glMultMatrixf(&ObjMat.val[0][0]);
  //Set color
  CalculateSpriteColor(isSun,SunColor);
  //Get sprite
  sprite=(ENSpriteLoaded*)src;
  //Set current frame
  sprite->SetCurrentFrame(data.CurrentFrame);
  //Transparency flags
  if(data.Mode==EN_SHOW_TRANSPARENT)
  {
   glEnable(GL_ALPHA_TEST);
   glAlphaFunc(GL_GREATER,0.4f);
  }
  //Draw sprite
  sprite->DrawInterp(enablealpha);
  //Reset transparency flags
  if(data.Mode==EN_SHOW_TRANSPARENT)
    glDisable(GL_ALPHA_TEST);
  //Pop matrix
  glPopMatrix();
 }
}

void   ENCoreObject::DrawSpriteAlpha(ENbool isSun,ENVector *SunColor)
{
 //Activate texture
 ENGLExtension::ActiveTexture(GL_TEXTURE1_ARB);
 glEnable(GL_TEXTURE_2D);
 //Activate shader
 ENCoreShaders::SetSpriteShader();
 //Draw sprite
 DrawSprite(isSun,SunColor,true);
 //Reset texture
 ENGLExtension::ActiveTexture(GL_TEXTURE1_ARB);
 glDisable(GL_TEXTURE_2D);
 ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
 //Reset shader
 ENCoreShaders::ResetShader();
}

void   ENCoreObject::GetSphereHull(ENVector &pos,ENfloat &range)
{
 //Get individual hull
 if(type==ENMODELOBJ)
 {
  //Get model sphere
  ((ENModelLoaded*)src)->GetSphere(pos,range,!data.StaticHull);
  //Scale position
  pos.v[0]*=data.Scale.v[0];
  pos.v[1]*=data.Scale.v[1];
  pos.v[2]*=data.Scale.v[2];
 }
 else
 {
  pos=ENVector(0.0f,0.0f,0.0f);
  range=10.0f;
 }
 //Transform hull
 pos=pos+data.Pos;
 range*=((data.Scale.v[0]+data.Scale.v[1]+data.Scale.v[2])/3.0f);
}

void   ENCoreObject::GetBoxHull(ENVector vBox[])
{
 //Get individual hull
 if(type==ENMODELOBJ)
 {
  if(data.StaticHull)
    ((ENModelLoaded*)src)->GetBoxStatic(vBox[0],vBox[1]);
  else
    ((ENModelLoaded*)src)->GetBox(vBox[0],vBox[1]);    
 }
 else
 {
  vBox[0]=ENVector(-10.0f,-10.0f,-10.0f);
  vBox[1]=ENVector( 10.0f, 10.0f, 10.0f);
 }
 //Scale
 ENVector bsize=(vBox[1]-vBox[0])/2.0f;
 ENVector vpos=(vBox[0]+vBox[1])/2.0f+data.Pos;

 bsize.v[0]*=data.Scale.v[0];
 bsize.v[1]*=data.Scale.v[1];
 bsize.v[2]*=data.Scale.v[2];

 vBox[0]=vpos-bsize;
 vBox[1]=vpos+bsize;
}

ENbool ENCoreObject::SphereCollision(ENVector pos,ENfloat range,ENVector &SurfaceVec)
{
 //Variables
 ENVector p;
 ENfloat  r;
 ENVector  vBox[2];
 ENCoreCollision::ENCOLSPHEREHULL sphere;
 //Check for passable
 if(data.Passable) return false;
 //Set sphere
 sphere.Init(pos,range);
 //Check for collision
 switch(data.Hull)
 {
  case EN_CORE_HULL_SPHERE:
    GetSphereHull(p,r);
    if(!ENCoreCollision::SphereSphere(p,pos,r,range))
      return false;
    else
    {
     SurfaceVec=ENNormalize(pos-data.Pos);
     return true;
    }
  case EN_CORE_HULL_BOX:
    GetBoxHull(vBox);
    //Test collision
    return ENCoreCollision::SphereBox(&sphere,vBox,&SurfaceVec);
  case EN_CORE_HULL_POLYGON:
    //GetBoxHull(vBox);
    //if(!ENCoreCollision::SphereBoxRaw(&sphere,vBox))
      //return false;
  default:
    return false;
 };
}

ENbool ENCoreObject::LineCollision(ENVector vLine[],ENVector &pos,
                                   ENbool ignorePassable)
{
 //Variables
 ENVector hullpos,intersp,diff;
 ENVector hullbox[2];
 ENfloat  alpha,range,d,l;
 //Check for passable
 if(!ignorePassable&&data.Passable) return false;
 //Check for intersection
 switch(data.Hull)
 {
  case EN_CORE_HULL_SPHERE:
   //Get hull
   GetSphereHull(hullpos,range);
   //Check for collision
   if(!ENCoreCollision::SphereLine(vLine,hullpos,range,intersp)) return false;
   //Check for start
   if((ENDistance(vLine[0],hullpos)<=range))
   {
    pos=vLine[0];
    return true;
   }
   //Calculate intersection position
   pos=intersp;
   d=ENDistance(hullpos,intersp);
   alpha=acos(d/range);
   l=sin(alpha)*range;
   diff=vLine[0]-vLine[1];
   pos=ENNormalize(diff)*l+pos;
   return true;
  case EN_CORE_HULL_BOX:
    //Get hull
    GetBoxHull(hullbox);
    //Check collision
    return ENBoxLineIntersect2(hullbox,vLine,pos);
  default:
    return false;
 }
}

ENScriptObject *ENCoreObject::GetScriptObject()
{
 return &data;
}

ENVector ENCoreObject::GetVertPos(ENuint ind)
{
 if(type==ENMODELOBJ)
 {
  ENModelLoaded::ENLoadedHeader h;
  ENModelLoaded *m=(ENModelLoaded*)src;
  m->SetCurrentFrame(data.CurrentFrame);
  ///////////////////
  ENVector v;
  m->InterpVertex(ind,v);
  ///////////////////
  ENVector4 r=ENTrans(ObjMat)*ENVector4(v.v[0],v.v[1],v.v[2],1.0f);
  return ENVector(r.v[0],r.v[1],r.v[2]);
 }
 else
   return ENVector(0.0f,0.0f,0.0f);
}

void ENCoreObject::DrawSunData()
{
 //Transform
 glPushMatrix();
 glMultMatrixf(&ObjMat.val[0][0]);
 //Init
 if(type==ENMODELOBJ)
 {
  ENModelLoaded *m=(ENModelLoaded*)src;
  if(m->isCurSkinBump())
  {
   glMatrixMode(GL_TEXTURE);
   glLoadMatrixf(&RotMat.val[0][0]);
   glMatrixMode(GL_MODELVIEW);
   ENCoreShaders::SetGenericSunBump();
  }
  else
    ENCoreShaders::SetGenericSun();

  m->DrawLightData(Vertexes,Normals,GL_TEXTURE0_ARB);
 }
 //Reset
 glPopMatrix();
}

void ENCoreObject::InitData(ENMapBase::ENMapObject *conf)
{
 data.Visible=true;
 data.Passable=false;
 data.ViewX=false;
 data.ViewY=false;
 data.Lighting=true;
 data.DepthBuffer=true;
 data.CullFace=true;
 data.Shadow=true;
 data.StaticHull=true;
 data.Mode=EN_SHOW_NORMAL;
 data.Hull=EN_CORE_HULL_SPHERE;
 data.Red=255;
 data.Green=255;
 data.Blue=255;
 data.Alpha=255;
 data.CurrentFrame=0;
 data.CurrentSkin=0;
 data.Addr=(void*)this; 
 if(!conf)
 {
  data.Pos=ENVector(0.0f,0.0f,0.0f);
  data.Angle=ENVector(0.0f,0.0f,0.0f);
  data.Scale=ENVector(1.0f,1.0f,1.0f);
 }
 else
 {
  data.Pos=conf->Pos;
  conf->Angle.v[1]*=-1.0f;
  data.Angle=conf->Angle;
  data.Scale=conf->Scale;
 }
}
//////////////////////////////////////////////////////////////
//// Engine core object's manager
//////////////////////////////////////////////////////////////
const ENuint ENCoreObjects::AllocP=0x80;
const ENuint ENCoreObjects::FreeP=0xFF;
ENArrayBasic *ENCoreObjects::objects=NULL;

void ENCoreObjects::Init()
{
 objects=new ENArrayBasic(sizeof(ENCoreObject*),AllocP,FreeP);
}

void ENCoreObjects::Free()
{
 Clear();
 delete objects;
}

void ENCoreObjects::Clear()
{
 //Variables
 ENuint numobjs,a;
 ENCoreObject *obj;
 //Delete all objects
 numobjs=objects->GetNum();
 for(a=0;a<numobjs;a++)
 {
  objects->GetElement(a,(ENubyte*)&obj);
  delete obj;
 }
 //Delete all list entries
 objects->Clear();
}

void ENCoreObjects::Idle()
{
 //Variables
 ENuint numobjs,a;
 ENCoreObject *obj;
 //Process all objects
 numobjs=objects->GetNum();
 for(a=0;a<numobjs;a++)
 {
  objects->GetElement(a,(ENubyte*)&obj);
  obj->Process();
 }
}

void ENCoreObjects::UpdateObjects(ENuint num,ENMapBase::ENMapObject *mobjs)
{
 //Variables
 void *src;
 ENCoreObject *obj;
 ENCoreObject::ENObjType type;
 //Delete all objects
 Clear();
 //Load new objects
 for(ENuint a=0;a<num;a++)
 {
  //Get data for object
  src=GetObjData(mobjs[a].Name,type);
  //If data has been found, add object
  if(src)
  {
   ENint FuncAddr=ENCoreScript::FindAction(mobjs[a].Func);
   obj=new ENCoreObject(src,type,mobjs+a,FuncAddr);
   objects->AddElement((ENubyte*)&obj);
  }
 }
}

ENScriptObject *ENCoreObjects::CreateObject(char *Name,ENint func)
{
 //Variables
 void *src;
 ENCoreObject *obj,*tempobj;
 ENScriptObject *me;
 ENCoreObject::ENObjType type;
 //Get data for object
 src=GetObjData(Name,type);
 //If data has been found, add object
 if(src)
 {
  obj=new ENCoreObject(src,type,NULL,func);
  objects->AddElement((ENubyte*)&obj);
  //When me is defined
  me=ENCoreObjectProcess::GetScriptMe();
  if(me)
    ENCoreProcessManager::AddSimpleTrigger(me->Func,me,obj->GetScriptObject(),
                                           ENCoreObjectProcess::ENCREATED);
   return obj->GetScriptObject();
 }
 return NULL;
}

void ENCoreObjects::DrawGroundData()
{
 //Variables
 ENCoreObject *obj;
 ENuint a,numobjs;
 //Draw ground data of objects
 numobjs=objects->GetNum();
 for(a=0;a<numobjs;a++)
 {
  //Get object
  objects->GetElement(a,(ENubyte*)&obj);
  //Calculate if object is visible
  obj->CalculateVisible();
  //If object is visible, draw ground data
  if(obj->isVisible())
    obj->DrawGround();
 }
}

ENbool ENCoreObjects::DrawShadowData()
{
 //Variables
 ENCoreObject *obj;
 ENuint a,numobjs;
 ENbool res=false;
 //Draw raw data of objects
 numobjs=objects->GetNum();
 for(a=0;a<numobjs;a++)
 {
  //Get object
  objects->GetElement(a,(ENubyte*)&obj);
  //Draw shadow data
  if(obj->isShadow())
    if(obj->DrawRaw())
      res=true;
 }
 //Finished
 return res;
}

void   ENCoreObjects::DrawTexData()
{
 //Variables
 ENCoreObject *obj;
 ENuint a,numobjs;
 //Draw texture data of objects
 numobjs=objects->GetNum();
 for(a=0;a<numobjs;a++)
 {
  //Get object
  objects->GetElement(a,(ENubyte*)&obj);
  //Draw texture data
  if(obj->isVisible())
    obj->DrawTex();
 }
}

void   ENCoreObjects::DrawSprites()
{
 //Variables
 ENCoreObject *obj;
 ENuint a,numobjs;
 ENbool isSun;
 ENubyte sc[4];
 ENVector SunColor;
 //Get sun flag
 isSun=ENCoreMap::isSun(sc);
 SunColor=ENVector((ENfloat)sc[0]/255.0f,(ENfloat)sc[1]/255.0f,(ENfloat)sc[2]/255.0f);
 //Activate texture
 ENGLExtension::ActiveTexture(GL_TEXTURE1_ARB);
 glEnable(GL_TEXTURE_2D);
 //Activate shader
 ENCoreShaders::SetSpriteShader();
 //Get number of objects
 numobjs=objects->GetNum();
 //Draw object if it's a sprite
 for(a=0;a<numobjs;a++)
 {
  //Get object
  objects->GetElement(a,(ENubyte*)&obj);
  //Draw texture data
  if(obj->isVisible())
  {
   if(!obj->isAlpha())
     obj->DrawSprite(isSun,&SunColor,false);
   else
   {
    //Config alpha sprite object
    ENAlphaSprite *aspr=new ENAlphaSprite;
    aspr->isSun=isSun;
    aspr->SunColor=SunColor;
    aspr->sprite=(void*)obj;
    //Add alpha sprite object
    ENCoreAlphaBuffer::AddAlphaObj(ENCoreAlphaBuffer::ENALPHASPRITE,
                                   obj->ZDist(),(ENint)aspr);
   }
  }
 }
 //Reset texture
 ENGLExtension::ActiveTexture(GL_TEXTURE1_ARB);
 glDisable(GL_TEXTURE_2D);
 ENGLExtension::ActiveTexture(GL_TEXTURE0_ARB);
 //Reset shader
 ENCoreShaders::ResetShader();
}

void   ENCoreObjects::DrawSunData()
{
 //Variables
 ENCoreObject *obj;
 ENuint a,numobjs;
 //Draw raw data of objects
 numobjs=objects->GetNum();
 for(a=0;a<numobjs;a++)
 {
  //Get object
  objects->GetElement(a,(ENubyte*)&obj);
  //Draw light data
  if(obj->isVisible())
    obj->DrawSunData();
 }
}

void   ENCoreObjects::DrawLightData(ENbool omni)
{
 //Variables
 ENCoreObject *obj;
 ENuint a,numobjs;
 //Draw raw data of objects
 numobjs=objects->GetNum();
 for(a=0;a<numobjs;a++)
 {
  //Get object
  objects->GetElement(a,(ENubyte*)&obj);
  //Draw light data
  if(obj->isVisible())
    obj->DrawLightData(omni);
 }
}

ENuint ENCoreObjects::GetNum()
{
 return objects->GetNum();
}

void ENCoreObjects::DeleteObject(ENCoreObject *object)
{
 //Variables
 ENuint numobjs,a;
 ENCoreObject *obj;
 //Process all objects
 numobjs=objects->GetNum();
 for(a=0;a<numobjs;a++)
 {
  objects->GetElement(a,(ENubyte*)&obj);
  if(object==obj)
  {
   objects->DelElement(a);
   delete object;
   return;
  }
 }
}

ENbool ENCoreObjects::SphereCollision(ENVector pos,ENfloat range,void *src)
{
 //Variables
 ENCoreObject *obj,*sobj;
 ENScriptObject *me,*you;
 ENuint a,numobjs;
 ENVector SurfaceVec;
 //Get object
 sobj=(ENCoreObject*)src;
 //Check all objects
 numobjs=objects->GetNum();
 for(a=0;a<numobjs;a++)
 {
  //Get object
  objects->GetElement(a,(ENubyte*)&obj);
  //Check Object
  if(obj!=sobj)
    if(obj->SphereCollision(pos,range,SurfaceVec))
    {
     //Compute reflect vector
     ENVector ReflectVec=ENCoreCollision::ComputeReflectVector(SurfaceVec);
     //Add touch event
     me=obj->GetScriptObject();
     if(sobj)
       you=sobj->GetScriptObject();
     else
       you=NULL;
     ENCoreProcessManager::AddSimpleTrigger(me->Func,me,you,ENCoreObjectProcess::ENTOUCH);
     //Add collision event
     if(you)
       ENCoreProcessManager::AddCollisionTrigger(you->Func,you,me,ReflectVec,SurfaceVec);
     //Return collision true
     return true;
    }
 }

 return false;
}

ENScriptObject *ENCoreObjects::Trace(ENVector vLine[],ENint func,ENbool OnlyFirst,
                                     ENbool EnablePassable,ENScriptObject *me,ENVector &pos)
{
 //Variables
 ENuint         numobjs,a;
 ENCoreObject   *obj;
 ENVector       tempvec;
 ENfloat        tempdist,neardist=-1.0f;
 ENScriptObject *nearobj=NULL,*you;
 //Process all objects
 numobjs=objects->GetNum();
 for(a=0;a<numobjs;a++)
 {
  //Get object
  objects->GetElement(a,(ENubyte*)&obj);
  //Get script object
  you=obj->GetScriptObject();
  //Check, that object is not source object
  if(me!=obj->GetScriptObject())
    if(obj->LineCollision(vLine,tempvec,EnablePassable))//Check for intersection
    {
     if(OnlyFirst)
     {
      //Calculate quadratic(faster) distance to collision point
      tempdist=ENDistance(tempvec,vLine[0]);
      //Check if new collision is nearer
      if(tempdist<=neardist||neardist==-1.0f)
      {
       neardist=tempdist;
       pos=tempvec;
       nearobj=obj->GetScriptObject();
      }
     }
     else
       if(func>=0)//Trace event
         ENCoreProcessManager::AddTraceTrigger(func,me,you,tempvec,false);
     //Traced event
     ENCoreProcessManager::AddTraceTrigger(you->Func,you,me,tempvec,true);
    }
 }
 //Finished
 return nearobj;
}

void *ENCoreObjects::GetObjData(char *SrcName,ENCoreObject::ENObjType &type)
{
 //Variables
 void *res;
 //Init type
 type=ENCoreObject::ENMODELOBJ;
 //Find source
 res=ENCorePacker::GetSource(SrcName,ENPackerBase::ENMODEL);
 if(!res)
 {
  type=ENCoreObject::ENSPRITEOBJ;
  res=ENCorePacker::GetSource(SrcName,ENPackerBase::ENSPRITE);
  if(!res)
  {
   ENCoreMain::ShowError("Cannot find resource for object!!!");
   return NULL;
  }
 }          
 return res;
}

