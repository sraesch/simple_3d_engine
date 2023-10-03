//---------------------------------------------------------------------------
#include "ENLensFlare.h"
#include "ENCoreMain.h"
#include "ENCoreView.h"
#include "ENCoreWindow.h"
//---------------------------------------------------------------------------
ENArray *ENLensFlare::LensTypes=new ENArray(sizeof(ENLensFlareType));
ENArray *ENLensFlare::LensFlares=new ENArray(sizeof(ENLensFlareObj));
ENuint   ENLensFlare::LensIDCounter=0;

void ENLensFlare::Free()
{
 Clear();
 delete LensTypes;
 delete LensFlares;
}

void ENLensFlare::Clear()
{
 //Clear list
 LensFlares->Clear();
 //Reset lensflare id counter
 LensIDCounter=0;
}

void ENLensFlare::AddRing(char *LensType,char *picsrc,ENVector2 size)
{
 //Variables
 ENint lensind;
 ENLensFlareType lt;
 ENLensFlareRing  ring;
 //Search picture source
 ring.pic=(ENPictureLoaded*)ENCorePacker::GetSource(picsrc,ENPackerBase::ENPICTURE);
 if(!ring.pic)
   ENCoreMain::ShowError("Cannot find source for lens flare ring!!!");
 else
 {
  //Set size
  ring.size=size;
  //Create or get lensflare type
  lensind=SearchLensFlareType(LensType);
  if(lensind>=0)
    LensTypes->GetElement(lensind,(ENubyte*)&lt);
  else
  {//Init lensflare type
   strcpy(lt.Name,LensType);
   lt.NumRings=0;
   lt.Rings=NULL;
  }
  //Add ring
  lt.Rings=(ENLensFlareRing*)ENAddElement((ENubyte*)lt.Rings,sizeof(ENLensFlareRing),
                                           lt.NumRings,(ENubyte*)&ring);
  //Set or add lensflare type
  if(lensind>=0)
    LensTypes->SetElement(lensind,(ENubyte*)&lt);
  else
    LensTypes->AddElement((ENubyte*)&lt);
 }
}

ENint ENLensFlare::CreateLens(char *LensType)
{
 //Variables
 ENLensFlareObj lobj;
 //Check
 ENint ind=SearchLensFlareType(LensType);
 if(ind<0)
 {
  ENCoreMain::ShowError("Flare type not found!!!");
  return -1;
 }
 //Init lens flare object
 strcpy(lobj.Name,LensType);
 lobj.pos=ENVector(0,0,0);
 lobj.r=255;
 lobj.g=255;
 lobj.b=255;
 lobj.id=LensIDCounter;
 LensIDCounter++;
 //Add lens flare object
 LensFlares->AddElement((ENubyte*)&lobj);
 //Finished
 return (ENint)lobj.id;
}

void ENLensFlare::SetLensPos(ENuint id,ENVector pos)
{
 //Variables
 ENuint numflares=LensFlares->GetNum();
 ENLensFlareObj  lfo;
 //Search flare effect
 for(ENuint a=0;a<numflares;a++)
 {
  //Get lens flare object
  LensFlares->GetElement(a,(ENubyte*)&lfo);
  //Update position
  if(lfo.id==id)
  {
   lfo.pos=pos;
   //Set lens flare object
   LensFlares->SetElement(a,(ENubyte*)&lfo);
   return;
  }
 }
}

void ENLensFlare::SetLensColor(ENuint id,ENint r,ENint g,ENint b)
{
 //Variables
 ENuint numflares=LensFlares->GetNum();
 ENLensFlareObj  lfo;
 //Search flare effect
 for(ENuint a=0;a<numflares;a++)
 {
  //Get lens flare object
  LensFlares->GetElement(a,(ENubyte*)&lfo);
  //Update position
  if(lfo.id==id)
  {
   lfo.r=r;
   lfo.g=g;
   lfo.b=b;
   //Set lens flare object
   LensFlares->SetElement(a,(ENubyte*)&lfo);
   return;
  }
 }
}

