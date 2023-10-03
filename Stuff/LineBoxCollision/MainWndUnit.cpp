//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainWndUnit.h"

#pragma comment(lib,"EngineSystem.lib")
#pragma comment(lib,"EngineMedia.lib")
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

ENVector CamPos=ENVector(0.0f,0.0f,60.0f);
ENVector CamAngle=ENVector(0.0f,0.0f,0.0f);
ENVector vBox1[2]={ENVector(-20,-20,-20),ENVector(20,20,20)};
ENVector Line[2]={ENVector(-10,-10,-10),ENVector(10,10,10)};
//ENVector
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
 globj=new ENGLClass((void*)Handle,true,false);
 if(!globj->Init())
   ShowMessage("Cannot init OpenGL object!!!");

 if(!ENGLExtension::Load())
   ShowMessage("Cannot init opengl extensions!!!");   

 glEnable(GL_DEPTH_TEST);
 //Generate query
 ENGLExtension::GenerateQueries(1,&QueryID);
}

void TForm1::DrawScene()
{
 globj->Start();
 //Transform matrices
 TransformMatrices();
 //Draw box
 DrawBox(vBox1);
 //Draw line
 if(ENBoxLineIntersect2(vBox1,Line))
   glColor3f(1,0,0);
 else
   glColor3f(0,1,0);
 //Line occlusion query
 ENGLExtension::BeginQuery(GL_SAMPLES_PASSED_ARB,QueryID);
   glBegin(GL_LINES);
   glVertex3fv((ENfloat*)&Line[0]);
   glVertex3fv((ENfloat*)&Line[1]);
   glEnd();
 ENGLExtension::EndQuery(GL_SAMPLES_PASSED_ARB);
 //Get number of fragments
 ENuint count;
 ENGLExtension::GetQueryObjectuiv(QueryID,GL_QUERY_RESULT_ARB,&count);
 Caption=count;
 globj->End();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
 ENGLExtension::DeleteQueries(1,&QueryID);
 delete globj;
}
//---------------------------------------------------------------------------
void TForm1::TransformMatrices()
{
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glViewport(0,0,ClientWidth,ClientHeight);
 ENfloat aspect=(ENfloat)ClientWidth/ClientHeight;
 gluPerspective(60, aspect,1.0, 1000.0f);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glTranslatef(-CamPos.v[0],-CamPos.v[1],-CamPos.v[2]); 
 glRotatef(CamAngle.v[0],1,0,0);
 glRotatef(CamAngle.v[1],0,1,0);
}
//---------------------------------------------------------------------------
void TForm1::DrawBox(ENVector Box[])
{
 glColor3f(1,1,1);
 //Top
 glColor3f(0,0,1);
 glBegin(GL_QUADS);
 glVertex3f(Box[0].v[0],Box[0].v[1],Box[1].v[2]);
 glVertex3f(Box[1].v[0],Box[0].v[1],Box[1].v[2]);
 glVertex3f(Box[1].v[0],Box[0].v[1],Box[0].v[2]);
 glVertex3f(Box[0].v[0],Box[0].v[1],Box[0].v[2]);
 glEnd();
 //bottom
 glColor3f(1,1,1);
 glBegin(GL_QUADS);
 glVertex3f(Box[0].v[0],Box[1].v[1],Box[1].v[2]);
 glVertex3f(Box[1].v[0],Box[1].v[1],Box[1].v[2]);
 glVertex3f(Box[1].v[0],Box[1].v[1],Box[0].v[2]);
 glVertex3f(Box[0].v[0],Box[1].v[1],Box[0].v[2]);
 glEnd();
 //Left
 glColor3f(0.5,0.5,0.5);
 glBegin(GL_QUADS);
 glVertex3f(Box[0].v[0],Box[0].v[1],Box[0].v[2]);
 glVertex3f(Box[0].v[0],Box[0].v[1],Box[1].v[2]);
 glVertex3f(Box[0].v[0],Box[1].v[1],Box[1].v[2]);
 glVertex3f(Box[0].v[0],Box[1].v[1],Box[0].v[2]);
 glEnd();
 //Right
 glColor3f(1,1,0);
 glBegin(GL_QUADS);
 glVertex3f(Box[1].v[0],Box[0].v[1],Box[0].v[2]);
 glVertex3f(Box[1].v[0],Box[0].v[1],Box[1].v[2]);
 glVertex3f(Box[1].v[0],Box[1].v[1],Box[1].v[2]);
 glVertex3f(Box[1].v[0],Box[1].v[1],Box[0].v[2]);
 glEnd();
 //Front
 glColor3f(0,1,1);
 glBegin(GL_QUADS);
 glVertex3f(Box[0].v[0],Box[0].v[1],Box[0].v[2]);
 glVertex3f(Box[1].v[0],Box[0].v[1],Box[0].v[2]);
 glVertex3f(Box[1].v[0],Box[1].v[1],Box[0].v[2]);
 glVertex3f(Box[0].v[0],Box[1].v[1],Box[0].v[2]);
 glEnd();
 //Back
 glColor3f(1,0,1);
 glBegin(GL_QUADS);
 glVertex3f(Box[0].v[0],Box[0].v[1],Box[1].v[2]);
 glVertex3f(Box[1].v[0],Box[0].v[1],Box[1].v[2]);
 glVertex3f(Box[1].v[0],Box[1].v[1],Box[1].v[2]);
 glVertex3f(Box[0].v[0],Box[1].v[1],Box[1].v[2]);
 glEnd();
}

