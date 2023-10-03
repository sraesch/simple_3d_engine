//---------------------------------------------------------------------------
#include "ENCore2D.h"
#include "ENCoreWindow.h"
#include "ENCoreInput.h"
#include "ENCoreScript.h"
#include "ENCoreMain.h"
#include "ENCorePacker.h"
#include "ENCoreLoader.h"
#include "ENLensFlare.h"
//---------------------------------------------------------------------------
#define EN_MAX_LAYER 16
/////////////////////////////////////////////////////////////
///Cursor class
/////////////////////////////////////////////////////////////
class ENCoreCursor
{
 public:
   static ENbool Init()
   {
    //Register script variables
    if(!RegisterScript()) return false;
    //Init cursor values
    cursor->Size=ENVector2(10,10);
    cursor->Mode=EN_SHOW_NORMAL;
    cursor->MirrorX=false;
    cursor->MirrorY=false;
    for(ENuint a=0;a<16;a++)
       cursor->Colors[a]=255;
    //Finished
    return true;
   }
   static void Draw()
   {
    //Variables
    ENint  CursorPos[2];
    //Get cursor pos
    ENCoreInput::GetCursorPos(CursorPos[0],CursorPos[1]);
    if(ENCoreInput::GetCursorMode()!=EN_CURSOR_FORCE)
    {
     //Set texture
     if(cursorbmp)
       cursorbmp->Set((ENuint)cursor->Mode);
     else
       glBindTexture(GL_TEXTURE_2D,0);
     //Begin to draw
     glBegin(GL_QUADS);
     //1.
     glTexCoord2i(0,0);
     glColor4ub((ENubyte)cursor->Colors[0],(ENubyte)cursor->Colors[1],
                (ENubyte)cursor->Colors[2],(ENubyte)cursor->Colors[3]);
     glVertex2i(CursorPos[0],CursorPos[1]);
     //2.
     glTexCoord2i(1,0);
     glColor4ub((ENubyte)cursor->Colors[4],(ENubyte)cursor->Colors[5],
                (ENubyte)cursor->Colors[6],(ENubyte)cursor->Colors[7]);
     glVertex2i(CursorPos[0]+cursor->Size.v[0],CursorPos[1]);
     //3.
     glTexCoord2i(1,1);
     glColor4ub((ENubyte)cursor->Colors[8],(ENubyte)cursor->Colors[9],
                (ENubyte)cursor->Colors[10],(ENubyte)cursor->Colors[11]);
     glVertex2i(CursorPos[0]+cursor->Size.v[0],CursorPos[1]+cursor->Size.v[1]);
     //4.
     glTexCoord2i(0,1);
     glColor4ub((ENubyte)cursor->Colors[12],(ENubyte)cursor->Colors[13],
                (ENubyte)cursor->Colors[14],(ENubyte)cursor->Colors[15]);
     glVertex2i(CursorPos[0],CursorPos[1]+cursor->Size.v[1]);
     glEnd();
    }
   }
   static void SetBmp(char *Name)
   {
    cursorbmp=(ENPictureLoaded*)ENCorePacker::GetSource(Name,ENPackerBase::ENPICTURE);
    if(!cursorbmp)
    {
     char Buffer[80];
     sprintf(Buffer,"Can not find resource %s!!!",Name);
     ENCoreMain::ShowError(Buffer);
    }
   }
 private:
   static ENScriptCursor  *cursor;
   static ENPictureLoaded *cursorbmp;

   static ENbool RegisterScript()
   {
    //Cursor
    cursor=(ENScriptCursor*)ENCoreScript::GetScriptVar("cursor");
    if(!cursor)
    {
     ENCoreMain::ShowError("Cannot initialize script variable cursor!!!");
     return false;
    }
    //Finished
    return true;
   }
};

