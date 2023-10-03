//---------------------------------------------------------------------------
#include "ENCoreSound.h"
#include "ENCorePacker.h"
#include "ENCoreMain.h"
#include "ENCoreLoader.h"
//---------------------------------------------------------------------------
const ENuint 	                ENCoreSound::SOUNDBUFFERSTEP=0x5;
ENuint		                ENCoreSound::BufferSize=0;
ENuint		                ENCoreSound::NumSounds=0;
ENuint                          ENCoreSound::SoundIDCount=0;
ENSoundRec                     *ENCoreSound::sounds=NULL;
ENSoundDevice                  *ENCoreSound::snddev=NULL;

void ENCoreSound::Init()
{
 //Loader message
 ENCoreLoader::AddMsg("Initialize engine core Sound...");
 if(!ENSoundAPI::isLoaded())
   ENCoreMain::ShowError("EngineSound.dll is not loaded!!!");

 snddev=new ENSoundDevice();
}

void ENCoreSound::Free()
{
 //Delete all sounds
 Clear();
 //Delete sound device
 delete snddev;
 snddev=NULL;
}

void ENCoreSound::Clear()
{
 //Delete all sound source
 for(ENuint s=0;s<NumSounds;s++)
   delete (ENSoundSource*)sounds[s].src;
 //Free list
 ENFreeMemory(sounds);
 sounds=NULL;
 BufferSize=0;
 NumSounds=0;
 //Reset ID counter
 SoundIDCount=0;
}

void       ENCoreSound::SetListener(ENVector pos,ENVector dir)
{
 static ENVector lastpos=pos;
 //Update direction
 ENVector vdir[2]={dir,ENVector(0.0f,1.0f,0.0f)};
 //Update position
 snddev->SetListenerPos(&pos);
 //Update direction
 snddev->SetListenerDir(vdir);
 //Update velocity
 snddev->SetListenerVel(pos-lastpos);
 lastpos=pos;
}

ENSoundRec ENCoreSound::CreateSound(char *source)
{
 //Variables
 ENSoundRec snd;
 ENSoundLoaded *lsource;
 //Init sound
 snd.id=-1;
 //Get source
 lsource=(ENSoundLoaded*)ENCorePacker::GetSource(source,ENPackerBase::ENSOUND);
 if(!lsource)
 {
  ENCoreMain::ShowError("Cannot find sound source!!!");
  return snd;
 }
 //Check buffer
 CheckSoundBuffer();
 //Init sound
 snd.pos=ENVector(0,0,0);
 snd.range=100.0f;
 snd.dir=ENVector2(0.0f,0.0f);
 snd.angle=360.0f;
 snd.volume=1.0f;
 snd.speed=1.0f;
 snd.loop=false;
 snd.id=SoundIDCount;
 snd.src=(ENint)(new ENSoundSource(lsource));
 ApplySound(&snd);
 //Set sound
 sounds[NumSounds]=snd;
 SoundIDCount++;
 NumSounds++;

 return sounds[NumSounds-1];
}

ENint ENCoreSound::SearchSound(ENuint ID)
{
 for(ENuint a=0;a<NumSounds;a++)
   if(sounds[a].id==(ENint)ID)
	 return a;

 return -1;
}

void ENCoreSound::DeleteSound(ENuint ID)
{
 //Variables
 ENint          index;
 ENSoundRec     sound;
 //Search index
 index=SearchSound(ID);
 if(index==-1) return;
 //Get sound
 sound=sounds[index];
 //Delete source
 delete (ENSoundSource*)sound.src;
 //Copy
 memmove(sounds+index,sounds+index+1,(NumSounds-index-1)*sizeof(ENSoundRec));
 NumSounds--;
}

void ENCoreSound::SetSound(ENSoundRec *sound)
{
 //Get index
 ENint index=SearchSound(sound->id);
 if(index==-1) return;
 //Set sound
 sounds[index]=*sound;
 ApplySound(sound);
}

void ENCoreSound::SetSoundState(ENSoundRec *sound,SOUNDSTATE state)
{
 //Get index
 ENint index=SearchSound(sound->id);
 if(index==-1) return;
 //Set sound state
 ENSoundSource *src=(ENSoundSource*)sounds[index].src;
 switch(state)
 {
  case ENSOUNDPLAY:
    src->Play();
  break;
  case ENSOUNDSTOP:
    src->Stop();
  break;
  case ENSOUNDPAUSE:
    src->Pause();
  break;
 };
}

ENSoundRec ENCoreSound::GetSound(ENuint ID)
{
 //Get index
 ENint index=SearchSound(ID);
 if(index==-1)
   return CreateSound("");
 //Get sound
 return sounds[index];
}

ENuint ENCoreSound::GetNumSounds()
{
 return NumSounds;
}

void ENCoreSound::UpdateSounds(ENuint num,ENSoundSrc *msounds)
{
 //Release
 Clear();
 //Set new sounds
 for(ENuint s=0;s<num;s++)
 {
  ENSoundRec snd=CreateSound(msounds->src);
  snd.pos=msounds[s].pos;
  snd.range=msounds[s].range;
  snd.dir=msounds[s].dir;
  snd.angle=msounds[s].angle;
  snd.volume=msounds[s].volume;
  snd.speed=msounds[s].speed;
  snd.loop=msounds[s].loop;
  SetSound(&snd);
  SetSoundState(&snd,ENSOUNDPLAY);
 }
}

void ENCoreSound::CheckSoundBuffer()
{
 if(NumSounds+1>=BufferSize)
 {
  BufferSize+=SOUNDBUFFERSTEP;
  sounds=(ENSoundRec*)ENReallocMemory(sounds,
	                 sizeof(ENSoundRec)*BufferSize);
 }
}

void ENCoreSound::ApplySound(ENSoundRec *snd)
{
 //Variables
 ENSoundSource *src;
 //Get sound source
 src=(ENSoundSource*)snd->src;
 //Apply settings
 src->SetPosition(&snd->pos);
 src->SetRange(snd->range);
 src->SetDir(ENVecDir(1.0f,snd->dir));
 src->SetAngle(snd->angle);
 src->SetVolume(snd->volume);
 src->SetSpeed(snd->speed);
 src->SetLoopMode(snd->loop);
}