void __fastcall TForm1::FormPaint(TObject *Sender)
{
 DrawScene();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
 CamAngle.v[1]+=0.1f;
 DrawScene();
}
//---------------------------------------------------------------------------
ENbool TForm1::ENBoxLineIntersect(ENVector vBox[],ENVector vLine[])
{
 //Variables
 ENVector vDir,vec,vec2;
 ENfloat  s;
 //Calculate line direction vector
 vDir=vLine[1]-vLine[0];
 for(ENuint a=0;a<2;a++)
 {
  //XY Plane's
  if(vDir.v[2]!=0.0f)
  {
   s=(vBox[a].v[2]-vLine[0].v[2])/vDir.v[2];
   if(s>=0.0f&&s<=1.0f)
   {
    vec2.v[0]=vLine[0].v[0]+vDir.v[0]*s;
    vec2.v[1]=vLine[0].v[1]+vDir.v[1]*s;
    if(vec2.v[0]>=vBox[0].v[0]&&vec2.v[0]<=vBox[1].v[0]&&
       vec2.v[1]>=vBox[0].v[1]&&vec2.v[1]<=vBox[1].v[1])
       return true;
   }
  }
  //YZ Plane's
  if(vDir.v[0]!=0.0f)
  {
   s=(vBox[a].v[0]-vLine[0].v[0])/vDir.v[0];
   if(s>=0.0f&&s<=1.0f)
   {
    vec2.v[1]=vLine[0].v[1]+vDir.v[1]*s;
    vec2.v[2]=vLine[0].v[2]+vDir.v[2]*s;
    if(vec2.v[1]>=vBox[0].v[1]&&vec2.v[1]<=vBox[1].v[1]&&
       vec2.v[2]>=vBox[0].v[2]&&vec2.v[2]<=vBox[1].v[2])
       return true;
   }
  }
  //XZ Plane's
  if(vDir.v[1]!=0.0f)
  {
   s=(vBox[a].v[1]-vLine[0].v[1])/vDir.v[1];
   if(s>=0.0f&&s<=1.0f)
   {
    vec2.v[0]=vLine[0].v[0]+vDir.v[0]*s;
    vec2.v[2]=vLine[0].v[2]+vDir.v[2]*s;
    if(vec2.v[0]>=vBox[0].v[0]&&vec2.v[0]<=vBox[1].v[0]&&
       vec2.v[2]>=vBox[0].v[2]&&vec2.v[2]<=vBox[1].v[2])
       return true;
   }
  }
 }
 //Check if line is complete in box
 return (vLine[0].v[0]>=vBox[0].v[0]&&vLine[0].v[0]<=vBox[1].v[0]&&
         vLine[0].v[1]>=vBox[0].v[1]&&vLine[0].v[1]<=vBox[1].v[1]&&
         vLine[0].v[2]>=vBox[0].v[2]&&vLine[0].v[2]<=vBox[1].v[2]);
}

