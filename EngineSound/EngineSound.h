#ifndef          ENGINE_SOUND_H_
#define          ENGINE_SOUND_H_

#include "EngineSoundAPI.h"
#include "ENSound.h"
#include "ENSoundDevice.h"
#include "ENSoundSrc.h"

#pragma pack (push,1)

struct ENSoundRec
{
 ENVector  pos;
 ENfloat   range;
 ENVector2 dir;
 ENfloat   angle;
 ENfloat   volume;
 ENfloat   speed;
 ENint     loop;
 ENint     id;
 ENint     src;
};

#pragma pack (pop)

#endif
 