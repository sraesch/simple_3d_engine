//---------------------------------------------------------------------------
#include "EngineSoundAPI.h"
//---------------------------------------------------------------------------
ENbool  ENSoundAPI::Loaded=false;
HMODULE ENSoundAPI::LibHandle=NULL;
//al functions
ALAPI_LISTENERI               ENSoundAPI::alListeneri;
ALAPI_LISTENERF               ENSoundAPI::alListenerf;
ALAPI_LISTENER3F              ENSoundAPI::alListener3f;
ALAPI_LISTENERFV              ENSoundAPI::alListenerfv;
ALAPI_ENABLE                  ENSoundAPI::alEnable;
ALAPI_DISABLE                 ENSoundAPI::alDisable;
ALAPI_GEN_SOURCES             ENSoundAPI::alGenSources;
ALAPI_DELETE_SOURCES          ENSoundAPI::alDeleteSources;
ALAPI_IS_SOURCE               ENSoundAPI::alIsSource;
ALAPI_SOURCEI                 ENSoundAPI::alSourcei;
ALAPI_SOURCEF                 ENSoundAPI::alSourcef;
ALAPI_SOURCE3F                ENSoundAPI::alSource3f;
ALAPI_SOURCEFV                ENSoundAPI::alSourcefv;
ALAPI_SOURCE_PLAYV            ENSoundAPI::alSourcePlayv;
ALAPI_SOURCE_PAUSEV           ENSoundAPI::alSourcePausev;
ALAPI_SOURCE_STOPV            ENSoundAPI::alSourceStopv;
ALAPI_SOURCE_REWINDV          ENSoundAPI::alSourceRewindv;
ALAPI_SOURCE_PLAY             ENSoundAPI::alSourcePlay;
ALAPI_SOURCE_PAUSE            ENSoundAPI::alSourcePause;
ALAPI_SOURCE_STOP             ENSoundAPI::alSourceStop;
ALAPI_SOURCE_REWIND           ENSoundAPI::alSourceRewind;
ALAPI_GEN_BUFFERS             ENSoundAPI::alGenBuffers;
ALAPI_DELETE_BUFFERS          ENSoundAPI::alDeleteBuffers;
ALAPI_IS_BUFFER               ENSoundAPI::alIsBuffer;
ALAPI_BUFFER_DATA             ENSoundAPI::alBufferData;
ALAPI_GET_BUFFERI             ENSoundAPI::alGetBufferi;
ALAPI_GET_BUFFERF             ENSoundAPI::alGetBufferf;
ALAPI_DISTANCE_MODEL          ENSoundAPI::alDistanceModel;
ALAPI_DOPPLER_FACTOR          ENSoundAPI::alDopplerFactor;
ALAPI_DOPPLER_VELOCITY        ENSoundAPI::alDopplerVelocity;
ALAPI_GET_ERROR               ENSoundAPI::alGetError;
ALAPI_GET_ENUM_VALUE          ENSoundAPI::alGetEnumValue;
//alc functions
ALCAPI_OPEN_DEVICE            ENSoundAPI::alcOpenDevice;
ALCAPI_CREATE_CONTEXT         ENSoundAPI::alcCreateContext;
ALCAPI_MAKE_CONTEXT_CURRENT   ENSoundAPI::alcMakeContextCurrent;
ALCAPI_GET_CURRENT_CONTEXT    ENSoundAPI::alcGetCurrentContext;
ALCAPI_GET_CONTEXTS_DEVICE    ENSoundAPI::alcGetContextsDevice;
ALCAPI_CLOSE_DEVICE           ENSoundAPI::alcCloseDevice;
ALCAPI_DESTROY_CONTEXT        ENSoundAPI::alcDestroyContext;

