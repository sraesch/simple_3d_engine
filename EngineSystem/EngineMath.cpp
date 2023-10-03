//---------------------------------------------------------------------------
#include <math.h>
#include <float.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "EngineSystem.h"

enum ENPlaneData
{
 A = 0,
 B = 1,
 C = 2,
 D = 3
};

enum ENSpherePos
{
 BEHIND		= 0,
 INTERSECTS	= 1,
 FRONT		= 2
};
//-----------------------Matrices-----------------------------//
//Init functions
ENMatrix2X2::ENMatrix2X2()
{
 val[0][0]=1.0f;
 val[0][1]=0.0f;

 val[1][0]=0.0f;
 val[1][1]=1.0f;
}

ENMatrix2X2::ENMatrix2X2(const ENfloat x1,const ENfloat y1,
                         const ENfloat x2,const ENfloat y2)
{
 val[0][0]=x1;
 val[0][1]=y1;

 val[1][0]=x2;
 val[1][1]=y2;
}

ENMatrix3X3::ENMatrix3X3()
{
 val[0][0]=1.0f;
 val[0][1]=0.0f;
 val[0][2]=0.0f;

 val[1][0]=0.0f;
 val[1][1]=1.0f;
 val[1][2]=0.0f;

 val[2][0]=0.0f;
 val[2][1]=0.0f;
 val[2][2]=1.0f;
}

ENMatrix3X3::ENMatrix3X3(const ENfloat x1,const ENfloat y1,const ENfloat z1,
                         const ENfloat x2,const ENfloat y2,const ENfloat z2,
                         const ENfloat x3,const ENfloat y3,const ENfloat z3)
{
 //Row 1
 val[0][0]=x1;
 val[0][1]=y1;
 val[0][2]=z1;
 //Row 2
 val[1][0]=x2;
 val[1][1]=y2;
 val[1][2]=z2;
 //Row 3
 val[2][0]=x3;
 val[2][1]=y3;
 val[2][2]=z3;
}

ENMatrix4X4::ENMatrix4X4()
{
 val[0][0]=1.0f;
 val[0][1]=0.0f;
 val[0][2]=0.0f;
 val[0][3]=0.0f;

 val[1][0]=0.0f;
 val[1][1]=1.0f;
 val[1][2]=0.0f;
 val[1][3]=0.0f;

 val[2][0]=0.0f;
 val[2][1]=0.0f;
 val[2][2]=1.0f;
 val[2][3]=0.0f;

 val[3][0]=0.0f;
 val[3][1]=0.0f;
 val[3][2]=0.0f;
 val[3][3]=1.0f;
}

ENMatrix4X4::ENMatrix4X4(const ENfloat x1,const ENfloat y1,const ENfloat z1,const ENfloat w1,
                         const ENfloat x2,const ENfloat y2,const ENfloat z2,const ENfloat w2,
                         const ENfloat x3,const ENfloat y3,const ENfloat z3,const ENfloat w3,
                         const ENfloat x4,const ENfloat y4,const ENfloat z4,const ENfloat w4)
{
 //Row 1
 val[0][0]=x1;
 val[0][1]=y1;
 val[0][2]=z1;
 val[0][3]=w1;
 //Row 2
 val[1][0]=x2;
 val[1][1]=y2;
 val[1][2]=z2;
 val[1][3]=w2;
 //Row 3
 val[2][0]=x3;
 val[2][1]=y3;
 val[2][2]=z3;
 val[2][3]=w3;
 //Row 4
 val[3][0]=x4;
 val[3][1]=y4;
 val[3][2]=z4;
 val[3][3]=w4;
}

//Matrix2X Operators
ENMatrix2X2 ENMatrix2X2::operator+(const ENfloat num) const
{
 ENMatrix2X2 res;
 //////
 res.val[0][0]=val[0][0]+num;
 res.val[0][1]=val[0][1];

 res.val[1][0]=val[1][0];
 res.val[1][1]=val[1][1]+num;

 return res;
}

ENMatrix2X2 ENMatrix2X2::operator-(const ENfloat num) const
{
 ENMatrix2X2 res;
 //////
 res.val[0][0]=val[0][0]-num;
 res.val[0][1]=val[0][1];

 res.val[1][0]=val[1][0];
 res.val[1][1]=val[1][1]-num;

 return res;
}

ENMatrix2X2 ENMatrix2X2::operator*(const ENfloat num) const
{
 ENMatrix2X2 res;
 //////
 res.val[0][0]=val[0][0]*num;
 res.val[0][1]=val[0][1]*num;

 res.val[1][0]=val[1][0]*num;
 res.val[1][1]=val[1][1]*num;

 return res;
}

ENMatrix2X2 ENMatrix2X2::operator/(const ENfloat num) const
{
 ENMatrix2X2 res;
 //////
 res.val[0][0]=val[0][0]/num;
 res.val[0][1]=val[0][1]/num;

 res.val[1][0]=val[1][0]/num;
 res.val[1][1]=val[1][1]/num;

 return res;
}

ENMatrix2X2 ENMatrix2X2::operator+(const ENMatrix2X2& mat) const
{
 ENMatrix2X2 res;
 ///////
 res.val[0][0]=val[0][0]+mat.val[0][0];
 res.val[0][1]=val[0][1]+mat.val[0][1];

 res.val[1][0]=val[1][0]+mat.val[1][0];
 res.val[1][1]=val[1][1]+mat.val[1][1];

 return res;
}

ENMatrix2X2 ENMatrix2X2::operator-(const ENMatrix2X2& mat) const
{
 ENMatrix2X2 res;
 ///////
 res.val[0][0]=val[0][0]-mat.val[0][0];
 res.val[0][1]=val[0][1]-mat.val[0][1];

 res.val[1][0]=val[1][0]-mat.val[1][0];
 res.val[1][1]=val[1][1]-mat.val[1][1];

 return res;
}

ENMatrix2X2 ENMatrix2X2::operator*(const ENMatrix2X2& mat) const
{
 ENMatrix2X2 res;
 //mat0|0
 res.val[0][0]=val[0][0]*mat.val[0][0];
 res.val[0][0]+=val[0][1]*mat.val[1][0];
 //mat0|1
 res.val[0][1]=val[0][0]*mat.val[0][1];
 res.val[0][1]+=val[0][1]*mat.val[1][1];
 //mat1|0
 res.val[1][0]=val[1][0]*mat.val[0][0];
 res.val[1][0]+=val[1][1]*mat.val[1][0];
 //mat1|1
 res.val[1][1]=val[1][0]*mat.val[0][1];
 res.val[1][1]+=val[1][1]*mat.val[1][1];

 return res;
}

ENVector2 ENMatrix2X2::operator*(const ENVector2& vec) const
{
 ENVector2 res;
 ////
 res.v[0]=val[0][0]*vec.v[0]+val[0][1]*vec.v[1];
 res.v[1]=val[1][0]*vec.v[0]+val[1][1]*vec.v[1];

 return res;
}

//Matrix3X Operators
ENMatrix3X3 ENMatrix3X3::operator+(const ENfloat num) const
{
 ENMatrix3X3 res;
 //////
 res.val[0][0]=val[0][0]+num;
 res.val[0][1]=val[0][1];
 res.val[0][2]=val[0][2];

 res.val[1][0]=val[1][0];
 res.val[1][1]=val[1][1]+num;
 res.val[1][2]=val[1][2];

 res.val[2][0]=val[2][0];
 res.val[2][1]=val[2][1];
 res.val[2][2]=val[2][2]+num;

 return res;
}

ENMatrix3X3 ENMatrix3X3::operator-(const ENfloat num) const
{
 ENMatrix3X3 res;
 //////
 res.val[0][0]=val[0][0]-num;
 res.val[0][1]=val[0][1];
 res.val[0][2]=val[0][2];

 res.val[1][0]=val[1][0];
 res.val[1][1]=val[1][1]-num;
 res.val[1][2]=val[1][2];

 res.val[2][0]=val[2][0];
 res.val[2][1]=val[2][1];
 res.val[2][2]=val[2][2]-num;

 return res;
}

ENMatrix3X3 ENMatrix3X3::operator*(const ENfloat num) const
{
 ENMatrix3X3 res;
 //////
 res.val[0][0]=val[0][0]*num;
 res.val[0][1]=val[0][1]*num;
 res.val[0][2]=val[0][2]*num;

 res.val[1][0]=val[1][0]*num;
 res.val[1][1]=val[1][1]*num;
 res.val[1][2]=val[1][2]*num;

 res.val[2][0]=val[2][0]*num;
 res.val[2][1]=val[2][1]*num;
 res.val[2][2]=val[2][2]*num;

 return res;
}