ENint ENLensFlare::SearchLensFlareType(char *Name)
{
 //Variables
 ENuint numLens;
 //Init
 numLens=LensTypes->GetNum();
 //Search lens flare type
 for(ENuint a=0;a<numLens;a++)
 {
  ENLensFlareType ft;
  LensTypes->GetElement(a,(ENubyte*)&ft);
  if(strcmpi(ft.Name,Name)==0) return a;
 }
 //Nothing found
 return -1;
}

void ENLensFlare::DeleteLens(ENuint id)
{
 //Variables
 ENuint numflares=LensFlares->GetNum();
 ENLensFlareObj  lfo;
 //Search flare effect
 for(ENuint a=0;a<numflares;a++)
 {
  //Get lens flare object
  LensFlares->GetElement(a,(ENubyte*)&lfo);
  //Update position
  if(lfo.id==id)
  {
   LensFlares->DelElement(a);
   return;
  }
 }
}

void ENLensFlare::Draw()
{
 //Variables
 ENuint          numflares;
 ENint           ind;
 ENLensFlareObj  lfo;
 ENLensFlareType lft;
 ENVector        pos;
 ENuint          WindowSize[2];
 ENVector2       wsize;
 ENfloat         bufferZ;
 //Get flares
 numflares=LensFlares->GetNum();
 //Get engine window size
 ENCoreWindow::GetWindowSize(WindowSize[0],WindowSize[1]);
 wsize=ENVector2((ENfloat)WindowSize[0],(ENfloat)WindowSize[1]);
 //Draw all flares
 for(ENuint a=0;a<numflares;a++)
 {
  //Get lens flare object
  LensFlares->GetElement(a,(ENubyte*)&lfo);
  //Get lens flare type
  ind=SearchLensFlareType(lfo.Name);
  LensTypes->GetElement(ind,(ENubyte*)&lft);
  //Convert
  pos=ENCoreView::From3DTo2D2(lfo.pos);
  //Draw lens flare
  if(pos.v[0]>=0.0f&&pos.v[0]<wsize.v[0]&&pos.v[1]>=0&&pos.v[1]<wsize.v[1]&&
     pos.v[2]<1.0f)
  {
   //Read depth bit
   glReadPixels(pos.v[0],wsize.v[1]-pos.v[1],1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&bufferZ);
   if(bufferZ>pos.v[2])
   {
    //lightcolor
    glColor3ub(lfo.r,lfo.g,lfo.b);
    //Draw lens flare
    DrawLensFlare(&lft,ENVector2(pos.v[0],pos.v[1]),wsize);
   }
  }
 }
}

void ENLensFlare::DrawLensFlare(ENLensFlareType *lt,ENVector2 pos,ENVector2 wsize)
{
 //Variables
 ENLensFlareRing lr;
 ENVector2       lensdir;
 ENVector2       lp=pos;
 ENVector2       ringsize;
 //Calculate lens direction
 lensdir=(wsize/2.0f-pos)/lt->NumRings*2.0f;
 //Draw all flares
 for(ENuint a=0;a<lt->NumRings;a++)
 {
  //Set lens flare
  lt->Rings[a].pic->Set(EN_SHOW_ALPHA);
  //Draw ring
  ringsize=lt->Rings[a].size/100.0f*wsize;
  glBegin(GL_QUADS);
  glTexCoord2f(0,0);
  glVertex2f(lp.v[0]-ringsize.v[0],lp.v[1]-ringsize.v[1]);
  glTexCoord2f(1,0);
  glVertex2f(lp.v[0]+ringsize.v[0],lp.v[1]-ringsize.v[1]);
  glTexCoord2f(1,1);
  glVertex2f(lp.v[0]+ringsize.v[0],lp.v[1]+ringsize.v[1]);
  glTexCoord2f(0,1);
  glVertex2f(lp.v[0]-ringsize.v[0],lp.v[1]+ringsize.v[1]);
  glEnd();
  //Increase vector
  lp=lp+lensdir;
 }
}

