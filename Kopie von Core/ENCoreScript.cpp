//---------------------------------------------------------------------------
#include "ENCoreScript.h"
#include "ENCoreMain.h"
#include "ENCoreProcesses.h"
#include "ENCoreInput.h"
#include "ENCore2D.h"
#include "ENCorePacker.h"
#include "ENCoreLoader.h"
#include "ENCoreMap.h"
#include "ENCoreMath.h"
#include "ENCoreCollision.h"
#include "ENCoreLight.h"
#include "ENCoreView.h"
#include "ENCoreSound.h"
#include "ENLensFlare.h"
//---------------------------------------------------------------------------
#pragma warn -8057

ENScriptCode *ENCoreScript::code=NULL;

ENint ENCoreScript::SizeInt;
ENint ENCoreScript::SizeFloat;
ENint ENCoreScript::SizeString;
ENint ENCoreScript::SizeVec2;
ENint ENCoreScript::SizeVec3;
ENint ENCoreScript::SizeVec4;
ENint ENCoreScript::SizeMat2;
ENint ENCoreScript::SizeMat3;
ENint ENCoreScript::SizeMat4;
ENint ENCoreScript::SizeLight;

ENbool ENCoreScript::Init()
{
 //Loader message
 ENCoreLoader::AddMsg("Initialize engine core script...");
 //Init code object
 code=new ENScriptCode();
 //Init script funcs
 return InitScriptFuncs();
}

void ENCoreScript::Free()
{
 delete code;
}

void *ENCoreScript::GetScriptVar(char *Name)
{
 ENint pos;
 //Get position of script variable
 pos=ENScriptExtern::GetExtVarPos(Name);
 if(pos==-1)
 {
  ENCoreMain::ShowError("Cannot find script variable!!!");
  return NULL;
 }
 //Add stack address
 pos+=code->GetStack();
 //return result
 return (void*)pos;
}

ENbool ENCoreScript::LoadScript(char *FileName)
{
 //Loader message
 ENCoreLoader::AddMsg("Load script...");
 //Check if code object already exists
 if(!code)
 {
  ENCoreMain::ShowError("Code object is not initialized!!!");
  return false;
 }
 //Load compiled code
 if(!code->LoadFromFile(FileName))
 {
  ENCoreMain::ShowError("Cannot load compiled code!!!");
  return false;
 }
 //Add main process
 ENCoreProcessManager::AddBaseProcess(code->GetMainFunc(),false);
 //Finished
 return true;
}

ENScriptCode *ENCoreScript::GetCodeObj()
{
 return code;
}

ENuint ENCoreScript::GetNumPaths()
{
 return code->GetNumPaths();
}

void ENCoreScript::GetPath(ENuint ind,ENScriptCode::ENPATH &path)
{
 code->GetPath(ind,path);
}

ENuint         ENCoreScript::GetNumPanels()
{
 return code->GetNumPanels();
}

ENScriptPanel *ENCoreScript::GetPanel(ENuint ind)
{
 return (ENScriptPanel*)code->GetPanelAddr(ind);
}

ENuint         ENCoreScript::GetNumTexts()
{
 return code->GetNumTexts();
}

ENScriptText *ENCoreScript::GetText(ENuint ind)
{
 return (ENScriptText*)code->GetTextAddr(ind);
}

void ENCoreScript::GetPackage(char *package)
{
 code->GetPackage(package);
}

ENint ENCoreScript::FindAction(char *Name)
{
 if(!Name[0])
   return -1;
 else
 {
  ENint res=code->GetActionPos(Name);
  if(res==-1)
  {
   char Msg[80];
   sprintf(Msg,"Cannot find object function %s!!!",Name);
   ENCoreMain::ShowError(Msg);
  }
  return res;
 }
}

