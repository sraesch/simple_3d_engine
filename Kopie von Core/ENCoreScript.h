//---------------------------------------------------------------------------
#ifndef ENCoreScriptH
#define ENCoreScriptH
//---------------------------------------------------------------------------
#include "EngineScript.h"

class ENCoreScript
{
 public:
   typedef void*        ENScriptVar;
   static ENbool        Init();
   static void          Free();
   static void         *GetScriptVar(char *Name);
   static ENbool        LoadScript(char *FileName);
   static ENScriptCode *GetCodeObj();
   static ENuint        GetNumPaths();
   static void          GetPath(ENuint ind,ENScriptCode::ENPATH &path);
   static ENuint        GetNumPanels();
   static ENScriptPanel *GetPanel(ENuint ind);
   static ENuint        GetNumTexts();
   static ENScriptText *GetText(ENuint ind);
   static void          GetPackage(char *package);
   static ENint         FindAction(char *Name);
 private:
   static ENScriptCode *code;
   static ENint  SizeInt;
   static ENint  SizeFloat;
   static ENint  SizeString;
   static ENint  SizeVec2;
   static ENint  SizeVec3;
   static ENint  SizeVec4;
   static ENint  SizeMat2;
   static ENint  SizeMat3;
   static ENint  SizeMat4;
   static ENint  SizeLight;   

   static ENbool        InitScriptFuncs();   
   static ENbool        GetTypeSize();
   static ENbool        SetScriptFunc(char *Name,
                                      ENScriptExtern::ENSCRIPTEXTERNFUNCTION func);
   static void          ScriptSHUTDOWN(ENubyte *Data,void *res);
   static void          ScriptSETKEYDOWN(ENubyte *Data,void *res);
   static void          ScriptSETKEYUP(ENubyte *Data,void *res);
   static void          ScriptSETCURSOR(ENubyte *Data,void *res);
   static void          ScriptSETBITMAP(ENubyte *Data,void *res);
   static void          ScriptSETFONT(ENubyte *Data,void *res);
   static void          ScriptLOADMAP(ENubyte *Data,void *res);
   static void          ScriptSHOWMESSAGE(ENubyte *Data,void *res);
   static void          ScriptVEC3_MOVE(ENubyte *Data,void *res);
   static void          ScriptVEC3_MOVE_COLLISION(ENubyte *Data,void *res);
   static void          ScriptVEC3_MOVE_COLLISION_SMOOTH(ENubyte *Data,void *res);
   static void          ScriptVEC3_MOVE_COLLISION2(ENubyte *Data,void *res);
   static void          ScriptVEC3_MOVE_COLLISION_SMOOTH2(ENubyte *Data,void *res);
   static void          ScriptOBJECT_MOVE(ENubyte *Data,void *res);
   static void          ScriptOBJECT_MOVE_SMOOTH(ENubyte *Data,void *res);
   static void          ScriptOBJECT_MOVE2(ENubyte *Data,void *res);
   static void          ScriptOBJECT_MOVE_SMOOTH2(ENubyte *Data,void *res);
   static void          ScriptISKEYPRESSED(ENubyte *Data,void *res);
   static void          ScriptCREATELIGHT(ENubyte *Data,void *res);
   static void          ScriptSETLIGHT(ENubyte *Data,void *res);
   static void          ScriptGETLIGHT(ENubyte *Data,void *res);
   static void          ScriptDELETELIGHT(ENubyte *Data,void *res);
   static void          ScriptDELETEOBJECT(ENubyte *Data,void *res);
   static void          ScriptGETNUMPROCESSES(ENubyte *Data,void *res);
   static void          ScriptWORLD_TO_SCREEN(ENubyte *Data,void *res);
   static void          ScriptSCREEN_TO_WORLD(ENubyte *Data,void *res);
   static void          ScriptCREATEOBJECT(ENubyte *Data,void *res);
   static void          ScriptCREATEOBJECTPOS(ENubyte *Data,void *res);
   static void          ScriptRANDOM(ENubyte *Data,void *res);
   static void          ScriptSPHERECOLLISIONMAP(ENubyte *Data,void *res);
   static void          ScriptCREATESOUND(ENubyte *Data,void *res);
   static void          ScriptGETSOUND(ENubyte *Data,void *res);
   static void          ScriptSETSOUND(ENubyte *Data,void *res);
   static void          ScriptDELETESOUND(ENubyte *Data,void *res);
   static void          ScriptSETSOUNDSTATE(ENubyte *Data,void *res);
   static void          ScriptGETMODELVERT(ENubyte *Data,void *res);
   static void          ScriptLENS_ADD(ENubyte *Data,void *res);
   static void          ScriptLENS_CREATE(ENubyte *Data,void *res);
   static void          ScriptLENS_POS(ENubyte *Data,void *res);
   static void          ScriptLENS_COLOR(ENubyte *Data,void *res);
   static void          ScriptLENS_DELETE(ENubyte *Data,void *res);
   static void          ScriptTRACE(ENubyte *Data,void *res);
};

#endif