ENMatrix3X3 ENMatrix3X3::operator/(const ENfloat num) const
{
 ENMatrix3X3 res;
 //////
 res.val[0][0]=val[0][0]/num;
 res.val[0][1]=val[0][1]/num;
 res.val[0][2]=val[0][2]/num;

 res.val[1][0]=val[1][0]/num;
 res.val[1][1]=val[1][1]/num;
 res.val[1][2]=val[1][2]/num;

 res.val[2][0]=val[2][0]/num;
 res.val[2][1]=val[2][1]/num;
 res.val[2][2]=val[2][2]/num;

 return res;
}

ENMatrix3X3 ENMatrix3X3::operator+(const ENMatrix3X3& mat) const
{
 ENMatrix3X3 res;
 ///////
 res.val[0][0]=val[0][0]+mat.val[0][0];
 res.val[0][1]=val[0][1]+mat.val[0][1];
 res.val[0][2]=val[0][2]+mat.val[0][2];

 res.val[1][0]=val[1][0]+mat.val[1][0];
 res.val[1][1]=val[1][1]+mat.val[1][1];
 res.val[1][2]=val[1][2]+mat.val[1][2];

 res.val[2][0]=val[2][0]+mat.val[2][0];
 res.val[2][1]=val[2][1]+mat.val[2][1];
 res.val[2][2]=val[2][2]+mat.val[2][2];
 return res;
}

ENMatrix3X3 ENMatrix3X3::operator-(const ENMatrix3X3& mat) const
{
 ENMatrix3X3 res;
 ///////
 res.val[0][0]=val[0][0]-mat.val[0][0];
 res.val[0][1]=val[0][1]-mat.val[0][1];
 res.val[0][2]=val[0][2]-mat.val[0][2];

 res.val[1][0]=val[1][0]-mat.val[1][0];
 res.val[1][1]=val[1][1]-mat.val[1][1];
 res.val[1][2]=val[1][2]-mat.val[1][2];

 res.val[2][0]=val[2][0]-mat.val[2][0];
 res.val[2][1]=val[2][1]-mat.val[2][1];
 res.val[2][2]=val[2][2]-mat.val[2][2];
 return res;
}
ENMatrix3X3 ENMatrix3X3::operator*(const ENMatrix3X3& mat) const
{
 ENMatrix3X3 res;
 //mat0|0
 res.val[0][0]=val[0][0]*mat.val[0][0];
 res.val[0][0]+=val[0][1]*mat.val[1][0];
 res.val[0][0]+=val[0][2]*mat.val[2][0];
 //mat0|1
 res.val[0][1]=val[0][0]*mat.val[0][1];
 res.val[0][1]+=val[0][1]*mat.val[1][1];
 res.val[0][1]+=val[0][2]*mat.val[2][1];
 //mat0|2
 res.val[0][2]=val[0][0]*mat.val[0][2];
 res.val[0][2]+=val[0][1]*mat.val[1][2];
 res.val[0][2]+=val[0][2]*mat.val[2][2];
 //mat1|0
 res.val[1][0]=val[1][0]*mat.val[0][0];
 res.val[1][0]+=val[1][1]*mat.val[1][0];
 res.val[1][0]+=val[1][2]*mat.val[2][0];
 //mat1|1
 res.val[1][1]=val[1][0]*mat.val[0][1];
 res.val[1][1]+=val[1][1]*mat.val[1][1];
 res.val[1][1]+=val[1][2]*mat.val[2][1];
 //mat1|2
 res.val[1][2]=val[1][0]*mat.val[0][2];
 res.val[1][2]+=val[1][1]*mat.val[1][2];
 res.val[1][2]+=val[1][2]*mat.val[2][2];
 //mat2|0
 res.val[2][0]=val[2][0]*mat.val[0][0];
 res.val[2][0]+=val[2][1]*mat.val[1][0];
 res.val[2][0]+=val[2][2]*mat.val[2][0];
 //mat2|1
 res.val[2][1]=val[2][0]*mat.val[0][1];
 res.val[2][1]+=val[2][1]*mat.val[1][1];
 res.val[2][1]+=val[2][2]*mat.val[2][1];
 //mat2|2
 res.val[2][2]=val[2][0]*mat.val[0][2];
 res.val[2][2]+=val[2][1]*mat.val[1][2];
 res.val[2][2]+=val[2][2]*mat.val[2][2];

 return res;
}

ENVector ENMatrix3X3::operator*(const ENVector& vec) const
{
 ENVector res;
 ////
 res.v[0]=val[0][0]*vec.v[0]+val[0][1]*vec.v[1]+val[0][2]*vec.v[2];
 res.v[1]=val[1][0]*vec.v[0]+val[1][1]*vec.v[1]+val[1][2]*vec.v[2];
 res.v[2]=val[2][0]*vec.v[0]+val[2][1]*vec.v[1]+val[2][2]*vec.v[2];

 return res;
}

//Matrix4X Operators
ENMatrix4X4 ENMatrix4X4::operator+(const ENfloat num) const
{
 ENMatrix4X4 res;
 //////
 res.val[0][0]=val[0][0]+num;
 res.val[0][1]=val[0][1];
 res.val[0][2]=val[0][2];
 res.val[0][3]=val[0][3];

 res.val[1][0]=val[1][0];
 res.val[1][1]=val[1][1]+num;
 res.val[1][2]=val[1][2];
 res.val[1][3]=val[1][3];

 res.val[2][0]=val[2][0];
 res.val[2][1]=val[2][1];
 res.val[2][2]=val[2][2]+num;
 res.val[2][3]=val[2][3];

 res.val[3][0]=val[3][0];
 res.val[3][1]=val[3][1];
 res.val[3][2]=val[3][2];
 res.val[3][3]=val[3][3]+num;

 return res;
}

ENMatrix4X4 ENMatrix4X4::operator-(const ENfloat num) const
{
 ENMatrix4X4 res;
 //////
 res.val[0][0]=val[0][0]-num;
 res.val[0][1]=val[0][1];
 res.val[0][2]=val[0][2];
 res.val[0][3]=val[0][3];

 res.val[1][0]=val[1][0];
 res.val[1][1]=val[1][1]-num;
 res.val[1][2]=val[1][2];
 res.val[1][3]=val[1][3];

 res.val[2][0]=val[2][0];
 res.val[2][1]=val[2][1];
 res.val[2][2]=val[2][2]-num;
 res.val[2][3]=val[2][3];

 res.val[3][0]=val[3][0];
 res.val[3][1]=val[3][1];
 res.val[3][2]=val[3][2];
 res.val[3][3]=val[3][3]-num;

 return res;
}

ENMatrix4X4 ENMatrix4X4::operator*(const ENfloat num) const
{
 ENMatrix4X4 res;
 //////
 res.val[0][0]=val[0][0]*num;
 res.val[0][1]=val[0][1]*num;
 res.val[0][2]=val[0][2]*num;
 res.val[0][3]=val[0][3]*num;

 res.val[1][0]=val[1][0]*num;
 res.val[1][1]=val[1][1]*num;
 res.val[1][2]=val[1][2]*num;
 res.val[1][3]=val[1][3]*num;

 res.val[2][0]=val[2][0]*num;
 res.val[2][1]=val[2][1]*num;
 res.val[2][2]=val[2][2]*num;
 res.val[2][3]=val[2][3]*num;

 res.val[3][0]=val[3][0]*num;
 res.val[3][1]=val[3][1]*num;
 res.val[3][2]=val[3][2]*num;
 res.val[3][3]=val[3][3]*num;

 return res;
}

ENMatrix4X4 ENMatrix4X4::operator/(const ENfloat num) const
{
 ENMatrix4X4 res;
 //////
 res.val[0][0]=val[0][0]/num;
 res.val[0][1]=val[0][1]/num;
 res.val[0][2]=val[0][2]/num;
 res.val[0][3]=val[0][3]/num;

 res.val[1][0]=val[1][0]/num;
 res.val[1][1]=val[1][1]/num;
 res.val[1][2]=val[1][2]/num;
 res.val[1][3]=val[1][3]/num;

 res.val[2][0]=val[2][0]/num;
 res.val[2][1]=val[2][1]/num;
 res.val[2][2]=val[2][2]/num;
 res.val[2][3]=val[2][3]/num;

 res.val[3][0]=val[3][0]/num;
 res.val[3][1]=val[3][1]/num;
 res.val[3][2]=val[3][2]/num;
 res.val[3][3]=val[3][3]/num;

 return res;
}

