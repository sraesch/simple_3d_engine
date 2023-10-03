//---------------------------------------------------------------------------
#ifndef ENSoundH
#define ENSoundH
//---------------------------------------------------------------------------
#include "EngineSoundAPI.h"
#include "EngineSystem.h"

class ENSound
{
 friend class ENSoundLoaded;
 public:
   #pragma pack (push,1)
   struct ENSoundHeader
   {
    char         ID[5];
    ENuint       size;
    ENuint       freq;
    ENint        loop;
    ENint        format;
   };
   #pragma pack (pop)
   __declspec(dllexport) ENSound();
   __declspec(dllexport) ~ENSound();
   __declspec(dllexport) void Clear();

   __declspec(dllexport) ENbool LoadWAV(char *FileName);
   
   __declspec(dllexport) ENbool SaveToFile(char *FileName);
   __declspec(dllexport) void   SaveToHandle(ENFile handle);
   __declspec(dllexport) ENbool LoadFromFile(char *FileName);
   __declspec(dllexport) ENbool LoadFromHandle(ENFile handle);
   __declspec(dllexport) ENuint GetNumBytes();
   __declspec(dllexport) void   GetBytes(ENubyte *bytes);
   __declspec(dllexport) static ENbool IsSound(char *FileName);
 protected:
   ENSoundHeader        header;
   void                 *data;
   static ENuint        FORMAT_MONO8;
   static ENuint        FORMAT_MONO16;
   static ENuint        FORMAT_STEREO8;
   static ENuint        FORMAT_STEREO16;
};

class ENSoundLoaded
{
 friend class ENSoundSource;
 public:
   __declspec(dllexport) ENSoundLoaded(ENSound *snd);
   __declspec(dllexport) ~ENSoundLoaded();
 protected:
   ENuint BufferID;
};

#endif
