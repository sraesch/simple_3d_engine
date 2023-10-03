//---------------------------------------------------------------------------
#ifndef ScriptFormContainerH
#define ScriptFormContainerH
//---------------------------------------------------------------------------
#include <vcl.h>
#include "EngineScript.h"

class ScriptFormClass
{
 public:
   static void Init(TForm *parent,TListBox *ScriptFileBox);
   static void Free();
   static TForm *AddForm();
   static void DeleteForm(void *form);
   static TForm *GetCurrent();
   static ENbool Save();
   static ENbool SaveAs();
   static void SaveAll();
   static void Load(AnsiString FileName);
   static AnsiString GetMainFile();
   static void Undo();
   static void Search(TFindDialog *fd);
   static void CopyText();
   static void CutText();
   static void PasteText();
   static void Activate(ENuint ind);
   static void SetMyTitle(TObject *Sender,AnsiString str);
 private:
   static TForm *parent;
   static TList *FormContainer;
   static TListBox *ScriptFileBox;
};

#endif
