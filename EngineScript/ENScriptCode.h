#ifndef       _EN_SCRIPTCODE_H_
#define       _EN_SCRIPTCODE_H_

#include "ENScriptTypes.h"

class ENScriptCode
{
 public:
  struct ENACTIONREC
  {
   char Name[1024];
   ENint Pos;
  };
   enum ENSCRIPTCOMID {ENSCRIPTEXIT,ENSCRIPTJUMP,ENSCRIPTJUMPTRUE,ENSCRIPTJUMPFALSE,
                    ENSCRIPTEXTERNCALL,
                    ENPUSH,ENPUSHCONST,ENPUSHRESULT,ENPUSHLOCAL,ENPUSHBYADDR,
					ENPOP,ENPOPLOCAL,ENPOPRESULT,ENPOPLOGIC,ENPOPTIME,ENPOPBYADDR,
					ENSAVESTACK,ENRESETSTACK,ENADDSTACK,
                    ENCONVIF,ENCONVIS,ENCONVFI,ENCONVFS,ENCONVSI,ENCONVSF,
                    ENOPCOM,
                    ENSAVEPOS,ENRESETPOS,ENWAIT,ENWAITT,ENSETADDR};

   struct ENSCRIPTCOMMAND
   {
    ENSCRIPTCOMID comid;
    ENint a,b;
   };
   struct ENPATH
   {
    char Path[MAX_PATH];
   };
   __declspec(dllexport) ENScriptCode();
   __declspec(dllexport) ~ENScriptCode();

   __declspec(dllexport) void ClearAll();
   __declspec(dllexport) ENbool LoadFromFile(char *FileName);
   __declspec(dllexport) ENbool SaveToFile(char *FileName);
   //Set methods
   __declspec(dllexport) void   AddText(ENint txt);
   __declspec(dllexport) ENuint AddConst(ENubyte *cons,ENuint size);
   __declspec(dllexport) void   AddPath(ENPATH path);
   __declspec(dllexport) void   SetMainFunc(ENint pos);
   __declspec(dllexport) void   SetLoadFunc(ENint pos);
   __declspec(dllexport) void   SetPackage(char *fname);
   __declspec(dllexport) void   AddPanel(ENint panel);
   __declspec(dllexport) void   AddAction(ENACTIONREC action);
   __declspec(dllexport) void   AddCommand(ENSCRIPTCOMID id,ENint a,ENint b);
   __declspec(dllexport) void   SetCommandParamA(ENint ind,ENint a);
   __declspec(dllexport) void   SetStackPointer(ENint pos);
   //Get methods
   __declspec(dllexport) void   GetPackage(char *dst);
   __declspec(dllexport) ENuint GetNumPaths();
   __declspec(dllexport) ENuint GetNumTexts();
   __declspec(dllexport) ENuint GetTextAddr(ENuint ind);
   __declspec(dllexport) ENuint GetPanelAddr(ENuint ind);
   __declspec(dllexport) ENuint GetNumPanels();
   __declspec(dllexport) ENuint GetNumCommands();
   __declspec(dllexport) void   GetPath(ENuint ind,ENPATH &path);
   __declspec(dllexport) ENuint GetStackPointer();
   __declspec(dllexport) ENint  GetStack();
   __declspec(dllexport) ENint  GetMainFunc();
   __declspec(dllexport) ENint  GetLoadFunc();
   __declspec(dllexport) ENScriptCode::ENSCRIPTCOMMAND GetCommand(ENuint ind);
   __declspec(dllexport) ENint  GetConstAddr();
   __declspec(dllexport) ENint  GetNumActions();
   __declspec(dllexport) ENTEXTSTR GetActionName(ENuint ind);
   __declspec(dllexport) ENint  GetActionPos(char *name);
 protected:
   //Constant values
   ENubyte *_ENConst;
   ENuint   _ENSizeConst;
   //Paths
   ENuint     NumPaths;
   ENPATH     *Paths;
   //Jump pos
   ENint     MainFunc,LoadFunc;
   //Text
   ENuint       NumTexts;
   ENint       *Texts;
   //Panels
   ENuint       NumPanels;
   ENint       *Panels;
   //Actions
   ENuint       NumActions; 
   ENACTIONREC *Actions;
   //Package
   char  Package[256];
   //Code
   ENuint NumCommands;
   ENSCRIPTCOMMAND *Commands;
   //Stack
   ENubyte                 ENStack[_EN_SCRIPT_STACK_SIZE];
   ENint                   _ENStackPointer;
};

#endif