ENbool ENCoreScript::InitScriptFuncs()
{
 //Get size of types
 if(!GetTypeSize())
 {
  ENCoreMain::ShowError("Cannot get sizes of script types!!!");
  return false;
 }
 //Set ShutDown function
 if(!SetScriptFunc("ShutDown",ScriptSHUTDOWN)) return false;
 //Set SetKeyDown function
 if(!SetScriptFunc("SetKeyDown",ScriptSETKEYDOWN)) return false;
 //Set SetKeyUp function
 if(!SetScriptFunc("SetKeyUp",ScriptSETKEYUP)) return false;
 //Set SetCursor function 
 if(!SetScriptFunc("SetCursor",ScriptSETCURSOR)) return false;
 //Set SetBitmap function
 if(!SetScriptFunc("SetBitmap",ScriptSETBITMAP)) return false;
 //Set SetFont function
 if(!SetScriptFunc("SetFont",ScriptSETFONT)) return false;
 //Set LoadMap function
 if(!SetScriptFunc("LoadMap",ScriptLOADMAP)) return false;
 //Set ShowMessage function
 if(!SetScriptFunc("ShowMessage",ScriptSHOWMESSAGE)) return false;
 //Set vec3_move function
 if(!SetScriptFunc("vec3_move",ScriptVEC3_MOVE)) return false;
 //Set vec3_move_collision function
 if(!SetScriptFunc("vec3_move_collision",ScriptVEC3_MOVE_COLLISION)) return false;
 //Set vec3_move_collision_smooth function
 if(!SetScriptFunc("vec3_move_collision_smooth",ScriptVEC3_MOVE_COLLISION_SMOOTH)) return false;
 //Set vec3_move_collision2 function
 if(!SetScriptFunc("vec3_move_collision2",ScriptVEC3_MOVE_COLLISION2)) return false;
 //Set vec3_move_collision_smooth2 function
 if(!SetScriptFunc("vec3_move_collision_smooth2",ScriptVEC3_MOVE_COLLISION_SMOOTH2)) return false;
 //Set object_move function
 if(!SetScriptFunc("object_move",ScriptOBJECT_MOVE)) return false;
 //Set object_move_smooth function
 if(!SetScriptFunc("object_move_smooth",ScriptOBJECT_MOVE_SMOOTH)) return false;
 //Set object_move2 function
 if(!SetScriptFunc("object_move2",ScriptOBJECT_MOVE2)) return false;
 //Set object_move_smooth2 function
 if(!SetScriptFunc("object_move_smooth2",ScriptOBJECT_MOVE_SMOOTH2)) return false;
 //Set IsKeyPressed function
 if(!SetScriptFunc("IsKeyPressed",ScriptISKEYPRESSED)) return false;
 //Set CreateLight function
 if(!SetScriptFunc("CreateLight",ScriptCREATELIGHT)) return false;
 //Set SetLight function
 if(!SetScriptFunc("SetLight",ScriptSETLIGHT)) return false;
 //Set GetLight function
 if(!SetScriptFunc("GetLight",ScriptGETLIGHT)) return false;
 //Set DeleteLight function
 if(!SetScriptFunc("DeleteLight",ScriptDELETELIGHT)) return false;
 //Set DeleteObject function
 if(!SetScriptFunc("DeleteObject",ScriptDELETEOBJECT)) return false;
 //Set GetNumProcesses function
 if(!SetScriptFunc("GetNumProcesses",ScriptGETNUMPROCESSES)) return false;
 //Set world_to_screen function
 if(!SetScriptFunc("world_to_screen",ScriptWORLD_TO_SCREEN)) return false;
 //Set screen_to_world function
 if(!SetScriptFunc("screen_to_world",ScriptSCREEN_TO_WORLD)) return false;
 //Set CreateObject function
 if(!SetScriptFunc("CreateObject",ScriptCREATEOBJECT)) return false;
 //Set CreateObjectPos function
 if(!SetScriptFunc("CreateObjectPos",ScriptCREATEOBJECTPOS)) return false;
 //Set random function
 if(!SetScriptFunc("random",ScriptRANDOM)) return false;
 //Set SphereCollisionMap function
 if(!SetScriptFunc("SphereCollisionMap",ScriptSPHERECOLLISIONMAP)) return false;
 //Set CreateSound function
 if(!SetScriptFunc("CreateSound",ScriptCREATESOUND)) return false;
 //Set DeleteSound function
 if(!SetScriptFunc("DeleteSound",ScriptDELETESOUND)) return false;
 //Set SetSound function
 if(!SetScriptFunc("SetSound",ScriptSETSOUND)) return false;
 //Set GetSound function
 if(!SetScriptFunc("GetSound",ScriptGETSOUND)) return false;
 //Set SetSoundState function
 if(!SetScriptFunc("SetSoundState",ScriptSETSOUNDSTATE)) return false;
 //Set GetModelVert function
 if(!SetScriptFunc("GetModelVert",ScriptGETMODELVERT)) return false;
 //Set lens_add function
 if(!SetScriptFunc("lens_add",ScriptLENS_ADD)) return false;
 //Set lens_create function
 if(!SetScriptFunc("lens_create",ScriptLENS_CREATE)) return false;
 //Set lens_pos function
 if(!SetScriptFunc("lens_pos",ScriptLENS_POS)) return false;
 //Set lens_delete function
 if(!SetScriptFunc("lens_delete",ScriptLENS_DELETE)) return false;
 //Set lens_color function
 if(!SetScriptFunc("lens_color",ScriptLENS_COLOR)) return false;
 //Set trace function
 if(!SetScriptFunc("trace",ScriptTRACE)) return false;
 //Finished
 return true;
}

