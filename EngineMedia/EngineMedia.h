//---------------------------------------------------------------------------
#ifndef EngineMediaH
#define EngineMediaH
//---------------------------------------------------------------------------
#include "EngineSystem.h"
#include "BspTree.h"
                                 
#define EN_SHOW_NORMAL      0
#define EN_SHOW_TRANSPARENT 1
#define EN_SHOW_ALPHA       2

///////////////////////////////////////////////////////////////////////
/// Engine Vertex Array
///////////////////////////////////////////////////////////////////////
class ENVertexArray : public ENArray
{
 public:
   __declspec(dllexport) ENVertexArray();
   __declspec(dllexport) ~ENVertexArray();
   __declspec(dllexport) void   SetGLVertexPointer();   
};

///////////////////////////////////////////////////////////////////////
///Engine Picture
///////////////////////////////////////////////////////////////////////
#define ENGINE_IMAGEERROR_NO            0
#define ENGINE_IMAGEERROR_OPEN          1
#define ENGINE_IMAGEERROR_UNKNOWN_FMT   2
#define ENGINE_IMAGEERROR_COMPRESSED    3
#define ENGINE_IMAGEERROR_INVALID       4
#define ENGINE_IMAGEERROR_PROCESS       5
#define ENGINE_IMAGEERROR_WRITE         6
#define ENGINE_IMAGEERROR_UNKNOWN       7
#define ENGINE_IMAGEERROR_INTERNAL      8

#define ENGINE_IMGTYPE_UNKNOWN 0
#define ENGINE_IMGTYPE_OWN     1
#define ENGINE_IMGTYPE_BMP     2
#define ENGINE_IMGTYPE_PCX     3
#define ENGINE_IMGTYPE_JPG     4
#define ENGINE_IMGTYPE_PNG     5
#define ENGINE_IMGTYPE_TGA     6
///////////////////////////////////////////////////////////////////////
///Engine Picture
///////////////////////////////////////////////////////////////////////
#pragma pack(push)
#pragma pack(2)

struct ENBITMAPFILEHEADER {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
};

#pragma pack(pop)

class ENPicture
{
 public:
   struct ENIMGAGEHEADER
   {
    char    ID[2];
    ENuint  VERSION;
    ENuint    width,height,bpp;
   };              
   __declspec(dllexport) ENPicture();
   __declspec(dllexport) ENPicture(ENPicture *pic);
   __declspec(dllexport) ENPicture(ENubyte bytesperpixel,ENuint w,ENuint h,ENubyte *src);
   __declspec(dllexport) ~ENPicture();
   __declspec(dllexport) void         InitGLTex();
   __declspec(dllexport) void         InitGLTexNoAlpha();
   __declspec(dllexport) void         InitGLMipMap();
   __declspec(dllexport) void         Set(ENPicture *pic);
   __declspec(dllexport) void         Set(ENubyte bytesperpixel,ENuint w,ENuint h,ENubyte *src);
   __declspec(dllexport) void         Clear();
   __declspec(dllexport) void         Resize(ENuint w,ENuint h);
   __declspec(dllexport) void         Power2();
   __declspec(dllexport) void         DeleteAlpha();
   __declspec(dllexport) void         SetStdAlpha();
   __declspec(dllexport) void         SetTransparent(ENubyte red,ENubyte green,ENubyte blue);
   __declspec(dllexport) ENbool       Load(char *FileName);
   __declspec(dllexport) ENbool       LoadBMP(char *FileName);
   __declspec(dllexport) ENbool       LoadPCX(char *FileName);
   __declspec(dllexport) ENbool       LoadJPG(char *FileName);
   __declspec(dllexport) ENbool       LoadPNG(char *FileName);
   __declspec(dllexport) ENbool       LoadTGA(char *FileName);
   __declspec(dllexport) ENbool       LoadFromFile(char *FileName);
   __declspec(dllexport) ENbool       LoadFromHandle(ENFile handle);
   __declspec(dllexport) void         SaveToHandle(ENFile handle);
   __declspec(dllexport) ENbool       WriteBMP(char *FileName);
   __declspec(dllexport) ENbool       WritePNG(char *FileName);
   __declspec(dllexport) ENbool       SaveToFile(char *FileName);
   __declspec(dllexport) ENuint       GetPixel(ENuint x,ENuint y);
   __declspec(dllexport) void         SetAlpha(ENuint x,ENuint y,ENubyte alpha);
   __declspec(dllexport) ENubyte      GetAlpha(ENuint x,ENuint y);
   __declspec(dllexport) ENuint       GetLastError();
   __declspec(dllexport) void         GetLastErrorStr(char *errorstr);
   __declspec(dllexport) ENuint       GetWidth();
   __declspec(dllexport) ENuint       GetHeight();
   __declspec(dllexport) ENuint       GetBpp();
   __declspec(dllexport) void         GetData(ENubyte *dst);
   __declspec(dllexport) void         GetScanLine(ENubyte *dst,ENuint y);
   __declspec(dllexport) ENbool       GetImgType(char *FileName,ENuint &ImgType);
   __declspec(dllexport) ENuint       GetNumBytes();
   __declspec(dllexport) void         GetBytes(ENubyte *bytes);
   __declspec(dllexport) static       ENbool IsPicture(char *FileName);
   __declspec(dllexport) static const char  *GetImageExtStr();
 protected:
   //Types
   struct       ENPCXHEADER;
   //Variables
   ENuint       width,height,bpp;
   ENubyte      *data;
   ENuint       LastError;
   static const char   *ENIMAGEERRORSTRS[];
   static const char   *ENIMAGEEXT;
   //Functions
   __declspec(dllexport) ENbool       CheckBMPHeader(ENBITMAPFILEHEADER *bmpheader,BITMAPINFOHEADER *bmpinfo);
   __declspec(dllexport) ENbool       CheckPCXHeader(ENPCXHEADER *pcxheader);
   __declspec(dllexport) ENbool       CheckPNGHeader(ENFile handle);
   __declspec(dllexport) void         ProcessBMP24Data(ENubyte *pixeldata,ENuint TotalSize);
   __declspec(dllexport) void         ProcessBMP8Data(ENubyte *pixeldata,ENuint TotalSize,ENubyte *Colors);
   __declspec(dllexport) void         ProcessBMP4Data(ENubyte *pixeldata,ENuint TotalSize,ENubyte *Colors);
   __declspec(dllexport) void         ProcessBMP1Data(ENubyte *pixeldata,ENuint TotalSize);
   __declspec(dllexport) void         ProcessPCX24Data(ENFile handle,ENuint BPL);
   __declspec(dllexport) void         ProcessPCX8Data(ENFile handle,ENuint BPL);
   __declspec(dllexport) void         ProcessPCX4Data(ENFile handle,ENuint BPL,ENubyte *Colormap);
   __declspec(dllexport) void         ProcessPCX1Data(ENFile handle,ENuint BPL);
   __declspec(dllexport) void         ProcessPNG8GRAYData(ENubyte **Rows);
   __declspec(dllexport) void         ProcessPNG8GRAYAData(ENubyte **Rows);
   __declspec(dllexport) void         ProcessPNG8PALData(ENubyte **Rows,ENubyte *palette);
   __declspec(dllexport) void         ProcessPNG8RGBData(ENubyte **Rows);
   __declspec(dllexport) void         ProcessPNG8RGBAData(ENubyte **Rows);
   __declspec(dllexport) void         DecodeJPG(void *_cinfo);
   __declspec(dllexport) void         TwistBytes(ENubyte *data,ENuint size);
   __declspec(dllexport) ENuint       Make2(ENuint x);
};
///////////////////////////////////////////////////////////////////////
///Engine Picture loaded
///////////////////////////////////////////////////////////////////////
class ENPictureLoaded
{
 public:
   __declspec(dllexport) ENPictureLoaded(ENPicture *data);
   __declspec(dllexport) ~ENPictureLoaded();
   __declspec(dllexport) void Set(ENuint mode);
 protected:
   ENuint IDs[3];

