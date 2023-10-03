#include <stdlib.h>
#include <math.h>
#include "ENSysFuncs.h"
#include "ENScriptExtern.h"

void ENScriptSysFuncs::SysVEC2(ENubyte *Data,void *res)
{
 ENfloat x,y;
 ENVector2 tmp;
 memcpy(&y,Data-4,4); 
 memcpy(&x,Data-8,4);
 tmp=ENVector2(x,y);
 memcpy(res,&tmp,sizeof(ENVector2)); 
}

void ENScriptSysFuncs::SysVEC3(ENubyte *Data,void *res)
{
 ENfloat x,y,z;
 ENVector tmp;
 memcpy(&z,Data-4,4); 
 memcpy(&y,Data-8,4);
 memcpy(&x,Data-12,4);
 tmp=ENVector(x,y,z);
 memcpy(res,&tmp,sizeof(ENVector)); 
}

void ENScriptSysFuncs::SysVEC4(ENubyte *Data,void *res)
{
 ENfloat x,y,z,w;
 ENVector4 tmp;
 memcpy(&w,Data-4,4); 
 memcpy(&z,Data-8,4);
 memcpy(&y,Data-12,4);
 memcpy(&x,Data-16,4);
 tmp=ENVector4(x,y,z,w);
 memcpy(res,&tmp,sizeof(ENVector4)); 
}

void ENScriptSysFuncs::SysMAT2(ENubyte *Data,void *res)
{
 ENMatrix2X2 tmp; 
 memcpy(&tmp.val[0][0],Data-16,4); 
 memcpy(&tmp.val[0][1],Data-12,4);
 memcpy(&tmp.val[1][0],Data-8,4);
 memcpy(&tmp.val[1][1],Data-4,4);
 
 memcpy(res,&tmp,sizeof(ENMatrix2X2)); 
}

void ENScriptSysFuncs::SysMAT3(ENubyte *Data,void *res)
{
 ENMatrix3X3 tmp;
 memcpy(&tmp.val[0][0],Data-36,4); 
 memcpy(&tmp.val[0][1],Data-32,4);
 memcpy(&tmp.val[0][2],Data-28,4);

 memcpy(&tmp.val[1][0],Data-24,4); 
 memcpy(&tmp.val[1][1],Data-20,4);
 memcpy(&tmp.val[1][2],Data-16,4);

 memcpy(&tmp.val[2][0],Data-12,4); 
 memcpy(&tmp.val[2][1],Data-8,4);
 memcpy(&tmp.val[2][2],Data-4,4);
 
 memcpy(res,&tmp,sizeof(ENMatrix3X3)); 
}

void ENScriptSysFuncs::SysMAT4(ENubyte *Data,void *res)
{
 ENMatrix4X4 tmp;

 memcpy(&tmp.val[0][0],Data-64,4); 
 memcpy(&tmp.val[0][1],Data-60,4);
 memcpy(&tmp.val[0][2],Data-56,4);
 memcpy(&tmp.val[0][3],Data-52,4);

 memcpy(&tmp.val[1][0],Data-48,4); 
 memcpy(&tmp.val[1][1],Data-44,4);
 memcpy(&tmp.val[1][2],Data-40,4);
 memcpy(&tmp.val[1][3],Data-36,4);

 memcpy(&tmp.val[2][0],Data-32,4); 
 memcpy(&tmp.val[2][1],Data-28,4);
 memcpy(&tmp.val[2][2],Data-24,4);
 memcpy(&tmp.val[2][3],Data-20,4);

 memcpy(&tmp.val[3][0],Data-16,4); 
 memcpy(&tmp.val[3][1],Data-12,4);
 memcpy(&tmp.val[3][2],Data-8,4);
 memcpy(&tmp.val[3][3],Data-4,4);
 
 memcpy(res,&tmp,sizeof(ENMatrix4X4)); 
}

void ENScriptSysFuncs::SysVEC2_LEN(ENubyte *Data,void *res)
{
 ENVector2 vec;
 ENfloat  tmp;
 memcpy(&vec,Data-sizeof(ENVector2),sizeof(ENVector2));
 tmp=ENLength(vec);
 memcpy(res,&tmp,sizeof(ENfloat));
}

