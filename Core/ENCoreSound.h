//---------------------------------------------------------------------------
#ifndef ENCoreSoundH
#define ENCoreSoundH
//---------------------------------------------------------------------------
#include "EngineSystem.h"
#include "EngineSound.h"

class ENCoreSound
{
 public:
   enum SOUNDSTATE {ENSOUNDSTOP=0,ENSOUNDPLAY=1,ENSOUNDPAUSE=2};

   static void Init();
   static void Clear();
   static void Free();
   static ENSoundRec                  CreateSound(char *source);
   static ENint                       SearchSound(ENuint ID);
   static void                        DeleteSound(ENuint ID);
   static void                        SetSound(ENSoundRec *sound);
   static ENSoundRec                  GetSound(ENuint ID);
   static ENuint                      GetNumSounds();
   static void                        UpdateSounds(ENuint num,ENSoundSrc *msounds);
   static void                        SetSoundState(ENSoundRec *sound,SOUNDSTATE state);
   static void                        SetListener(ENVector pos,ENVector dir);
 private:
   static const ENuint 	  SOUNDBUFFERSTEP;
   static ENuint          BufferSize;
   static ENuint          NumSounds;
   static ENuint          SoundIDCount;
   static ENSoundRec     *sounds;
   static ENSoundDevice  *snddev;

   static void            CheckSoundBuffer();
   static void            ApplySound(ENSoundRec *snd);
};

#endif
 