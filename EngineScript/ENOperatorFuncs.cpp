#include <math.h>
#include "ENOperatorFuncs.h"

ENint  ENScriptOperators::SizeInt;
ENint  ENScriptOperators::SizeFloat;
ENint  ENScriptOperators::SizeString;
ENint  ENScriptOperators::SizeVec2;
ENint  ENScriptOperators::SizeVec3;
ENint  ENScriptOperators::SizeVec4;
ENint  ENScriptOperators::SizeMat2;
ENint  ENScriptOperators::SizeMat3;
ENint  ENScriptOperators::SizeMat4;
//////////////////////////////////////////////////////
////Push and pop function for Operator-functions
//////////////////////////////////////////////////////

inline void ENScriptOperators::Push(ENubyte *ENStack,ENint &_ENStackPointer,
				    ENScriptPointer Src,ENint size)
{
 memcpy(ENStack+_ENStackPointer,Src,size);
 _ENStackPointer+=size;
}

inline void ENScriptOperators::Pop(ENubyte *ENStack,ENint &_ENStackPointer,
				   ENScriptPointer Dst,ENint size)
{
 memcpy(Dst,ENStack+_ENStackPointer-size,size);
 _ENStackPointer-=size;
}

//////////////////////////////////////////////////////
////Operator-functions(assign,'=')
//////////////////////////////////////////////////////
void ENScriptOperators::INTisFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENint b;
 //Get 
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 b=(ENint)a;
 //Set
 Push(ENStack,_ENStackPointer,&b,SizeInt);
}

void ENScriptOperators::INTisSTR(ENubyte *ENStack,ENint &_ENStackPointer)
{
 char a[1024];
 ENint b;
 //Get 
 Pop(ENStack,_ENStackPointer,a,SizeString); 
 //Operator 
 b=atoi(a);
 //Set
 Push(ENStack,_ENStackPointer,&b,SizeInt);
}

void ENScriptOperators::FLOATisINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENfloat b;
 //Get 
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 b=(ENfloat)a;
 //Set
 Push(ENStack,_ENStackPointer,&b,SizeFloat);
}

void ENScriptOperators::FLOATisSTR(ENubyte *ENStack,ENint &_ENStackPointer)
{
 char a[1024];
 ENfloat b;
 //Get 
 Pop(ENStack,_ENStackPointer,a,SizeString); 
 //Operator
 b=(ENfloat)atof(a);
 //Set
 Push(ENStack,_ENStackPointer,&b,SizeFloat);
}

void ENScriptOperators::STRisINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 char  b[1024];
 //Get 
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 sprintf(b,"%i",a);
 //Set
 Push(ENStack,_ENStackPointer,b,SizeString);
}

void ENScriptOperators::STRisFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 char  b[1024];
 //Get 
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 sprintf(b,"%f",a);
 //Set
 Push(ENStack,_ENStackPointer,b,SizeString);
}
//////////////////////////////////////////////////////
////Operator-functions(plus,'+')
//////////////////////////////////////////////////////
void ENScriptOperators::INTisINTplusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENint b;
 ENint c;
 //Get 
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=a+b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::FLOATisINTplusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint   a;
 ENfloat b;
 ENfloat c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(ENfloat)a+b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::STRisINTplusSTR(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 char b[1024];
 char c[1024];
 //Get
 Pop(ENStack,_ENStackPointer,b,SizeString);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 sprintf(c,"%i%s",a,b);
 //Set
 Push(ENStack,_ENStackPointer,c,SizeString);
}

void ENScriptOperators::FLOATisFLOATplusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENfloat b;
 ENfloat c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=a+b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::FLOATisFLOATplusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENint b;
 ENfloat c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=a+(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::STRisFLOATplusSTR(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 char b[1024];
 char c[1024];
 //Get
 Pop(ENStack,_ENStackPointer,b,SizeString);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 sprintf(c,"%f%s",a,b);
 //Set
 Push(ENStack,_ENStackPointer,c,SizeString);
}

void ENScriptOperators::VEC2isVEC2plusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector2 a;
 ENint     b;
 ENVector2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeVec2); 
 //Operator
 c=a+(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec2);
}

void ENScriptOperators::VEC2isVEC2plusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector2 a;
 ENfloat   b;
 ENVector2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeVec2); 
 //Operator
 c=a+b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec2);
}

void ENScriptOperators::VEC2isVEC2plusVEC2(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector2 a;
 ENVector2 b;
 ENVector2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec2);
 Pop(ENStack,_ENStackPointer,&a,SizeVec2); 
 //Operator
 c=a+b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec2);
}

void ENScriptOperators::VEC3isVEC3plusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector a;
 ENint    b;
 ENVector c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeVec3); 
 //Operator
 c=a+(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec3);
}

void ENScriptOperators::VEC3isVEC3plusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector  a;
 ENfloat   b;
 ENVector  c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeVec3); 
 //Operator
 c=a+b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec3);
}

void ENScriptOperators::VEC3isVEC3plusVEC3(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector a;
 ENVector b;
 ENVector c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec3);
 Pop(ENStack,_ENStackPointer,&a,SizeVec3); 
 //Operator
 c=a+b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec3);
}

void ENScriptOperators::VEC4isVEC4plusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector4 a;
 ENint     b;
 ENVector4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeVec4); 
 //Operator
 c=a+(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec4);
}

void ENScriptOperators::VEC4isVEC4plusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector4  a;
 ENfloat    b;
 ENVector4  c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeVec4); 
 //Operator
 c=a+b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec4);
}

void ENScriptOperators::VEC4isVEC4plusVEC4(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector4 a;
 ENVector4 b;
 ENVector4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec4);
 Pop(ENStack,_ENStackPointer,&a,SizeVec4); 
 //Operator
 c=a+b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec4);
}

void ENScriptOperators::STRisSTRplusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 char a[1024];
 ENint      b;
 char c[1024];
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,a,SizeString); 
 //Operator
 sprintf(c,"%s%i",a,b);
 //Set
 Push(ENStack,_ENStackPointer,c,SizeString);
}

void ENScriptOperators::STRisSTRplusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 char a[1024];
 ENfloat    b;
 char c[1024];
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,a,SizeString); 
 //Operator
 sprintf(c,"%s%f",a,b);
 //Set
 Push(ENStack,_ENStackPointer,c,SizeString);
}

void ENScriptOperators::STRisSTRplusSTR(ENubyte *ENStack,ENint &_ENStackPointer)
{
 char a[1024];
 char b[1024];
 char c[1024];
 //Get
 Pop(ENStack,_ENStackPointer,b,SizeString);
 Pop(ENStack,_ENStackPointer,a,SizeString); 
 //Operator
 sprintf(c,"%s%s",a,b);
 //Set
 Push(ENStack,_ENStackPointer,c,SizeString);
}

void ENScriptOperators::MAT2isMAT2plusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix2X2 a;
 ENint       b;
 ENMatrix2X2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeMat2); 
 //Operator
 c=a+(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat2);
}

void ENScriptOperators::MAT2isMAT2plusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix2X2 a;
 ENfloat     b;
 ENMatrix2X2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeMat2); 
 //Operator
 c=a+b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat2);
}

void ENScriptOperators::MAT2isMAT2plusMAT2(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix2X2 a;
 ENMatrix2X2 b;
 ENMatrix2X2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeMat2);
 Pop(ENStack,_ENStackPointer,&a,SizeMat2); 
 //Operator
 c=a+b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat2);
}