void ENScriptSysFuncs::SysVEC3_LEN(ENubyte *Data,void *res)
{
 ENVector vec;
 ENfloat  tmp;
 memcpy(&vec,Data-sizeof(ENVector),sizeof(ENVector));
 tmp=ENLength(vec);
 memcpy(res,&tmp,sizeof(ENfloat));
}

void ENScriptSysFuncs::SysVEC4_LEN(ENubyte *Data,void *res)
{
 ENVector4 vec;
 ENfloat  tmp;
 memcpy(&vec,Data-sizeof(ENVector4),sizeof(ENVector4));
 tmp=ENLength(vec);
 memcpy(res,&tmp,sizeof(ENfloat));
}

void ENScriptSysFuncs::SysVEC2_NORMALIZE(ENubyte *Data,void *res)
{
 ENVector2 vec;
 ENVector2  tmp;
 memcpy(&vec,Data-sizeof(ENVector2),sizeof(ENVector2));
 tmp=ENNormalize(vec);
 memcpy(res,&tmp,sizeof(ENVector2));
}

void ENScriptSysFuncs::SysVEC3_NORMALIZE(ENubyte *Data,void *res)
{
 ENVector vec;
 ENVector  tmp;
 memcpy(&vec,Data-sizeof(ENVector),sizeof(ENVector));
 tmp=ENNormalize(vec);
 memcpy(res,&tmp,sizeof(ENVector));
}

void ENScriptSysFuncs::SysVEC4_NORMALIZE(ENubyte *Data,void *res)
{
 ENVector4 vec;
 ENVector4  tmp;
 memcpy(&vec,Data-sizeof(ENVector4),sizeof(ENVector4));
 tmp=ENNormalize(vec);
 memcpy(res,&tmp,sizeof(ENVector4));
}   

void ENScriptSysFuncs::SysVEC2_DOT(ENubyte *Data,void *res)
{
 ENVector2 vec1;
 ENVector2 vec2;
 ENfloat  tmp;
 memcpy(&vec2,Data-sizeof(ENVector2),sizeof(ENVector2));
 memcpy(&vec1,Data-sizeof(ENVector2)*2,sizeof(ENVector2));
 tmp=ENDot(vec1,vec2);
 memcpy(res,&tmp,sizeof(ENfloat));
}

void ENScriptSysFuncs::SysVEC3_DOT(ENubyte *Data,void *res)
{
 ENVector vec1;
 ENVector vec2;
 ENfloat  tmp;
 memcpy(&vec2,Data-sizeof(ENVector),sizeof(ENVector));
 memcpy(&vec1,Data-sizeof(ENVector)*2,sizeof(ENVector));
 tmp=ENDot(vec1,vec2);
 memcpy(res,&tmp,sizeof(ENfloat));
}

void ENScriptSysFuncs::SysVEC4_DOT(ENubyte *Data,void *res)
{
 ENVector4 vec1;
 ENVector4 vec2;
 ENfloat   tmp;
 memcpy(&vec2,Data-sizeof(ENVector4),sizeof(ENVector4));
 memcpy(&vec1,Data-sizeof(ENVector4)*2,sizeof(ENVector4));
 tmp=ENDot(vec1,vec2);
 memcpy(res,&tmp,sizeof(ENfloat));
}

void ENScriptSysFuncs::SysVEC3_ANGLE(ENubyte *Data,void *res)
{
 //Get parameter
 ENVector A,B;
 memcpy(&B,Data-sizeof(ENVector),sizeof(ENVector)); 
 memcpy(&A,Data-sizeof(ENVector)*2,sizeof(ENVector));
 //Calculate angle
 ENVector2 C;
 C=ENPolar(B-A);
 //Return result
 memcpy(res,&C,sizeof(ENVector2));
}

void ENScriptSysFuncs::SysRANDOM(ENubyte *Data,void *res)
{
 ENint x;
 ENint  r;
 memcpy(&x,Data-4,4);

 r=rand()%x;

 memcpy(res,&r,sizeof(ENint));
}

void ENScriptSysFuncs::SysSIN(ENubyte *Data,void *res)
{
 ENfloat x;
 memcpy(&x,Data-4,4); 

 x=(ENfloat)sin(ENRadians(x));

 memcpy(res,&x,sizeof(ENfloat)); 
}

void ENScriptSysFuncs::SysCOS(ENubyte *Data,void *res)
{
 ENfloat x;
 memcpy(&x,Data-4,4); 

 x=(ENfloat)cos(ENRadians(x));

 memcpy(res,&x,sizeof(ENfloat)); 
}