ENMatrix4X4 ENMatrix4X4::operator+(const ENMatrix4X4& mat) const
{
 ENMatrix4X4 res;
 ///////
 res.val[0][0]=val[0][0]+mat.val[0][0];
 res.val[0][1]=val[0][1]+mat.val[0][1];
 res.val[0][2]=val[0][2]+mat.val[0][2];
 res.val[0][3]=val[0][3]+mat.val[0][3];

 res.val[1][0]=val[1][0]+mat.val[1][0];
 res.val[1][1]=val[1][1]+mat.val[1][1];
 res.val[1][2]=val[1][2]+mat.val[1][2];
 res.val[1][3]=val[1][3]+mat.val[1][3];

 res.val[2][0]=val[2][0]+mat.val[2][0];
 res.val[2][1]=val[2][1]+mat.val[2][1];
 res.val[2][2]=val[2][2]+mat.val[2][2];
 res.val[2][3]=val[2][3]+mat.val[2][3];

 res.val[3][0]=val[3][0]+mat.val[3][0];
 res.val[3][1]=val[3][1]+mat.val[3][1];
 res.val[3][2]=val[3][2]+mat.val[3][2];
 res.val[3][3]=val[3][3]+mat.val[3][3];
 return res;
}

ENMatrix4X4 ENMatrix4X4::operator-(const ENMatrix4X4& mat) const
{
 ENMatrix4X4 res;
 ///////
 res.val[0][0]=val[0][0]-mat.val[0][0];
 res.val[0][1]=val[0][1]-mat.val[0][1];
 res.val[0][2]=val[0][2]-mat.val[0][2];
 res.val[0][3]=val[0][3]-mat.val[0][3];

 res.val[1][0]=val[1][0]-mat.val[1][0];
 res.val[1][1]=val[1][1]-mat.val[1][1];
 res.val[1][2]=val[1][2]-mat.val[1][2];
 res.val[1][3]=val[1][3]-mat.val[1][3];

 res.val[2][0]=val[2][0]-mat.val[2][0];
 res.val[2][1]=val[2][1]-mat.val[2][1];
 res.val[2][2]=val[2][2]-mat.val[2][2];
 res.val[2][3]=val[2][3]-mat.val[2][3];

 res.val[3][0]=val[3][0]-mat.val[3][0];
 res.val[3][1]=val[3][1]-mat.val[3][1];
 res.val[3][2]=val[3][2]-mat.val[3][2];
 res.val[3][3]=val[3][3]-mat.val[3][3];
 return res;
}

ENMatrix4X4 ENMatrix4X4::operator*(const ENMatrix4X4& mat) const
{
 ENMatrix4X4 res;
 //mat0|0
 res.val[0][0]=val[0][0]*mat.val[0][0];
 res.val[0][0]+=val[0][1]*mat.val[1][0];
 res.val[0][0]+=val[0][2]*mat.val[2][0];
 res.val[0][0]+=val[0][3]*mat.val[3][0];
 //mat0|1
 res.val[0][1]=val[0][0]*mat.val[0][1];
 res.val[0][1]+=val[0][1]*mat.val[1][1];
 res.val[0][1]+=val[0][2]*mat.val[2][1];
 res.val[0][1]+=val[0][3]*mat.val[3][1];
 //mat0|2
 res.val[0][2]=val[0][0]*mat.val[0][2];
 res.val[0][2]+=val[0][1]*mat.val[1][2];
 res.val[0][2]+=val[0][2]*mat.val[2][2];
 res.val[0][2]+=val[0][3]*mat.val[3][2];
 //mat0|3
 res.val[0][3]=val[0][0]*mat.val[0][3];
 res.val[0][3]+=val[0][1]*mat.val[1][3];
 res.val[0][3]+=val[0][2]*mat.val[2][3];
 res.val[0][3]+=val[0][3]*mat.val[3][3];

 //mat1|0
 res.val[1][0]=val[1][0]*mat.val[0][0];
 res.val[1][0]+=val[1][1]*mat.val[1][0];
 res.val[1][0]+=val[1][2]*mat.val[2][0];
 res.val[1][0]+=val[1][3]*mat.val[3][0];
 //mat1|1
 res.val[1][1]=val[1][0]*mat.val[0][1];
 res.val[1][1]+=val[1][1]*mat.val[1][1];
 res.val[1][1]+=val[1][2]*mat.val[2][1];
 res.val[1][1]+=val[1][3]*mat.val[3][1];
 //mat1|2
 res.val[1][2]=val[1][0]*mat.val[0][2];
 res.val[1][2]+=val[1][1]*mat.val[1][2];
 res.val[1][2]+=val[1][2]*mat.val[2][2];
 res.val[1][2]+=val[1][3]*mat.val[3][2];
 //mat1|3
 res.val[1][3]=val[1][0]*mat.val[0][3];
 res.val[1][3]+=val[1][1]*mat.val[1][3];
 res.val[1][3]+=val[1][2]*mat.val[2][3];
 res.val[1][3]+=val[1][3]*mat.val[3][3];

 //mat2|0
 res.val[2][0]=val[2][0]*mat.val[0][0];
 res.val[2][0]+=val[2][1]*mat.val[1][0];
 res.val[2][0]+=val[2][2]*mat.val[2][0];
 res.val[2][0]+=val[2][3]*mat.val[3][0];
 //mat2|1
 res.val[2][1]=val[2][0]*mat.val[0][1];
 res.val[2][1]+=val[2][1]*mat.val[1][1];
 res.val[2][1]+=val[2][2]*mat.val[2][1];
 res.val[2][1]+=val[2][3]*mat.val[3][1];
 //mat2|2
 res.val[2][2]=val[2][0]*mat.val[0][2];
 res.val[2][2]+=val[2][1]*mat.val[1][2];
 res.val[2][2]+=val[2][2]*mat.val[2][2];
 res.val[2][2]+=val[2][3]*mat.val[3][2];
 //mat2|3
 res.val[2][3]=val[2][0]*mat.val[0][3];
 res.val[2][3]+=val[2][1]*mat.val[1][3];
 res.val[2][3]+=val[2][2]*mat.val[2][3];
 res.val[2][3]+=val[2][3]*mat.val[3][3];

 //mat3|0
 res.val[3][0]=val[3][0]*mat.val[0][0];
 res.val[3][0]+=val[3][1]*mat.val[1][0];
 res.val[3][0]+=val[3][2]*mat.val[2][0];
 res.val[3][0]+=val[3][3]*mat.val[3][0];
 //mat3|1
 res.val[3][1]=val[3][0]*mat.val[0][1];
 res.val[3][1]+=val[3][1]*mat.val[1][1];
 res.val[3][1]+=val[3][2]*mat.val[2][1];
 res.val[3][1]+=val[3][3]*mat.val[3][1];
 //mat3|2
 res.val[3][2]=val[3][0]*mat.val[0][2];
 res.val[3][2]+=val[3][1]*mat.val[1][2];
 res.val[3][2]+=val[3][2]*mat.val[2][2];
 res.val[3][2]+=val[3][3]*mat.val[3][2];
 //mat3|3
 res.val[3][3]=val[3][0]*mat.val[0][3];
 res.val[3][3]+=val[3][1]*mat.val[1][3];
 res.val[3][3]+=val[3][2]*mat.val[2][3];
 res.val[3][3]+=val[3][3]*mat.val[3][3];

 return res;
}

ENVector4 ENMatrix4X4::operator*(const ENVector4& vec) const
{
 ENVector4 res;
 ////
 for(ENuint a=0;a<4;a++)
   res.v[a]=val[a][0]*res.v[0]+val[a][1]*res.v[1]+val[a][2]*res.v[2]+
   val[a][3]*res.v[3];

 return res;
}
//------------------Matrices functions-----------------//
ENfloat ENDet(const ENMatrix2X2& mat)
{
 return mat.val[0][0]*mat.val[1][1]-mat.val[0][1]*mat.val[1][0];
}