   __declspec(dllexport) void SetTexFlags();
};
///////////////////////////////////////////////////////////////////////
///Engine Material
///////////////////////////////////////////////////////////////////////
class ENMaterial
{
 public:
   struct ENMaterialHeader
   {
    char ID[5];
    ENbool   EnableBump,EnableAlpha,EnableLuminance;
    ENuint   Lumiwidth,Lumiheight;
    ENuint   Basewidth,Baseheight;
    ENuint   Bumpwidth,Bumpheight;
   };
   __declspec(dllexport) ENMaterial();
   __declspec(dllexport) ENMaterial(ENMaterial *mat);
   __declspec(dllexport) ~ENMaterial();
   __declspec(dllexport) void   Set(ENMaterial *mat);
   __declspec(dllexport) void   SetBaseData(ENPicture *src,ENubyte red,ENubyte green,ENubyte blue);
   __declspec(dllexport) ENbool SetAlphaData(ENPicture *src);
   __declspec(dllexport) void   SetBumpData(ENPicture *src);
   __declspec(dllexport) void   SetLumiData(ENPicture *src);
   __declspec(dllexport) void   FreeBumpData();
   __declspec(dllexport) void   FreeLumiData();
   __declspec(dllexport) ENbool Save(char *FileName);
   __declspec(dllexport) ENbool Load(char *FileName);
   __declspec(dllexport) ENbool LoadFromHandle(ENFile handle);
   __declspec(dllexport) void   SaveToHandle(ENFile handle);
   __declspec(dllexport) void   GetHeader(ENMaterial::ENMaterialHeader &h);
   __declspec(dllexport) void   GetBase(ENPicture *dst);
   __declspec(dllexport) void   GetAlpha(ENPicture *dst);
   __declspec(dllexport) void   GetBump(ENPicture *dst);
   __declspec(dllexport) void   GetLumi(ENPicture *dst);
 protected:
   ENMaterialHeader  Header;
   ENubyte          *BaseImg;
   ENubyte          *AlphaImg;
   ENubyte          *BumpData;
   ENubyte          *LumiData;
};