ENbool ENSoundAPI::Init()
{
 //Check if API is already loaded
 if(Loaded) return true;
 //Load Library
 LibHandle=LoadLibrary("OpenAL32.dll");
 if(!LibHandle)
 {
  MessageBox(NULL,"Cannot load OpenAL32.dll!!!","Engine Sound Error",MB_TASKMODAL | MB_TOPMOST);
  return false;
 }
 //Init al functions
 if(!InitALFuncs())
 {
  MessageBox(NULL,"Cannot load al functions!!!","Engine Sound Error",MB_TASKMODAL | MB_TOPMOST);
  return false;
 }
 //Init alc functions
 if(!InitALCFuncs())
 {
  MessageBox(NULL,"Cannot load alc functions!!!","Engine Sound Error",MB_TASKMODAL | MB_TOPMOST);
  return false;
 }
 //Finished
 Loaded=true;
 return true;
}

ENbool ENSoundAPI::isLoaded()
{
 return Loaded;
}

ENbool ENSoundAPI::InitALFuncs()
{
 //alListeneri
 alListeneri=(ALAPI_LISTENERI)GetProcAddress(LibHandle,"alListeneri");
 if(!alListeneri) return false;
 //alListenerf
 alListenerf=(ALAPI_LISTENERF)GetProcAddress(LibHandle,"alListenerf");
 if(!alListenerf) return false;
 //alListener3f
 alListener3f=(ALAPI_LISTENER3F)GetProcAddress(LibHandle,"alListener3f");
 if(!alListener3f) return false;
 //alListenerfv
 alListenerfv=(ALAPI_LISTENERFV)GetProcAddress(LibHandle,"alListenerfv");
 if(!alListenerfv) return false;
 //alEnable
 alEnable=(ALAPI_ENABLE)GetProcAddress(LibHandle,"alEnable");
 if(!alEnable) return false;
 //alDisable
 alDisable=(ALAPI_DISABLE)GetProcAddress(LibHandle,"alDisable");
 if(!alDisable) return false;
 //alGenSources
 alGenSources=(ALAPI_GEN_SOURCES)GetProcAddress(LibHandle,"alGenSources");
 if(!alGenSources) return false;
 //alDeleteSources
 alDeleteSources=(ALAPI_DELETE_SOURCES)GetProcAddress(LibHandle,"alDeleteSources");
 if(!alDeleteSources) return false;
 //alIsSource
 alIsSource=(ALAPI_IS_SOURCE)GetProcAddress(LibHandle,"alIsSource");
 if(!alIsSource) return false;
 //alSourcei
 alSourcei=(ALAPI_SOURCEI)GetProcAddress(LibHandle,"alSourcei");
 if(!alSourcei) return false;
 //alSourcef
 alSourcef=(ALAPI_SOURCEF)GetProcAddress(LibHandle,"alSourcef");
 if(!alSourcef) return false;
 //alSource3f
 alSource3f=(ALAPI_SOURCE3F)GetProcAddress(LibHandle,"alSource3f");
 if(!alSource3f) return false;
 //alSourcefv
 alSourcefv=(ALAPI_SOURCEFV)GetProcAddress(LibHandle,"alSourcefv");
 if(!alSourcefv) return false;
 //alSourcePlayv
 alSourcePlayv=(ALAPI_SOURCE_PLAYV)GetProcAddress(LibHandle,"alSourcePlayv");
 if(!alSourcePlayv) return false;
 //alSourcePausev
 alSourcePausev=(ALAPI_SOURCE_PAUSEV)GetProcAddress(LibHandle,"alSourcePausev");
 if(!alSourcePausev) return false;
 //alSourceStopv
 alSourceStopv=(ALAPI_SOURCE_STOPV)GetProcAddress(LibHandle,"alSourceStopv");
 if(!alSourceStopv) return false;
 //alSourceRewindv
 alSourceRewindv=(ALAPI_SOURCE_REWINDV)GetProcAddress(LibHandle,"alSourceRewindv");
 if(!alSourceRewindv) return false;
 //alSourcePlay
 alSourcePlay=(ALAPI_SOURCE_PLAY)GetProcAddress(LibHandle,"alSourcePlay");
 if(!alSourcePlay) return false;
 //alSourcePause
 alSourcePause=(ALAPI_SOURCE_PAUSE)GetProcAddress(LibHandle,"alSourcePause");
 if(!alSourcePause) return false;
 //alSourceStop
 alSourceStop=(ALAPI_SOURCE_STOP)GetProcAddress(LibHandle,"alSourceStop");
 if(!alSourceStop) return false;
 //alSourceRewind
 alSourceRewind=(ALAPI_SOURCE_REWIND)GetProcAddress(LibHandle,"alSourceRewind");
 if(!alSourceRewind) return false;
 //alGenBuffers
 alGenBuffers=(ALAPI_GEN_BUFFERS)GetProcAddress(LibHandle,"alGenBuffers");
 if(!alGenBuffers) return false;
 //alDeleteBuffers
 alDeleteBuffers=(ALAPI_DELETE_BUFFERS)GetProcAddress(LibHandle,"alDeleteBuffers");
 if(!alDeleteBuffers) return false;
 //alIsBuffer
 alIsBuffer=(ALAPI_IS_BUFFER)GetProcAddress(LibHandle,"alIsBuffer");
 if(!alIsBuffer) return false;
 //alBufferData
 alBufferData=(ALAPI_BUFFER_DATA)GetProcAddress(LibHandle,"alBufferData");
 if(!alBufferData) return false;
 //alGetBufferi
 alGetBufferi=(ALAPI_GET_BUFFERI)GetProcAddress(LibHandle,"alGetBufferi");
 if(!alGetBufferi) return false;
 //alGetBufferf
 alGetBufferf=(ALAPI_GET_BUFFERF)GetProcAddress(LibHandle,"alGetBufferf");
 if(!alGetBufferf) return false;
 //alDistanceModel
 alDistanceModel=(ALAPI_DISTANCE_MODEL)GetProcAddress(LibHandle,"alDistanceModel");
 if(!alDistanceModel) return false;
 //alDopplerFactor
 alDopplerFactor=(ALAPI_DOPPLER_FACTOR)GetProcAddress(LibHandle,"alDopplerFactor");
 if(!alDopplerFactor) return false;
 //alDopplerVelocity
 alDopplerVelocity=(ALAPI_DOPPLER_VELOCITY)GetProcAddress(LibHandle,"alDopplerVelocity");
 if(!alDopplerVelocity) return false;
 //alGetError
 alGetError=(ALAPI_GET_ERROR)GetProcAddress(LibHandle,"alGetError");
 if(!alGetError) return false;
 //alGetEnumValue
 alGetEnumValue=(ALAPI_GET_ENUM_VALUE)GetProcAddress(LibHandle,"alGetEnumValue");
 if(!alGetEnumValue) return false;   
 //Finished
 return true;
}

