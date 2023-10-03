//---------------------------------------------------------------------------
#include <windows.h>
#include <GL/gl.h>
#include <stdio.h>
#include <string.h>
#include "math.h"
#include "EngineSystem.h"

#pragma warning(disable:4244)
//--------------------------------ENVector2----------------------------//
ENVector2::ENVector2()
{}

ENVector2::ENVector2(const ENfloat _x,const ENfloat _y)
{
 v[0]=_x;
 v[1]=_y;
}

ENVector2 ENVector2::operator+(const ENVector2& vVector) const
{
 return ENVector2(vVector.v[0]+v[0],vVector.v[1]+v[1]);
}

ENVector2 ENVector2::operator+(const ENfloat val) const
{
 return ENVector2(val+v[0],val+v[1]);
}

ENVector2 ENVector2::operator-(const ENVector2& vVector) const
{
 return ENVector2(v[0]-vVector.v[0],v[1]-vVector.v[1]);
}

ENVector2 ENVector2::operator-(const ENfloat val) const
{
 return ENVector2(v[0]-val,v[1]-val);
}

ENVector2 ENVector2::operator*(const ENVector2& vVector) const
{
 return ENVector2(vVector.v[0]*v[0],vVector.v[1]*v[1]);
}

ENVector2 ENVector2::operator*(const ENfloat num) const
{
 return ENVector2(v[0]*num,v[1]*num);
}

ENVector2 ENVector2::operator/(const ENfloat num) const
{
 return ENVector2(v[0]/num,v[1]/num);
}

ENbool ENVector2::operator==(const ENVector2& vVector) const
{
 return (vVector.v[0]==v[0]&&vVector.v[1]==v[1]);
}

ENbool ENVector2::operator!=(const ENVector2& vVector) const
{
 return !(vVector.v[0]==v[0]&&vVector.v[1]==v[1]);
}

ENbool ENVector2::operator<(const ENVector2& vVector) const
{
 return (v[0]<vVector.v[0]&&v[1]<vVector.v[1]);
}

ENbool ENVector2::operator>(const ENVector2& vVector) const
{
 return (v[0]>vVector.v[0]&&v[1]>vVector.v[1]);
}

ENbool ENVector2::operator<=(const ENVector2& vVector) const
{
 return (v[0]<=vVector.v[0]&&v[1]<=vVector.v[1]);
}

ENbool ENVector2::operator>=(const ENVector2& vVector) const
{
 return (v[0]>=vVector.v[0]&&v[1]>=vVector.v[1]);
}
//--------------------------------ENVector----------------------------//
ENVector::ENVector()
{}

ENVector::ENVector(const ENfloat _x,const ENfloat _y,const ENfloat _z)
{
 v[0]=_x;
 v[1]=_y;
 v[2]=_z;
}

ENVector ENVector::operator+(const ENVector& vVector) const
{
 return ENVector(vVector.v[0]+v[0],vVector.v[1]+v[1],vVector.v[2]+v[2]);
}

ENVector ENVector::operator+(const ENfloat val) const
{
 return ENVector(val+v[0],val+v[1],val+v[2]);
}

ENVector ENVector::operator-(const ENVector& vVector) const
{
 return ENVector(v[0]-vVector.v[0],v[1]-vVector.v[1],v[2]-vVector.v[2]);
}

ENVector ENVector::operator-(const ENfloat val) const
{
 return ENVector(v[0]-val,v[1]-val,v[2]-val);
}

ENVector ENVector::operator*(const ENVector& vVector) const
{
 return ENVector(vVector.v[0]*v[0],vVector.v[1]*v[1],vVector.v[2]*v[2]);
}

ENVector ENVector::operator*(const ENfloat num) const
{
 return ENVector(v[0]*num,v[1]*num,v[2]*num);
}

ENVector ENVector::operator/(const ENfloat num) const
{
 if(num!=0.0f)
   return ENVector(v[0]/num,v[1]/num,v[2]/num);
 else
   return ENVector(v[0],v[1],v[2]);   
}

ENbool ENVector::operator==(const ENVector& vVector) const
{
 return (vVector.v[0]==v[0]&&vVector.v[1]==v[1]&&vVector.v[2]==v[2]);
}