class ENMaterialLoaded
{
 public:
   __declspec(dllexport) ENMaterialLoaded(ENMaterial *mat,ENbool mipmap);
   __declspec(dllexport) ~ENMaterialLoaded();
   __declspec(dllexport) void SetBase();
   __declspec(dllexport) void SetAlpha();
   __declspec(dllexport) void SetBump();
   __declspec(dllexport) void SetLuminance();
   ENbool isAlphaSupported();
   ENbool isBumpSupported();
   ENbool isLuminanceSupported();
 protected:
   ENbool EnableBump,EnableAlpha,EnableLuminance;
   ENuint BaseID,AlphaID,BumpID,LumiID;
   __declspec(dllexport) void InitTex(ENPicture *pic,ENbool mipmap);
};
///////////////////////////////////////////////////////////////////////
///Engine Font
///////////////////////////////////////////////////////////////////////
class ENFont
{
 public:
   struct ENFontHeader
   {
    char   ID[5];
    ENbool AvailableLetter[256*2];
   };
   __declspec(dllexport) ENFont();
   __declspec(dllexport) ~ENFont();
   __declspec(dllexport) void Clear();
   __declspec(dllexport) static ENbool CheckFontHeader(ENFontHeader fh);
   __declspec(dllexport) ENbool Load(char *FileName);
   __declspec(dllexport) ENbool LoadFromHandle(ENFile handle);
   __declspec(dllexport) ENbool Write(char *FileName);
   __declspec(dllexport) ENFont::ENFontHeader GetHeader();
   __declspec(dllexport) ENbool GetLetter(ENuint ind,ENbool base,ENPicture *res);
   __declspec(dllexport) void   SetLetter(ENuint ind,ENbool base,ENPicture *res);
   __declspec(dllexport) ENbool DelLetter(ENuint ind,ENbool base);
   __declspec(dllexport) static ENbool IsFont(char *FileName);
 protected:
   ENFontHeader header;
   ENPicture *Letter[2][256];
};

///////////////////////////////////////////////////////////////////////
///Engine Font loaded
///////////////////////////////////////////////////////////////////////

class ENFontLoaded
{
 public:
   __declspec(dllexport) ENFontLoaded(ENFont *data);
   __declspec(dllexport) ~ENFontLoaded();
   __declspec(dllexport) ENbool IsLetterAvailable(ENuint index,ENbool base);
   __declspec(dllexport) void SetLetter(ENuint ind,ENuint mode);
 protected:
   struct ENLetterLoaded
   {
    ENuint Normal,Transparent,Alpha;
   };

   ENFont::ENFontHeader header;
   ENLetterLoaded       Letters[256];

   __declspec(dllexport) void SetGLFontFlags();
};
///////////////////////////////////////////////////////////////////////
///Engine Packer Base class
///////////////////////////////////////////////////////////////////////
class ENPackerBase
{
 public:
   enum ENPackageType {ENUNKNOWN,ENPICTURE,ENFONT,ENMODEL,ENSPRITE,ENSOUND};
   struct ENPackageHeader
   {
    ENPackageType Type;
    ENuint        Size;
    ENuint        Pos;
    char          Name[33];
   };
   struct ENPackerHeader
   {
    char    PACKID[5];
    ENuint  NumPackages;
   };
   struct ENPackerID
   {
    ENuint ID[10];
   };
 protected:
   __declspec(dllexport) static void GeneratePackerID(ENPackerID &id);
   __declspec(dllexport) static ENbool PackType(char *FileName,
                                ENPackerBase::ENPackageType &type);
   __declspec(dllexport) static ENbool CompareIDs(ENPackerID *id1,ENPackerID *id2);
};
///////////////////////////////////////////////////////////////////////
///Engine Packer
///////////////////////////////////////////////////////////////////////
class ENPacker : public ENPackerBase
{
 public:
   __declspec(dllexport) ENPacker();
   __declspec(dllexport) ~ENPacker();
   __declspec(dllexport) void Clear();
   __declspec(dllexport) ENint  SearchPackage(char *Name);
   __declspec(dllexport) ENbool AddPackage(char *FileName,char *Name);
   __declspec(dllexport) ENbool AddPackage(ENubyte *Data,ENuint size,
                                           ENPackerBase::ENPackageType Type,char *Name);
   __declspec(dllexport) void   DelPackage(ENuint ind);
   __declspec(dllexport) void   DelPackage(char *Name);
   __declspec(dllexport) ENbool RenamePackage(ENuint ind,char *Name);
   __declspec(dllexport) void   GetPackageHeader(ENuint ind,ENPackageHeader &h);
   __declspec(dllexport) void   GetPackageData(ENuint ind,ENubyte *data);
   __declspec(dllexport) ENPackerBase::ENPackerHeader GetHeader();
   __declspec(dllexport) ENbool Save(char *FileName);
   __declspec(dllexport) ENbool Load(char *FileName);
 protected:
   struct ENPackage
   {
    ENPackageHeader header;
    ENubyte *data;
   };
   ENArray *Packages;
   __declspec(dllexport) ENuint PackagesSize();
   __declspec(dllexport) ENbool AddDataRaw(ENPackage &pack,char *FileName);
};
///////////////////////////////////////////////////////////////////////
///Engine Packer loaded
///////////////////////////////////////////////////////////////////////
class ENPackerLoaded : public ENPackerBase
{
 public:
   __declspec(dllexport) ENPackerLoaded();
   __declspec(dllexport) ~ENPackerLoaded();
   __declspec(dllexport) void         Clear();
   __declspec(dllexport) ENbool       Load(char *FileName);
   __declspec(dllexport) ENbool       LoadAll();
   __declspec(dllexport) void        *GetSource(char *Name,ENPackageType type);
   __declspec(dllexport) ENPackerBase::ENPackageType GetType(ENuint ind);
   __declspec(dllexport) ENint        SearchPackage(char *Name);
   __declspec(dllexport) ENuint       GetNum();
   __declspec(dllexport) void         GetName(ENuint ind,char *Name);
 protected:
   struct ENPackageLoaded
   {
    ENPackageHeader header;
    ENbool          Loaded;
    void           *Data;
   };
   char                 PackageFile[MAX_PATH];
   ENbool               Loaded;
   ENPackerID           id;
   ENuint               NumPackages;
   ENuint               DataStartPos;
   ENPackageLoaded     *Packages;