ENfloat ENDet(const ENMatrix3X3& mat)
{
 ENfloat res=0;
 //
 res=mat.val[0][0]*mat.val[1][1]*mat.val[2][2];
 res+=mat.val[0][1]*mat.val[1][2]*mat.val[2][0];
 res+=mat.val[0][2]*mat.val[1][0]*mat.val[2][1];
 
 res-=mat.val[2][0]*mat.val[1][1]*mat.val[0][2];
 res-=mat.val[2][1]*mat.val[1][2]*mat.val[0][0];
 res-=mat.val[2][2]*mat.val[1][0]*mat.val[0][1];

 return res;
}

ENfloat ENDet(const ENMatrix4X4& mat)
{
 ENMatrix3X3 _mat3x3;
 ENfloat res=0;
 //Part 1 of 4
 memcpy(&_mat3x3.val[0][0],&mat.val[1][1],sizeof(ENfloat)*3);
 memcpy(&_mat3x3.val[1][0],&mat.val[2][1],sizeof(ENfloat)*3);
 memcpy(&_mat3x3.val[2][0],&mat.val[3][1],sizeof(ENfloat)*3);
 res+=mat.val[0][0]*ENDet(_mat3x3);
 //Part 2 of 4
 memcpy(&_mat3x3.val[0][0],&mat.val[0][1],sizeof(ENfloat)*3);
 res-=mat.val[1][0]*ENDet(_mat3x3);
 //Part 3 of 4
 memcpy(&_mat3x3.val[1][0],&mat.val[1][1],sizeof(ENfloat)*3);
 res+=mat.val[2][0]*ENDet(_mat3x3);
 //Part 4 of 4
 memcpy(&_mat3x3.val[2][0],&mat.val[2][1],sizeof(ENfloat)*3);
 res-=mat.val[3][0]*ENDet(_mat3x3);

 return res;
}

ENMatrix2X2 ENInv(const ENMatrix2X2& mat)
{
 ENMatrix2X2 res;
 ENfloat  determinant=ENDet(mat);
 if(determinant==0) return res;

 res.val[0][0]=mat.val[1][1]/determinant;
 res.val[1][1]=mat.val[0][0]/determinant;

 res.val[0][1]=-mat.val[0][1]/determinant;
 res.val[1][0]=-mat.val[1][0]/determinant;

 return res;
}

ENbool ENInv2(ENMatrix2X2& mat)
{
 ENMatrix2X2 res;
 ENfloat  determinant=ENDet(mat);
 if(determinant==0.0f) return false;

 res.val[0][0]=mat.val[1][1]/determinant;
 res.val[1][1]=mat.val[0][0]/determinant;

 res.val[0][1]=-mat.val[0][1]/determinant;
 res.val[1][0]=-mat.val[1][0]/determinant;

 mat=res;
 return true;
}

ENMatrix3X3 ENInv(const ENMatrix3X3& mat)
{
 ENMatrix3X3 res;
 ENfloat  determinant=ENDet(mat);
 if(determinant==0) return res;

 res.val[0][0] = (mat.val[1][1]*mat.val[2][2] - mat.val[1][2]*mat.val[2][1])/determinant;
 res.val[0][1] = (mat.val[0][2]*mat.val[2][1] - mat.val[0][1]*mat.val[2][2])/determinant;
 res.val[0][2] = (mat.val[0][1]*mat.val[1][2] - mat.val[0][2]*mat.val[1][1])/determinant;

 res.val[1][0] = (mat.val[1][2]*mat.val[2][0] - mat.val[1][0]*mat.val[2][2])/determinant;
 res.val[1][1] = (mat.val[0][0]*mat.val[2][2] - mat.val[0][2]*mat.val[2][0])/determinant;
 res.val[1][2] = (mat.val[0][2]*mat.val[1][0] - mat.val[0][0]*mat.val[1][2])/determinant;

 res.val[2][0] = (mat.val[1][0]*mat.val[2][1] - mat.val[1][1]*mat.val[2][0])/determinant;
 res.val[2][1] = (mat.val[0][1]*mat.val[2][0] - mat.val[0][0]*mat.val[2][1])/determinant;
 res.val[2][2] = (mat.val[0][0]*mat.val[1][1] - mat.val[0][1]*mat.val[1][0])/determinant;

 return res;
}

ENbool ENInv2(ENMatrix3X3& mat)
{
 ENMatrix3X3 res;
 ENfloat  determinant=ENDet(mat);
 if(determinant==0.0f) return false;

 res.val[0][0] = (mat.val[1][1]*mat.val[2][2] - mat.val[1][2]*mat.val[2][1])/determinant;
 res.val[0][1] = (mat.val[0][2]*mat.val[2][1] - mat.val[0][1]*mat.val[2][2])/determinant;
 res.val[0][2] = (mat.val[0][1]*mat.val[1][2] - mat.val[0][2]*mat.val[1][1])/determinant;

 res.val[1][0] = (mat.val[1][2]*mat.val[2][0] - mat.val[1][0]*mat.val[2][2])/determinant;
 res.val[1][1] = (mat.val[0][0]*mat.val[2][2] - mat.val[0][2]*mat.val[2][0])/determinant;
 res.val[1][2] = (mat.val[0][2]*mat.val[1][0] - mat.val[0][0]*mat.val[1][2])/determinant;

 res.val[2][0] = (mat.val[1][0]*mat.val[2][1] - mat.val[1][1]*mat.val[2][0])/determinant;
 res.val[2][1] = (mat.val[0][1]*mat.val[2][0] - mat.val[0][0]*mat.val[2][1])/determinant;
 res.val[2][2] = (mat.val[0][0]*mat.val[1][1] - mat.val[0][1]*mat.val[1][0])/determinant;

 mat=res;
 return true;
}

ENMatrix4X4 ENInv(const ENMatrix4X4& mat)
{
 ENMatrix4X4 res;
 ENMatrix3X3 TempMatrix;
 ENfloat  determinant=ENDet(mat);

 if(determinant==0) return res;

 for(ENuint l=0;l<4;l++)
   for(ENuint r=0;r<4;r++)
   {
	ENuint x,y;
	//Upper left part
	for(x=0;x<l;x++)
	  for(y=0;y<r;y++)
		TempMatrix.val[x][y]=mat.val[x][y];
	//Lower left part
	for(x=l+1;x<4;x++)
	  for(y=0;y<r;y++)
		TempMatrix.val[x-1][y]=mat.val[x][y];
	//Lower right part
	for(x=l+1;x<4;x++)
	  for(y=r+1;y<4;y++)
		TempMatrix.val[x-1][y-1]=mat.val[x][y];
	//Upper right part
	for(x=0;x<l;x++)
	  for(y=r+1;y<4;y++)
		TempMatrix.val[x][y-1]=mat.val[x][y];

	if((r+l%2)%2)
	  res.val[r][l]=-ENDet(TempMatrix)/determinant;
	else
	  res.val[r][l]=ENDet(TempMatrix)/determinant;
   }


 return res;
}

ENbool ENInv2(ENMatrix4X4& mat)
{
 ENMatrix4X4 res;
 ENMatrix3X3 TempMatrix;
 ENfloat  determinant=ENDet(mat);

 if(determinant==0.0f) return false;

 for(ENuint l=0;l<4;l++)
   for(ENuint r=0;r<4;r++)
   {
	ENuint x,y;
	//Upper left part
	for(x=0;x<l;x++)
	  for(y=0;y<r;y++)
		TempMatrix.val[x][y]=mat.val[x][y];
	//Lower left part
	for(x=l+1;x<4;x++)
	  for(y=0;y<r;y++)
		TempMatrix.val[x-1][y]=mat.val[x][y];
	//Lower right part
	for(x=l+1;x<4;x++)
	  for(y=r+1;y<4;y++)
		TempMatrix.val[x-1][y-1]=mat.val[x][y];
	//Upper right part
	for(x=0;x<l;x++)
	  for(y=r+1;y<4;y++)
		TempMatrix.val[x][y-1]=mat.val[x][y];

	if((r+l%2)%2)
	  res.val[r][l]=-ENDet(TempMatrix)/determinant;
	else
	  res.val[r][l]=ENDet(TempMatrix)/determinant;
   }


 mat=res;
 return true;
}

ENMatrix2X2 ENTrans(const ENMatrix2X2& mat)
{
 ENMatrix2X2 res;
 for(ENuint x=0;x<2;x++)
   for(ENuint y=0;y<2;y++)
     res.val[x][y]=mat.val[y][x];

 return res;     
}

ENMatrix3X3 ENTrans(const ENMatrix3X3& mat)
{
 ENMatrix3X3 res;
 for(ENuint x=0;x<3;x++)
   for(ENuint y=0;y<3;y++)
     res.val[x][y]=mat.val[y][x];

 return res;
}