ENbool TForm1::ENBoxLineIntersectNew(ENVector vBox[],ENVector vLine[])
{
 //Variables
 ENVector vDir=vLine[1]-vLine[0];
 ENVector vec;
 ENfloat  sfactor;
 //Check if linestart is inside of cube
 if(vLine[0]>=vBox[0]&&vLine[0]<=vBox[1]) return true;
 //Check sides
 for(ENuint ori=0;ori<3;ori++)
   if(vDir.v[ori]!=0.0f)
     for(ENuint side=0;side<2;side++)
     {
      //Calculate sfactor
      sfactor=(vBox[side].v[ori]-vLine[0].v[ori])/vDir.v[ori];
      //If intersection point is in range
      if(sfactor>=0.0f&&sfactor<=1.0f)
      {
       //Calculate 2D intersection point
       vec=vLine[0]+vDir*sfactor;
       if(vec>=vBox[0]&&vec<=vBox[1]) return true;
      }
     }
 //Finished
 return false;
}

ENbool TForm1::ENBoxLineIntersect2(ENVector vBox[],ENVector vLine[])
{
 //Raw check
 if(vLine[0].v[0]>vBox[1].v[0]&&vLine[1].v[0]>vBox[1].v[0]) return false;
 if(vLine[0].v[0]<vBox[0].v[0]&&vLine[1].v[0]<vBox[0].v[0]) return false;

 if(vLine[0].v[1]>vBox[1].v[1]&&vLine[1].v[1]>vBox[1].v[1]) return false;
 if(vLine[0].v[1]<vBox[0].v[1]&&vLine[1].v[1]<vBox[0].v[1]) return false;

 if(vLine[0].v[2]>vBox[1].v[2]&&vLine[1].v[2]>vBox[1].v[2]) return false;
 if(vLine[0].v[2]<vBox[0].v[2]&&vLine[1].v[2]<vBox[0].v[2]) return false;
 //Check if intersect
 return ENBoxLineIntersectNew(vBox,vLine);
}

ENbool TForm1::ENTriangleLineIntersect(ENVector vTri[],ENVector vLine[])
{
 //Variables
 ENVector    x;
 ENMatrix3X3 M;
 //Set matrix
 M=ENMatrix3X3(vTri[1].v[0]-vTri[0].v[0],vTri[2].v[0]-vTri[0].v[0],vLine[1].v[0]-vLine[0].v[0],
               vTri[1].v[1]-vTri[0].v[1],vTri[2].v[1]-vTri[0].v[1],vLine[1].v[1]-vLine[0].v[1],
               vTri[1].v[2]-vTri[0].v[2],vTri[2].v[2]-vTri[0].v[2],vLine[1].v[2]-vLine[0].v[2]);
 //Invert matrix
 if(!ENInv2(M)) return false;
 //Calculate factors
 x=M*(vLine[0]-vTri[0]);
 //Return result
 return (x.v[0]>=0.0f&&x.v[0]<=1.0f&&
         x.v[1]>=0.0f&&x.v[1]<=1.0f&&
         -x.v[2]>=0.0f&&-x.v[2]<=1.0f&&
         x.v[0]+x.v[1]<=1.0f);
}

void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 switch(Key)
 {
  case VK_DOWN:
    vBox1[0].v[2]--;
    vBox1[1].v[2]--;
  break;
  case VK_UP:
    vBox1[0].v[2]++;
    vBox1[1].v[2]++;
  break;    
  case VK_LEFT:
    vBox1[0].v[0]--;
    vBox1[1].v[0]--;
  break;
  case VK_RIGHT:
    vBox1[0].v[0]++;
    vBox1[1].v[0]++;
  break;
  case 'W':
  case 'w':
    vBox1[0].v[1]--;
    vBox1[1].v[1]--;
  break;
  case 'S':
  case 's':
    vBox1[0].v[1]++;
    vBox1[1].v[1]++;
  break;
 }
 DrawScene();
}
//---------------------------------------------------------------------------



void __fastcall TForm1::FormDblClick(TObject *Sender)
{
 Timer1->Enabled=!Timer1->Enabled;        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
 ENuint a;
 ENVector TempLines[100*2];
 //Init temp lines
 for(a=0;a<200;a++)
   TempLines[a]=ENVector(rand(),rand(),rand());
 //Check temp lines
 ENuint t=GetTickCount();
 for(a=0;a<1000000;a++)
   ENBoxLineIntersect(vBox1,TempLines+a%100*2);

 t=GetTickCount()-t;
 ShowMessage(t);   
}
//---------------------------------------------------------------------------