   __declspec(dllexport) void DeletePackage(ENPackageLoaded *package);
   __declspec(dllexport) ENbool LoadPackage(ENPackageLoaded *package);
   __declspec(dllexport) ENFile PrepareToLoadPackage(ENPackageLoaded *package);
};
///////////////////////////////////////////////////////////////////////
///Engine Sprite
///////////////////////////////////////////////////////////////////////
class ENSprite
{
 public:
   struct ENSpriteHeader
   {
    ENuint NumFrames;
    char   ID[5];
   };
   __declspec(dllexport) ENSprite();
   __declspec(dllexport) ~ENSprite();
   __declspec(dllexport) void Clear();
   __declspec(dllexport) ENbool AddFrame(ENPicture *Base,ENPicture *Alpha,
                                         ENubyte red,ENubyte green,ENubyte blue);
   __declspec(dllexport) void   DeleteFrame(ENuint frame);                                         
   __declspec(dllexport) ENbool Load(char *FileName);
   __declspec(dllexport) ENbool LoadFromHandle(ENFile handle);
   __declspec(dllexport) ENbool Save(char *FileName);
   __declspec(dllexport) ENuint GetNumFrames();
   __declspec(dllexport) void   GetBase(ENPicture *pic,ENuint frame);
   __declspec(dllexport) void   GetAlpha(ENPicture *pic,ENuint frame);
   __declspec(dllexport) void   GetHeader(ENSpriteHeader &header);
   __declspec(dllexport) static ENbool IsSprite(char *FileName);
 protected:
   struct ENSpriteFrame
   {
    ENPicture *Normal,*Alpha;
   };
   ENArray        *frames;
};

///////////////////////////////////////////////////////////////////////
///Engine Sprite loaded
///////////////////////////////////////////////////////////////////////

class ENSpriteLoaded
{
 public:
   __declspec(dllexport) ENSpriteLoaded(ENSprite *data);
   __declspec(dllexport) ~ENSpriteLoaded();
   __declspec(dllexport) void SetCurrentFrame(ENfloat frame);
   __declspec(dllexport) void DrawRaw();
   __declspec(dllexport) void DrawLightData();
   __declspec(dllexport) void Draw();
   __declspec(dllexport) void DrawInterp(ENbool alpha);
   __declspec(dllexport) void GetBox(ENVector &min,ENVector &max);
 protected:
   struct ENSpriteFrameLoaded
   {
    ENuint Normal,Alpha;
   };
   ENuint NumFrames;
   ENSpriteFrameLoaded *frames;

   __declspec(dllexport) void SetGLTexFlags();
 private:
   ENuint  curframe,nxtframe;
   ENfloat framefact;
};
///////////////////////////////////////////////////////////////////////
///Engine Map base
///////////////////////////////////////////////////////////////////////
class ENMapBase
{
 public:
   struct ENMapHeader
   {
    char ID[8];
    char PackageFile[MAX_PATH];
    ENuint NumVertexes;
    ENuint NumFaces;
    ENuint NumTextures;
    ENuint NumLights;
    ENuint NumGroups;
    ENuint NumObjects;
    ENuint NumSounds;
   };
   struct ENFaceProp
   {
    ENbool Passable;
    ENbool Invisible;
    ENbool Lighting;
    ENbool Shadow;
    ENbool Animated;
    ENuint Mode;
   };
   struct ENMapFace
   {
    ENuint     indices[3];
    ENVector2  texcoord[3];
    ENFaceProp props;
    ENint      texind;
    ENuint     numframes;
    ENuint     framelen;
   };
   struct ENMapObject
   {
    char     Func[256];
    char     Name[33];
    ENVector Pos;
    ENVector Angle;
    ENVector Scale;
   };
   struct ENMapTexture
   {
    char        Name[33];
    ENMaterial *mat;
   };
   struct ENMapGroup
   {
    char    Name[33];
    ENArray *Indices;
   };
   struct ENMapSun
   {
    ENbool    Enable;
    ENVector2 dir;
    ENubyte   SunColor[4];
   };