void ENScriptOperators::MAT3isMAT3plusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix3X3 a;
 ENint       b;
 ENMatrix3X3 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeMat3); 
 //Operator
 c=a+(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat3);
}

void ENScriptOperators::MAT3isMAT3plusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix3X3 a;
 ENfloat     b;
 ENMatrix3X3 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeMat3); 
 //Operator
 c=a+b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat3);
}

void ENScriptOperators::MAT3isMAT3plusMAT3(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix3X3 a;
 ENMatrix3X3 b;
 ENMatrix3X3 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeMat3);
 Pop(ENStack,_ENStackPointer,&a,SizeMat3); 
 //Operator
 c=a+b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat3);
}

void ENScriptOperators::MAT4isMAT4plusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix4X4 a;
 ENint       b;
 ENMatrix4X4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeMat4); 
 //Operator
 c=a+(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat4);
}

void ENScriptOperators::MAT4isMAT4plusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix4X4 a;
 ENfloat     b;
 ENMatrix4X4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeMat4); 
 //Operator
 c=a+b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat4);
}

void ENScriptOperators::MAT4isMAT4plusMAT4(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix4X4 a;
 ENMatrix4X4 b;
 ENMatrix4X4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeMat4);
 Pop(ENStack,_ENStackPointer,&a,SizeMat4); 
 //Operator
 c=a+b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat4);
}
//////////////////////////////////////////////////////
////Operator-functions(minus ,'-')
//////////////////////////////////////////////////////
void ENScriptOperators::INTisINTminusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=a-b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::FLOATisINTminusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint   a;
 ENfloat b;
 ENfloat c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(ENfloat)a-b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::FLOATisFLOATminusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENfloat b;
 ENfloat c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=a-b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::FLOATisFLOATminusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENint b;
 ENfloat c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=a-(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::VEC2isVEC2minusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector2 a;
 ENint     b;
 ENVector2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeVec2); 
 //Operator
 c=a-(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec2);
}

void ENScriptOperators::VEC2isVEC2minusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector2 a;
 ENfloat   b;
 ENVector2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeVec2); 
 //Operator
 c=a-b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec2);
}

void ENScriptOperators::VEC2isVEC2minusVEC2(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector2 a;
 ENVector2 b;
 ENVector2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec2);
 Pop(ENStack,_ENStackPointer,&a,SizeVec2); 
 //Operator
 c=a-b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec2);
}

void ENScriptOperators::VEC3isVEC3minusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector a;
 ENint    b;
 ENVector c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeVec3); 
 //Operator
 c=a-(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec3);
}

void ENScriptOperators::VEC3isVEC3minusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector  a;
 ENfloat   b;
 ENVector  c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeVec3); 
 //Operator
 c=a-b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec3);
}

void ENScriptOperators::VEC3isVEC3minusVEC3(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector a;
 ENVector b;
 ENVector c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec3);
 Pop(ENStack,_ENStackPointer,&a,SizeVec3); 
 //Operator
 c=a-b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec3);
}

void ENScriptOperators::VEC4isVEC4minusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector4 a;
 ENint     b;
 ENVector4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeVec4); 
 //Operator
 c=a-(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec4);
}

void ENScriptOperators::VEC4isVEC4minusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector4  a;
 ENfloat    b;
 ENVector4  c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeVec4); 
 //Operator
 c=a-b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec4);
}

void ENScriptOperators::VEC4isVEC4minusVEC4(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector4 a;
 ENVector4 b;
 ENVector4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec4);
 Pop(ENStack,_ENStackPointer,&a,SizeVec4); 
 //Operator
 c=a-b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec4);
}

void ENScriptOperators::MAT2isMAT2minusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix2X2 a;
 ENint       b;
 ENMatrix2X2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeMat2); 
 //Operator
 c=a-(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat2);
}

void ENScriptOperators::MAT2isMAT2minusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix2X2 a;
 ENfloat     b;
 ENMatrix2X2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeMat2); 
 //Operator
 c=a-b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat2);
}

void ENScriptOperators::MAT2isMAT2minusMAT2(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix2X2 a;
 ENMatrix2X2 b;
 ENMatrix2X2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeMat2);
 Pop(ENStack,_ENStackPointer,&a,SizeMat2); 
 //Operator
 c=a-b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat2);
}

void ENScriptOperators::MAT3isMAT3minusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix3X3 a;
 ENint       b;
 ENMatrix3X3 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeMat3); 
 //Operator
 c=a-(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat3);
}

void ENScriptOperators::MAT3isMAT3minusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix3X3 a;
 ENfloat     b;
 ENMatrix3X3 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeMat3); 
 //Operator
 c=a-b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat3);
}

void ENScriptOperators::MAT3isMAT3minusMAT3(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix3X3 a;
 ENMatrix3X3 b;
 ENMatrix3X3 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeMat3);
 Pop(ENStack,_ENStackPointer,&a,SizeMat3); 
 //Operator
 c=a-b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat3);
}

void ENScriptOperators::MAT4isMAT4minusINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix4X4 a;
 ENint       b;
 ENMatrix4X4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeMat4); 
 //Operator
 c=a-(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat4);
}

void ENScriptOperators::MAT4isMAT4minusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix4X4 a;
 ENfloat     b;
 ENMatrix4X4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeMat4); 
 //Operator
 c=a-b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat4);
}

void ENScriptOperators::MAT4isMAT4minusMAT4(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix4X4 a;
 ENMatrix4X4 b;
 ENMatrix4X4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeMat4);
 Pop(ENStack,_ENStackPointer,&a,SizeMat4); 
 //Operator
 c=a-b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat4);
}

//////////////////////////////////////////////////////
////Operator-functions(mul ,'*')
//////////////////////////////////////////////////////
void ENScriptOperators::INTisINTmulINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=a*b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::FLOATisINTmulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint   a;
 ENfloat b;
 ENfloat c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(ENfloat)a*b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::FLOATisFLOATmulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENfloat b;
 ENfloat c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=a*b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::FLOATisFLOATmulINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENint b;
 ENfloat c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=a*(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::VEC2isVEC2mulINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector2 a;
 ENint     b;
 ENVector2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeVec2); 
 //Operator
 c=a*(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec2);
}

void ENScriptOperators::VEC2isVEC2mulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector2 a;
 ENfloat   b;
 ENVector2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeVec2); 
 //Operator
 c=a*b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec2);
}

void ENScriptOperators::FLOATisVEC2mulVEC2(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector2 a;
 ENVector2 b;
 ENfloat   c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec2);
 Pop(ENStack,_ENStackPointer,&a,SizeVec2); 
 //Operator
 c=ENDot(a,b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::VEC3isVEC3mulINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector a;
 ENint    b;
 ENVector c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeVec3); 
 //Operator
 c=a*(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec3);
}

void ENScriptOperators::VEC3isVEC3mulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector  a;
 ENfloat   b;
 ENVector  c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeVec3); 
 //Operator
 c=a*b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec3);
}