ENMatrix4X4 ENTrans(const ENMatrix4X4& mat)
{
 ENMatrix4X4 res;
 for(ENuint x=0;x<4;x++)
   for(ENuint y=0;y<4;y++)
     res.val[x][y]=mat.val[y][x];

 return res;
}

ENMatrix3X3 ENTangentMatrix(const ENVector& vNormal)
{
 ENVector A,B;
 ENMatrix3X3 matrix;
 ENVector Abs=ENAbsolute(vNormal);
 if(Abs.v[0]>=Abs.v[1]&&Abs.v[0]>=Abs.v[2])
   A=ENVector(vNormal.v[2],vNormal.v[1],vNormal.v[2]-vNormal.v[0]);
 else
   if(Abs.v[1]>=Abs.v[0]&&Abs.v[1]>=Abs.v[2])
     A=ENVector(vNormal.v[1],-vNormal.v[0],vNormal.v[2]);
   else
	 if(Abs.v[2]>=Abs.v[0]&&Abs.v[2]>Abs.v[1])
   	   A=ENVector(vNormal.v[0],-vNormal.v[2],vNormal.v[1]);

 B=ENCross(A,vNormal);
 B=ENNormalize(B);
 //Fill matrix
 matrix.val[0][0]=A.v[0];
 matrix.val[1][0]=A.v[1];
 matrix.val[2][0]=A.v[2];
 matrix.val[0][1]=B.v[0];
 matrix.val[1][1]=B.v[1];
 matrix.val[2][1]=B.v[2];
 matrix.val[0][2]=vNormal.v[0];
 matrix.val[1][2]=vNormal.v[1];
 matrix.val[2][2]=vNormal.v[2];
 return matrix;
}

ENMatrix3X3 ENRotMatrixX(const ENfloat angle)
{
 //VARS
 ENMatrix3X3 vmat;
 //Config matrix
 vmat.val[0][0]=1.0f;
 vmat.val[1][0]=0.0f;
 vmat.val[2][0]=0.0f;

 vmat.val[0][1]=0.0f;
 vmat.val[1][1]=(ENfloat)cos(angle);
 vmat.val[2][1]=-(ENfloat)sin(angle);

 vmat.val[0][2]=0.0f;
 vmat.val[1][2]=(ENfloat)sin(angle);
 vmat.val[2][2]=(ENfloat)cos(angle);
 //Return result

 return vmat;
}

ENMatrix3X3 ENRotMatrixY(const ENfloat angle)
{
 //VARS
 ENMatrix3X3 vmat;
 //Config matrix
 vmat.val[0][0]=(ENfloat)cos(angle);
 vmat.val[1][0]=0.0f;
 vmat.val[2][0]=(ENfloat)sin(angle);

 vmat.val[0][1]=0.0f;
 vmat.val[1][1]=1.0f;
 vmat.val[2][1]=0.0f;

 vmat.val[0][2]=(ENfloat)-sin(angle);
 vmat.val[1][2]=0.0f;
 vmat.val[2][2]=(ENfloat)cos(angle);
 //Return result
 return vmat;
}

ENMatrix3X3 ENRotMatrixZ(const ENfloat angle)
{
 //VARS
 ENMatrix3X3 vmat;
 //Config matrix
 vmat.val[0][0]=(ENfloat)cos(angle);
 vmat.val[1][0]=(ENfloat)sin(angle);
 vmat.val[2][0]=0.0f;

 vmat.val[0][1]=-(ENfloat)sin(angle);
 vmat.val[1][1]=(ENfloat)cos(angle);
 vmat.val[2][1]=0.0f;

 vmat.val[0][2]=0.0f;
 vmat.val[1][2]=0.0f;
 vmat.val[2][2]=1.0f;
 //Return result
 return vmat;
}

ENMatrix4X4 ENBiasMatrix()
{
 //Static bias matrix
 static ENMatrix4X4 biasMatrix=
 ENMatrix4X4(0.5f,0.0f,0.0f,0.0f,
             0.0f,0.5f,0.0f,0.0f,
             0.0f,0.0f,0.5f,0.0f,
             0.5f,0.5f,0.5f,1.0f);
 //Return bias matrix
 return biasMatrix;
}
//------------------Common math functions-----------------//
ENfloat ENRadians(const ENfloat Degree)
{
 return Degree*(ENfloat)M_PI/180;
}

ENfloat ENSign(const ENfloat num)
{
 if(num < 0.0f) return -1.0f;
 if(num > 0.0f) return  1.0f;

 return 0.0f;
}

ENfloat ENAbsolute(const ENfloat num)
{
 if(num < 0)
 return -num;

 return num;
}

ENVector2 ENAbsolute(const ENVector2& vec)
{
 ENVector2 res;
 res.v[0]=ENAbsolute(vec.v[0]);
 res.v[1]=ENAbsolute(vec.v[1]);
 return res; 
}

ENVector ENAbsolute(const ENVector& vec)
{
 ENVector res;
 res.v[0]=ENAbsolute(vec.v[0]);
 res.v[1]=ENAbsolute(vec.v[1]);
 res.v[2]=ENAbsolute(vec.v[2]);
 return res;
}

ENVector4 ENAbsolute(const ENVector4& vec)
{
 ENVector4 res;
 res.v[0]=ENAbsolute(vec.v[0]);
 res.v[1]=ENAbsolute(vec.v[1]);
 res.v[2]=ENAbsolute(vec.v[2]);
 res.v[3]=ENAbsolute(vec.v[3]);
 return res;
}

ENfloat ENLength(const ENVector2& vec)
{
 return (ENfloat)sqrt(vec.v[0]*vec.v[0]+vec.v[1]*vec.v[1]);
}

ENfloat ENLength(const ENVector& vec)
{
 return (ENfloat)sqrt(vec.v[0]*vec.v[0]+vec.v[1]*vec.v[1]+vec.v[2]*vec.v[2]);
}

ENfloat ENLength(const ENVector4& vec)
{
 return (ENfloat)sqrt(vec.v[0]*vec.v[0]+vec.v[1]*vec.v[1]+vec.v[2]*vec.v[2]+vec.v[3]*vec.v[3]);
}

ENVector2 ENNormalize(const ENVector2& vNormal)
{
 ENfloat len=ENLength(vNormal);
 if(len!=0.0f)
   return vNormal/len;
 else
   return vNormal;   
}

ENVector ENNormalize(const ENVector& vNormal)
{
 ENfloat len=ENLength(vNormal);
 if(len!=0.0f)
   return vNormal/len;
 else
   return vNormal;   
}

ENVector4 ENNormalize(const ENVector4& vNormal)
{
 ENfloat len=ENLength(vNormal);
 if(len!=0.0f)
   return vNormal/len;
 else
   return vNormal;   
}

ENfloat ENDot(const ENVector2& vec1,const ENVector2& vec2)
{
 return ((vec1.v[0]*vec2.v[0])+(vec1.v[1]*vec2.v[1]));
}

ENfloat ENDot(const ENVector& vec1,const ENVector& vec2)
{
 return ((vec1.v[0]*vec2.v[0])+(vec1.v[1]*vec2.v[1])+(vec1.v[2]*vec2.v[2]));
}

ENfloat ENDot(const ENVector4& vec1,const ENVector4& vec2)
{
 return ((vec1.v[0]*vec2.v[0])+(vec1.v[1]*vec2.v[1])+(vec1.v[2]*vec2.v[2])+(vec1.v[3]*vec2.v[3]));
}

ENfloat ENDistance(const ENVector2& vec1,const ENVector2& vec2)
{
 return ENLength(vec1-vec2);
}

ENfloat ENDistance(const ENVector& vec1,const ENVector& vec2)
{
 return ENLength(vec1-vec2);
}

ENfloat ENDistance(const ENVector4& vec1,const ENVector4& vec2)
{
 return ENLength(vec1-vec2);
}

ENfloat ENDistance2(const ENVector2& vec1,const ENVector2& vec2)
{
 ENVector2 v=vec1-vec2;
 return ENDot(v,v);
}

ENfloat ENDistance2(const ENVector& vec1,const ENVector& vec2)
{
 ENVector v=vec1-vec2;
 return ENDot(v,v);
}

ENfloat ENDistance2(const ENVector4& vec1,const ENVector4& vec2)
{
 ENVector4 v=vec1-vec2;
 return ENDot(v,v);
}

ENint ENInterpLinear(const ENint a,const ENint b,const ENfloat fac)
{
 return a+(b-a)*fac;
}

