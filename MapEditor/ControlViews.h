//---------------------------------------------------------------------------
#ifndef ControlViewsH
#define ControlViewsH
//---------------------------------------------------------------------------
#include <vcl.h>
#include "EngineMedia.h"

class ENViewControl
{
 public:
   static void Init(TForm *Parent,TPopupMenu *menu);
   static void Cascade();
   static void Tile();
   static void ShowAll();
   static void DrawAll();
   static void SelAllViewsDefault();
 private:
   static TForm *Parent;

};

#endif
