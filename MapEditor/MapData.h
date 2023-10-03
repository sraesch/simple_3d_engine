//---------------------------------------------------------------------------
#ifndef MapDataH
#define MapDataH
//---------------------------------------------------------------------------
#include "EngineMedia.h"
#include "ENDrawClass.h"

struct ENImportQ3Flags
{
 ENbool mesh,billb,patches,textures,polygones;
 char   TexturePath[MAX_PATH];
};

class ENMapData
{
 public:
   enum ENModelPrefab {ENPREFABCUBE,ENPREFABCONE,ENPREFABCYLINDER,ENPREFABSPHERE};

   static void     Init();
   static void     Free();
   static void     New();
   static void     AddVertex(ENVector2 mpos,ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec);
   static void     AddLight(ENVector2 mpos,ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec);
   static void     AddSound(ENVector2 mpos,ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec);
   static void     AddObject(ENVector2 mpos,ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec);
   static void     AddPrefab(ENMapData::ENModelPrefab prefab,ENVector2 mpos,
                   ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec);
   static void     Select(ENVector2 a,ENVector2 b,ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec);
   static ENint    SelectOneVert(ENVector2 p,ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec);
   static void     AddFace(ENuint Ind[]);
   static void     AddFace2(ENMapBase::ENMapFace *face);
   static void     AddFaces(ENMapBase::ENMapFace *faces,ENuint num);
   static void     AddVertexRaw(ENVector vec);
   static void     AddVertexes(ENVector *verts,ENuint num);
   static ENVector From2DTo3D(ENVector2 mpos,ENfloat depth,ENMatrixSpec mspec);
   static ENVector From3DTo2D(ENVector  vec,ENMatrixSpec mspec);
   static void     SetGLVertexes();
   static ENVector GetVertex(ENuint ind);
   static ENLight  GetLight(ENuint ind);
   static ENSoundSrc  GetSound(ENuint ind);
   static ENMapBase::ENMapObject GetObject(ENuint ind);
   static ENMapBase::ENMapFace GetFace(ENuint ind);
   static void     SetFace(ENuint ind,ENMapBase::ENMapFace face);
   static ENMapBase::ENMapHeader GetHeader();
   static void     SelectAll();
   static void     SelectNone();
   static void     InvertSelected();
   static void     SelectConnected();
   static ENfloat  GetMinVal(char coord);
   static ENfloat  GetMaxVal(char coord);   
   static ENbool   IsVertSelected(ENuint ind);
   static ENbool   IsLightSelected(ENuint ind);
   static ENbool   IsSoundSelected(ENuint ind);
   static ENbool   IsObjectSelected(ENuint ind);
   static ENbool   IsCameraSelected();
   static ENbool   IsFaceSelected(ENuint ind);
   static ENbool   Save();
   static void     Save(char *FileName);
   static void     Open(char *FileName);
   static void     Translate(ENVector Pos,ENbool selected);
   static void     TranslateTex(ENVector2 Pos);
   static void     Scale(ENVector center,ENVector svec,ENbool selected);
   static void     ScaleTex(ENVector2 svec);   
   static void     Rotate(ENVector center,ENfloat angle,char Axis,ENbool selected);
   static void     RotateTex(ENfloat angle);
   static ENbool   HasToSave();
   static ENuint   GetNumSelected();
   static void     GetSelectedVerts(ENVector *Vertexes);
   static void     Box(ENVector &min,ENVector &max);
   static void     BoxSelected(ENVector &min,ENVector &max);
   static ENVector Center(ENbool selected);
   static AnsiString GetGroupName(ENuint ind);
   static void     CreateGroup(char *GroupName);
   static void     RenameGroup(ENuint ind,char *GroupName);
   static void     DeleteGroup(ENuint ind);
   static void     SelectGroup(ENuint ind);
   static void     AddGeometryToGroup(ENuint ind);
   static void     DeleteSelectedFaces();
   static void     DeleteSelected();
   static void     FlipNormals();
   static void     Optimize(ENbool UnusedVerts,ENbool Verts,ENfloat toleranz);
   static void     ConnectAll(ENfloat tolerance);
   static AnsiString GetTextureName(ENuint ind);
   static void     CreateTexture(char *Name,char *FileName);
   static void     CreateTexture(char *Name,ENMaterial *mat);
   static void     RenameTexture(ENuint ind,char *Name);
   static void     DeleteTexture(ENuint ind);
   static void     AssignTexture(ENuint ind);
   static void     GetTexturePreview(Graphics::TBitmap *bmp,ENuint ind);
   static void     GetTextureInfo(ENuint ind,char *info);
   static void     ActivateTexture(ENint ind);
   static void     CubeCoord(ENMapBase::ENMapFace *face,
                             ENVector2 pos,ENVector2 scale,ENfloat angle);
   static void     MakeCubeCoord(ENVector vertex,ENVector2 &coord);                             
   static void     RotateCoord(ENVector2 &coord,ENfloat angle);                             
   static void     ExportToASCII(char *FileName);
   static void     ImportFromASCII(char *FileName);
   static void     ImportFromQuake3(char *FileName,ENImportQ3Flags flags);
   static void     EditLight();
   static void     EditObject();
   static void     EditSound();
   static void     SetPackage(char *FileName);
   static void     Build(char *FileName);

   static void     SetSky(ENuint ind,ENPicture *pic);
   static void     GetSky(ENuint ind,ENPicture *pic);

   static void     SetSun(ENMapBase::ENMapSun *sun);
   static void     GetSun(ENMapBase::ENMapSun *sun);

   static ENCamera GetCamera();
   static void     SetCamera(ENCamera cam);

   static void     GetPreview(Graphics::TBitmap *bmp,ENPicture *pic,ENfloat w,ENfloat h);
 private:
   static char    CurrentFile[MAX_PATH];
   static ENbool  MustSave;
   static ENMap  *map;
   static ENBooleanArray *VertSelected;
   static ENBooleanArray *LightSelected;
   static ENBooleanArray *SoundSelected;
   static ENBooleanArray *ObjectSelected;
   static ENbool          CameraSelected;
   static ENMaterialLoaded **materials;

   static ENVector FaceNormal(ENMapBase::ENMapFace face);
   static void   SelectConnectedRe(ENuint index);
   static ENbool IsVertexUnused(ENuint ind);
   static void   ConnectVertexes(ENuint a,ENuint b);
   static void   DeleteAllTextures();
   static void   InitAllTextures();
   static void   MakeCubeCoord(ENVector vertex,ENVector normal,ENVector2 &coord);
   static void   FindExt(char *srcstr,char *dststr);
};
#endif