ENbool ENCoreScript::GetTypeSize()
{
 //VARS
 ENScriptTypeClass::ENScriptDataType type;
 //Set size of integer 
 if(!ENScriptTypeClass::GetType(ENINT,&type)) return false;
 SizeInt=type.Size;
 //Set size of float
 if(!ENScriptTypeClass::GetType(ENFLOAT,&type)) return false;
 SizeFloat=type.Size;
 //Set size of string
 if(!ENScriptTypeClass::GetType(ENSTRING,&type)) return false;
 SizeString=type.Size;
 //Set size of vec2
 if(!ENScriptTypeClass::FindType("vector2",&type)) return false;
 SizeVec2=type.Size;
 //Set size of vec3
 if(!ENScriptTypeClass::FindType("vector3",&type)) return false;
 SizeVec3=type.Size;
 //Set size of vec4
 if(!ENScriptTypeClass::FindType("vector4",&type)) return false;
 SizeVec4=type.Size;
 //Set size of mat2
 if(!ENScriptTypeClass::FindType("matrix2",&type)) return false;
 SizeMat2=type.Size;
 //Set size of mat3
 if(!ENScriptTypeClass::FindType("matrix3",&type)) return false;
 SizeMat3=type.Size;
 //Set size of mat4
 if(!ENScriptTypeClass::FindType("matrix4",&type)) return false;
 SizeMat4=type.Size;
 //Set size of light
 if(!ENScriptTypeClass::FindType("light",&type)) return false;
 SizeLight=type.Size;
 //Finished
 return true;
}

ENbool ENCoreScript::SetScriptFunc(char *Name,ENScriptExtern::ENSCRIPTEXTERNFUNCTION func)
{
 //Variables
 static char MsgBuffer[128];
 //Set function
 if(!ENScriptExtern::SetExtFunc(Name,func))
 {
  sprintf(MsgBuffer,"Cannot initialize script function %s!!!",Name);
  ENCoreMain::ShowError(MsgBuffer);
  return false;
 }
 else
   return true;
}

void ENCoreScript::ScriptSHUTDOWN(ENubyte *Data,void *res)
{
 ENCoreMain::StopApplication();
}

void ENCoreScript::ScriptSETKEYDOWN(ENubyte *Data,void *res)
{
 ENint key,func;
 memcpy(&func,Data-SizeInt,SizeInt);
 memcpy(&key,Data-SizeInt*2,SizeInt);
 ENCoreInput::SetKeyFunc(key,true,func);
}

void ENCoreScript::ScriptSETKEYUP(ENubyte *Data,void *res)
{
 ENint key,func;
 memcpy(&func,Data-SizeInt,SizeInt);
 memcpy(&key,Data-SizeInt*2,SizeInt);
 ENCoreInput::SetKeyFunc(key,false,func);
}

