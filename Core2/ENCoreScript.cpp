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
 //Set vec3_move function
 if(!SetScriptFunc("vec3_move",ScriptVEC3_MOVE)) return false;
 //Set vec3_move_collision function
 if(!SetScriptFunc("vec3_move_collision",ScriptVEC3_MOVE_COLLISION)) return false;
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
 ENCoreCollision::MoveSphere(&sph,Angles,range);
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