ENfloat ENInterpLinear(const ENfloat a,const ENfloat b,const ENfloat fac)
{
 return a+(b-a)*fac;
}

ENVector2 ENInterpLinear(const ENVector2& a,const ENVector2& b,const ENfloat fac)
{
 return a+(b-a)*fac;
}

ENVector ENInterpLinear(const ENVector& a,const ENVector& b,const ENfloat fac)
{
 return a+(b-a)*fac;
}

ENVector4 ENInterpLinear(const ENVector4& a,const ENVector4& b,const ENfloat fac)
{
 return a+(b-a)*fac;
}

ENVector ENCross(const ENVector& vec1,const ENVector& vec2)
{
 ENVector vNormal;

 vNormal.v[0]=((vec1.v[1]*vec2.v[2])-(vec1.v[2]*vec2.v[1]));
 vNormal.v[1]=((vec1.v[2]*vec2.v[0])-(vec1.v[0]*vec2.v[2]));
 vNormal.v[2]=((vec1.v[0]*vec2.v[1])-(vec1.v[1]*vec2.v[0]));

 return vNormal;
}

ENVector ENNormal(const ENVector tri[])
{
 ENVector res=ENCross(tri[2]-tri[0],tri[1]-tri[0]);
 return res/ENLength(res);
}

ENbool ENNormal(const ENVector tri[],ENVector &res)
{
 res=ENCross(tri[2]-tri[0],tri[1]-tri[0]);
 ENfloat len=ENLength(res);
 if(len==0.0f) return false;
 res=res/len;
 return true;
}

ENfloat ENPlaneDistance(ENVector Normal,ENVector Point)
{
 return -((Normal.v[0]*Point.v[0])+(Normal.v[1]*Point.v[1])+(Normal.v[2]*Point.v[2]));
}

ENfloat ENTriangleVolume(ENVector *tri)
{
 ENMatrix3X3 mat;
 ENVector    A;
 //A1
 mat.val[0][0]=tri[0].v[1];
 mat.val[0][1]=tri[0].v[2];
 mat.val[0][2]=1.0f;

 mat.val[1][0]=tri[1].v[1];
 mat.val[1][1]=tri[1].v[2];
 mat.val[1][2]=1.0f;

 mat.val[2][0]=tri[2].v[1];
 mat.val[2][1]=tri[2].v[2];
 mat.val[2][2]=1.0f;
 
 A.v[0]=ENDet(mat);
 //A2
 mat.val[0][0]=tri[0].v[2];
 mat.val[0][1]=tri[0].v[0];
 mat.val[0][2]=1;

 mat.val[1][0]=tri[1].v[2];
 mat.val[1][1]=tri[1].v[0];
 mat.val[1][2]=1;

 mat.val[2][0]=tri[2].v[2];
 mat.val[2][1]=tri[2].v[0];
 mat.val[2][2]=1;
 
 A.v[1]=ENDet(mat);
 //A3
 mat.val[0][0]=tri[0].v[0];
 mat.val[0][1]=tri[0].v[1];
 mat.val[0][2]=1;

 mat.val[1][0]=tri[1].v[0];
 mat.val[1][1]=tri[1].v[1];
 mat.val[1][2]=1;

 mat.val[2][0]=tri[2].v[0];
 mat.val[2][1]=tri[2].v[1];
 mat.val[2][2]=1;
 
 A.v[2]=ENDet(mat);
 return ENLength(A);
}


ENbool ENIntersectedPlane(ENVector vPoly[],ENVector vLine[],
                          ENVector &vNormal,ENfloat &originDistance)
{
 ENfloat distance1=0, distance2=0;

 vNormal = ENNormal(vPoly);

 originDistance = ENPlaneDistance(vNormal, vPoly[0]);
 distance1 = ((vNormal.v[0] * vLine[0].v[0])  +
		         (vNormal.v[1] * vLine[0].v[1])  +
				 (vNormal.v[2] * vLine[0].v[2])) + originDistance;

 distance2 = ((vNormal.v[0] * vLine[1].v[0])  +
		         (vNormal.v[1] * vLine[1].v[1])  +
				 (vNormal.v[2] * vLine[1].v[2])) + originDistance;

 if(distance1 * distance2 >= 0)
   return false;

 return true;
}

ENVector ENIntersectionPoint(ENVector vNormal,ENVector vLine[],ENfloat distance)
{
 ENVector vPoint, vLineDir;
 ENfloat Numerator = 0.0, Denominator = 0.0, dist = 0.0;

 vLineDir = vLine[1] - vLine[0];
 vLineDir = ENNormalize(vLineDir);

 Numerator = - (vNormal.v[0] * vLine[0].v[0] +
	        vNormal.v[1] * vLine[0].v[1] +
	        vNormal.v[2] * vLine[0].v[2] + distance);

 Denominator = ENDot(vNormal, vLineDir);

 if( Denominator == 0.0)
    return vLine[0];

 dist = Numerator / Denominator;

 vPoint.v[0] = (float)(vLine[0].v[0] + (vLineDir.v[0] * dist));
 vPoint.v[1] = (float)(vLine[0].v[1] + (vLineDir.v[1] * dist));
 vPoint.v[2] = (float)(vLine[0].v[2] + (vLineDir.v[2] * dist));

 return vPoint;
}

ENVector ENClosestPointOnPlane(ENVector &vPos,ENVector &vNormal,ENVector &point)
{
 //Variables
 ENVector vLine[2];
 ENfloat vPlaneDistance; 
 //Calculate plane distance
 vPlaneDistance=ENPlaneDistance(vNormal,vPos);
 //Create line
 vLine[0]=point;
 vLine[1]=point+vNormal;
 return ENIntersectionPoint(vNormal,vLine,vPlaneDistance);
}

ENVector ENClosestPointOnLine(ENVector vA,ENVector vB,ENVector vPoint)
{
 ENVector vVector1=vPoint-vA;
 ENVector vVector2=ENNormalize(vB-vA);
 ENfloat d = ENDistance(vA,vB);
 ENfloat t = ENDot(vVector2,vVector1);
 if(t<=0) 
   return vA;

 if (t>=d) 
   return vB;

 ENVector vVector3 = vVector2*t;
 ENVector vClosestPoint = vA+vVector3;

 return vClosestPoint;
}

ENbool ENBoxLineIntersect(ENVector vBox[],ENVector vLine[])
{
 //Variables
 ENVector vDir;
 ENVector vec;
 ENfloat  sfactor;
 //Check if linestart is inside of the box
 if(vLine[0]>=vBox[0]&&vLine[0]<=vBox[1]) return true;
 //Calculate direction vector
 vDir=vLine[1]-vLine[0];
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
       //Calculate intersection point
       vec=vLine[0]+vDir*sfactor;
       //Check if intersection point is in sphere
       if(vec>=vBox[0]&&vec<=vBox[1]) return true;
      }
     }
 //Check if line is complete in box
 return false;
}

ENbool ENBoxLineIntersect(ENVector vBox[],ENVector vLine[],ENVector &pos)
{
 //Variables
 ENVector vDir,vec;
 ENfloat  sfactor;
 ENfloat  lfactor=2.0f;
 ENbool   res=false;
 //Check if linestart is inside of box
 if(vLine[0]>=vBox[0]&&vLine[0]<=vBox[1])
 {
  pos=vLine[0];
  return true;
 }
 //Computer directional vector
 vDir=vLine[1]-vLine[0];
 //Check sides
 for(ENuint ori=0;ori<3;ori++)
   if(vDir.v[ori]!=0.0f)
     for(ENuint side=0;side<2;side++)
     {
      //Calculate sfactor
      sfactor=(vBox[side].v[ori]-vLine[0].v[ori])/vDir.v[ori];
      //If intersection point is in range
      if(sfactor>=0.0f&&sfactor<=1.0f&&sfactor<lfactor)
      {
       //Calculate intersection point
       vec=vLine[0]+vDir*sfactor;
       //Check if intersection point is in sphere
       if(vec>=vBox[0]&&vec<=vBox[1])
       {
        pos=vec;
        lfactor=sfactor;
        res=true;
       }
      }
     }
 //Finished
 return res;
}