void ENCoreScript::ScriptSETCURSOR(ENubyte *Data,void *res)
{
 ENCore2D::SetCursorBmp((char*)Data-SizeString);
}

void ENCoreScript::ScriptSETBITMAP(ENubyte *Data,void *res)
{
 ENint Src=(ENint)ENCorePacker::GetSource((char*)Data-SizeString,
                                          ENPackerBase::ENPICTURE);
 if(!Src)
 {
  char MsgBuffer[128];
  sprintf(MsgBuffer,"Cannot find bitmap %s!!!",(char*)Data-SizeString);
  ENCoreMain::ShowError(MsgBuffer);
 }
 *((ENint*)res)=Src;
}

void ENCoreScript::ScriptSETFONT(ENubyte *Data,void *res)
{
 ENint Src=(ENint)ENCorePacker::GetSource((char*)Data-SizeString,
                                          ENPackerBase::ENFONT);
 if(!Src)
 {
  char MsgBuffer[128];
  sprintf(MsgBuffer,"Cannot find font %s!!!",(char*)Data-SizeString);
  ENCoreMain::ShowError(MsgBuffer);
 }
 *((ENint*)res)=Src;
}

void ENCoreScript::ScriptLOADMAP(ENubyte *Data,void *res)
{
 ENCoreMap::Load((char*)Data-SizeString);  
}

void ENCoreScript::ScriptSHOWMESSAGE(ENubyte *Data,void *res)
{
 MessageBox(NULL,(char*)Data-SizeString,"Engine message",MB_TASKMODAL | MB_TOPMOST);
}

void ENCoreScript::ScriptVEC3_MOVE(ENubyte *Data,void *res)
{
 //Variables
 ENVector  rvec,pos;
 ENVector2 dir;
 ENfloat   range;
 //Get parameters
 dir=*((ENVector2*)(Data-SizeVec2));
 range=*((ENfloat*)(Data-SizeVec2-SizeFloat));
 pos=*((ENVector*)(Data-SizeVec2-SizeFloat-SizeVec3));
 //Start function
 rvec=ENCoreMath::MoveVec(pos,dir,range);
 memcpy(res,&rvec,SizeVec3);
}

void ENCoreScript::ScriptVEC3_MOVE_COLLISION(ENubyte *Data,void *res)
{
 //Variables
 ENVector2 Angles;
 ENfloat   range;
 ENint     p=0;
 ENCoreCollision::ENCOLSPHEREHULL sph;
 //Get parameters
 p+=SizeInt;
 memcpy(&sph.range,Data-p,4);
 p+=SizeVec2;
 memcpy(&Angles,Data-p,8);
 p+=SizeFloat;
 memcpy(&range,Data-p,4);
 p+=SizeVec3;
 memcpy(&sph.pos,Data-p,12);
 //start function
 ENCoreCollision::MoveSphere(&sph,Angles,range,NULL);
 *((ENVector*)res)=sph.pos;
}

void ENCoreScript::ScriptVEC3_MOVE_COLLISION2(ENubyte *Data,void *res)
{
 //Variables
 ENVector  dir;
 ENfloat   range;
 ENint     p=0;
 ENCoreCollision::ENCOLSPHEREHULL sph;
 //Get parameters
 p+=SizeFloat;
 memcpy(&sph.range,Data-p,SizeFloat);
 p+=SizeVec3;
 memcpy(&dir,Data-p,SizeVec3);
 p+=SizeVec3;
 memcpy(&sph.pos,Data-p,SizeVec3);
 //start function
 ENCoreCollision::MoveSphere(&sph,dir,NULL);
 *((ENVector*)res)=sph.pos;
}

void ENCoreScript::ScriptOBJECT_MOVE(ENubyte *Data,void *res)
{ 
 //VARS
 ENScriptObject *obj;
 ENfloat   range;
 ENVector2 Angles; 
 //Get parameters
 memcpy(&Angles,Data-8,8);
 memcpy(&range,Data-12,4);
 memcpy(&obj,Data-16,4);
 //start function
 if(obj) ENCoreCollision::MoveObject(obj,Angles,range,false);
}

