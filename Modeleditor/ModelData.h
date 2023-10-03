//---------------------------------------------------------------------------
#ifndef ModelDataH
#define ModelDataH
//---------------------------------------------------------------------------
#include "EngineMedia.h"
#include "ENDrawClass.h"

class ENModelData
{
 public:
   enum ENModelPrefab {ENPREFABCUBE,ENPREFABCONE,ENPREFABCYLINDER,ENPREFABSPHERE};

   static void     Init();
   static void     Free();
   static void     New();
   static void     AddVertex(ENVector2 mpos,ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec);
   static void     AddPrefab(ENModelData::ENModelPrefab prefab,ENVector2 mpos,
                   ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec);
   static void     Select(ENVector2 a,ENVector2 b,ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec);
   static void     SelectCoord(ENVector2 a,ENVector2 b);
   static ENint    SelectOneVert(ENVector2 p,ENDrawClass::ENDrawClassTypes type,ENMatrixSpec mspec);
   static void     AddFace(ENuint Ind[]);
   static void     AddFaces(ENModel::ENModelFace *faces,ENuint num);
   static void     AddFrame();
   static void     DeleteFrame();
   static void     AddVertexRaw(ENVector vec);
   static void     AddVertexes(ENVector *verts,ENuint num);
   static ENVector From2DTo3D(ENVector2 mpos,ENfloat depth,ENMatrixSpec mspec);
   static ENVector From3DTo2D(ENVector  vec,ENMatrixSpec mspec);
   static void     SetGLVertexes();
   static ENVector GetVertex(ENuint ind);
   static ENModel::ENModelFace GetFace(ENuint ind);
   static ENfloat  GetMinVal(char coord);
   static ENfloat  GetMaxVal(char coord);
   static ENModel::ENModelHeader GetHeader();
   static void     SelectAll();
   static void     SelectNone();
   static void     InvertSelected();
   static void     SelectConnected();
   static ENbool   IsVertSelected(ENuint ind);
   static ENbool   IsFaceSelected(ENuint ind);
   static ENbool   IsCoordSelected(ENuint ind);   
   static void     SelectAllCoord();
   static void     SelectNoneCoord();
   static ENbool   Save();
   static void     Save(char *FileName);
   static void     Open(char *FileName);
   static void     Translate(ENVector Pos,ENbool selected);
   static void     TranslateCoord(ENVector2 Pos,ENbool selected);
   static void     Scale(ENVector center,ENVector svec,ENbool selected);
   static void     ScaleCoord(ENVector2 center,ENVector2 svec,ENbool selected);
   static void     Rotate(ENVector center,ENfloat angle,char Axis,ENbool selected);
   static void     RotateCoord(ENVector2 center,ENfloat angle,ENbool selected);   
   static ENbool   HasToSave();
   static ENuint   GetNumSelected();
   static ENuint   GetNumSelectedCoord();   
   static void     GetSelectedVerts(ENVector *Vertexes);
   static void     GetSelectedCoord(ENVector2 *Coord);
   static void     Box(ENVector &min,ENVector &max);
   static void     BoxSelected(ENVector &min,ENVector &max);
   static void     BoxSelectedCoord(ENVector2 &min,ENVector2 &max);
   static ENVector Center(ENbool selected);
   static void     SetCurrentFrame(ENuint ind);
   static ENuint   GetCurrentFrame();
   static AnsiString GetGroupName(ENuint ind);
   static void     CreateGroup(char *GroupName);
   static void     RenameGroup(ENuint ind,char *GroupName);
   static void     DeleteGroup(ENuint ind);
   static void     SelectGroup(ENuint ind);
   static void     AddGeometryToGroup(ENuint ind);
   static void     DeleteSelectedFaces();
   static void     DeleteSelectedVertexes();
   static void     FlipNormals();
   static void     Optimize(ENbool UnusedVerts,ENbool Verts,ENfloat toleranz);
   static void     ConnectAll(ENfloat tolerance);
   static AnsiString GetSkinName(ENuint ind);
   static void     CreateSkin(char *SkinName,char *FileName);
   static void     CreateSkin(char *SkinName,ENMaterial *mat);   
   static void     RenameSkin(ENuint ind,char *SkinName);
   static void     DeleteSkin(ENuint ind);
   static ENint    GetCurrentSkin();
   static void     SetCurrentSkin(ENint cskin);
   static void     ExportSkin(char *FileName,ENuint cskin);
   static void     ActivateCurrentSkin();
   static void     MakeSkinGridTop();
   static void     MakeSkinGridFront();
   static void     MakeSkinGridSide();
   static void     MakeSkinGridCube();
   static void     ExportToASCII(char *FileName);
   static void     ImportFromASCII(char *FileName);
   static ENint    GetSelectedVertInd();
 private:
   static ENint   CurrentSkin;
   static char    CurrentFile[MAX_PATH];
   static ENbool  MustSave;
   static ENModel *model;
   static ENubyte *VertSelected;
   static ENubyte *CoordSelected;
   static ENMaterialLoaded **materials;
   static ENuint   CurrentFrame;

   static ENVector FaceNormal(ENModel::ENModelFace face);
   static void   SelectConnectedRe(ENuint index);
   static ENbool IsVertexUnused(ENuint ind);
   static void   ConnectVertexes(ENuint a,ENuint b);
   static void   DeleteAllSkins();
   static void   InitAllSkins();
};
#endif
