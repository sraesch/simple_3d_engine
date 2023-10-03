//---------------------------------------------------------------------------
#include "ENSoundSrc.h"
//---------------------------------------------------------------------------
#ifndef AL_CONE_OUTER_ANGLE
#define AL_CONE_OUTER_ANGLE                       0x1002
#endif

#ifndef AL_PITCH
#define AL_PITCH                                 0x1003
#endif

#ifndef AL_POSITION
#define AL_POSITION                              0x1004
#endif

#ifndef AL_DIRECTION
#define AL_DIRECTION                             0x1005
#endif

#ifndef AL_LOOPING
#define AL_LOOPING                               0x1007
#endif

#ifndef AL_BUFFER
#define AL_BUFFER                                0x1009
#endif

#ifndef AL_GAIN
#define AL_GAIN                                  0x100A
#endif

#ifndef AL_REFERENCE_DISTANCE
#define AL_REFERENCE_DISTANCE                    0x1020
#endif

#ifndef AL_ROLLOFF_FACTOR
#define AL_ROLLOFF_FACTOR                        0x1021
#endif

#ifndef AL_MAX_DISTANCE
#define AL_MAX_DISTANCE                          0x1023
#endif

ENSoundSource::ENSoundSource(ENSoundLoaded *buffer)
{
 //Generate source ID
 ENSoundAPI::alGenSources(1,&SourceID);
 //Get buffer ID
 BufferID=buffer->BufferID;
 //Config source
 ENSoundAPI::alSourcei(SourceID,AL_BUFFER,BufferID);
 ENSoundAPI::alSource3f(SourceID,AL_POSITION,0.0f,0.0f,0.0f);
 ENSoundAPI::alSourcef(SourceID,AL_GAIN,1.0f);
 ENSoundAPI::alSourcef(SourceID,AL_PITCH,1.0f);
 ENSoundAPI::alSourcei(SourceID,AL_LOOPING,false);
 SetRange(100.0f);
}

ENSoundSource::~ENSoundSource()
{
 ENSoundAPI::alDeleteSources(1,&SourceID);
}

void ENSoundSource::SetPosition(ENVector vec)
{
 SetPosition(&vec);
}

void ENSoundSource::SetPosition(ENVector *vec)
{
 ENSoundAPI::alSourcefv(SourceID,AL_POSITION,(ENfloat*)vec);
}

void ENSoundSource::SetSpeed(ENfloat speed)
{
 ENSoundAPI::alSourcef(SourceID,AL_PITCH,speed);
}

void ENSoundSource::SetVolume(ENfloat vol)
{
 ENSoundAPI::alSourcef(SourceID,AL_GAIN,vol);
}

void ENSoundSource::SetRange(ENfloat range)
{
 ENSoundAPI::alSourcef(SourceID,AL_MAX_DISTANCE,range);
}

void ENSoundSource::SetLoopMode(ENbool enable)
{
 ENSoundAPI::alSourcei(SourceID,AL_LOOPING,enable);
}

void ENSoundSource::Play()
{
 ENSoundAPI::alSourcePlay(SourceID);
}

void ENSoundSource::Stop()
{
 ENSoundAPI::alSourceStop(SourceID);
}

void ENSoundSource::Pause()
{
 ENSoundAPI::alSourcePause(SourceID);
}

void ENSoundSource::SetDir(ENVector dir)
{
 ENSoundAPI::alSourcefv(SourceID,AL_DIRECTION,(ENfloat*)&dir);
}

void ENSoundSource::SetAngle(ENfloat angle)
{
 ENSoundAPI::alSourcef(SourceID,AL_CONE_OUTER_ANGLE,angle);
}