   __declspec(dllexport) static void  InitMapFace(ENMapFace &face);
   __declspec(dllexport) static void  InitDefaultCamera(ENCamera &cam);
 protected:
   char PackageFile[MAX_PATH];
   ENCamera       CameraObj;
   ENPicture     *SkyBox[6];
   ENVertexArray *Vertexes;
   ENArray       *Faces;
   ENArray       *lights;
   ENArray       *Sounds;
   ENArray       *Objects;
   ENMapSun       Sun;        

   __declspec(dllexport) static void   InitHeader(ENMapHeader &header);
   __declspec(dllexport) static ENbool CheckHeader(ENMapHeader *header);
};

///////////////////////////////////////////////////////////////////////
///Engine Map
///////////////////////////////////////////////////////////////////////

class ENMap : ENMapBase
{
 public:
   __declspec(dllexport) ENMap();
   __declspec(dllexport) ~ENMap();
   __declspec(dllexport) void   Clear();
   __declspec(dllexport) void   GetErrorLog(char *str);
   __declspec(dllexport) ENbool Write(char *FileName);
   __declspec(dllexport) ENbool Load(char *FileName);
   __declspec(dllexport) void   AddVertex(ENVector vert);
   __declspec(dllexport) void   AddVertexes(ENVector *verts,ENuint num);
   __declspec(dllexport) void   AddFace(ENMap::ENMapFace face);
   __declspec(dllexport) void   AddFaces(ENMap::ENMapFace *faces,ENuint num);
   __declspec(dllexport) void   DeleteFace(ENuint ind);
   __declspec(dllexport) void   DeleteVertex(ENuint ind);
   __declspec(dllexport) void   SetGLVertexPointer();
   __declspec(dllexport) ENMapHeader GetHeader();
   __declspec(dllexport) ENfloat GetMinX();
   __declspec(dllexport) ENfloat GetMinY();
   __declspec(dllexport) ENfloat GetMinZ();
   __declspec(dllexport) ENfloat GetMaxX();
   __declspec(dllexport) ENfloat GetMaxY();
   __declspec(dllexport) ENfloat GetMaxZ();

   __declspec(dllexport) ENVector GetVertex(ENuint ind);
   __declspec(dllexport) void     SetVertex(ENuint ind,ENVector vec);
   __declspec(dllexport) ENMap::ENMapFace GetFace(ENuint ind);
   __declspec(dllexport) void     SetFace(ENuint ind,ENMap::ENMapFace face);

   __declspec(dllexport) void     SetPackage(char *FileName);

   __declspec(dllexport) void     AddLight(ENLight light);
   __declspec(dllexport) void     SetLight(ENuint ind,ENLight light);
   __declspec(dllexport) ENLight  GetLight(ENuint ind);
   __declspec(dllexport) void     DeleteLight(ENuint ind);

   __declspec(dllexport) void     AddSound(ENSoundSrc snd);
   __declspec(dllexport) void     SetSound(ENuint ind,ENSoundSrc snd);
   __declspec(dllexport) ENSoundSrc GetSound(ENuint ind);
   __declspec(dllexport) void     DeleteSound(ENuint ind);

   __declspec(dllexport) void     AddObject(ENMapObject object);
   __declspec(dllexport) void     SetObject(ENuint ind,ENMapObject object);
   __declspec(dllexport) ENMap::ENMapObject GetObject(ENuint ind);
   __declspec(dllexport) void     DeleteObject(ENuint ind);

   __declspec(dllexport) ENint    GetIndexOfGroup(char *Name);
   __declspec(dllexport) ENbool   CreateGroup(char *Name);
   __declspec(dllexport) void     GetGroupName(ENuint ind,char *Name);
   __declspec(dllexport) ENbool   RenameGroup(ENuint ind,char *Name);
   __declspec(dllexport) void     DeleteGroup(ENuint ind);
   __declspec(dllexport) void     AddGeometry2Group(ENuint ind,ENuint *Indices,ENuint num);
   __declspec(dllexport) void     ReleaseGeoemtryFromGroups(ENuint *Indices,ENuint num);
   __declspec(dllexport) ENuint   GetNumGroupIndices(ENuint ind);
   __declspec(dllexport) void     GetGroupIndices(ENuint ind,ENuint *Indices);

   __declspec(dllexport) void     GetSkyBox(ENuint ind,ENPicture *pic);
   __declspec(dllexport) void     SetSkyBox(ENuint ind,ENPicture *pic);
   __declspec(dllexport) void     SetSun(ENMapSun *sun);
   __declspec(dllexport) void     GetSun(ENMapSun *sun);

   __declspec(dllexport) ENCamera GetCamera();
   __declspec(dllexport) void     SetCamera(ENCamera cam);

