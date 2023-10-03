//---------------------------------------------------------------------------
#include "ENSoundDevice.h"
//---------------------------------------------------------------------------
#ifndef AL_POSITION
#define AL_POSITION                              0x1004
#endif

#ifndef AL_VELOCITY
#define AL_VELOCITY                              0x1006
#endif

#ifndef AL_ORIENTATION
#define AL_ORIENTATION                           0x100F
#endif

#ifndef AL_LINEAR_DISTANCE
#define AL_LINEAR_DISTANCE                       0xD003
#endif

ENSoundDevice::ENSoundDevice()
{
 //Variables
 MIXERCAPS mixerCaps;
 //Open device
 mixerGetDevCaps(0,&mixerCaps,sizeof(MIXERCAPS));
 device=ENSoundAPI::alcOpenDevice((ALubyte*)&mixerCaps.szPname);
 if(!device)
   device=ENSoundAPI::alcOpenDevice("DirectSound3D");

 //Create context
 context=ENSoundAPI::alcCreateContext(device,0);
 //Set active context
 ENSoundAPI::alcMakeContextCurrent(context);
 //Config OpenAL context
 ENfloat listenerOri[]={0.0,0.0,1.0, 0.0,1.0,0.0};
 ENSoundAPI::alListener3f(AL_POSITION,0.0f,0.0f,0.0f);
 ENSoundAPI::alListener3f(AL_VELOCITY,0.0f,0.0f,0.0f);
 ENSoundAPI::alListenerfv(AL_ORIENTATION,listenerOri);
 ENSoundAPI::alDistanceModel(AL_LINEAR_DISTANCE);
}

ENSoundDevice::~ENSoundDevice()
{
 //Variables
 ALcontext* con;
 //Get active context
 con=ENSoundAPI::alcGetCurrentContext();
 //Disable context
 if(con==context) ENSoundAPI::alcMakeContextCurrent(NULL);
 //Release context(s)
 ENSoundAPI::alcDestroyContext(context);
 //Close device
 ENSoundAPI::alcCloseDevice(device);
}

void ENSoundDevice::SetListenerPos(ENVector pos)
{
 SetListenerPos(&pos);
}

void ENSoundDevice::SetListenerPos(ENVector *pos)
{
 ENSoundAPI::alListenerfv(AL_POSITION,(ENfloat*)pos);
}

void ENSoundDevice::SetListenerDir(ENVector dir[])
{
 ENSoundAPI::alListenerfv(AL_ORIENTATION,(ENfloat*)dir);
}

void ENSoundDevice::SetListenerVel(ENVector vel)
{
 SetListenerVel(&vel);
}

void ENSoundDevice::SetListenerVel(ENVector *vel)
{
 ENSoundAPI::alListenerfv(AL_VELOCITY,(ENfloat*)vel);
}


