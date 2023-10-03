//---------------------------------------------------------------------------
#ifndef ENSoundSrcH
#define ENSoundSrcH
//---------------------------------------------------------------------------
#include "ENSound.h"

class ENSoundSource
{
 public:
   __declspec(dllexport) ENSoundSource(ENSoundLoaded *buffer);
   __declspec(dllexport) ~ENSoundSource();

   __declspec(dllexport) void SetPosition(ENVector vec);
   __declspec(dllexport) void SetPosition(ENVector *vec);
   __declspec(dllexport) void SetSpeed(ENfloat speed);
   __declspec(dllexport) void SetVolume(ENfloat vol);
   __declspec(dllexport) void SetRange(ENfloat range);
   __declspec(dllexport) void SetDir(ENVector dir);
   __declspec(dllexport) void SetAngle(ENfloat angle);

   __declspec(dllexport) void SetLoopMode(ENbool enable);
   __declspec(dllexport) void Play();
   __declspec(dllexport) void Stop();
   __declspec(dllexport) void Pause();
 protected:
   ENuint    SourceID;
   ENuint    BufferID;
};

#endif
 