void ENCoreScript::ScriptOBJECT_MOVE2(ENubyte *Data,void *res)
{ 
 //VARS
 ENScriptObject *obj;
 ENVector dir;
 //Get parameters
 memcpy(&dir,Data-12,12);
 memcpy(&obj,Data-16,4);
 //start function 
 if(obj) ENCoreCollision::MoveObject(obj,dir,false);
}

void ENCoreScript::ScriptOBJECT_MOVE_SMOOTH(ENubyte *Data,void *res)
{ 
 //VARS
 ENScriptObject *obj;
 ENfloat   range;
 ENVector2 Angles;
 //Get parameters
 memcpy(&Angles,Data-8,8);
 memcpy(&range,Data-12,4);
 memcpy(&obj,Data-16,4);
 //start function 
 if(obj) ENCoreCollision::MoveObject(obj,Angles,range,true);
}

void ENCoreScript::ScriptOBJECT_MOVE_SMOOTH2(ENubyte *Data,void *res)
{ 
 //VARS
 ENScriptObject *obj;
 ENVector dir;
 //Get parameters
 memcpy(&dir,Data-12,12);
 memcpy(&obj,Data-16,4);
 //start function 
 if(obj) ENCoreCollision::MoveObject(obj,dir,true);
}

void ENCoreScript::ScriptVEC3_MOVE_COLLISION_SMOOTH(ENubyte *Data,void *res)
{
 //Variables
 ENVector2 Angles;
 ENfloat   range;
 ENint     p=0;
 ENCoreCollision::ENCOLSPHEREHULL sph;
 //Get parameters
 p+=SizeInt;
 memcpy(&sph.range,Data-p,4);
 p+=SizeVec2;
 memcpy(&Angles,Data-p,8);
 p+=SizeFloat;
 memcpy(&range,Data-p,4);
 p+=SizeVec3;
 memcpy(&sph.pos,Data-p,12);
 //start function
 ENCoreCollision::MoveSphereSmooth(&sph,Angles,range,NULL);
 *((ENVector*)res)=sph.pos;
}

void ENCoreScript::ScriptVEC3_MOVE_COLLISION_SMOOTH2(ENubyte *Data,void *res)
{
 //Variables
 ENVector  dir;
 ENfloat   range;
 ENint     p=0;
 ENCoreCollision::ENCOLSPHEREHULL sph;
 //Get parameters
 p+=SizeFloat;
 memcpy(&sph.range,Data-p,SizeFloat);
 p+=SizeVec3;
 memcpy(&dir,Data-p,SizeVec3);
 p+=SizeVec3;
 memcpy(&sph.pos,Data-p,SizeVec3);
 //start function
 ENCoreCollision::MoveSphereSmooth(&sph,dir,NULL);
 *((ENVector*)res)=sph.pos;
}

void ENCoreScript::ScriptISKEYPRESSED(ENubyte *Data,void *res)
{
 //Return result
 *((ENint*)res)=(ENint)ENCoreInput::GetKeyStat(*(ENint*)(Data-4));
}

void ENCoreScript::ScriptCREATELIGHT(ENubyte *Data,void *res)
{
 //Variables
 ENScriptLight		      light;
 ENCoreLight::ENDynamicLight dlight;
 //Create light
 dlight=ENCoreLight::CreateLight();
 //Return result
 light.r=dlight.light->color.r;
 light.g=dlight.light->color.g;
 light.b=dlight.light->color.b;
 light.pos=dlight.light->pos;
 light.dir=dlight.light->dir;
 light.spotcutoff=dlight.light->spotcut;
 light.range=dlight.light->range;
 light.ID=dlight.ID;

 *((ENScriptLight*)res)=light;
}

void ENCoreScript::ScriptSETLIGHT(ENubyte *Data,void *res)
{
 //Variables
 ENScriptLight light;
 //Get param
 memcpy(&light,Data-SizeLight,SizeLight);
 //Process
 ENCoreLight::SetLight(&light);
}

