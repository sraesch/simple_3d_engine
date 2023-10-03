//---------------------------------------------------------------------------
#ifndef EngineSoundAPIH
#define EngineSoundAPIH
//---------------------------------------------------------------------------
#include <windows.h>
#include "al.h"
#include "alc.h"
#include "EngineSystem.h"

class ENSoundAPI
{
 friend class ENSoundDevice;
 friend class ENSound;
 friend class ENSoundLoaded;
 friend class ENSoundSource; 
 public:
   __declspec(dllexport) static ENbool Init();
   __declspec(dllexport) static ENbool isLoaded();
 protected:
   static ENbool Loaded;
   static HMODULE LibHandle;
   //al functions
   static ALAPI_LISTENERI              alListeneri;
   static ALAPI_LISTENERF              alListenerf;
   static ALAPI_LISTENER3F             alListener3f;
   static ALAPI_LISTENERFV             alListenerfv;
   static ALAPI_ENABLE                 alEnable;
   static ALAPI_DISABLE                alDisable;
   static ALAPI_GEN_SOURCES            alGenSources;
   static ALAPI_DELETE_SOURCES         alDeleteSources;
   static ALAPI_IS_SOURCE              alIsSource;
   static ALAPI_SOURCEI                alSourcei;
   static ALAPI_SOURCEF                alSourcef;
   static ALAPI_SOURCE3F               alSource3f;
   static ALAPI_SOURCEFV               alSourcefv;
   static ALAPI_SOURCE_PLAYV           alSourcePlayv;
   static ALAPI_SOURCE_PAUSEV          alSourcePausev;
   static ALAPI_SOURCE_STOPV           alSourceStopv;
   static ALAPI_SOURCE_REWINDV         alSourceRewindv;
   static ALAPI_SOURCE_PLAY            alSourcePlay;
   static ALAPI_SOURCE_PAUSE           alSourcePause;
   static ALAPI_SOURCE_STOP            alSourceStop;
   static ALAPI_SOURCE_REWIND          alSourceRewind;
   static ALAPI_GEN_BUFFERS            alGenBuffers;
   static ALAPI_DELETE_BUFFERS         alDeleteBuffers;
   static ALAPI_IS_BUFFER              alIsBuffer;
   static ALAPI_BUFFER_DATA            alBufferData;
   static ALAPI_GET_BUFFERI            alGetBufferi;
   static ALAPI_GET_BUFFERF            alGetBufferf;
   static ALAPI_DISTANCE_MODEL         alDistanceModel;
   static ALAPI_DOPPLER_FACTOR         alDopplerFactor;
   static ALAPI_DOPPLER_VELOCITY       alDopplerVelocity;
   static ALAPI_GET_ERROR              alGetError;
   static ALAPI_GET_ENUM_VALUE         alGetEnumValue;
   //alc functions
   static ALCAPI_OPEN_DEVICE           alcOpenDevice;
   static ALCAPI_CREATE_CONTEXT        alcCreateContext;
   static ALCAPI_MAKE_CONTEXT_CURRENT  alcMakeContextCurrent;
   static ALCAPI_GET_CURRENT_CONTEXT   alcGetCurrentContext;
   static ALCAPI_GET_CONTEXTS_DEVICE   alcGetContextsDevice;
   static ALCAPI_CLOSE_DEVICE          alcCloseDevice;
   static ALCAPI_DESTROY_CONTEXT       alcDestroyContext;

   __declspec(dllexport) static ENbool InitALFuncs();
   __declspec(dllexport) static ENbool InitALCFuncs();
};

#endif
 