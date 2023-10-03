//---------------------------------------------------------------------------
#include "ENCoreObjects.h"
#include "ENCorePacker.h"
#include "ENCoreMain.h"
#include "ENCoreShaders.h"
#include "ENCoreProcesses.h"
#include "ENCoreScript.h"
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
 //Set data
 InitData(conf);
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
  m->InterpData(Vertexes,Normals);
  m->GetBox(min,max);
 }
 else
   ((ENSpriteLoaded*)src)->GetBox(min,max);
 //Get model matrix
 glPushMatrix();
 glLoadIdentity();
 glTranslatef(data.Pos.x,data.Pos.y,data.Pos.z);
 glRotated(data.Angle.x,1,0,0);
 glRotated(data.Angle.y,0,1,0);
 glRotated(data.Angle.z,1,0,1);
 glScalef(data.Scale.x,data.Scale.y,data.Scale.z);
 glGetFloatv(GL_MODELVIEW_MATRIX,&ObjMat.val[0][0]);
 glPopMatrix();
 //Get rotation matrix
 glPushMatrix();
 glLoadIdentity();
 glRotated(-data.Angle.x,1,0,0);
 glRotated(-data.Angle.y,0,1,0);
 glRotated(-data.Angle.z,1,0,1);
 glGetFloatv(GL_MODELVIEW_MATRIX,&RotMat.val[0][0]);
 glPopMatrix();
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
  f=ENCalculateFrustumPlanes();
  glPopMatrix();
  //Calculate if object is visible
  Visible=ENBoxInFrustum(min.x,min.y,min.z,max.x,max.y,max.z,&f);
 }  
}

ENbool ENCoreObject::isVisible()
{
 return Visible;
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
 else
   ((ENSpriteLoaded*)src)->DrawRaw();
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
 else
   ((ENSpriteLoaded*)src)->Draw();
 //Reset
 glPopMatrix();
}

void ENCoreObject::DrawRaw()
{
 //Transform
 glPushMatrix();
 glMultMatrixf(&ObjMat.val[0][0]);
 //Init
 if(type==ENMODELOBJ)
  ((ENModelLoaded*)src)->DrawRaw(Vertexes);
 else
   ((ENSpriteLoaded*)src)->DrawRaw();
 //Reset
 glPopMatrix();
}

void ENCoreObject::DrawLightData(ENbool omni)
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
   if(omni)
     ENCoreShaders::SetShader(ENGENERICBUMPOMNIMODEL);
   else
     ENCoreShaders::SetShader(ENGENERICBUMPSPOTMODEL);

   ENCoreShaders::SetMatrix(&ObjMat,&RotMat);
  }
  else
  {
   if(!omni)
     ENCoreShaders::SetShader(ENGENERICSPOTMODEL);
   else
     ENCoreShaders::SetShader(ENGENERICOMNIMODEL);

   ENCoreShaders::SetMatrix(&ObjMat,NULL);
  }
  m->DrawLightData(Vertexes,Normals);  
 }
 else
   ((ENSpriteLoaded*)src)->DrawLightData();
 //Reset
 glPopMatrix();
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
    ENCoreShaders::SetShader(ENGENERICSUNBUMP);
  else
    ENCoreShaders::SetShader(ENGENERICSUN);

  m->DrawLightData(Vertexes,Normals);
 }
 else
   ((ENSpriteLoaded*)src)->DrawLightData();
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
 data.Mode=EN_SHOW_NORMAL;
 data.Hull=COL_HULL_SPHERE;
 data.Red=255;
 data.Green=255;
 data.Blue=255;
 data.Alpha=255;
 data.CurrentFrame=0;
 data.CurrentSkin=0;
 data.Event=-1;
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

void   ENCoreObjects::DrawGroundData()
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

void   ENCoreObjects::DrawRawData()
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
  //Draw raw data
  obj->DrawRaw();
 }
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