ENbool ENVector::operator!=(const ENVector& vVector) const
{
 return !(vVector.v[0]==v[0]&&vVector.v[1]==v[1]&&vVector.v[2]==v[2]);
}

ENbool ENVector::operator<(const ENVector& vVector) const
{
 return (v[0]<vVector.v[0]&&v[1]<vVector.v[1]&&v[2]<vVector.v[2]);
}

ENbool ENVector::operator>(const ENVector& vVector) const
{
 return (v[0]>vVector.v[0]&&v[1]>vVector.v[1]&&v[2]>vVector.v[2]);
}

ENbool ENVector::operator<=(const ENVector& vVector) const
{
 return (v[0]<=vVector.v[0]&&v[1]<=vVector.v[1]&&v[2]<=vVector.v[2]);
}

ENbool ENVector::operator>=(const ENVector& vVector) const
{
 return (v[0]>=vVector.v[0]&&v[1]>=vVector.v[1]&&v[2]>=vVector.v[2]);
}
//--------------------------------ENVector4----------------------------//
ENVector4::ENVector4()
{}

ENVector4::ENVector4(const ENfloat _x,const ENfloat _y,const ENfloat _z,const ENfloat _w)
{
 v[0]=_x;
 v[1]=_y;
 v[2]=_z;
 v[3]=_w;
}

ENVector4 ENVector4::operator+(const ENVector4& vVector) const
{
 return ENVector4(vVector.v[0]+v[0],vVector.v[1]+v[1],vVector.v[2]+v[2],vVector.v[3]+v[3]);
}

ENVector4 ENVector4::operator+(const ENfloat val) const
{
 return ENVector4(val+v[0],val+v[1],val+v[2],val+v[3]);
}

ENVector4 ENVector4::operator-(const ENVector4& vVector) const
{
 return ENVector4(v[0]-vVector.v[0],v[1]-vVector.v[1],v[2]-vVector.v[2],v[3]-vVector.v[3]);
}

ENVector4 ENVector4::operator-(const ENfloat val) const
{
 return ENVector4(v[0]-val,v[1]-val,v[2]-val,v[3]-val);
}

ENVector4 ENVector4::operator*(const ENVector4& vVector) const
{
 return ENVector4(vVector.v[0]*v[0],vVector.v[1]*v[1],vVector.v[2]*v[2],vVector.v[3]*v[3]);
}

ENVector4 ENVector4::operator*(const ENfloat num) const
{
 return ENVector4(v[0]*num,v[1]*num,v[2]*num,v[3]*num);
}

ENVector4 ENVector4::operator/(const ENfloat num) const
{
 return ENVector4(v[0]/num,v[1]/num,v[2]/num,v[3]/num);
}

ENbool ENVector4::operator==(const ENVector4& vVector) const
{
 return (vVector.v[0]==v[0]&&vVector.v[1]==v[1]&&vVector.v[2]==v[2]&&vVector.v[3]==v[3]);
}

ENbool ENVector4::operator!=(const ENVector4& vVector) const
{
 return !(vVector.v[0]==v[0]&&vVector.v[1]==v[1]&&vVector.v[2]==v[2]&&vVector.v[3]==v[3]);
}

ENbool ENVector4::operator<(const ENVector4& vVector) const
{
 return (v[0]<vVector.v[0]&&v[1]<vVector.v[1]&&v[2]<vVector.v[2]&&v[3]<vVector.v[3]);
}

ENbool ENVector4::operator>(const ENVector4& vVector) const
{
 return (v[0]>vVector.v[0]&&v[1]>vVector.v[1]&&v[2]>vVector.v[2]&&v[3]>vVector.v[3]);
}

ENbool ENVector4::operator<=(const ENVector4& vVector) const
{
 return (v[0]<=vVector.v[0]&&v[1]<=vVector.v[1]&&v[2]<=vVector.v[2]&&v[3]<=vVector.v[3]);
}

ENbool ENVector4::operator>=(const ENVector4& vVector) const
{
 return (v[0]>=vVector.v[0]&&v[1]>=vVector.v[1]&&v[2]>=vVector.v[2]&&v[3]>=vVector.v[3]);
}
//--------------------------------ENColor----------------------------//

ENColor::ENColor()

{}


ENColor::ENColor(ENubyte red,ENubyte green,ENubyte blue,ENubyte alpha)

{
 r=red;
 g=green;
 b=blue;
 a=alpha;
}


