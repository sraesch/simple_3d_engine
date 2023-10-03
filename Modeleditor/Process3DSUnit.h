//---------------------------------------------------------------------------
#ifndef Process3DSUnitH
#define Process3DSUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include "EngineSystem.h"
#include "EngineMedia.h"

class ENProcess3DS
{
 public:
   struct ENSkinDim
   {
    ENVector2 Pos;
    ENVector2 Size;
   };
   static ENbool Import(AnsiString FileName,TLabel *stat,TProgressBar *statbar,
                        ENbool materials);
 private:
   static ENuint       NumMaterials;
   static ENPicture  **mats;
   static TLabel *stat;
   static TProgressBar *statbar;
   static ENSkinDim    *SkinDims;
   static ENuint        SkinWidth;
   static ENuint        SkinHeight;

   static ENVector2     ConvertCoord(ENfloat x,ENfloat y,ENuint matID);
   static ENbool        LoadMaterial(ENPicture *mat,AnsiString FName,AnsiString ModelFile);
   static void          CreateSkin();
   static void          SetMsg(AnsiString msg);
   static void          StartControls();
   static void          EndControls();
};
#endif