   __declspec(dllexport) ENint    GetIndexOfTexture(char *Name);
   __declspec(dllexport) ENbool   CreateTexture(char *Name,char *FileName);
   __declspec(dllexport) ENbool   CreateTexture(char *Name,ENMaterial *mat);
   __declspec(dllexport) void     GetTextureName(ENuint ind,char *Name);
   __declspec(dllexport) ENbool   RenameTexture(ENuint ind,char *Name);
   __declspec(dllexport) void     DeleteTexture(ENuint ind);
   __declspec(dllexport) void     GetTextureData(ENMaterial *mat,ENuint ind);
   __declspec(dllexport) void     GetMaterialHeader(ENuint ind,ENMaterial::ENMaterialHeader &header);
   __declspec(dllexport) static   ENbool IsMap(char *FileName);
 protected:
   char          ErrorLog[1024];
   ENArray      *Groups;
   ENArray      *Textures;

   __declspec(dllexport) void DeleteVertexFromGroup(ENuint indG,ENuint indV);
};

///////////////////////////////////////////////////////////////////////
///Engine Map builder
///////////////////////////////////////////////////////////////////////
class ENMapBuildBase
{
 public:
   struct ENPortal
   {
    ENuint NumFaces,NumVerts;
    ENMapBase::ENMapFace       *faces;
    ENVector                   *vertexes;
    ENVector                   *normals;
    ENVector min,max;
   };
   struct ENMapBuildHeader
   {
    ENuint NumFaces;
    ENuint NumPortals;
    ENuint NumTexs;
    ENuint NumLights;
    ENuint NumObjects;
    ENuint NumSounds;
    char Package[MAX_PATH];
   };
};
///////////////////////////////////////////////////////////////////////
///Engine Map builder
///////////////////////////////////////////////////////////////////////

class ENMapBuilder : public ENMapBuildBase
{
 public:
   typedef void (*ENREPORTFUNC)(char *Msg,ENuint pos,ENuint max);

   __declspec(dllexport) ENMapBuilder(ENMap *map);
   __declspec(dllexport) ~ENMapBuilder();
   __declspec(dllexport) void SetReportFunc(ENREPORTFUNC rpfunc);
   __declspec(dllexport) ENbool Build(char *FileName);
 protected:
   ENMap *mapsrc;
   ENREPORTFUNC reportfunc;

   __declspec(dllexport) void  ReportMsg(char *Msg,ENuint pos,ENuint max);
   __declspec(dllexport) void  WriteHeader(ENFile handle);
   __declspec(dllexport) void  WriteBody(ENFile handle);
   __declspec(dllexport) void  WritePortals(ENFile handle);
   __declspec(dllexport) void  WriteTextures(ENFile handle);
   __declspec(dllexport) void  WriteBsp(ENFile handle);
 private:
   __declspec(dllexport) void  CalculateFaceNormals(ENVector *Normals);
   __declspec(dllexport) void  CalculateVertNormals(ENVector *Normals);
   __declspec(dllexport) void  WritePortal(ENbool *FreeFaces,ENVector *vertnormals,ENuint ind,ENFile handle);
   __declspec(dllexport) void  FindPortalFaces(ENbool *FreeFaces,ENArray *pfaces,ENuint ind);
   __declspec(dllexport) void  FindPortalVerts(ENArray *pfaces,ENArray *pverts,ENArray *pnormals,ENVector *vertnormals);
   __declspec(dllexport) void  BuildBox(ENArray *verts,ENVector &min,ENVector &max);
   __declspec(dllexport) ENint GetFacePos(ENuint ind,ENArray *FaceOrder);
};
///////////////////////////////////////////////////////////////////////
///Engine builded map loader
///////////////////////////////////////////////////////////////////////
class ENMapBuildLoader : public ENMapBuildBase
{
 public:
   enum ENMATTYPE {ENMATBASE,ENMATALPHA,ENMATBUMP,ENMATLUMI};
   __declspec(dllexport) ENMapBuildLoader();
   __declspec(dllexport) ~ENMapBuildLoader();
   __declspec(dllexport) void Clear();
   __declspec(dllexport) ENbool Load(char *FileName);
   __declspec(dllexport) ENMapBuildBase::ENMapBuildHeader GetHeader();
   __declspec(dllexport) ENMapBuildLoader::ENPortal *GetPortal(ENuint ind);
   __declspec(dllexport) void SetMaterial(ENuint ind,ENMATTYPE type);
   __declspec(dllexport) void GetSun(ENMapBase::ENMapSun *sun);
   __declspec(dllexport) ENbool SupportMaterialType(ENuint ind,ENMATTYPE type);
   __declspec(dllexport) void GetLights(ENLight *lightsdst);
   __declspec(dllexport) void GetSounds(ENSoundSrc *soundsdst);
   __declspec(dllexport) void GetObjects(ENMapBase::ENMapObject *objectsdst);
   __declspec(dllexport) ENbool SetSkyBox(ENuint ind);
   __declspec(dllexport) ENCamera GetCamera();
   __declspec(dllexport) ENBSPTree *GetBspTree();
 protected:
   ENMapBase::ENMapSun           Sun;
   ENCamera                      CameraObj;
   ENPictureLoaded              *SkyBox[6];
   ENMapBuildHeader              header;
   ENMaterialLoaded            **materials;
   ENLight                      *lights;
   ENSoundSrc                   *sounds;
   ENMapBase::ENMapObject       *objects;
   ENPortal                     *portals;
   ENBSPTree                    *bsptree;
};
///////////////////////////////////////////////////////////////////////
///Engine Model
///////////////////////////////////////////////////////////////////////
class ENModel
{
 public:
   struct ENModelFrame
   {
    ENVector *Vertexes;
   };
   struct ENModelGroup
   {
    char   Name[33];
    ENuint NumIndices;
    ENuint *Indices;
   };
   struct ENModelHeader
   {
    char ID[5];
    ENuint  NumFrames,NumSkins,NumVertexes,NumFaces,NumGroups;
   };
   struct ENModelFace
   {
    ENuint    Indices[3];
    ENVector2 Coord[3];
   };
   struct ENModelSkin
   {
    ENMaterial *mat;
    char        SkinName[33];
   };
   __declspec(dllexport) ENModel();
   __declspec(dllexport) ~ENModel();
   __declspec(dllexport) void Clear();
   __declspec(dllexport) ENbool Write(char *FileName);
   __declspec(dllexport) ENbool Load(char *FileName);
   __declspec(dllexport) ENbool LoadFromHandle(ENFile handle);
   __declspec(dllexport) void   AddVertex(ENVector vert);
   __declspec(dllexport) void   AddVertexes(ENVector *verts,ENuint num);
   __declspec(dllexport) void   AddFace(ENModel::ENModelFace face);
   __declspec(dllexport) void   AddFaces(ENModel::ENModelFace *faces,ENuint num);
   __declspec(dllexport) void   AddFrame(ENuint srcframe);
   __declspec(dllexport) void   DeleteFrame(ENuint ind);
   __declspec(dllexport) void   SetGLVertexPointer(ENuint frame);
   __declspec(dllexport) ENModelHeader GetHeader();
   __declspec(dllexport) ENfloat GetMinX(ENuint frame);
   __declspec(dllexport) ENfloat GetMinY(ENuint frame);
   __declspec(dllexport) ENfloat GetMinZ(ENuint frame);
   __declspec(dllexport) ENfloat GetMaxX(ENuint frame);
   __declspec(dllexport) ENfloat GetMaxY(ENuint frame);
   __declspec(dllexport) ENfloat GetMaxZ(ENuint frame);
   __declspec(dllexport) ENVector GetVertex(ENuint indV,ENuint indF);
   __declspec(dllexport) void     SetVertex(ENuint indV,ENuint indF,ENVector vec);
   __declspec(dllexport) ENModel::ENModelFace GetFace(ENuint ind);
   __declspec(dllexport) void     SetFace(ENuint ind,ENModel::ENModelFace face);
   __declspec(dllexport) void     DeleteFace(ENuint ind);
   __declspec(dllexport) void     DeleteVertex(ENuint ind);   