ENColor ENColor::operator+ (ENColor vColor)
{
 return ENColor(r+vColor.r,g+vColor.g,b+vColor.b,a+vColor.a);
}


ENColor ENColor::operator- (ENColor vColor)
{
 return ENColor(r-vColor.r,g-vColor.g,b-vColor.b,a-vColor.a);
}


ENColor ENColor::operator* (ENfloat num)
{
 return ENColor(r*num,g*num,b*num,a*num);
}


ENColor ENColor::operator/ (ENfloat num)
{
 return ENColor((ENubyte)r/num,(ENubyte)g/num,(ENubyte)b/num,(ENubyte)a/num);
}


ENbool  ENColor::operator==(ENColor vColor)
{
 return (vColor.r==r&&vColor.g==g&&vColor.b==b&&vColor.a==a);
}

ENbool  ENColor::operator!=(ENColor vColor)
{
 return !(vColor.r==r&&vColor.g==g&&vColor.b==b&&vColor.a==a);
}

//--------------------------------ENLight----------------------------//

ENLight::ENLight()

{}


ENLight::ENLight(ENColor _color,ENVector _pos,ENfloat _range,ENVector2 _dir,ENfloat _spotcut)

{
 color=_color;
 pos=_pos;
 range=_range;
 dir=_dir;
 spotcut=_spotcut;
}

void ENLight::SetGLLight(GLenum index)
{
 glPushMatrix();
 glLoadIdentity();
 SetGLLightTrans(index);
 glPopMatrix();
}

void ENLight::SetGLLightTrans(GLenum index)
{
 ENVector ldir=ENVecDir(1.0f,dir);
 ENfloat vp[4]={pos.v[0],pos.v[1],pos.v[2],1.0f};
 glLightfv(index,GL_POSITION,vp);
 glLightfv(index,GL_SPOT_DIRECTION,(ENfloat*)&ldir);
 if(!isSpotLight())
   glLightf(index,GL_SPOT_CUTOFF,1.0f);
 else
   glLightf(index,GL_SPOT_CUTOFF,cos(ENRadians(spotcut)));
 glLightf(index,GL_LINEAR_ATTENUATION,range*range);
 ENfloat tmpvec[4]={color.r/255.0f,color.g/255.0f,color.b/255.0f,1};
 glLightfv(index,GL_AMBIENT,tmpvec);
}

void ENLight::BuildLightMatrix(ENMatrix4X4 *lmat)
{
 glLoadIdentity();
 //Projection matrix
 gluPerspective(spotcut*2,1.0f,0.1f,range*2.0f);
 //Modelview matrix
 ENVector ldir=ENVecDir(1.0f,dir)+pos;
 gluLookAt(pos.v[0],pos.v[1],pos.v[2],
           ldir.v[0],ldir.v[1],ldir.v[2],
           0.0f,1.0f,0.0f);
 //Get matrix data
 if(lmat) glGetFloatv(GL_MODELVIEW_MATRIX,&lmat->val[0][0]);
}

void ENLight::BuildOmniLightMatrix(ENMatrix4X4 *lmat,ENuint ind)
{
 glLoadIdentity();
 //Projection matrix
 gluPerspective(90.0f,1.0f,0.1f,range*2.0f);
 //Modelview matrix
 switch(ind)
 {
  case 1:
    glRotatef(90,0,1,0);
  break;
  case 2:
    glRotatef(180,0,1,0);
  break;
  case 3:
    glRotatef(270,0,1,0);
  break;
  case 4:
    glRotatef(90,1,0,0);
  break;
  case 5:
    glRotatef(-90,1,0,0);
  break;
 };
 glTranslatef(-pos.v[0],-pos.v[1],-pos.v[2]);
 //Get matrix data
 if(lmat) glGetFloatv(GL_MODELVIEW_MATRIX,&lmat->val[0][0]);
}

ENbool ENLight::isSpotLight()
{
 return (spotcut>0.0f&&spotcut<=60.0f);
}