void ENScriptOperators::FLOATisVEC3mulVEC3(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector a;
 ENVector b;
 ENfloat  c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec3);
 Pop(ENStack,_ENStackPointer,&a,SizeVec3); 
 //Operator
 c=ENDot(a,b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::VEC4isVEC4mulINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector4 a;
 ENint     b;
 ENVector4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeVec4); 
 //Operator
 c=a*(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec4);
}

void ENScriptOperators::VEC4isVEC4mulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector4  a;
 ENfloat    b;
 ENVector4  c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeVec4); 
 //Operator
 c=a*b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec4);
}

void ENScriptOperators::FLOATisVEC4mulVEC4(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector4 a;
 ENVector4 b;
 ENfloat   c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec4);
 Pop(ENStack,_ENStackPointer,&a,SizeVec4); 
 //Operator
 c=ENDot(a,b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::MAT2isMAT2mulINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix2X2 a;
 ENint       b;
 ENMatrix2X2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeMat2); 
 //Operator
 c=a*(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat2);
}

void ENScriptOperators::MAT2isMAT2mulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix2X2 a;
 ENfloat     b;
 ENMatrix2X2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeMat2); 
 //Operator
 c=a*b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat2);
}

void ENScriptOperators::VEC2isMAT2mulVEC2(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix2X2 a;
 ENVector2   b;
 ENVector2   c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec2);
 Pop(ENStack,_ENStackPointer,&a,SizeMat2); 
 //Operator
 c=a*b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec2);
}

void ENScriptOperators::MAT2isMAT2mulMAT2(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix2X2 a;
 ENMatrix2X2 b;
 ENMatrix2X2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeMat2);
 Pop(ENStack,_ENStackPointer,&a,SizeMat2); 
 //Operator
 c=a*b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat2);
}

void ENScriptOperators::MAT3isMAT3mulINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix3X3 a;
 ENint       b;
 ENMatrix3X3 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeMat3); 
 //Operator
 c=a*(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat3);
}

void ENScriptOperators::MAT3isMAT3mulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix3X3 a;
 ENfloat     b;
 ENMatrix3X3 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeMat3); 
 //Operator
 c=a*b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat3);
}

void ENScriptOperators::VEC3isMAT3mulVEC3(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix3X3 a;
 ENVector   b;
 ENVector   c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec3);
 Pop(ENStack,_ENStackPointer,&a,SizeMat3); 
 //Operator
 c=a*b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec3);
}

void ENScriptOperators::MAT3isMAT3mulMAT3(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix3X3 a;
 ENMatrix3X3 b;
 ENMatrix3X3 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeMat3);
 Pop(ENStack,_ENStackPointer,&a,SizeMat3); 
 //Operator
 c=a*b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat3);
}

void ENScriptOperators::MAT4isMAT4mulINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix4X4 a;
 ENint       b;
 ENMatrix4X4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeMat4); 
 //Operator
 c=a*(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat4);
}

void ENScriptOperators::MAT4isMAT4mulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix4X4 a;
 ENfloat     b;
 ENMatrix4X4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeMat4); 
 //Operator
 c=a*b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat4);
}

void ENScriptOperators::VEC4isMAT4mulVEC4(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix4X4 a;
 ENVector4   b;
 ENVector4   c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec4);
 Pop(ENStack,_ENStackPointer,&a,SizeMat4); 
 //Operator
 c=a*b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec4);
}

void ENScriptOperators::MAT4isMAT4mulMAT4(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix4X4 a;
 ENMatrix4X4 b;
 ENMatrix4X4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeMat4);
 Pop(ENStack,_ENStackPointer,&a,SizeMat4); 
 //Operator
 c=a*b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat4);
}

//////////////////////////////////////////////////////
////Operator-functions(div ,'/')
//////////////////////////////////////////////////////
void ENScriptOperators::INTisINTdivINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=a/b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::FLOATisINTdivFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint   a;
 ENfloat b;
 ENfloat c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(ENfloat)a/b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::FLOATisFLOATdivFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENfloat b;
 ENfloat c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=a/b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::FLOATisFLOATdivINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENint b;
 ENfloat c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=a/(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::VEC2isVEC2divINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector2 a;
 ENint     b;
 ENVector2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeVec2); 
 //Operator
 c=a/(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec2);
}

void ENScriptOperators::VEC2isVEC2divFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector2 a;
 ENfloat   b;
 ENVector2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeVec2); 
 //Operator
 c=a/b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec2);
}

void ENScriptOperators::VEC3isVEC3divINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector a;
 ENint    b;
 ENVector c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeVec3); 
 //Operator
 c=a/(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec3);
}

void ENScriptOperators::VEC3isVEC3divFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector  a;
 ENfloat   b;
 ENVector  c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeVec3); 
 //Operator
 c=a/b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec3);
}

void ENScriptOperators::VEC4isVEC4divINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector4 a;
 ENint     b;
 ENVector4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeVec4); 
 //Operator
 c=a/(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec4);
}

void ENScriptOperators::VEC4isVEC4divFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector4  a;
 ENfloat    b;
 ENVector4  c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeVec4); 
 //Operator
 c=a/b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeVec4);
}

void ENScriptOperators::MAT2isMAT2divINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix2X2 a;
 ENint       b;
 ENMatrix2X2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeMat2); 
 //Operator
 c=a/(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat2);
}

void ENScriptOperators::MAT2isMAT2divFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix2X2 a;
 ENfloat     b;
 ENMatrix2X2 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeMat2); 
 //Operator
 c=a/b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat2);
}

void ENScriptOperators::MAT3isMAT3divINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix3X3 a;
 ENint       b;
 ENMatrix3X3 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeMat3); 
 //Operator
 c=a/(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat3);
}

void ENScriptOperators::MAT3isMAT3divFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix3X3 a;
 ENfloat     b;
 ENMatrix3X3 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeMat3); 
 //Operator
 c=a/b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat3);
}

void ENScriptOperators::MAT4isMAT4divINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix4X4 a;
 ENint       b;
 ENMatrix4X4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeMat4); 
 //Operator
 c=a/(ENfloat)b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat4);
}

void ENScriptOperators::MAT4isMAT4divFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix4X4 a;
 ENfloat     b;
 ENMatrix4X4 c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeMat4); 
 //Operator
 c=a/b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeMat4);
}

