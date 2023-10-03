//---------------------------------------------------------------------------
#ifndef ENSoundDeviceH
#define ENSoundDeviceH
//---------------------------------------------------------------------------
#include "EngineSoundAPI.h"

class ENSoundDevice
{
 public:
   __declspec(dllexport) ENSoundDevice();
   __declspec(dllexport) ~ENSoundDevice();
   __declspec(dllexport) void SetListenerPos(ENVector pos);
   __declspec(dllexport) void SetListenerPos(ENVector *pos);
   __declspec(dllexport) void SetListenerDir(ENVector dir[]);
   __declspec(dllexport) void SetListenerVel(ENVector vel);
   __declspec(dllexport) void SetListenerVel(ENVector *vel);
 protected:
   ALcontext* context;
   ALdevice*  device;
};

#endif