void ENScriptSysFuncs::SysTAN(ENubyte *Data,void *res)
{
 ENfloat x;
 memcpy(&x,Data-4,4); 

 x=(ENfloat)tan(ENRadians(x));

 memcpy(res,&x,sizeof(ENfloat)); 
}

void ENScriptSysFuncs::SysASIN(ENubyte *Data,void *res)
{
 ENfloat x;
 memcpy(&x,Data-4,4); 

 x=(ENfloat)asin(x);
 x*=(ENfloat)(180.0/M_PI);

 memcpy(res,&x,sizeof(ENfloat)); 
}

void ENScriptSysFuncs::SysACOS(ENubyte *Data,void *res)
{
 ENfloat x;
 memcpy(&x,Data-4,4); 

 x=(ENfloat)acos(x);
 x*=(ENfloat)(180.0/M_PI);

 memcpy(res,&x,sizeof(ENfloat)); 
}

void ENScriptSysFuncs::SysATAN(ENubyte *Data,void *res)
{
 ENfloat x;
 memcpy(&x,Data-4,4); 

 x=(ENfloat)atan(x);
 x*=(ENfloat)(180.0/M_PI);

 memcpy(res,&x,sizeof(ENfloat)); 
}

ENbool ENScriptSysFuncs::Init()
{
 if(!ENScriptExtern::SetExtFunc("vec2",ENScriptSysFuncs::SysVEC2)) return false;
 if(!ENScriptExtern::SetExtFunc("vec3",ENScriptSysFuncs::SysVEC3)) return false;
 if(!ENScriptExtern::SetExtFunc("vec4",ENScriptSysFuncs::SysVEC4)) return false;
 if(!ENScriptExtern::SetExtFunc("mat2",ENScriptSysFuncs::SysMAT2)) return false;
 if(!ENScriptExtern::SetExtFunc("mat3",ENScriptSysFuncs::SysMAT3)) return false;
 if(!ENScriptExtern::SetExtFunc("mat4",ENScriptSysFuncs::SysMAT4)) return false;
 if(!ENScriptExtern::SetExtFunc("vec2_len",ENScriptSysFuncs::SysVEC2_LEN)) return false;
 if(!ENScriptExtern::SetExtFunc("vec3_len",ENScriptSysFuncs::SysVEC3_LEN)) return false;
 if(!ENScriptExtern::SetExtFunc("vec4_len",ENScriptSysFuncs::SysVEC4_LEN)) return false;
 if(!ENScriptExtern::SetExtFunc("vec2_normalize",ENScriptSysFuncs::SysVEC2_NORMALIZE)) return false;
 if(!ENScriptExtern::SetExtFunc("vec3_normalize",ENScriptSysFuncs::SysVEC3_NORMALIZE)) return false;
 if(!ENScriptExtern::SetExtFunc("vec4_normalize",ENScriptSysFuncs::SysVEC4_NORMALIZE)) return false;
 if(!ENScriptExtern::SetExtFunc("vec2_dot",ENScriptSysFuncs::SysVEC2_DOT)) return false;
 if(!ENScriptExtern::SetExtFunc("vec3_dot",ENScriptSysFuncs::SysVEC3_DOT)) return false;
 if(!ENScriptExtern::SetExtFunc("vec4_dot",ENScriptSysFuncs::SysVEC4_DOT)) return false;
 if(!ENScriptExtern::SetExtFunc("vec3_angle",ENScriptSysFuncs::SysVEC3_ANGLE)) return false;
 if(!ENScriptExtern::SetExtFunc("random",ENScriptSysFuncs::SysRANDOM)) return false;
 if(!ENScriptExtern::SetExtFunc("sin",ENScriptSysFuncs::SysSIN)) return false;
 if(!ENScriptExtern::SetExtFunc("cos",ENScriptSysFuncs::SysCOS)) return false;
 if(!ENScriptExtern::SetExtFunc("tan",ENScriptSysFuncs::SysTAN)) return false;
 if(!ENScriptExtern::SetExtFunc("asin",ENScriptSysFuncs::SysASIN)) return false;
 if(!ENScriptExtern::SetExtFunc("acos",ENScriptSysFuncs::SysACOS)) return false;
 if(!ENScriptExtern::SetExtFunc("atan",ENScriptSysFuncs::SysATAN)) return false;
 //Finished
 return true;
}