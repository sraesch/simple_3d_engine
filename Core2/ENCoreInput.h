//---------------------------------------------------------------------------
#ifndef ENCoreInputH
#define ENCoreInputH
//---------------------------------------------------------------------------
#include "EngineSystem.h"

#define EN_CURSOR_FORCE      0
#define EN_CURSOR_SHOW       1
#define EN_CURSOR_SHOW_FORCE 2

class ENCoreInput
{
 public:
   static ENbool Init();
   static void   UpdateKey(ENuint key,ENbool stat);
   static void   UpdateCursor(ENint x,ENint y);
   static void   SetKeyFunc(ENuint key,ENbool stat,ENint func);
   static ENint  GetCursorMode();
   static void   GetCursorPos(ENint &x,ENint &y);
   static ENbool GetKeyStat(ENuint key);
 private:
   struct ENInputFunc
   {
    ENint down;
    ENint up;
   };

   static ENbool        KeyStats[256];
   static ENInputFunc   KeyFuncs[256];
   static ENint        *CursorX;
   static ENint        *CursorY;
   static ENint        *CursorMode;
   static ENVector2    *CursorForce;

   static ENbool        RegisterScript();
   static void          UpdateCursorForce(ENint &x,ENint &y);
   static void          RunKeyFunc(ENuint key,ENbool pressed);
   static void          ExecStdKeyFunc(ENuint key,ENbool pressed);
};
#endif