void ENCoreScript::ScriptGETLIGHT(ENubyte *Data,void *res)
{
 //Variables
 ENint  ind;
 ENScriptLight  light;
 ENCoreLight::ENDynamicLight dlight;
 //Get parameter
 memcpy(&ind,Data-SizeInt,SizeInt);
 //Get light
 dlight=ENCoreLight::GetLight(ind);
 //Return result
 light.r=dlight.light->color.r;
 light.g=dlight.light->color.g;
 light.b=dlight.light->color.b;
 light.pos=dlight.light->pos;
 light.dir=dlight.light->dir;
 light.range=dlight.light->range;
 light.spotcutoff=dlight.light->spotcut;
 light.ID=dlight.ID;

 *((ENScriptLight*)res)=light;
}

void ENCoreScript::ScriptDELETELIGHT(ENubyte *Data,void *res)
{
 //Variables
 ENint  ind;
 //Get parameter
 memcpy(&ind,Data-SizeInt,SizeInt);
 //Delete light
 ENCoreLight::DeleteLight(ind);
}

void ENCoreScript::ScriptDELETEOBJECT(ENubyte *Data,void *res)
{
 //Variables
 ENScriptObject *obj;
 //Get parameter 
 memcpy(&obj,Data-4,4);
 //Call function
 if(obj) ENCoreObjects::DeleteObject((ENCoreObject*)obj->Addr);
}

void ENCoreScript::ScriptGETNUMPROCESSES(ENubyte *Data,void *res)
{
 *((ENint*)res)=(ENint)ENCoreProcessManager::GetNumProcesses();
}

void ENCoreScript::ScriptWORLD_TO_SCREEN(ENubyte *Data,void *res)
{
 //Variables
 ENVector vec;
 //Get parameter
 memcpy(&vec,Data-SizeVec3,SizeVec3);
 //Call function
 *((ENVector*)res)=ENCoreView::From3DTo2D(vec);
}

void ENCoreScript::ScriptSCREEN_TO_WORLD(ENubyte *Data,void *res)
{
 //Variables
 ENVector vec;
 //Get parameter
 memcpy(&vec,Data-SizeVec3,SizeVec3);
 //Call function
 *((ENVector*)res)=ENCoreView::From2DTo3D(vec);
}

void ENCoreScript::ScriptCREATEOBJECT(ENubyte *Data,void *res)
{
 ENCoreObjects::CreateObject((char*)Data-SizeString-SizeInt,
                             *((ENint*)(Data-SizeInt)));
}

void ENCoreScript::ScriptCREATEOBJECTPOS(ENubyte *Data,void *res)
{
 ENScriptObject *sobj=ENCoreObjects::CreateObject(
                              (char*)Data-SizeString-SizeInt-SizeVec3,
                             *((ENint*)(Data-SizeInt-SizeVec3)));
 if(sobj)
   sobj->Pos=*((ENVector*)(Data-SizeVec3));
}

void ENCoreScript::ScriptRANDOM(ENubyte *Data,void *res)
{
 *((ENint*)res)=random(*((ENint*)(Data-SizeInt)));
}

void ENCoreScript::ScriptSPHERECOLLISIONMAP(ENubyte *Data,void *res)
{
 //Variables
 ENCoreCollision::ENCOLSPHEREHULL sph;
 //Get parameter
 memcpy(&sph.range,Data-SizeFloat,SizeFloat);
 memcpy(&sph.pos,Data-SizeFloat-SizeVec3,SizeVec3);
 //Call function
 *((ENint*)res)=ENCoreCollision::SphereMap(&sph);
}

void ENCoreScript::ScriptCREATESOUND(ENubyte *Data,void *res)
{
 //Variables
 char SourceStr[1024];
 //Get parameters
 memcpy(SourceStr,Data-SizeString,SizeString);
 //Call
 *((ENSoundRec*)res)=ENCoreSound::CreateSound(SourceStr);
}

void ENCoreScript::ScriptGETSOUND(ENubyte *Data,void *res)
{
 //Variables
 ENint id;
 //Get index
 memcpy(&id,Data-SizeInt,SizeInt);
 //Call
 *((ENSoundRec*)res)=ENCoreSound::GetSound(id);
}

