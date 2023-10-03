//---------------------------------------------------------------------------
#include <vcl.h>
#include <iterator.h>
#include <list.h>
#include <stack.h>
#pragma hdrstop

#include "MainWndUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainWnd *MainWnd;
ENuint numCHTris,numCHPlanes;
ENVector CamPos=ENVector(0.0f,0.0f,50.0f);
ENVector CamAngle=ENVector(0.0f,0.0f,0.0f);

ENVector vtri[3]={ENVector(-2.0f,0.0f,-2.0f),
                  ENVector(0.0f,7.0f,-2.0f),
                  ENVector(4.0f,2.0f,-2.0f)};

ENVector vplane[3]={ENVector(-4.0f,0.0f,0.0f),
                    ENVector(-4.0f,10.0f,-5.0f),
                    ENVector(-4.0f,0.0f,-10.0f)};

ENVector spherepos=ENVector(0.0f,20.0f,-20.0f);
ENfloat  sphererange=10.0f;
ENuint  numc=0;
//---------------------------------------------------------------------------
__fastcall TMainWnd::TMainWnd(TComponent* Owner)
        : TForm(Owner)
{
 bsp=NULL;
 bsptree=NULL;
 colors=NULL;
 colors=new ENubyte[120000];
 randomize();
 for(ENuint a=0;a<40000;a++)
 {
  colors[a*3]=random(256);
  colors[a*3+1]=random(256);
  colors[a*3+2]=255;
 }
}
//---------------------------------------------------------------------------
void __fastcall TMainWnd::Button1Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
 {
  AnsiString FName=OpenDialog1->FileName;
  eam->Load(FName.c_str());
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormCreate(TObject *Sender)
{
 eam=new ENASCIIMesh();
 globj=new ENGLClass(Handle,true,false);
 if(!globj->Init())
   ShowMessage("Cannot init OpenGL!!!");

 glEnable(GL_DEPTH_TEST);
 glPolygonMode(GL_FRONT,GL_FILL);
 glPolygonMode(GL_BACK,GL_LINE);
 //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormDestroy(TObject *Sender)
{
 delete eam;
 delete globj;
 if(bsp) delete bsp;
 if(bsptree) delete bsptree;
}
//---------------------------------------------------------------------------
void MsgFunc(char *msg,ENuint pos,ENuint max)
{
 MainWnd->ProgressBar1->Max=max;
 MainWnd->ProgressBar1->Position=max-pos;
 if(msg) MainWnd->Memo1->Lines->Add(msg);
 Application->ProcessMessages();
}

void __fastcall TMainWnd::Button2Click(TObject *Sender)
{
 if(SaveDialog1->Execute())
 {
  if(bsp) delete bsp;
  bsp=new ENBSPBuilder(MsgFunc);
  //Get eam data
  ENuint numfaces=eam->GetNumFaces();
  ENASCIIMesh::ENMeshFace *faces=new ENASCIIMesh::ENMeshFace[numfaces];
  ENuint numverts=eam->GetNumVertexes();
  ENVector *verts=new ENVector[numverts];

  eam->GetFaces(faces);
  eam->GetVertexes(verts);
  //Copy data
  for(ENuint a=0;a<numfaces;a++)
  {
   ENBSPPolygon poly;
   poly.verts=(ENVector*)ENAllocMemory(sizeof(ENVector)*3);
   poly.num=3;
   poly.verts[0]=verts[faces[a].Indices[0]];
   poly.verts[1]=verts[faces[a].Indices[1]];
   poly.verts[2]=verts[faces[a].Indices[2]];
   bsp->AddPolygon(poly);
  }
  //Release
  delete[] faces;
  delete[] verts;
  //Init memo
  Memo1->Clear();
  Memo1->Visible=true;
  //Compute
  ENBSPTree *tempbsp=bsp->BuildTree();
  //Save
  AnsiString fn=SaveDialog1->FileName;
  if(!tempbsp->WriteBsp(fn.c_str()))
    ShowMessage("Cannot save bsp!!!");

  delete tempbsp;    
  //Finished
  ShowMessage("Finished");
 }  
}
//---------------------------------------------------------------------------
void TMainWnd::TransformMatrices()
{
 CamPos=spherepos;

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glViewport(0,0,ClientWidth,ClientHeight);
 ENfloat aspect=(ENfloat)ClientWidth/ClientHeight;
 gluPerspective(60, aspect,1.0, 1000.0f);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glRotatef(CamAngle.v[2],0,0,1);
 glRotatef(CamAngle.v[1],0,1,0);
 glRotatef(CamAngle.v[0],1,0,0);
 glTranslatef(-CamPos.v[0],-CamPos.v[1],-CamPos.v[2]);
}

ENbool ENSphereTriangleCollision(ENBSPTree *bt,const ENVector &pos,const ENfloat range)
{
 //Variables
 ENfloat distance;
 ENPlane::Classify sphereclass;
 ENVector vOffset,vPosition;
 ENuint numtris;
 //Check tree
 if(!bt) return false;
 //Classify sphere
 numCHPlanes++;
 sphereclass=bt->partition.ClassifySphere(pos,range,distance);
 //Compute classification
 switch(sphereclass)
 {
  case ENPlane::ENIN_FRONT_OF:
    return ENSphereTriangleCollision(bt->front,pos,range);
  case ENPlane::ENIN_BACK_OF:
    return ENSphereTriangleCollision(bt->back,pos,range);
  case ENPlane::ENDOINTERSECT:
    vOffset   = bt->partition.normal*distance;
    vPosition = pos-vOffset;
    //Check all polygones
    numtris=bt->numpolys;
    for(ENuint a=0;a<numtris;a++)
    {
     numCHTris++;
     if(ENInsidePolygon(vPosition,bt->polylist[a].verts,bt->polylist[a].num)) return true;
     if(ENEdgeSphereCollision(pos,bt->polylist[a].verts,bt->polylist[a].num,range)) return true;
    }
    //No collision, so we have to check both sides
    if(ENSphereTriangleCollision(bt->front,pos,range)) return true;
    return ENSphereTriangleCollision(bt->back,pos,range);
 }
 //Never happend (-:
 return false;
}

void TMainWnd::DrawScene()
{
 globj->Start();
 TransformMatrices();
 //Draw map
 numc=0;
 DrawTree(bsptree);
 //Draw sphere
 glPushMatrix();
 glTranslatef(spherepos.v[0],spherepos.v[1],spherepos.v[2]);
 numCHTris=0;
 numCHPlanes=0;
 if(ENSphereTriangleCollision(bsptree,spherepos,sphererange))
   glColor3f(1,0,0);
 else
   glColor3f(0,1,0);

 Caption="NumTris:("+AnsiString(numCHTris)+") NumPlanes:("+AnsiString(numCHPlanes)+")";
 GLUquadricObj *sph=gluNewQuadric();
 gluSphere(sph,sphererange,10,10);
 gluDeleteQuadric(sph);
 glPopMatrix();


 globj->End();
}

void TMainWnd::DrawTree(ENBSPTree *bt)
{
 if(!bt) return;
 //Draw all triangles
 for(ENuint a=0;a<bt->numpolys;a++)
 {
  glColor3ubv(colors+numc*3);
  numc++;
  glBegin(GL_TRIANGLE_FAN);
    for(ENuint b=0;b<bt->polylist[a].num;b++)
     glVertex3fv((ENfloat*)&bt->polylist[a].verts[b]);
  glEnd();
 }
 //Draw others
 DrawTree(bt->front);
 DrawTree(bt->back);
}

void __fastcall TMainWnd::FormPaint(TObject *Sender)
{
 DrawScene();
}
//---------------------------------------------------------------------------
void TMainWnd::MakeStressTest()
{
 ENuint t=GetTickCount();
 for(ENuint a=0;a<10000;a++)
 ENSphereTriangleCollision(bsptree,spherepos,sphererange);

 t=GetTickCount()-t;
 ShowMessage(t);
}

void __fastcall TMainWnd::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 ENVector transvec=ENVector(0,0,0);
 switch(Key)
 {
  case VK_DOWN:
    transvec.v[2]--;
  break;
  case VK_UP:
    transvec.v[2]++;
  break;
  case VK_LEFT:
    transvec.v[0]--;
  break;
  case VK_RIGHT:
    transvec.v[0]++;
  break;
  case 'W':
  case 'w':
    transvec.v[1]--;
  break;
  case 'S':
  case 's':
    transvec.v[1]++;
  break;
 }
 if(Key==VK_SPACE)
   CamAngle.v[1]++;

 if(Key==VK_CONTROL)
   CamAngle.v[1]--;

 if(Key==VK_RETURN)
   MakeStressTest();   

 if(!ENSphereTriangleCollision(bsptree,spherepos+transvec,10.0f))
   spherepos=spherepos+transvec;
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Timer1Timer(TObject *Sender)
{
 DrawScene();        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::FormDblClick(TObject *Sender)
{
 Timer1->Enabled=!Timer1->Enabled;
}
//---------------------------------------------------------------------------





void __fastcall TMainWnd::Memo1DblClick(TObject *Sender)
{
 Memo1->Visible=false;        
}
//---------------------------------------------------------------------------

void __fastcall TMainWnd::Loadbsp1Click(TObject *Sender)
{
 if(OpenDialog2->Execute())
 {
  AnsiString fn=OpenDialog2->FileName;

  if(bsptree) delete bsptree;
  bsptree=new ENBSPTree();
  if(!bsptree->LoadBsp(fn.c_str()))
    ShowMessage("No bsp!!!");
 }
}
//---------------------------------------------------------------------------