ENScriptCursor *ENCoreCursor::cursor=NULL;
ENPictureLoaded *ENCoreCursor::cursorbmp=NULL;
/////////////////////////////////////////////////////////////
///Panels class
/////////////////////////////////////////////////////////////
class ENCorePanels
{
 public:
   static void Init()
   {
    //Get number of panels from script class
    NumPanels=ENCoreScript::GetNumPanels();
    //Alloc space
    panels=(ENScriptPanel**)ENAllocMemory(sizeof(ENScriptPanel*)*NumPanels);
    //Init panels
    for(ENuint a=0;a<NumPanels;a++)
    {
     panels[a]=ENCoreScript::GetPanel(a);
     panels[a]->Layer=0;
     panels[a]->Src=NULL;
     panels[a]->Pos=ENVector2(0.0f,0.0f);
     panels[a]->Size=ENVector2(100.0f,100.0f);
     panels[a]->Visible=false;
     panels[a]->MirrorX=false;
     panels[a]->MirrorY=false;
     panels[a]->Mode=EN_SHOW_NORMAL;
     panels[a]->ColorR=255;
     panels[a]->ColorG=255;
     panels[a]->ColorB=255;
     panels[a]->ColorA=255;
    }
   }
   static void Free()
   {
    ENFreeMemory(panels);
   }
   static void Draw(ENuint layer)
   {
    for(ENuint a=0;a<NumPanels;a++)
      if(((ENuint)panels[a]->Layer)%EN_MAX_LAYER==layer&&panels[a]->Visible)
        DrawPanel(panels[a]);
   }
 private:
   static ENuint          NumPanels;
   static ENScriptPanel **panels;

   static void DrawPanel(ENScriptPanel *panel)
   {
    //Variables
    ENPictureLoaded *pic;
    ENVector2 t[4];
    //Set texture
    pic=(ENPictureLoaded*)panel->Src;
    if(pic)
       pic->Set((ENuint)panel->Mode);
     else
       glBindTexture(GL_TEXTURE_2D,0);
    //TexCoord
    if(!panel->MirrorX) t[0].v[0]=0; else t[0].v[0]=1;
    if(!panel->MirrorX) t[1].v[0]=1; else t[1].v[0]=0;
    if(!panel->MirrorX) t[2].v[0]=1; else t[2].v[0]=0;
    if(!panel->MirrorX) t[3].v[0]=0; else t[3].v[0]=1;
    if(!panel->MirrorY) t[0].v[1]=1; else t[0].v[1]=0;
    if(!panel->MirrorY) t[1].v[1]=1; else t[1].v[1]=0;
    if(!panel->MirrorY) t[2].v[1]=0; else t[2].v[1]=1;
    if(!panel->MirrorY) t[3].v[1]=0; else t[3].v[1]=1;
    //Set color
    glColor4ub((ENubyte)panel->ColorR,(ENubyte)panel->ColorG,
	       (ENubyte)panel->ColorB,(ENubyte)panel->ColorA);
    //Draw
    glBegin(GL_QUADS);
    glTexCoord2fv((ENfloat*)&t[0]);
    glVertex2f(panel->Pos.v[0],panel->Pos.v[1]);
    glTexCoord2fv((ENfloat*)&t[1]);
    glVertex2f(panel->Pos.v[0]+panel->Size.v[0],panel->Pos.v[1]);
    glTexCoord2fv((ENfloat*)&t[2]);
    glVertex2f(panel->Pos.v[0]+panel->Size.v[0],panel->Pos.v[1]+panel->Size.v[1]);
    glTexCoord2fv((ENfloat*)&t[3]);
    glVertex2f(panel->Pos.v[0],panel->Pos.v[1]+panel->Size.v[1]);
    glEnd();
   }
};

ENuint          ENCorePanels::NumPanels=0;
ENScriptPanel **ENCorePanels::panels=NULL;
/////////////////////////////////////////////////////////////
///Text class
/////////////////////////////////////////////////////////////
class ENCoreTexts
{
 public:
   static void Init()
   {
    //Get number of texts from script class
    NumTexts=ENCoreScript::GetNumTexts();
    //Alloc space
    texts=(ENScriptText**)ENAllocMemory(sizeof(ENScriptText*)*NumTexts);
    //Init texts
    for(ENuint a=0;a<NumTexts;a++)
    {
     texts[a]=ENCoreScript::GetText(a);
     texts[a]->Layer=0;
     texts[a]->Text[0]=0;
     texts[a]->Src=NULL;
     texts[a]->Pos=ENVector2(0.0f,0.0f);
     texts[a]->Size=ENVector2(10.0f,10.0f);
     texts[a]->Visible=false;
     texts[a]->Mirror=false;
     texts[a]->Mode=EN_SHOW_NORMAL;
     texts[a]->ColorR=255;
     texts[a]->ColorG=255;
     texts[a]->ColorB=255;
     texts[a]->ColorA=255;
    }
   }
   static void Free()
   {
    ENFreeMemory(texts);
   }
   static void Draw(ENuint layer)
   {
    for(ENuint a=0;a<NumTexts;a++)
      if(((ENuint)texts[a]->Layer)%EN_MAX_LAYER==layer&&texts[a]->Visible)
        DrawText(texts[a]);
   }
 private:
   static ENuint          NumTexts;
   static ENScriptText  **texts;