//////////////////////////////////////////////////////
////Operator-functions(mod ,'%')
//////////////////////////////////////////////////////
void ENScriptOperators::INTisINTmodINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=a%b;
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::FLOATisINTmodFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint   a;
 ENfloat b;
 ENfloat c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator 
 c=(ENfloat)fmod((double)a,(double)b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::FLOATisFLOATmodFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENfloat b;
 ENfloat c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(ENfloat)fmod((double)a,(double)b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

void ENScriptOperators::FLOATisFLOATmodINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENint b;
 ENfloat c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(ENfloat)fmod((double)a,(double)b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeFloat);
}

//////////////////////////////////////////////////////
////Operator-functions(compare ,'=')
//////////////////////////////////////////////////////
void ENScriptOperators::INTisINTcmpINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(ENint)(a==b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisINTcmpFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint   a;
 ENfloat b;
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(ENint)((ENfloat)a==b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisINTcmpSTR(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint   a;
 char    b[1024];
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,b,SizeString);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(ENint)(a==atoi(b));
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisFLOATcmpINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENint   b;
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(ENint)(a==(ENfloat)b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisFLOATcmpFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENfloat b;
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(ENint)(a==b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisFLOATcmpSTR(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 char    b[1024];
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,b,SizeString);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(ENint)(a==atof(b));
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisVEC2cmpVEC2(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector2 a;
 ENVector2 b;
 ENint     c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec2);
 Pop(ENStack,_ENStackPointer,&a,SizeVec2); 
 //Operator
 c=(ENint)(a==b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisVEC3cmpVEC3(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector a;
 ENVector b;
 ENint     c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec3);
 Pop(ENStack,_ENStackPointer,&a,SizeVec3); 
 //Operator
 c=(ENint)(a==b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisVEC4cmpVEC4(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector4 a;
 ENVector4 b;
 ENint     c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec4);
 Pop(ENStack,_ENStackPointer,&a,SizeVec4); 
 //Operator
 c=(ENint)(a==b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisMAT2cmpMAT2(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix2X2 a;
 ENMatrix2X2 b;
 ENint     c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeMat2);
 Pop(ENStack,_ENStackPointer,&a,SizeMat2); 
 //Operator
 c=(ENint)(a.val[0][0]==b.val[0][0]&&
	       a.val[1][0]==b.val[1][0]&&
		   a.val[0][1]==b.val[0][1]&&
		   a.val[1][1]==b.val[1][1]);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisMAT3cmpMAT3(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix3X3 a;
 ENMatrix3X3 b;
 ENint     c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeMat3);
 Pop(ENStack,_ENStackPointer,&a,SizeMat3); 
 //Operator
 c=(ENint)(a.val[0][0]==b.val[0][0]&&
	       a.val[1][0]==b.val[1][0]&&
		   a.val[2][0]==b.val[2][0]&&
		   a.val[0][1]==b.val[0][1]&&
	       a.val[1][1]==b.val[1][1]&&
		   a.val[2][1]==b.val[2][1]&&
		   a.val[0][2]==b.val[0][2]&&
	       a.val[1][2]==b.val[1][2]&&
		   a.val[2][2]==b.val[2][2]);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisMAT4cmpMAT4(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix4X4 a;
 ENMatrix4X4 b;
 ENint     c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeMat4);
 Pop(ENStack,_ENStackPointer,&a,SizeMat4); 
 //Operator
 c=(ENint)(a.val[0][0]==b.val[0][0]&&
	       a.val[1][0]==b.val[1][0]&&
		   a.val[2][0]==b.val[2][0]&&
           a.val[3][0]==b.val[3][0]&&
		   a.val[0][1]==b.val[0][1]&&
	       a.val[1][1]==b.val[1][1]&&
		   a.val[2][1]==b.val[2][1]&&
		   a.val[3][1]==b.val[3][1]&&
		   a.val[0][2]==b.val[0][2]&&
	       a.val[1][2]==b.val[1][2]&&
		   a.val[2][2]==b.val[2][2]&&
		   a.val[3][2]==b.val[3][2]&&
		   a.val[0][3]==b.val[0][3]&&
	       a.val[1][3]==b.val[1][3]&&
		   a.val[2][3]==b.val[2][3]&&
		   a.val[3][3]==b.val[3][3]);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisSTRcmpINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 char    a[1024];
 ENint   b;
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,a,SizeString); 
 //Operator
 c=(ENint)(atoi(a)==b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisSTRcmpFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 char    a[1024];
 ENfloat b;
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,a,SizeString); 
 //Operator
 c=(ENint)(atof(a)==b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisSTRcmpSTR(ENubyte *ENStack,ENint &_ENStackPointer)
{
 char    a[1024];
 char    b[1024];
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,b,SizeString);
 Pop(ENStack,_ENStackPointer,a,SizeString); 
 //Operator
 c=(ENint)(strcmp(a,b)==0);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}
//////////////////////////////////////////////////////
////Operator-functions(not compare ,'!')
//////////////////////////////////////////////////////
void ENScriptOperators::INTisINTncmpINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(ENint)(a!=b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisINTncmpFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint   a;
 ENfloat b;
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(ENint)((ENfloat)a!=b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisINTncmpSTR(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint   a;
 char    b[1024];
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,b,SizeString);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(ENint)(a!=atoi(b));
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisFLOATncmpINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENint   b;
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(ENint)(a!=(ENfloat)b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisFLOATncmpFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENfloat b;
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(ENint)(a!=b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisFLOATncmpSTR(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 char    b[1024];
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,b,SizeString);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(ENint)(a!=atof(b));
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisVEC2ncmpVEC2(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector2 a;
 ENVector2 b;
 ENint     c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec2);
 Pop(ENStack,_ENStackPointer,&a,SizeVec2); 
 //Operator
 c=(ENint)(a!=b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisVEC3ncmpVEC3(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector a;
 ENVector b;
 ENint     c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec3);
 Pop(ENStack,_ENStackPointer,&a,SizeVec3); 
 //Operator
 c=(ENint)(a!=b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisVEC4ncmpVEC4(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENVector4 a;
 ENVector4 b;
 ENint     c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeVec4);
 Pop(ENStack,_ENStackPointer,&a,SizeVec4); 
 //Operator
 c=(ENint)(a!=b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisMAT2ncmpMAT2(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix2X2 a;
 ENMatrix2X2 b;
 ENint     c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeMat2);
 Pop(ENStack,_ENStackPointer,&a,SizeMat2); 
 //Operator
 c=(ENint)!(a.val[0][0]==b.val[0][0]&&
	       a.val[1][0]==b.val[1][0]&&
		   a.val[0][1]==b.val[0][1]&&
		   a.val[1][1]==b.val[1][1]);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisMAT3ncmpMAT3(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix3X3 a;
 ENMatrix3X3 b;
 ENint     c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeMat3);
 Pop(ENStack,_ENStackPointer,&a,SizeMat3); 
 //Operator
 c=(ENint)!(a.val[0][0]==b.val[0][0]&&
	       a.val[1][0]==b.val[1][0]&&
		   a.val[2][0]==b.val[2][0]&&
		   a.val[0][1]==b.val[0][1]&&
	       a.val[1][1]==b.val[1][1]&&
		   a.val[2][1]==b.val[2][1]&&
		   a.val[0][2]==b.val[0][2]&&
	       a.val[1][2]==b.val[1][2]&&
		   a.val[2][2]==b.val[2][2]);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisMAT4ncmpMAT4(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENMatrix4X4 a;
 ENMatrix4X4 b;
 ENint     c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeMat4);
 Pop(ENStack,_ENStackPointer,&a,SizeMat4); 
 //Operator
 c=(ENint)!(a.val[0][0]==b.val[0][0]&&
	       a.val[1][0]==b.val[1][0]&&
		   a.val[2][0]==b.val[2][0]&&
           a.val[3][0]==b.val[3][0]&&
		   a.val[0][1]==b.val[0][1]&&
	       a.val[1][1]==b.val[1][1]&&
		   a.val[2][1]==b.val[2][1]&&
		   a.val[3][1]==b.val[3][1]&&
		   a.val[0][2]==b.val[0][2]&&
	       a.val[1][2]==b.val[1][2]&&
		   a.val[2][2]==b.val[2][2]&&
		   a.val[3][2]==b.val[3][2]&&
		   a.val[0][3]==b.val[0][3]&&
	       a.val[1][3]==b.val[1][3]&&
		   a.val[2][3]==b.val[2][3]&&
		   a.val[3][3]==b.val[3][3]);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisSTRncmpINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 char    a[1024];
 ENint   b;
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,a,SizeString); 
 //Operator
 c=(ENint)(atoi(a)!=b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisSTRncmpFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 char    a[1024];
 ENfloat b;
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,a,SizeString); 
 //Operator
 c=(ENint)(atof(a)!=b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisSTRncmpSTR(ENubyte *ENStack,ENint &_ENStackPointer)
{
 char    a[1024];
 char    b[1024];
 ENint   c;
 //Get
 Pop(ENStack,_ENStackPointer,b,SizeString);
 Pop(ENStack,_ENStackPointer,a,SizeString); 
 //Operator
 c=(ENint)(strcmp(a,b)!=0);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}
//////////////////////////////////////////////////////
////Operator-functions(greater,'>')
//////////////////////////////////////////////////////
void ENScriptOperators::INTisINTgreaterINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(a>b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisINTgreaterFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENfloat b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=((ENfloat)a>b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisFLOATgreaterINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(a>(ENfloat)b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisFLOATgreaterFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENfloat b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(a>b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}
//////////////////////////////////////////////////////
////Operator-functions(greater same,'>=')
//////////////////////////////////////////////////////
void ENScriptOperators::INTisINTgreatersameINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(a>=b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisINTgreatersameFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENfloat b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=((ENfloat)a>=b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisFLOATgreatersameINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(a>=(ENfloat)b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisFLOATgreatersameFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENfloat b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(a>=b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}
//////////////////////////////////////////////////////
////Operator-functions(smaller,'<')
//////////////////////////////////////////////////////
void ENScriptOperators::INTisINTsmallerINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(a<b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisINTsmallerFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENfloat b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=((ENfloat)a<b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisFLOATsmallerINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(a<(ENfloat)b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisFLOATsmallerFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENfloat b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(a<b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}
//////////////////////////////////////////////////////
////Operator-functions(smaller same,'<=')
//////////////////////////////////////////////////////
void ENScriptOperators::INTisINTsmallersameINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(a<=b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisINTsmallersameFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENfloat b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=((ENfloat)a<=b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisFLOATsmallersameINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(a<=(ENfloat)b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}

void ENScriptOperators::INTisFLOATsmallersameFLOAT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENfloat a;
 ENfloat b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeFloat);
 Pop(ENStack,_ENStackPointer,&a,SizeFloat); 
 //Operator
 c=(a<=b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}
//////////////////////////////////////////////////////
////Operator-functions(and,'&')
//////////////////////////////////////////////////////

void ENScriptOperators::INTisINTandINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(a&&b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}
//////////////////////////////////////////////////////
////Operator-functions(or,'||')
//////////////////////////////////////////////////////
void ENScriptOperators::INTisINTorINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeInt); 
 //Operator
 c=(a||b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}
//////////////////////////////////////////////////////
////Operator-functions(bit or,'|')
//////////////////////////////////////////////////////
void ENScriptOperators::INTisINTbitorINT(ENubyte *ENStack,ENint &_ENStackPointer)
{
 ENint a;
 ENint b;
 ENint c;
 //Get
 Pop(ENStack,_ENStackPointer,&b,SizeInt);
 Pop(ENStack,_ENStackPointer,&a,SizeInt);
 //Operator
 c=(a|b);
 //Set
 Push(ENStack,_ENStackPointer,&c,SizeInt);
}
//////////////////////REGISTER OPERATORS///////////////////////////
ENbool ENScriptOperators::RegisterOperators()
{
 //ASSIGN
 //Assign int=float
 if(!ENScriptTypeClass::AddOperator(NULL,"int","float","=",INTisFLOAT)) return false;
 //Assign int=string
 if(!ENScriptTypeClass::AddOperator(NULL,"int","string","=",INTisSTR)) return false;    

 //Assign float=int
 if(!ENScriptTypeClass::AddOperator(NULL,"float","int","=",FLOATisINT)) return false;    
 //Assign float=string
 if(!ENScriptTypeClass::AddOperator(NULL,"float","string","=",FLOATisSTR)) return false;    

 //Assign string=int
 if(!ENScriptTypeClass::AddOperator(NULL,"string","int","=",STRisINT)) return false;    
 //Assign string=float
 if(!ENScriptTypeClass::AddOperator(NULL,"string","float","=",STRisFLOAT)) return false;    
 //PLUS
 //Add int = int+int     
 if(!ENScriptTypeClass::AddOperator("int","int","int","+",INTisINTplusINT)) return false;    
 //Add float = int+float  
 if(!ENScriptTypeClass::AddOperator("float","int","float","+",FLOATisINTplusFLOAT)) return false;     
 //Add string = int+string
 if(!ENScriptTypeClass::AddOperator("string","int","string","+",STRisINTplusSTR)) return false;

 //Add float = float+int  
 if(!ENScriptTypeClass::AddOperator("float","float","int","+",FLOATisFLOATplusINT)) return false;     
 //Add float = float+float     
 if(!ENScriptTypeClass::AddOperator("float","float","float","+",FLOATisFLOATplusFLOAT)) return false;    
 //Add string = float+string  
 if(!ENScriptTypeClass::AddOperator("string","float","string","+",STRisFLOATplusSTR)) return false;

 //Add vec2 = vec2+int
 if(!ENScriptTypeClass::AddOperator("vector2","vector2","int","+",VEC2isVEC2plusINT)) return false;    
 //Add vec2 = vec2+float
 if(!ENScriptTypeClass::AddOperator("vector2","vector2","float","+",VEC2isVEC2plusFLOAT)) return false;    
 //Add vec2 = vec2+vec2
 if(!ENScriptTypeClass::AddOperator("vector2","vector2","vector2","+",VEC2isVEC2plusVEC2)) return false;    

 //Add vec3 = vec3+int
 if(!ENScriptTypeClass::AddOperator("vector3","vector3","int","+",VEC3isVEC3plusINT)) return false;    
 //Add vec3 = vec3+float
 if(!ENScriptTypeClass::AddOperator("vector3","vector3","float","+",VEC3isVEC3plusFLOAT)) return false;    
 //Add vec3 = vec3+vec3
 if(!ENScriptTypeClass::AddOperator("vector3","vector3","vector3","+",VEC3isVEC3plusVEC3)) return false;    

 //Add vec4 = vec4+int
 if(!ENScriptTypeClass::AddOperator("vector4","vector4","int","+",VEC4isVEC4plusINT)) return false;    
 //Add vec4 = vec4+float
 if(!ENScriptTypeClass::AddOperator("vector4","vector4","float","+",VEC4isVEC4plusFLOAT)) return false;    
 //Add vec4 = vec4+vec4
 if(!ENScriptTypeClass::AddOperator("vector4","vector4","vector4","+",VEC4isVEC4plusVEC4)) return false;    

 //Add string = string+int
 if(!ENScriptTypeClass::AddOperator("string","string","int","+",STRisSTRplusINT)) return false;    
 //Add string = string+float
 if(!ENScriptTypeClass::AddOperator("string","string","float","+",STRisSTRplusFLOAT)) return false;    
 //Add string = string+string
 if(!ENScriptTypeClass::AddOperator("string","string","string","+",STRisSTRplusSTR)) return false;    

 //Add mat2 = mat2+int
 if(!ENScriptTypeClass::AddOperator("matrix2","matrix2","int","+",MAT2isMAT2plusINT)) return false;    
 //Add mat2 = mat2+float
 if(!ENScriptTypeClass::AddOperator("matrix2","matrix2","float","+",MAT2isMAT2plusFLOAT)) return false;    
 //Add mat2 = mat2+mat2
 if(!ENScriptTypeClass::AddOperator("matrix2","matrix2","matrix2","+",MAT2isMAT2plusMAT2)) return false;    

 //Add mat3 = mat3+int
 if(!ENScriptTypeClass::AddOperator("matrix3","matrix3","int","+",MAT3isMAT3plusINT)) return false;    
 //Add mat3 = mat3+float
 if(!ENScriptTypeClass::AddOperator("matrix3","matrix3","float","+",MAT3isMAT3plusFLOAT)) return false;    
 //Add mat3 = mat3+mat3
 if(!ENScriptTypeClass::AddOperator("matrix3","matrix3","matrix3","+",MAT3isMAT3plusMAT3)) return false;    

 //Add mat4 = mat4+int
 if(!ENScriptTypeClass::AddOperator("matrix4","matrix4","int","+",MAT4isMAT4plusINT)) return false;    
 //Add mat4 = mat4+float
 if(!ENScriptTypeClass::AddOperator("matrix4","matrix4","float","+",MAT4isMAT4plusFLOAT)) return false;    
 //Add mat4 = mat4+mat4
 if(!ENScriptTypeClass::AddOperator("matrix4","matrix4","matrix4","+",MAT4isMAT4plusMAT4)) return false;    
 //MINUS
 //Add int = int-int  
 if(!ENScriptTypeClass::AddOperator("int","int","int","-",INTisINTminusINT)) return false;    
 //Add float = int-float  
 if(!ENScriptTypeClass::AddOperator("float","int","float","-",FLOATisINTminusFLOAT)) return false;

 //Add float = float-float  
 if(!ENScriptTypeClass::AddOperator("float","float","float","-",FLOATisFLOATminusFLOAT)) return false;
 //Add float = float-int  
 if(!ENScriptTypeClass::AddOperator("float","float","int","-",FLOATisFLOATminusINT)) return false;

 //Add vec2 = vec2-int
 if(!ENScriptTypeClass::AddOperator("vector2","vector2","int","-",VEC2isVEC2minusINT)) return false;
 //Add vec2 = vec2-float
 if(!ENScriptTypeClass::AddOperator("vector2","vector2","float","-",VEC2isVEC2minusFLOAT)) return false;
 //Add vec2 = vec2-vec2
 if(!ENScriptTypeClass::AddOperator("vector2","vector2","vector2","-",VEC2isVEC2minusVEC2)) return false;

 //Add vec3 = vec3-int
 if(!ENScriptTypeClass::AddOperator("vector3","vector3","int","-",VEC3isVEC3minusINT)) return false;
 //Add vec3 = vec3-float
 if(!ENScriptTypeClass::AddOperator("vector3","vector3","float","-",VEC3isVEC3minusFLOAT)) return false;
 //Add vec3 = vec3-vec3
 if(!ENScriptTypeClass::AddOperator("vector3","vector3","vector3","-",VEC3isVEC3minusVEC3)) return false;

 //Add vec4 = vec4-int
 if(!ENScriptTypeClass::AddOperator("vector4","vector4","int","-",VEC4isVEC4minusINT)) return false;
 //Add vec4 = vec4-float
 if(!ENScriptTypeClass::AddOperator("vector4","vector4","float","-",VEC4isVEC4minusFLOAT)) return false;
 //Add vec4 = vec4-vec4
 if(!ENScriptTypeClass::AddOperator("vector4","vector4","vector4","-",VEC4isVEC4minusVEC4)) return false;

 //Add mat2 = mat2-int
 if(!ENScriptTypeClass::AddOperator("matrix2","matrix2","int","-",MAT2isMAT2minusINT)) return false;
 //Add mat2 = mat2-float
 if(!ENScriptTypeClass::AddOperator("matrix2","matrix2","float","-",MAT2isMAT2minusFLOAT)) return false;
 //Add mat2 = mat2-mat2
 if(!ENScriptTypeClass::AddOperator("matrix2","matrix2","matrix2","-",MAT2isMAT2minusMAT2)) return false;

 //Add mat3 = mat3-int
 if(!ENScriptTypeClass::AddOperator("matrix3","matrix3","int","-",MAT3isMAT3minusINT)) return false;
 //Add mat3 = mat3-float
 if(!ENScriptTypeClass::AddOperator("matrix3","matrix3","float","-",MAT3isMAT3minusFLOAT)) return false;
 //Add mat3 = mat3-mat3
 if(!ENScriptTypeClass::AddOperator("matrix3","matrix3","matrix3","-",MAT3isMAT3minusMAT3)) return false;

 //Add mat4 = mat4-int
 if(!ENScriptTypeClass::AddOperator("matrix4","matrix4","int","-",MAT4isMAT4minusINT)) return false;
 //Add mat4 = mat4-float
 if(!ENScriptTypeClass::AddOperator("matrix4","matrix4","float","-",MAT4isMAT4minusFLOAT)) return false;
 //Add mat4 = mat4-mat4
 if(!ENScriptTypeClass::AddOperator("matrix4","matrix4","matrix4","-",MAT4isMAT4minusMAT4)) return false;
 //MUL
 //Add int = int*int  
 if(!ENScriptTypeClass::AddOperator("int","int","int","*",INTisINTmulINT)) return false;
 //Add float = int*float  
 if(!ENScriptTypeClass::AddOperator("float","int","float","*",FLOATisINTmulFLOAT)) return false;

 //Add float = float*float  
 if(!ENScriptTypeClass::AddOperator("float","float","float","*",FLOATisFLOATmulFLOAT)) return false;
 //Add float = float*int  
 if(!ENScriptTypeClass::AddOperator("float","float","int","*",FLOATisFLOATmulINT)) return false;

 //Add vec2 = vec2*int
 if(!ENScriptTypeClass::AddOperator("vector2","vector2","int","*",VEC2isVEC2mulINT)) return false;
 //Add vec2 = vec2*float
 if(!ENScriptTypeClass::AddOperator("vector2","vector2","float","*",VEC2isVEC2mulFLOAT)) return false;
 //Add float = vec2*vec2
 if(!ENScriptTypeClass::AddOperator("float","vector2","vector2","*",FLOATisVEC2mulVEC2)) return false;

 //Add vec3 = vec3*int
 if(!ENScriptTypeClass::AddOperator("vector3","vector3","int","*",VEC3isVEC3mulINT)) return false;
 //Add vec3 = vec3*float
 if(!ENScriptTypeClass::AddOperator("vector3","vector3","float","*",VEC3isVEC3mulFLOAT)) return false;
 //Add float = vec3*vec3
 if(!ENScriptTypeClass::AddOperator("float","vector3","vector3","*",FLOATisVEC3mulVEC3)) return false;

 //Add vec4 = vec4*int
 if(!ENScriptTypeClass::AddOperator("vector4","vector4","int","*",VEC4isVEC4mulINT)) return false;
 //Add vec4 = vec4*float
 if(!ENScriptTypeClass::AddOperator("vector4","vector4","float","*",VEC4isVEC4mulFLOAT)) return false;
 //Add float = vec4*vec4
 if(!ENScriptTypeClass::AddOperator("float","vector4","vector4","*",FLOATisVEC4mulVEC4)) return false;

 //Add mat2 = mat2*int
 if(!ENScriptTypeClass::AddOperator("matrix2","matrix2","int","*",MAT2isMAT2mulINT)) return false;
 //Add mat2 = mat2*float
 if(!ENScriptTypeClass::AddOperator("matrix2","matrix2","float","*",MAT2isMAT2mulFLOAT)) return false;
 //Add mat2 = mat2*vec2
 if(!ENScriptTypeClass::AddOperator("vector2","matrix2","vector2","*",VEC2isMAT2mulVEC2)) return false;
 //Add mat2 = mat2*mat2
 if(!ENScriptTypeClass::AddOperator("matrix2","matrix2","matrix2","*",MAT2isMAT2mulMAT2)) return false;

 //Add mat3 = mat3*int
 if(!ENScriptTypeClass::AddOperator("matrix3","matrix3","int","*",MAT3isMAT3mulINT)) return false;
 //Add mat3 = mat3*float
 if(!ENScriptTypeClass::AddOperator("matrix3","matrix3","float","*",MAT3isMAT3mulFLOAT)) return false;
 //Add mat3 = mat3*vec3
 if(!ENScriptTypeClass::AddOperator("vector3","matrix3","vector3","*",VEC3isMAT3mulVEC3)) return false;
 //Add mat3 = mat3*mat3
 if(!ENScriptTypeClass::AddOperator("matrix3","matrix3","matrix3","*",MAT3isMAT3mulMAT3)) return false;
 
 //Add mat4 = mat4*int
 if(!ENScriptTypeClass::AddOperator("matrix4","matrix4","int","*",MAT4isMAT4mulINT)) return false;
 //Add mat4 = mat4*float
 if(!ENScriptTypeClass::AddOperator("matrix4","matrix4","float","*",MAT4isMAT4mulFLOAT)) return false;
 //Add mat4 = mat4*mat4
 if(!ENScriptTypeClass::AddOperator("vector4","matrix4","vector4","*",VEC4isMAT4mulVEC4)) return false;
 //Add mat4 = mat4*mat4
 if(!ENScriptTypeClass::AddOperator("matrix4","matrix4","matrix4","*",MAT4isMAT4mulMAT4)) return false;
 //DIV
 //Add int = int/int  
 if(!ENScriptTypeClass::AddOperator("int","int","int","/",INTisINTdivINT)) return false;
 //Add float = int/float  
 if(!ENScriptTypeClass::AddOperator("float","int","float","/",FLOATisINTdivFLOAT)) return false;

 //Add float = float/float  
 if(!ENScriptTypeClass::AddOperator("float","float","float","/",FLOATisFLOATdivFLOAT)) return false;
 //Add float = float/int  
 if(!ENScriptTypeClass::AddOperator("float","float","int","/",FLOATisFLOATdivINT)) return false;

 //Add vec2 = vec2/int
 if(!ENScriptTypeClass::AddOperator("vector2","vector2","int","/",VEC2isVEC2divINT)) return false;
 //Add vec2 = vec2/float
 if(!ENScriptTypeClass::AddOperator("vector2","vector2","float","/",VEC2isVEC2divFLOAT)) return false;

 //Add vec3 = vec3/int
 if(!ENScriptTypeClass::AddOperator("vector3","vector3","int","/",VEC3isVEC3divINT)) return false;
 //Add vec3 = vec3/float
 if(!ENScriptTypeClass::AddOperator("vector3","vector3","float","/",VEC3isVEC3divFLOAT)) return false;

 //Add vec4 = vec4/int
 if(!ENScriptTypeClass::AddOperator("vector4","vector4","int","/",VEC4isVEC4divINT)) return false;
 //Add vec4 = vec4/float
 if(!ENScriptTypeClass::AddOperator("vector4","vector4","float","/",VEC4isVEC4divFLOAT)) return false;

 //Add mat2 = mat2/int
 if(!ENScriptTypeClass::AddOperator("matrix2","matrix2","int","/",MAT2isMAT2divINT)) return false;
 //Add mat2 = mat2/float
 if(!ENScriptTypeClass::AddOperator("matrix2","matrix2","float","/",MAT2isMAT2divFLOAT)) return false;

 //Add mat3 = mat3/int
 if(!ENScriptTypeClass::AddOperator("matrix3","matrix3","int","/",MAT3isMAT3divINT)) return false;
 //Add mat3 = mat3/float
 if(!ENScriptTypeClass::AddOperator("matrix3","matrix3","float","/",MAT3isMAT3divFLOAT)) return false;
 
 //Add mat4 = mat4/int
 if(!ENScriptTypeClass::AddOperator("matrix4","matrix4","int","/",MAT4isMAT4divINT)) return false;
 //Add mat4 = mat4/float
 if(!ENScriptTypeClass::AddOperator("matrix4","matrix4","float","/",MAT4isMAT4divFLOAT)) return false;
 //MOD
 //Add int = int%int  
 if(!ENScriptTypeClass::AddOperator("int","int","int","%",INTisINTmodINT)) return false;
 //Add float = int%float  
 if(!ENScriptTypeClass::AddOperator("float","int","float","%",FLOATisINTmodFLOAT)) return false;

 //Add float = float%float  
 if(!ENScriptTypeClass::AddOperator("float","float","float","%",FLOATisFLOATmodFLOAT)) return false;
 //Add float = float%int  
 if(!ENScriptTypeClass::AddOperator("float","float","int","%",FLOATisFLOATmodINT)) return false;
 //CMP 
 //Add int = int=int  
 if(!ENScriptTypeClass::AddOperator("int","int","int","==",INTisINTcmpINT)) return false;    
 //Add int = int=float  
 if(!ENScriptTypeClass::AddOperator("int","int","float","==",INTisINTcmpFLOAT)) return false;    
 //Add int = int=string  
 if(!ENScriptTypeClass::AddOperator("int","int","string","==",INTisINTcmpSTR)) return false;    

 //Add int = float=float  
 if(!ENScriptTypeClass::AddOperator("int","float","float","==",INTisFLOATcmpFLOAT)) return false;    
 //Add int = float=int  
 if(!ENScriptTypeClass::AddOperator("int","float","int","==",INTisFLOATcmpINT)) return false;    
 //Add int = float=string  
 if(!ENScriptTypeClass::AddOperator("int","float","string","==",INTisFLOATcmpSTR)) return false;    

 //Add int = vec2=vec2
 if(!ENScriptTypeClass::AddOperator("int","vector2","vector2","==",INTisVEC2cmpVEC2)) return false;    

 //Add int = vec3=vec3
 if(!ENScriptTypeClass::AddOperator("int","vector3","vector3","==",INTisVEC3cmpVEC3)) return false;    

 //Add int = vec4=vec4
 if(!ENScriptTypeClass::AddOperator("int","vector4","vector4","==",INTisVEC4cmpVEC4)) return false;    

 //Add int = string=int
 if(!ENScriptTypeClass::AddOperator("int","string","int","==",INTisSTRcmpINT)) return false;    
 //Add int = string=float
 if(!ENScriptTypeClass::AddOperator("int","string","float","==",INTisSTRcmpFLOAT)) return false;    
 //Add int = string=string
 if(!ENScriptTypeClass::AddOperator("int","string","string","==",INTisSTRcmpSTR)) return false;    

 //Add int = mat2=mat2
 if(!ENScriptTypeClass::AddOperator("int","matrix2","matrix2","==",INTisMAT2cmpMAT2)) return false;  

 //Add int = mat3=mat3
 if(!ENScriptTypeClass::AddOperator("int","matrix3","matrix3","==",INTisMAT3cmpMAT3)) return false;  

 //Add int = mat4=mat4
 if(!ENScriptTypeClass::AddOperator("int","matrix4","matrix4","==",INTisMAT4cmpMAT4)) return false;  
 //NCMP
 //Add int = int!=int  
 if(!ENScriptTypeClass::AddOperator("int","int","int","!=",INTisINTncmpINT)) return false;
 //Add int = int!=float  
 if(!ENScriptTypeClass::AddOperator("int","int","float","!=",INTisINTncmpFLOAT)) return false;
 //Add int = int!=string  
 if(!ENScriptTypeClass::AddOperator("int","int","string","!=",INTisINTncmpSTR)) return false;

 //Add int = float!=float  
 if(!ENScriptTypeClass::AddOperator("int","float","float","!=",INTisFLOATncmpFLOAT)) return false;
 //Add int = float!=int  
 if(!ENScriptTypeClass::AddOperator("int","float","int","!=",INTisFLOATncmpINT)) return false;
 //Add int = float!=string  
 if(!ENScriptTypeClass::AddOperator("int","float","string","!=",INTisFLOATncmpSTR)) return false;

 //Add int = vec2!=vec2
 if(!ENScriptTypeClass::AddOperator("int","vector2","vector2","!=",INTisVEC2ncmpVEC2)) return false;

 //Add int = vec3!=vec3
 if(!ENScriptTypeClass::AddOperator("int","vector3","vector3","!=",INTisVEC3ncmpVEC3)) return false;

 //Add int = vec4!=vec4
 if(!ENScriptTypeClass::AddOperator("int","vector4","vector4","!=",INTisVEC4ncmpVEC4)) return false;

 //Add int = string!=int
 if(!ENScriptTypeClass::AddOperator("int","string","int","!=",INTisSTRncmpINT)) return false;
 //Add int = string!=float
 if(!ENScriptTypeClass::AddOperator("int","string","float","!=",INTisSTRncmpFLOAT)) return false;
 //Add int = string!=string
 if(!ENScriptTypeClass::AddOperator("int","string","string","!=",INTisSTRncmpSTR)) return false;

 //Add int = mat2!=mat2
 if(!ENScriptTypeClass::AddOperator("int","matrix2","matrix2","!=",INTisMAT2ncmpMAT2)) return false;

 //Add int = mat3!=mat3
 if(!ENScriptTypeClass::AddOperator("int","matrix3","matrix3","!=",INTisMAT3ncmpMAT3)) return false;

 //Add int = mat4!=mat4
 if(!ENScriptTypeClass::AddOperator("int","matrix4","matrix4","!=",INTisMAT4ncmpMAT4)) return false;
 //GREATER
 //Add int = int>int
 if(!ENScriptTypeClass::AddOperator("int","int","int",">",INTisINTgreaterINT)) return false;
 //Add int = int>float
 if(!ENScriptTypeClass::AddOperator("int","int","float",">",INTisINTgreaterFLOAT)) return false;

 //Add int = float>int
 if(!ENScriptTypeClass::AddOperator("int","float","int",">",INTisFLOATgreaterINT)) return false;
 //Add int = float>float
 if(!ENScriptTypeClass::AddOperator("int","float","float",">",INTisFLOATgreaterFLOAT)) return false;
 //GREATER SAME
 //Add int = int>=int
 if(!ENScriptTypeClass::AddOperator("int","int","int",">=",INTisINTgreatersameINT)) return false;
 //Add int = int>=float
 if(!ENScriptTypeClass::AddOperator("int","int","float",">=",INTisINTgreatersameFLOAT)) return false;

 //Add int = float>=int
 if(!ENScriptTypeClass::AddOperator("int","float","int",">=",INTisFLOATgreatersameINT)) return false;
 //Add int = float>=float
 if(!ENScriptTypeClass::AddOperator("int","float","float",">=",INTisFLOATgreatersameFLOAT)) return false;
 //SMALLER
 //Add int = int<int
 if(!ENScriptTypeClass::AddOperator("int","int","int","<",INTisINTsmallerINT)) return false;
 //Add int = int<float
 if(!ENScriptTypeClass::AddOperator("int","int","float","<",INTisINTsmallerFLOAT)) return false;

 //Add int = float<int
 if(!ENScriptTypeClass::AddOperator("int","float","int","<",INTisFLOATsmallerINT)) return false;
 //Add int = float<float
 if(!ENScriptTypeClass::AddOperator("int","float","float","<",INTisFLOATsmallerFLOAT)) return false;
 //SMALLER SAME
 //Add int = int<=int
 if(!ENScriptTypeClass::AddOperator("int","int","int","<=",INTisINTsmallersameINT)) return false;
 //Add int = int<=float
 if(!ENScriptTypeClass::AddOperator("int","int","float","<=",INTisINTsmallersameFLOAT)) return false;

 //Add int = float<=int
 if(!ENScriptTypeClass::AddOperator("int","float","int","<=",INTisFLOATsmallersameINT)) return false;
 //Add int = float<=float
 if(!ENScriptTypeClass::AddOperator("int","float","float","<=",INTisFLOATsmallersameFLOAT)) return false;
 //AND
 //Add int = int&int
 if(!ENScriptTypeClass::AddOperator("int","int","int","&",INTisINTandINT)) return false;
 //OR
 //Add int = int||int
 if(!ENScriptTypeClass::AddOperator("int","int","int","||",INTisINTorINT)) return false;
 //BIT OR
 //Add int = int | int
 if(!ENScriptTypeClass::AddOperator("int","int","int","|",INTisINTbitorINT)) return false;
 //Finished
 return true;
}

/////////////////INIT OPERATORS///////////////////////////////
ENbool ENScriptOperators::Init()
{
 //VARS
 ENScriptTypeClass::ENScriptDataType type;
 //Set size of integer 
 if(!ENScriptTypeClass::GetType(ENINT,&type)) return false;
 SizeInt=type.Size;
 //Set size of float
 if(!ENScriptTypeClass::GetType(ENFLOAT,&type)) return false;
 SizeFloat=type.Size;
 //Set size of string
 if(!ENScriptTypeClass::GetType(ENSTRING,&type)) return false;
 SizeString=type.Size;
 //Set size of vec2
 if(!ENScriptTypeClass::FindType("vector2",&type)) return false;
 SizeVec2=type.Size;
 //Set size of vec3
 if(!ENScriptTypeClass::FindType("vector3",&type)) return false;
 SizeVec3=type.Size;
 //Set size of vec4
 if(!ENScriptTypeClass::FindType("vector4",&type)) return false;
 SizeVec4=type.Size;
 //Set size of mat2
 if(!ENScriptTypeClass::FindType("matrix2",&type)) return false;
 SizeMat2=type.Size;
 //Set size of mat3
 if(!ENScriptTypeClass::FindType("matrix3",&type)) return false;
 SizeMat3=type.Size;
 //Set size of mat4
 if(!ENScriptTypeClass::FindType("matrix4",&type)) return false;
 SizeMat4=type.Size;
 //Finished
 return RegisterOperators();
}