   __declspec(dllexport) ENint    GetIndexOfGroup(char *Name);
   __declspec(dllexport) ENbool   CreateGroup(char *Name);
   __declspec(dllexport) void     GetGroupName(ENuint ind,char *Name);
   __declspec(dllexport) ENbool   RenameGroup(ENuint ind,char *Name);
   __declspec(dllexport) void     DeleteGroup(ENuint ind);
   __declspec(dllexport) void     AddGeometry2Group(ENuint ind,ENuint *Indices,ENuint num);
   __declspec(dllexport) void     ReleaseGeoemtryFromGroups(ENuint *Indices,ENuint num);
   __declspec(dllexport) ENuint   GetNumGroupIndices(ENuint ind);
   __declspec(dllexport) void     GetGroupIndices(ENuint ind,ENuint *Indices);

   __declspec(dllexport) ENint    GetIndexOfSkin(char *Name);
   __declspec(dllexport) ENbool   CreateSkin(char *Name,char *FileName);
   __declspec(dllexport) ENbool   CreateSkin(char *Name,ENMaterial *mat); 
   __declspec(dllexport) void     GetSkinName(ENuint ind,char *Name);
   __declspec(dllexport) ENbool   RenameSkin(ENuint ind,char *Name);
   __declspec(dllexport) void     DeleteSkin(ENuint ind);
   __declspec(dllexport) void     GetSkinData(ENMaterial *mat,ENuint ind);
   __declspec(dllexport) static   ENbool CheckModelHeader(ENModel::ENModelHeader header);
   __declspec(dllexport) static   ENbool IsModel(char *FileName);
 protected:
   ENModelHeader         Header;
   ENModelFace          *Faces;
   ENModelFrame         *Frames;
   ENModelGroup         *Groups;
   ENModelSkin          *Skins;
   __declspec(dllexport) void     DeleteVertexFromGroup(ENModelGroup *group,ENuint indV);
};