   static void DrawText(ENScriptText *text)
   {
    //Variables
    ENFontLoaded *font;
    ENVector2 t[4];
    ENVector2 txtpos=text->Pos;
    //TexCoord
    t[0].v[0]=0;
    t[1].v[0]=1;
    t[2].v[0]=1;
    t[3].v[0]=0;
    if(!text->Mirror) t[0].v[1]=0; else t[0].v[1]=1;
    if(!text->Mirror) t[1].v[1]=0; else t[1].v[1]=1;
    if(!text->Mirror) t[2].v[1]=1; else t[2].v[1]=0;
    if(!text->Mirror) t[3].v[1]=1; else t[3].v[1]=0;
    //Get font
    font=(ENFontLoaded*)text->Src;
    if(!font)
      glBindTexture(GL_TEXTURE_2D,0);
    //Draw text
    glColor4ub((ENubyte)text->ColorR,(ENubyte)text->ColorG,
	       (ENubyte)text->ColorB,(ENubyte)text->ColorA);
    ENuint txtlen=strlen(text->Text);
    for(ENuint a=0;a<txtlen;a++)
    {
    //Set letter
    if(font) font->SetLetter(text->Text[a],(ENuint)text->Mode);
    //Draw
    glBegin(GL_QUADS);
    glTexCoord2fv((ENfloat*)&t[0]);
    glVertex2f(txtpos.v[0],txtpos.v[1]);
    glTexCoord2fv((ENfloat*)&t[1]);
    glVertex2f(txtpos.v[0]+text->Size.v[0],txtpos.v[1]);
    glTexCoord2fv((ENfloat*)&t[2]);
    glVertex2f(txtpos.v[0]+text->Size.v[0],txtpos.v[1]+text->Size.v[1]);
    glTexCoord2fv((ENfloat*)&t[3]);
    glVertex2f(txtpos.v[0],txtpos.v[1]+text->Size.v[1]);
    glEnd();
    txtpos.v[0]+=text->Size.v[0];
    }
   }
};

ENuint          ENCoreTexts::NumTexts=0;
ENScriptText  **ENCoreTexts::texts=NULL;
/////////////////////////////////////////////////////////////
///Engine Core 2D class
/////////////////////////////////////////////////////////////
ENMatrixSpec ENCore2D::oldmatrices;

ENbool ENCore2D::Init()
{
 //Loader message
 ENCoreLoader::AddMsg("Initialize engine core 2D...");
 //Init cursor
 if(!ENCoreCursor::Init()) return false;
 //Init panels
 ENCorePanels::Init();
 //Init texts
 ENCoreTexts::Init();
 //Finished
 return true;
}

void ENCore2D::Free()
{
 ENCorePanels::Free();
 ENCoreTexts::Free();
}

void ENCore2D::Draw()
{
 //Init matrices
 InitMatrices();
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_CULL_FACE);
 glEnable(GL_BLEND);
 glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
 //Draw lens flare
 ENLensFlare::Draw();
 //Draw 2D
 for(ENuint l=0;l<EN_MAX_LAYER;l++)
 {
  ENCorePanels::Draw(l);
  ENCoreTexts::Draw(l);
 }   
 ENCoreCursor::Draw();
 //Reset matrices
 glDisable(GL_BLEND);
 glEnable(GL_CULL_FACE);
 glEnable(GL_DEPTH_TEST);
 ResetMatrices();
}

void ENCore2D::SetCursorBmp(char *Name)
{
 ENCoreCursor::SetBmp(Name);
}

void ENCore2D::InitMatrices()
{
 //Variables
 ENuint WindowSize[2];
 //Save old matrices
 oldmatrices.ENGetMatrixSpec();
 //Get engine window size
 ENCoreWindow::GetWindowSize(WindowSize[0],WindowSize[1]);
 //Init new matrices
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluOrtho2D(0.0,(ENdouble)WindowSize[0],(ENdouble)WindowSize[1],0.0);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}

void ENCore2D::ResetMatrices()
{
 glMatrixMode(GL_PROJECTION);
 glLoadMatrixd(oldmatrices.projMatrix);
 glMatrixMode(GL_MODELVIEW);
 glLoadMatrixd(oldmatrices.modelMatrix);
}

ENbool ENCore2D::RegisterScript()
{
 //Finished
 return true;
}
