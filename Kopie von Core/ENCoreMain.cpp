//---------------------------------------------------------------------------
#include "ENCoreMain.h"
#include "ENCoreLoader.h"
#include "ENCoreWindow.h"
#include "ENCoreScript.h"
#include "ENCoreGL.h"
#include "ENCoreView.h"
#include "ENCoreInput.h"
#include "ENCoreProcesses.h"
#include "ENCorePacker.h"
#include "ENCore2D.h"
#include "ENCoreMap.h"
#include "ENCoreLight.h"
#include "ENCoreSound.h"
#include "ENCoreShaders.h"
#include "ENCoreRenderer.h"
#include "ENCoreObjects.h"
#include "ENCoreConfig.h"
#include "ENLensFlare.h"
#include "ENCoreAlphaBuffer.h"
//---------------------------------------------------------------------------
ENbool   ENCoreMain::AppRunning=false;
ENint   *ENCoreMain::Fps=NULL;
ENfloat *ENCoreMain::TimeFac=NULL;
char     ENCoreMain::SysDir[MAX_PATH];

ENbool ENCoreMain::Run(HINSTANCE AppInst)
{
 //Initialize everything
 if(!Init(AppInst)) return false;
 //Start idle loop
 Idle();
 //Free everything
 Free();
 //Finished
 return true;
}

void ENCoreMain::ShowError(char *error)
{
 MessageBox(NULL,
            error,"Engine Error",
            MB_OK | MB_ICONERROR | MB_TASKMODAL | MB_TOPMOST);
}

void ENCoreMain::StopApplication()
{
 AppRunning=false;
}

ENbool ENCoreMain::SearchFile(char *FileName,char *res)
{
 //Variables
 ENScriptCode::ENPATH path;
 //Try system directory
 sprintf(res,"%s\\%s",SysDir,FileName);
 if(CheckFile(res)) return true;
 //Try script defined paths
 for(ENuint a=0;a<ENCoreScript::GetNumPaths();a++)
 {
  //Get script directory
  ENCoreScript::GetPath(a,path);
  //Create new path
  sprintf(res,"%s\\%s\\%s",SysDir,path.Path,FileName);
  //Check if file can now be found
  if(CheckFile(res)) return true;
 }
 //Try original version
 strcpy(res,FileName);
 if(CheckFile(res)) return true;
 //Finished without result
 *res=0;
 return false;
}

ENfloat ENCoreMain::GetTimeFac()
{
 return *TimeFac;
}

ENbool ENCoreMain::Init(HINSTANCE AppInst)
{
 //Get system directory
 GetCurrentDirectory(MAX_PATH,SysDir);
 //Start randomize
 randomize();
 //Init loader window
 if(!ENCoreLoader::Init(AppInst)) return false;
 //Init script
 ENCoreScript::Init();
 //Load config
 if(!ENCoreConfig::Load()) return false;
 //Register script variables
 if(!RegisterScript())
 {
  ShowError("Cannot init script variables of main object!!!");
  return false;
 }
 //Init window
 if(!ENCoreWindow::Init(AppInst)) return false;
 //Init shaders
 if(!ENCoreShaders::Init()) return false;
 //Init renderer
 ENCoreRenderer::Init();
 //Init input
 if(!ENCoreInput::Init()) return false;
 //Init view
 if(!ENCoreView::Init()) return false;
 //Init packer
 ENCorePacker::Init();
 //Init process manager
 if(!ENCoreProcessManager::Init()) return false;
 //Load script
 if(!ENCoreScript::LoadScript("ENGINECODE")) return false;
 //Init 2D
 if(!ENCore2D::Init()) return false;
 //Init objects
 ENCoreObjects::Init();
 //Init sound
 ENCoreSound::Init();
 //Load std package
 if(!LoadStdPack()) return false;
 //Init map
 ENCoreMap::Init();
 //Free loader window
 ENCoreLoader::Free();
 //Show window
 ENCoreWindow::Show();
 //Finished
 return true;
}

ENbool ENCoreMain::RegisterScript()
{
 //Fps
 Fps=(ENint*)ENCoreScript::GetScriptVar("Fps");
 if(!Fps) return false;
 //TimeFac
 TimeFac=(ENfloat*)ENCoreScript::GetScriptVar("TimeFac");
 if(!TimeFac) return false;
 //Finished
 return true;
}

ENbool ENCoreMain::LoadStdPack()
{
 //Variables
 char Package[MAX_PATH];
 //Get package
 ENCoreScript::GetPackage(Package);
 //Try to load stdandard package
 return ENCorePacker::LoadStdPackage(Package);
}

void ENCoreMain::Free()
{
 ENLensFlare::Free();
 ENCoreAlphaBuffer::Free();
 ENCoreObjects::Free();
 ENCoreLight::Free();
 ENCoreSound::Free();
 ENCoreRenderer::Free();
 ENCoreMap::Free();
 ENCorePacker::Free();
 ENCoreShaders::Free();
 ENCoreProcessManager::Free();
 ENCore2D::Free();
 ENCoreScript::Free();
 ENCoreWindow::Free();
}

void ENCoreMain::Idle()
{
 AppRunning=true;
 while(AppRunning)
 {
  //Process window
  ENCoreWindow::Idle();
  //Update frames per second
  UpdateFps();
  //Update time factor
  UpdateTimeFac();
  //Update listener
  ENCoreView::UpdateListener();
  //Execute process manager
  ENCoreProcessManager::Execute();
  //Process objects
  ENCoreObjects::Idle();
  //Draw command
  Draw();
 }
}

void ENCoreMain::Draw()
{
 ENCoreGL::BeginDraw();
 ENCoreRenderer::Render();
 ENCore2D::Draw();
 ENCoreGL::EndDraw();
}

void ENCoreMain::UpdateFps()
{
 //Variables
 static ENint  TempFps=0;
 static ENuint TimeDiff=GetTickCount();
 //New frame
 TempFps++;
 //Check for new update
 if(GetTickCount()-TimeDiff>=1000)
 {
  *Fps=TempFps;
  TempFps=0;
  TimeDiff=GetTickCount();
 }
}

void ENCoreMain::UpdateTimeFac()
{
 //Variables
 ENuint TimeDiff;
 static ENuint OldTime=GetTickCount();
 //Set time difference
 TimeDiff=GetTickCount()-OldTime;
 OldTime=GetTickCount();
 //Calculate time factor
 if(TimeDiff)
   *ENCoreMain::TimeFac=(ENfloat)TimeDiff/1000.0f;
 else
   *ENCoreMain::TimeFac=(ENfloat)0.001f;
}

ENbool ENCoreMain::CheckFile(char *FileName)
{
 ENFile handle=ENOpenFile(FileName,"rb");
 if(!handle)
   return false;
 else
 {
  ENCloseFile(handle);
  return true;
 }
}

void ENCoreMain::ExtractPath(char *FName,char *Path)
{
 char *pos=strrchr(FName,'\\');
 if(pos)
 {
  strcpy(Path,FName);
  ENuint ind=pos-FName;
  Path[ind]=0;
 }
 else
   *Path=0;
}

