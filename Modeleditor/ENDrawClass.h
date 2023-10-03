//---------------------------------------------------------------------------
#ifndef ENDrawClassH
#define ENDrawClassH
//---------------------------------------------------------------------------
#include <vcl.h>
#include "EngineMedia.h"

class ENDrawClass
{
 public:
   enum ENDrawClassTypes {ENDRAWTOP,ENDRAWFRONT,ENDRAWSIDE,ENDRAW3D};
   enum ENDrawClassTool  {ENDRAWMOVECAM,ENDRAWROTATECAM,ENDRAWZOOM,
                          ENDRAWMOVEOBJ,ENDRAWROTATEOBJ,ENDRAWSCALEOBJ,
                          ENDRAWADDVERTEX,ENDRAWADDFACE,
                          ENDRAWADDCUBE,ENDRAWADDCONE,ENDRAWADDCYLINDER,
                          ENDRAWADDSPHERE,
                          ENDRAWSELECT};
   enum ENDrawClassMode  {ENDRAWWIRE,ENDRAWSOLID,ENDRAWTEX};
   struct ENDrawOptions
   {
    ENVector GridColor;
    ENVector VertexColor;
    ENVector SVertexColor;
    ENVector FaceColor;
    ENVector SFaceColor;
    ENVector ToolColor;
    ENVector BackColor;
    ENfloat GridSize;
    ENint   NumGridLines;
    ENbool  Cullfacing;
    ENbool  EnableDrawVertexes;
    ENbool  EnableDrawFaces;
    ENbool  EnableDrawOrigin;
    ENbool  EnableDrawGrid;
   };
   ENDrawClass(TForm *form,ENDrawClass::ENDrawClassTypes type);
   ~ENDrawClass();
   static ENDrawClass::ENDrawOptions GetOptions();
   static void SetOptions(ENDrawClass::ENDrawOptions opt);
   static void InitOptions();
   static void SetDefaultOptions();
   void ConfigProjMatrix();
   void ConfigStdMatrix();
   void Draw();
   void OnMouseDown(ENint x,ENint y,ENbool eShift);
   void OnMouseMove(ENint x,ENint y);
   void OnMouseUp();
   static void SetToolMode(ENDrawClass::ENDrawClassTool tool);
   static ENDrawClass::ENDrawClassTool GetToolMode();
   static void SetViewMode(ENDrawClass::ENDrawClassMode mode);
   static void SetDirMode(char Direction);
   static void SetVertState(TStatusPanel *VertState);
 private:
   struct ENAddFaceStruct
   {
    ENuint state;
    ENuint Ind[3];
   };
   static ENDrawOptions         options;
   static ENDrawClassTool       tool;
   static ENDrawClassMode       mode;
   static char                  Direction;
   static TStatusPanel          *VertexState;
   ENbool MoveCam,RotateCam,ZoomCam,MoveObj,RotateObj,ScaleObj,Selection;
   ENMatrixSpec                 Matrices;
   ENVector2 Save2DPos;
   ENVector2 SelectFrom,SelectTo;
   ENVector  TempModVec,CenterVec;
   ENAddFaceStruct AddFaceStruct;
   ENGLClass *globj;
   ENDrawClassTypes type;
   TForm *form;
   void Transform();
   void DrawGrid();
   void DrawOrigin();
   void DrawVertexes();
   void DrawFaces();
   void DrawSelection();
   void DrawAddFaceTool();
   void BeginDraw2D();
   void EndDraw2D();

   void MoveMatrixOutOfDepth();
   ENVector2 DirectionRestriction(ENVector2 vec);
};

#endif