///////////////////////////////////////////////////////////////////////
///Engine Model loaded
///////////////////////////////////////////////////////////////////////
class ENModelLoaded
{
 public:
   struct ENLoadedFrame
   {
    ENVector  *Vertexes;
    ENVector  *Normals;
    ENVector   Min,Max;
    ENVector   Center;
    ENfloat    Range;
   };
   struct ENLoadedHeader
   {
    ENuint NumFrames;
    ENuint NumSkins;
    ENuint NumVertexes;
    ENuint NumFaces;
   };
   __declspec(dllexport) ENModelLoaded(ENModel *data);
   __declspec(dllexport) ~ENModelLoaded();
   __declspec(dllexport) void   DrawRaw(ENVector *verts);
   __declspec(dllexport) void   DrawGround(ENVector *verts);
   __declspec(dllexport) void   Draw(ENVector *verts,ENVector *norms);
   __declspec(dllexport) void   DrawTex(ENVector *verts);
   __declspec(dllexport) void   DrawLightData(ENVector *verts,ENVector *norms,ENuint tex);
   __declspec(dllexport) void   SetCurrentFrame(ENfloat frame);
   __declspec(dllexport) void   SetCurrentSkin(ENuint skin);
   __declspec(dllexport) void   GetBox(ENVector &min,ENVector &max);
   __declspec(dllexport) void   GetBoxStatic(ENVector &min,ENVector &max);
   __declspec(dllexport) void   GetSphere(ENVector &pos,ENfloat &range,ENbool Interp);
   __declspec(dllexport) void   GetHeader(ENLoadedHeader &h);
   __declspec(dllexport) void   InterpVertexes(ENVector *vertexes);
   __declspec(dllexport) void   InterpVertex(ENuint ind,ENVector &vertex);
   __declspec(dllexport) void   InterpNormals (ENVector *normals);
   __declspec(dllexport) void   InterpData(ENVector *vertexes,ENVector *normals);
   __declspec(dllexport) ENbool isCurSkinBump();
 protected:
   ENLoadedHeader           header;
   ENMaterialLoaded       **skins;
   ENLoadedFrame           *frames;
   ENModel::ENModelFace    *faces;

   void SetModelHeader(ENModel::ENModelHeader h);
   void SetModelFaces(ENModel *m);
   void SetModelSkins(ENModel *m);
   void SetModelFrames(ENModel *m);
   void CalculateFrameBox(ENLoadedFrame *frame);
   void CalculateFrameRange(ENLoadedFrame *frame);
   void CalculateVertexNormals(ENLoadedFrame *frame);
   void CalculateFaceNormals(ENLoadedFrame *frame,ENVector *Normals);
 private:
   ENuint curskin;
   ENuint curframe,nxtframe;
   ENfloat framefact;
};
///////////////////////////////////////////////////////////////////////
///Engine ASCII mesh file
///////////////////////////////////////////////////////////////////////
class ENASCIIMesh
{
 public:
   struct ENMeshFace
   {
    ENuint Indices[3];
   };
   __declspec(dllexport) ENASCIIMesh();
   __declspec(dllexport) ~ENASCIIMesh();
   __declspec(dllexport) void   Clear();
   __declspec(dllexport) ENbool Write(char *FileName);
   __declspec(dllexport) ENbool Load(char *FileName);
   __declspec(dllexport) ENuint GetNumVertexes();
   __declspec(dllexport) ENuint GetNumFaces();
   __declspec(dllexport) void   GetVertexes(ENVector *Verts);
   __declspec(dllexport) void   GetFaces(ENASCIIMesh::ENMeshFace *vFaces);
   __declspec(dllexport) void   AddVertexes(ENVector *Verts,ENuint num);
   __declspec(dllexport) void   AddFaces(ENASCIIMesh::ENMeshFace *vFaces,ENuint num);
 protected:
   ENuint NumVertexes;
   ENuint NumFaces;
   ENMeshFace *Faces;
   ENVector   *Vertexes;

   __declspec(dllexport) void  WriteText(ENFile handle,char *text);
   __declspec(dllexport) ENint ExtractInteger(char *text);
   __declspec(dllexport) char *ExtractVertex(char *text,ENVector &Vertex);
   __declspec(dllexport) char *ExtractTriangle(char *text,ENASCIIMesh::ENMeshFace &face);
};
/////////////////////////////////////////////////////////////////
///Engine shader base class
/////////////////////////////////////////////////////////////////
class ENShaderClass
{
 public:
   __declspec(dllexport) ENShaderClass(char *VertShader,char *FragShader);
   __declspec(dllexport) ENShaderClass(char *VertShader,char *FragShader,ENPacker *packer);
   __declspec(dllexport) ~ENShaderClass();
   __declspec(dllexport) virtual ENbool InitShader();
   __declspec(dllexport) virtual void Activate();
   __declspec(dllexport) virtual void Deactivate();
 protected:
   struct ENRESOURCE
   {
    ENbool UsePackage;
    char VertShader[MAX_PATH];
    char FragShader[MAX_PATH];
    ENPacker *packer;
   };

   ENbool  Active;
   //Variables
   ENRESOURCE resource;
   ENhandle ProgramHandle;
   //Functions
   __declspec(dllexport) void   ENprintInfoLog(ENhandle object);
   __declspec(dllexport) ENbool AddShader(const char *shaderSource,GLenum shaderType);
   __declspec(dllexport) char  *ReadSource(char *Name);
   __declspec(dllexport) char  *ReadSourceFile(char *FileName);
   __declspec(dllexport) char  *ReadSourcePack(char *Name);
};


#endif