ENbool ENBoxLineIntersect2(ENVector vBox[],ENVector vLine[])
{
 //Raw check
 if(vLine[0].v[0]>vBox[1].v[0]&&vLine[1].v[0]>vBox[1].v[0]) return false;
 if(vLine[0].v[0]<vBox[0].v[0]&&vLine[1].v[0]<vBox[0].v[0]) return false;

 if(vLine[0].v[1]>vBox[1].v[1]&&vLine[1].v[1]>vBox[1].v[1]) return false;
 if(vLine[0].v[1]<vBox[0].v[1]&&vLine[1].v[1]<vBox[0].v[1]) return false;

 if(vLine[0].v[2]>vBox[1].v[2]&&vLine[1].v[2]>vBox[1].v[2]) return false;
 if(vLine[0].v[2]<vBox[0].v[2]&&vLine[1].v[2]<vBox[0].v[2]) return false;
 //Check if intersect
 return ENBoxLineIntersect(vBox,vLine);
}

ENbool ENBoxLineIntersect2(ENVector vBox[],ENVector vLine[],ENVector &pos)
{
 //Raw check
 if(vLine[0].v[0]>vBox[1].v[0]&&vLine[1].v[0]>vBox[1].v[0]) return false;
 if(vLine[0].v[0]<vBox[0].v[0]&&vLine[1].v[0]<vBox[0].v[0]) return false;

 if(vLine[0].v[1]>vBox[1].v[1]&&vLine[1].v[1]>vBox[1].v[1]) return false;
 if(vLine[0].v[1]<vBox[0].v[1]&&vLine[1].v[1]<vBox[0].v[1]) return false;

 if(vLine[0].v[2]>vBox[1].v[2]&&vLine[1].v[2]>vBox[1].v[2]) return false;
 if(vLine[0].v[2]<vBox[0].v[2]&&vLine[1].v[2]<vBox[0].v[2]) return false;
 //Check if intersect
 return ENBoxLineIntersect(vBox,vLine,pos);
}

ENfloat ENVecAngle(ENVector Vector1,ENVector Vector2)
{
 ENfloat dotProduct = ENDot(Vector1, Vector2);
 ENfloat vectorsMagnitude = ENLength(Vector1) * ENLength(Vector2);

 ENfloat angle = (ENfloat)acos(dotProduct/vectorsMagnitude);

 if(_isnan(angle))
   return 0;

 return angle;
}

ENbool ENInsidePolygon(ENVector vIntersection,ENVector Poly[],long verticeCount)
{
 const ENfloat MATCH_FACTOR = 0.99f;
 double Angle = 0.0;
 ENVector vA, vB;

 for (int i = 0; i < verticeCount; i++)
 {
  vA = Poly[i] - vIntersection;
  vB = Poly[(i + 1) % verticeCount] - vIntersection;
  Angle += ENVecAngle(vA, vB);
 }

 if(Angle >= (MATCH_FACTOR * (2.0 * M_PI)))
		return true;

 return false;
}

ENint ENClassifySphere(ENVector &vCenter, 
				   ENVector &vNormal,ENVector &vPoint,ENfloat radius,ENfloat &distance)
{
 ENfloat d = ENPlaneDistance(vNormal,vPoint);
 
 distance = (vNormal.v[0]*vCenter.v[0]+vNormal.v[1]*vCenter.v[1]+vNormal.v[2]*vCenter.v[2]+d);

 if(ENAbsolute(distance)<radius)
   return INTERSECTS;
 else 
   if(distance >= radius)
	 return FRONT;
	
 return BEHIND;
}

ENbool ENEdgeSphereCollision(ENVector &vCenter,ENVector vPoly[],ENint vertexCount,ENfloat radius)
{
 ENVector vPoint;
 for(ENint i = 0; i < vertexCount; i++)
 {
  vPoint = ENClosestPointOnLine(vPoly[i], vPoly[(i+1)%vertexCount],vCenter);
  ENfloat distance=ENDistance(vPoint,vCenter);

  if(distance<radius)
	return true;
 }

 return false;
}

ENbool ENIntersectedPolygon(ENVector vPoly[],ENVector vLine[],ENint verticeCount)
{
 ENVector vNormal;
 ENfloat originDistance = 0;

 if(!ENIntersectedPlane(vPoly,vLine,vNormal,originDistance))
   return false;

 ENVector vIntersection = ENIntersectionPoint(vNormal, vLine, originDistance);

 if(ENInsidePolygon(vIntersection, vPoly, verticeCount))
   return true;

 return false;
}

ENbool ENIntersectedPolygon(ENVector vPoly[],ENVector vLine[],ENint verticeCount,ENVector &pos)
{
 ENVector vNormal;
 ENfloat originDistance = 0;

 if(!ENIntersectedPlane(vPoly,vLine,vNormal,originDistance))
   return false;

 pos = ENIntersectionPoint(vNormal, vLine, originDistance);

 if(ENInsidePolygon(pos, vPoly, verticeCount))
   return true;

 return false;
}

ENbool ENSpherePolygonCollision(ENVector vPoly[], 
							  ENVector &vCenter,ENint vertexCount,ENfloat radius)
{
 ENVector vNormal;
 if(!ENNormal(vPoly,vNormal)) return false;
 ENfloat distance = 0.0;
 ENint classification = ENClassifySphere(vCenter,vNormal,vPoly[0],radius,distance);
 if(classification==INTERSECTS) 
 {
  ENVector vOffset = vNormal*distance;
  ENVector vPosition = vCenter - vOffset;
  if(ENInsidePolygon(vPosition,vPoly,vertexCount))
    return true;
  else
  {
   if(ENEdgeSphereCollision(vCenter,vPoly,vertexCount,radius))
	 return true;
  }
 }
 
 return false;
}

ENVector ENVecDir(ENfloat range,ENVector2 Angles)
{
 //Vars
 ENVector diff;

 if(fmod(fabs(Angles.v[0]+90.0f),180.0f)!=0.0f)
 {
  //Convert angles from degrees to radians
  Angles.v[0]=ENRadians(Angles.v[0]);
  Angles.v[1]=ENRadians(Angles.v[1]);
  //Calculate new vector
  diff.v[0]=(ENfloat)sin(-Angles.v[1])*range*(ENfloat)cos(Angles.v[0]);
  diff.v[1]=(ENfloat)sin(Angles.v[0])*range;
  diff.v[2]=(ENfloat)cos(-Angles.v[1])*range*(ENfloat)cos(Angles.v[0]);
 }
 else
 {
  diff.v[0]=0.0f;
  diff.v[1]=ENSign(Angles.v[0]);
  diff.v[2]=0.0f;
 }
 //Add new vector and return result
 return diff;
}

ENVector2 ENPolar(ENVector vec)
{
 ENVector2 res;
 //Calculate length of vector
 float len=ENLength(vec);
 //Special cases 1
 if(len==0.0f) return ENVector2(0.0f,0.0f);
 if(len==vec.v[1]) return ENVector2(90.0f,0.0f);
 if(-len==vec.v[1]) return ENVector2(-90.0f,0.0f);
 //Calculate length of vector from top
 float lent=(float)sqrt(vec.v[0]*vec.v[0]+vec.v[2]*vec.v[2]);
 //Calculate X-Achse
 res.v[0]=(float)atan(vec.v[1]/lent);
 res.v[0]*=(float)(180.0/M_PI);
 //Calculate Y-Achse
 res.v[1]=(float)asin(vec.v[2]/lent);
 res.v[1]*=(float)(180.0/M_PI);
 res.v[1]+=90.0f;
 if(vec.v[0]<0.0f) res.v[1]=360.0f-res.v[1];
 //Return result
 return res;
}

void ENNormalizePlane(ENfloat frustum[6][4],ENint side)
{
	ENfloat magnitude = (ENfloat)sqrt( frustum[side][A] * frustum[side][A] + 
								   frustum[side][B] * frustum[side][B] + 
								   frustum[side][C] * frustum[side][C] );
        if(magnitude!=0.0f)
        {
         frustum[side][A] /= magnitude;
	 frustum[side][B] /= magnitude;
	 frustum[side][C] /= magnitude;
	 frustum[side][D] /= magnitude;
        }         
}

void ENCalculateFrustumPlane(ENVector *tri,ENEngineFrustum &vfrust,ENFrustumSide side)
{
 //Vars
 ENVector vNormal;
 //Calculate normal
 vNormal=ENNormal(tri);
 vfrust.Frustum[ENLEFT][A]=vNormal.v[0];
 vfrust.Frustum[ENLEFT][B]=vNormal.v[1];
 vfrust.Frustum[ENLEFT][C]=vNormal.v[2];
 vfrust.Frustum[ENLEFT][D]=ENDot(vNormal,tri[0]);
}

