//---------------------------------------------------------------------------
#include "ENSound.h"
//---------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////
//Engine sound class
////////////////////////////////////////////////////////////////////
#pragma pack (push,1)                           /* Turn off alignment */

typedef struct                                  /* WAV File-header */
{
    ALubyte  Id[4];
    ALsizei  Size;
    ALubyte  Type[4];

} WAVFileHdr_Struct;

typedef struct                                  /* WAV Fmt-header */
{
    ALushort Format;
    ALushort Channels;
    ALuint   SamplesPerSec;
    ALuint   BytesPerSec;
    ALushort BlockAlign;
    ALushort BitsPerSample;

} WAVFmtHdr_Struct;

typedef struct                                  /* WAV FmtEx-header */
{
    ALushort Size;
    ALushort SamplesPerBlock;
} WAVFmtExHdr_Struct;

typedef struct                                  /* WAV Smpl-header */
{
    ALuint   Manufacturer;
    ALuint   Product;
    ALuint   SamplePeriod;
    ALuint   Note;
    ALuint   FineTune;
    ALuint   SMPTEFormat;
    ALuint   SMPTEOffest;
    ALuint   Loops;
    ALuint   SamplerData;
    struct
    {
        ALuint Identifier;
        ALuint Type;
        ALuint Start;
        ALuint End;
        ALuint Fraction;
        ALuint Count;
    } Loop[1];

} WAVSmplHdr_Struct;

typedef struct                                  /* WAV Chunk-header */
{
    ALubyte  Id[4];
    ALuint   Size;

} WAVChunkHdr_Struct;

#pragma pack (pop)                              /* Default alignment */

ENuint        ENSound::FORMAT_MONO8=4352;
ENuint        ENSound::FORMAT_MONO16=4353;
ENuint        ENSound::FORMAT_STEREO8=4354;
ENuint        ENSound::FORMAT_STEREO16=4355;


ENSound::ENSound()
{
 //Init values
 header.format=FORMAT_MONO16;
 data=NULL;
 header.size=0;
 header.freq=22050;
 header.loop=false;
 strcpy(header.ID,"ENS1");
}

ENSound::~ENSound()
{
 Clear();
}

void ENSound::Clear()
{
 ENFreeMemory(data);
 header.format=FORMAT_MONO16;
 data=NULL;
 header.size=0;
 header.freq=22050;
 header.loop=false;
 strcpy(header.ID,"ENS1");
}

ENbool ENSound::LoadWAV(char *FileName)
{
 //Variables
 WAVChunkHdr_Struct     ChunkHdr;
 WAVFmtExHdr_Struct     FmtExHdr;
 WAVFileHdr_Struct      FileHdr;
 WAVSmplHdr_Struct      SmplHdr;
 WAVFmtHdr_Struct       FmtHdr;
 ENFile                 Stream;
 //Open file
 Stream=ENOpenFile(FileName,"rb");
 if(!Stream)
   return false;
 else
   Clear();
 //Read file header
 ENReadFile(&FileHdr,1,sizeof(WAVFileHdr_Struct),Stream);
 FileHdr.Size=((FileHdr.Size+1)&~1)-4;
 
 while((FileHdr.Size!=0)&&(ENReadFile(&ChunkHdr,1,sizeof(WAVChunkHdr_Struct),Stream)))
 {
  if(!memcmp(ChunkHdr.Id,"fmt ",4))
  {
   ENReadFile(&FmtHdr,1,sizeof(WAVFmtHdr_Struct),Stream);
   if(FmtHdr.Format==0x0001)
   {
    header.format=(FmtHdr.Channels==1?
           (FmtHdr.BitsPerSample==8?FORMAT_MONO8:FORMAT_MONO16):
           (FmtHdr.BitsPerSample==8?FORMAT_STEREO8:FORMAT_STEREO16));
    header.freq=FmtHdr.SamplesPerSec;
    ENSeekFile(Stream,ChunkHdr.Size-sizeof(WAVFmtHdr_Struct),SEEK_CUR);
   }
   else
   {
    ENReadFile(&FmtExHdr,1,sizeof(WAVFmtExHdr_Struct),Stream);
    ENSeekFile(Stream,ChunkHdr.Size-sizeof(WAVFmtHdr_Struct)-sizeof(WAVFmtExHdr_Struct),SEEK_CUR);
   }
  }
  else
    if(!memcmp(ChunkHdr.Id,"data",4))
    {
     if(FmtHdr.Format==0x0001)
     {
      header.size=ChunkHdr.Size;
      data=ENAllocMemory(ChunkHdr.Size+31);
      if(data)
        ENReadFile(data,FmtHdr.BlockAlign,ChunkHdr.Size/FmtHdr.BlockAlign,Stream);

      memset(((ENubyte*)data)+ChunkHdr.Size,0,31);
     }
     else
       if(FmtHdr.Format==0x0011)
       {
        //IMA ADPCM
       }
       else
         if(FmtHdr.Format==0x0055)
         {
          //MP3 WAVE
         }
     }
     else
       if(!memcmp(ChunkHdr.Id,"smpl",4))
       {
        ENReadFile(&SmplHdr,1,sizeof(WAVSmplHdr_Struct),Stream);
        header.loop=(SmplHdr.Loops?AL_TRUE:AL_FALSE);
        ENSeekFile(Stream,ChunkHdr.Size-sizeof(WAVSmplHdr_Struct),SEEK_CUR);
       }
       else
         ENSeekFile(Stream,ChunkHdr.Size,SEEK_CUR);

       ENSeekFile(Stream,ChunkHdr.Size&1,SEEK_CUR);
       FileHdr.Size-=(((ChunkHdr.Size+1)&~1)+8);
     }
 //Finished
 ENCloseFile(Stream);
 return true;
}