ENbool ENSoundAPI::InitALCFuncs()
{
 //OpenDevice
 alcOpenDevice=(ALCAPI_OPEN_DEVICE)GetProcAddress(LibHandle,"alcOpenDevice");
 if(!alcOpenDevice) return false;
 //CreateContext
 alcCreateContext=(ALCAPI_CREATE_CONTEXT)GetProcAddress(LibHandle,"alcCreateContext");
 if(!alcCreateContext) return false;
 //alcMakeContextCurrent
 alcMakeContextCurrent=(ALCAPI_MAKE_CONTEXT_CURRENT)GetProcAddress(LibHandle,"alcMakeContextCurrent");
 if(!alcMakeContextCurrent) return false;
 //alcGetCurrentContext
 alcGetCurrentContext=(ALCAPI_GET_CURRENT_CONTEXT)GetProcAddress(LibHandle,"alcGetCurrentContext");
 if(!alcGetCurrentContext) return false;
 //alcGetContextsDevice
 alcGetContextsDevice=(ALCAPI_GET_CONTEXTS_DEVICE)GetProcAddress(LibHandle,"alcGetContextsDevice");
 if(!alcGetContextsDevice) return false;
 //alcCloseDevice
 alcCloseDevice=(ALCAPI_CLOSE_DEVICE)GetProcAddress(LibHandle,"alcCloseDevice");
 if(!alcCloseDevice) return false;
 //alcDestroyContext
 alcDestroyContext=(ALCAPI_DESTROY_CONTEXT)GetProcAddress(LibHandle,"alcDestroyContext");
 if(!alcDestroyContext) return false;
 //Finished
 return true;
}