void ENCalculateFrustumPlanes(ENEngineFrustum &res)
{
 //Variables
 ENMatrix4X4                     proj,modl,clip;
 //Get OpenGL matrices
 glGetFloatv(GL_PROJECTION_MATRIX,&proj.val[0][0]);
 glGetFloatv(GL_MODELVIEW_MATRIX,&modl.val[0][0]);
 //Multiply matrices
 clip=modl*proj;
 //Calculate
 ENCalculateFrustumPlanes(res,&clip);
}

void ENCalculateFrustumPlanes(ENEngineFrustum &res,ENMatrix4X4 *matrix)
{
 //Variables
 ENMatrix4X4                     clip;
 //Set clip matrix
 clip=*matrix;
 //Right plane
 res.Frustum[ENRIGHT][A] = clip.val[0][3]-clip.val[0][0];
 res.Frustum[ENRIGHT][B] = clip.val[1][3]-clip.val[1][0];
 res.Frustum[ENRIGHT][C] = clip.val[2][3]-clip.val[2][0];
 res.Frustum[ENRIGHT][D] = clip.val[3][3]-clip.val[3][0];
 ENNormalizePlane(res.Frustum, ENRIGHT);
 //Left plane
 res.Frustum[ENLEFT][A] = clip.val[0][3]+clip.val[0][0];
 res.Frustum[ENLEFT][B] = clip.val[1][3]+clip.val[1][0];
 res.Frustum[ENLEFT][C] = clip.val[2][3]+clip.val[2][0];
 res.Frustum[ENLEFT][D] = clip.val[3][3]+clip.val[3][0];
 ENNormalizePlane(res.Frustum, ENLEFT);
 //Bottom plane
 res.Frustum[ENBOTTOM][A] = clip.val[0][3]+clip.val[0][1];
 res.Frustum[ENBOTTOM][B] = clip.val[1][3]+clip.val[1][1];
 res.Frustum[ENBOTTOM][C] = clip.val[2][3]+clip.val[2][1];
 res.Frustum[ENBOTTOM][D] = clip.val[3][3]+clip.val[3][1];
 ENNormalizePlane(res.Frustum, ENBOTTOM);
 //Top plane
 res.Frustum[ENTOP][A] = clip.val[0][3]-clip.val[0][1];
 res.Frustum[ENTOP][B] = clip.val[1][3]-clip.val[1][1];
 res.Frustum[ENTOP][C] = clip.val[2][3]-clip.val[2][1];
 res.Frustum[ENTOP][D] = clip.val[3][3]-clip.val[3][1];
 ENNormalizePlane(res.Frustum, ENTOP);
 //Back plane
 res.Frustum[ENBACK][A] = clip.val[0][3]-clip.val[0][2];
 res.Frustum[ENBACK][B] = clip.val[1][3]-clip.val[1][2];
 res.Frustum[ENBACK][C] = clip.val[2][3]-clip.val[2][2];
 res.Frustum[ENBACK][D] = clip.val[3][3]-clip.val[3][2];
 ENNormalizePlane(res.Frustum, ENBACK);
 //Front plane
 res.Frustum[ENFRONT][A] = clip.val[0][3]+clip.val[0][2];
 res.Frustum[ENFRONT][B] = clip.val[1][3]+clip.val[1][2];
 res.Frustum[ENFRONT][C] = clip.val[2][3]+clip.val[2][2];
 res.Frustum[ENFRONT][D] = clip.val[3][3]+clip.val[3][2];
 ENNormalizePlane(res.Frustum, ENFRONT);
}

bool ENPointInFrustum(ENfloat x,ENfloat y,ENfloat z,ENEngineFrustum *FrustumData)
{
	for(int i = 0; i < 6; i++ )
    	if(FrustumData->Frustum[i][A]*x+FrustumData->Frustum[i][B]*y+
           FrustumData->Frustum[i][C]*z+FrustumData->Frustum[i][D]<=0)
	  return false;


 return true;
}

bool ENSphereInFrustum(ENfloat x,ENfloat y,ENfloat z,ENfloat radius,ENEngineFrustum *FrustumData)
{
	for(int i = 0; i < 6; i++ )
		if( FrustumData->Frustum[i][A] * x + FrustumData->Frustum[i][B] * y + FrustumData->Frustum[i][C] * z + FrustumData->Frustum[i][D] <= -radius )
			return false;

		return true;
}

bool ENCubeInFrustum(ENfloat x,ENfloat y,ENfloat z,ENfloat size,ENEngineFrustum *FrustumData)
{
	for(int i = 0; i < 6; i++ )
	{
		if(FrustumData->Frustum[i][A] * (x - size) + FrustumData->Frustum[i][B] * (y - size) + FrustumData->Frustum[i][C] * (z - size) + FrustumData->Frustum[i][D] > 0)
		   continue;
		if(FrustumData->Frustum[i][A] * (x + size) + FrustumData->Frustum[i][B] * (y - size) + FrustumData->Frustum[i][C] * (z - size) + FrustumData->Frustum[i][D] > 0)
		   continue;
		if(FrustumData->Frustum[i][A] * (x - size) + FrustumData->Frustum[i][B] * (y + size) + FrustumData->Frustum[i][C] * (z - size) + FrustumData->Frustum[i][D] > 0)
		   continue;
		if(FrustumData->Frustum[i][A] * (x + size) + FrustumData->Frustum[i][B] * (y + size) + FrustumData->Frustum[i][C] * (z - size) + FrustumData->Frustum[i][D] > 0)
		   continue;
		if(FrustumData->Frustum[i][A] * (x - size) + FrustumData->Frustum[i][B] * (y - size) + FrustumData->Frustum[i][C] * (z + size) + FrustumData->Frustum[i][D] > 0)
		   continue;
		if(FrustumData->Frustum[i][A] * (x + size) + FrustumData->Frustum[i][B] * (y - size) + FrustumData->Frustum[i][C] * (z + size) + FrustumData->Frustum[i][D] > 0)
		   continue;
		if(FrustumData->Frustum[i][A] * (x - size) + FrustumData->Frustum[i][B] * (y + size) + FrustumData->Frustum[i][C] * (z + size) + FrustumData->Frustum[i][D] > 0)
		   continue;
		if(FrustumData->Frustum[i][A] * (x + size) + FrustumData->Frustum[i][B] * (y + size) + FrustumData->Frustum[i][C] * (z + size) + FrustumData->Frustum[i][D] > 0)
		   continue;

		return false;
	}

	return true;
}

bool ENBoxInFrustum(ENfloat x,ENfloat y,ENfloat z,ENfloat x2,ENfloat y2,ENfloat z2,ENEngineFrustum *FrustumData)
{
	for(int i = 0; i < 6; i++ )
	{
		if(FrustumData->Frustum[i][A] * x  + FrustumData->Frustum[i][B] * y  + FrustumData->Frustum[i][C] * z  + FrustumData->Frustum[i][D] > 0)  continue;
		if(FrustumData->Frustum[i][A] * x2 + FrustumData->Frustum[i][B] * y  + FrustumData->Frustum[i][C] * z  + FrustumData->Frustum[i][D] > 0)  continue;
		if(FrustumData->Frustum[i][A] * x  + FrustumData->Frustum[i][B] * y2 + FrustumData->Frustum[i][C] * z  + FrustumData->Frustum[i][D] > 0)  continue;
		if(FrustumData->Frustum[i][A] * x2 + FrustumData->Frustum[i][B] * y2 + FrustumData->Frustum[i][C] * z  + FrustumData->Frustum[i][D] > 0)  continue;
		if(FrustumData->Frustum[i][A] * x  + FrustumData->Frustum[i][B] * y  + FrustumData->Frustum[i][C] * z2 + FrustumData->Frustum[i][D] > 0)  continue;
		if(FrustumData->Frustum[i][A] * x2 + FrustumData->Frustum[i][B] * y  + FrustumData->Frustum[i][C] * z2 + FrustumData->Frustum[i][D] > 0)  continue;
		if(FrustumData->Frustum[i][A] * x  + FrustumData->Frustum[i][B] * y2 + FrustumData->Frustum[i][C] * z2 + FrustumData->Frustum[i][D] > 0)  continue;
		if(FrustumData->Frustum[i][A] * x2 + FrustumData->Frustum[i][B] * y2 + FrustumData->Frustum[i][C] * z2 + FrustumData->Frustum[i][D] > 0)  continue;

		return false;
	}

	return true;
}