ENbool ENSound::SaveToFile(char *FileName)
{
 //Variables
 ENFile handle;
 //Open file
 handle=ENOpenFile(FileName,"wb");
 if(!handle) return false;
 //Write data
 SaveToHandle(handle);
 //Finished
 ENCloseFile(handle);
 return true;
}

void ENSound::SaveToHandle(ENFile handle)
{
 //Write header
 ENWriteFile(&header,sizeof(ENSoundHeader),1,handle);
 //Write data
 ENWriteFile(data,1,header.size+31,handle);
}

ENbool ENSound::LoadFromFile(char *FileName)
{
 //Variables
 ENFile handle;
 ENbool res;
 //Open file
 handle=ENOpenFile(FileName,"rb");
 if(!handle) return false;
 //Read data
 res=LoadFromHandle(handle);
 //Finished
 ENCloseFile(handle);
 return res;
}

ENbool ENSound::LoadFromHandle(ENFile handle)
{
 //Variables
 ENSoundHeader h;
 //Read header
 ENReadFile(&h,sizeof(ENSoundHeader),1,handle);
 if(strcmp(h.ID,"ENS1")!=0)
   return false;
 else
   Clear();

 header=h;
 //Alloc space
 data=ENAllocMemory(header.size+31);
 //Read data
 ENReadFile(data,1,header.size+31,handle);
 //Finished
 return true;
}

ENuint ENSound::GetNumBytes()
{
 return sizeof(ENSoundHeader)+header.size+31;
}

void   ENSound::GetBytes(ENubyte *bytes)
{
 //Write header
 memcpy(bytes,&header,sizeof(ENSoundHeader));
 //Write sample data
 memcpy(bytes+sizeof(ENSoundHeader),data,header.size+31);
}

ENbool ENSound::IsSound(char *FileName)
{
 //Variables
 ENFile handle;
 WAVFileHdr_Struct waveheader;
 //Check for wave
 handle=ENOpenFile(FileName,"rb");
 if(!handle) return false;
 ENReadFile(&waveheader,sizeof(WAVFileHdr_Struct),1,handle);
 ENCloseFile(handle);
 if(memcmp(waveheader.Type,"WAVE",4)==0)
   return true;
 //Finished
 return false;
}
////////////////////////////////////////////////////////////////////
//Engine loaded sound class
////////////////////////////////////////////////////////////////////
ENSoundLoaded::ENSoundLoaded(ENSound *snd)
{
 //Generate buffer id
 ENSoundAPI::alGenBuffers(1,&BufferID);
 //Set buffer data
 ENSoundAPI::alBufferData(BufferID,snd->header.format,snd->data,
                          snd->header.size,snd->header.freq);
}

ENSoundLoaded::~ENSoundLoaded()
{
 ENSoundAPI::alDeleteBuffers(1,&BufferID);
}