void ENCoreScript::ScriptSETSOUND(ENubyte *Data,void *res)
{
 //Variables
 ENSoundRec snd;
 //Get sound
 memcpy(&snd,Data-sizeof(ENSoundRec),sizeof(ENSoundRec));
 //Call
 ENCoreSound::SetSound(&snd);
}

void ENCoreScript::ScriptDELETESOUND(ENubyte *Data,void *res)
{
 //Variables
 ENint id;
 //Get index
 memcpy(&id,Data-SizeInt,SizeInt);
 //Call
 ENCoreSound::DeleteSound(id);
}

void ENCoreScript::ScriptSETSOUNDSTATE(ENubyte *Data,void *res)
{
 //Variables
 ENSoundRec snd;
 ENint state;
 //Get parameters
 memcpy(&state,Data-SizeInt,SizeInt);
 memcpy(&snd,Data-SizeInt-sizeof(ENSoundRec),sizeof(ENSoundRec));
 //Call
 ENCoreSound::SetSoundState(&snd,(ENCoreSound::SOUNDSTATE)state);
}

void ENCoreScript::ScriptGETMODELVERT(ENubyte *Data,void *res)
{
 //Variables
 ENint ind;
 ENScriptObject *obj;
 ENCoreObject   *cobj;
 //Get parameters
 memcpy(&ind,Data-SizeInt,SizeInt);
 memcpy(&obj,Data-4-SizeInt,4);
 //Call
 if(obj)
 {
  cobj=(ENCoreObject*)obj->Addr;
  *((ENVector*)res)=cobj->GetVertPos((ENuint)ind);
 }  
}

void ENCoreScript::ScriptLENS_ADD(ENubyte *Data,void *res)
{
 //Variables
 char lensflare[1024];
 char picname[1024];
 ENVector2 size;
 //Get parameters
 memcpy(&size,Data-SizeVec2,SizeVec2);
 memcpy(picname,Data-SizeString-SizeVec2,SizeString);
 memcpy(lensflare,Data-SizeString*2-SizeVec2,SizeString);
 //Call
 ENLensFlare::AddRing(lensflare,picname,size);
}

void ENCoreScript::ScriptLENS_CREATE(ENubyte *Data,void *res)
{
 //Variables
 char lensflare[1024];
 //Get parameters
 memcpy(lensflare,Data-SizeString,SizeString);
 //Call
 *((ENint*)res)=ENLensFlare::CreateLens(lensflare);
}

void ENCoreScript::ScriptLENS_POS(ENubyte *Data,void *res)
{
 //Variables
 ENVector pos;
 ENint    id;
 //Get parameters
 memcpy(&pos,Data-SizeVec3,SizeVec3);
 memcpy(&id,Data-SizeVec3-SizeInt,SizeInt);
 //Call
 ENLensFlare::SetLensPos((ENuint)id,pos);
}

void ENCoreScript::ScriptLENS_COLOR(ENubyte *Data,void *res)
{
 //Variables
 ENint    color[3];
 ENint    id;
 //Get parameters
 memcpy(color,Data-SizeInt*3,SizeInt*3);
 memcpy(&id,Data-SizeInt*4,SizeInt);
 //Call
 ENLensFlare::SetLensColor((ENuint)id,color[0],color[1],color[2]);
}

void ENCoreScript::ScriptLENS_DELETE(ENubyte *Data,void *res)
{
 //Variables
 ENint    id;
 //Get parameters
 memcpy(&id,Data-SizeInt,SizeInt);
 //Call
 ENLensFlare::DeleteLens((ENuint)id);
}

void ENCoreScript::ScriptTRACE(ENubyte *Data,void *res)
{
 //Variables
 ENint    func;
 ENint    options;
 ENVector vec2;
 ENVector vec1;
 //Get parameters
 memcpy(&func,Data-SizeInt,SizeInt);
 memcpy(&options,Data-SizeInt*2,SizeInt);
 memcpy(&vec2,Data-SizeInt*2-SizeVec3,SizeVec3);
 memcpy(&vec1,Data-SizeInt*2-SizeVec3*2,SizeVec3);
 //Call
 ENCoreCollision::Trace(vec1,vec2,options,func);
}