ENVector ENLight::CalculateLightColor(ENVector vec)
{
 //Variables
 ENVector lightdir;
 ENfloat  distfac,spotfac;
 //Calculate light direction vector
 lightdir=pos-vec;
 //Calculate light distance factor
 distfac=1.0f-ENDot(lightdir,lightdir)/(range*range);
 if(distfac<0.0f) distfac=0.0f;
 //Calculate spot light factor
 if(isSpotLight())
 {
  //Calculate 3D direction vector
  ENVector ldir=ENVecDir(1.0f,dir);
  //Normalize vector from light to vector
  lightdir=ENNormalize(lightdir);
  //Calculate spot light factor
  spotfac=ENDot(lightdir,ldir*(-1));
  spotfac=(cos(ENRadians(spotcut))-spotfac)/(spotfac-1.0f);
  //Check spotfactor
  if(spotfac<0.0f)
    spotfac=0.0f;
  else
    if(spotfac>1.0f)
      spotfac=1.0f;
 }
 else
   spotfac=1.0f;
 //Return result
 return ENVector((ENfloat)color.r/255.0f,(ENfloat)color.g/255.0f,
                 (ENfloat)color.b/255.0f)*distfac*spotfac;
}
//--------------------------------ENSoundSrc----------------------------//

ENSoundSrc::ENSoundSrc()

{}


ENSoundSrc::ENSoundSrc(ENVector _pos,ENVector2 _dir,ENfloat _range,ENfloat _speed,
                       ENfloat _volume,ENfloat _angle,ENbool _loop)

{

 pos=_pos;

 dir=_dir;

 range=_range;

 speed=_speed;

 volume=_volume;

 angle=_angle;

 loop=_loop;
}
//---------------------------------ENMatrixSpec--------------------------//
void ENMatrixSpec::ENGetMatrixSpec()
{
 glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
 glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
 glGetIntegerv(GL_VIEWPORT,viewport);
}

//---------------------------------ENPlane--------------------------//
void ENPlane::BuildPlane(const ENVector &vNormal,const ENVector &point)
{
 normal=vNormal;
 dist=-ENDot(vNormal,point);
}

ENfloat ENPlane::PlaneDistance(const ENVector &Point)
{
 return ENDot(Point,normal)+dist;
}


ENPlane::Classify ENPlane::ClassifyPoint(const ENVector &Point)
{
 //Variables
 ENfloat dist;
 //Calculate plane distance

 dist=PlaneDistance(Point);
 //Return result
 if(dist<-0.01f)
   return ENIN_BACK_OF;
 else
   if(dist>0.01f)
     return ENIN_FRONT_OF;
   else
     return ENCOINCIDENT;
}

ENPlane::Classify ENPlane::ClassifyTriangle(const ENVector tridata[])
{
 //Variables
 ENint count[2]={0,0};
 //Classify all points
 for(ENuint a=0;a<3;a++)
   switch(ClassifyPoint(tridata[a]))
   {
    case ENIN_BACK_OF:
      count[0]++;
    break;
    case ENIN_FRONT_OF:
      count[1]++;
    break;
   }
 //Finished
 if(count[0]&&count[1])
   return ENDOINTERSECT;
 else
 {
  if(count[0]) return ENIN_BACK_OF;
  if(count[1]) return ENIN_FRONT_OF;
  return ENCOINCIDENT;
 }
}

ENPlane::Classify ENPlane::ClassifyPoly(const ENVector polydata[],const ENuint num)
{
 //Variables
 ENint count[2]={0,0};
 //Classify all points
 for(ENuint a=0;a<num;a++)
   switch(ClassifyPoint(polydata[a]))
   {
    case ENIN_BACK_OF:
      count[0]++;
    break;
    case ENIN_FRONT_OF:
      count[1]++;
    break;
   }
 //Finished
 if(count[0]&&count[1])
   return ENDOINTERSECT;
 else
 {
  if(count[0]) return ENIN_BACK_OF;
  if(count[1]) return ENIN_FRONT_OF;
  return ENCOINCIDENT;
 }
}

ENPlane::Classify ENPlane::ClassifySphere(const ENVector &pos,const ENfloat range,ENfloat &distance)
{
 distance=PlaneDistance(pos);
 if(ENAbsolute(distance)<range)
   return ENDOINTERSECT;
 else
   if(distance>=range)
     return ENIN_FRONT_OF;
   else
     return ENIN_BACK_OF;
}

ENPlane::Classify ENPlane::ClassifySphere(const ENVector &pos,const ENfloat range)
{
 ENfloat distance;
 return ClassifySphere(pos,range,distance);
}



