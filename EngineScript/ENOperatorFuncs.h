#ifndef   _EN_OPEARTORFUNCS_H_
#define   _EN_OPEARTORFUNCS_H_

#include "ENScriptTypes.h"

class ENScriptOperators
{
 public:
   static ENbool Init();
 protected:
   typedef void* ENScriptPointer;

   static ENint  SizeInt;
   static ENint  SizeFloat;
   static ENint  SizeString;
   static ENint  SizeVec2;
   static ENint  SizeVec3;
   static ENint  SizeVec4;
   static ENint  SizeMat2;
   static ENint  SizeMat3;
   static ENint  SizeMat4;

   static inline void Push(ENubyte *ENStack,ENint &_ENStackPointer,
                           ENScriptPointer Src,ENint size);
   static inline void Pop(ENubyte *ENStack,ENint &_ENStackPointer,
                           ENScriptPointer Src,ENint size);
   static ENbool RegisterOperators();
 private:
 //Assign
 static void INTisFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisSTR(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisSTR(ENubyte *ENStack,ENint &_ENStackPointer);
 static void STRisINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void STRisFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 //Plus
 static void INTisINTplusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisINTplusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void STRisINTplusSTR(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisFLOATplusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisFLOATplusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void STRisFLOATplusSTR(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC2isVEC2plusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC2isVEC2plusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC2isVEC2plusVEC2(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC3isVEC3plusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC3isVEC3plusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC3isVEC3plusVEC3(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC4isVEC4plusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC4isVEC4plusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC4isVEC4plusVEC4(ENubyte *ENStack,ENint &_ENStackPointer);
 static void STRisSTRplusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void STRisSTRplusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void STRisSTRplusSTR(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT2isMAT2plusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT2isMAT2plusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT2isMAT2plusMAT2(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT3isMAT3plusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT3isMAT3plusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT3isMAT3plusMAT3(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT4isMAT4plusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT4isMAT4plusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT4isMAT4plusMAT4(ENubyte *ENStack,ENint &_ENStackPointer);
 //minus
 static void INTisINTminusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisINTminusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisFLOATminusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisFLOATminusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC2isVEC2minusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC2isVEC2minusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC2isVEC2minusVEC2(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC3isVEC3minusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC3isVEC3minusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC3isVEC3minusVEC3(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC4isVEC4minusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC4isVEC4minusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC4isVEC4minusVEC4(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT2isMAT2minusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT2isMAT2minusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT2isMAT2minusMAT2(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT3isMAT3minusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT3isMAT3minusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT3isMAT3minusMAT3(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT4isMAT4minusINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT4isMAT4minusFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT4isMAT4minusMAT4(ENubyte *ENStack,ENint &_ENStackPointer);
 //mul
 static void INTisINTmulINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisINTmulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisFLOATmulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisFLOATmulINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC2isVEC2mulINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC2isVEC2mulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisVEC2mulVEC2(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC3isVEC3mulINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC3isVEC3mulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisVEC3mulVEC3(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC4isVEC4mulINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC4isVEC4mulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisVEC4mulVEC4(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT2isMAT2mulINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT2isMAT2mulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC2isMAT2mulVEC2(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT2isMAT2mulMAT2(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT3isMAT3mulINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT3isMAT3mulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC3isMAT3mulVEC3(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT3isMAT3mulMAT3(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT4isMAT4mulINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT4isMAT4mulFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC4isMAT4mulVEC4(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT4isMAT4mulMAT4(ENubyte *ENStack,ENint &_ENStackPointer);
 //div
 static void INTisINTdivINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisINTdivFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisFLOATdivFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisFLOATdivINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC2isVEC2divINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC2isVEC2divFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC3isVEC3divINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC3isVEC3divFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC4isVEC4divINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void VEC4isVEC4divFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT2isMAT2divINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT2isMAT2divFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT3isMAT3divINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT3isMAT3divFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT4isMAT4divINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void MAT4isMAT4divFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 //mod
 static void INTisINTmodINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisINTmodFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisFLOATmodFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void FLOATisFLOATmodINT(ENubyte *ENStack,ENint &_ENStackPointer);
 //compare
 static void INTisINTcmpINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisINTcmpFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisINTcmpSTR(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisFLOATcmpINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisFLOATcmpFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisFLOATcmpSTR(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisVEC2cmpVEC2(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisVEC3cmpVEC3(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisVEC4cmpVEC4(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisMAT2cmpMAT2(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisMAT3cmpMAT3(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisMAT4cmpMAT4(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisSTRcmpINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisSTRcmpFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisSTRcmpSTR(ENubyte *ENStack,ENint &_ENStackPointer);
 //Not compare
 static void INTisINTncmpINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisINTncmpFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisINTncmpSTR(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisFLOATncmpINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisFLOATncmpFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisFLOATncmpSTR(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisVEC2ncmpVEC2(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisVEC3ncmpVEC3(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisVEC4ncmpVEC4(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisMAT2ncmpMAT2(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisMAT3ncmpMAT3(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisMAT4ncmpMAT4(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisSTRncmpINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisSTRncmpFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisSTRncmpSTR(ENubyte *ENStack,ENint &_ENStackPointer);
 //greater
 static void INTisINTgreaterINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisINTgreaterFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisFLOATgreaterINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisFLOATgreaterFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 //greater same
 static void INTisINTgreatersameINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisINTgreatersameFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisFLOATgreatersameINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisFLOATgreatersameFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 //smaller
 static void INTisINTsmallerINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisINTsmallerFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisFLOATsmallerINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisFLOATsmallerFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 //smaller same
 static void INTisINTsmallersameINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisINTsmallersameFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisFLOATsmallersameINT(ENubyte *ENStack,ENint &_ENStackPointer);
 static void INTisFLOATsmallersameFLOAT(ENubyte *ENStack,ENint &_ENStackPointer);
 //and
 static void INTisINTandINT(ENubyte *ENStack,ENint &_ENStackPointer);
 //or
 static void INTisINTorINT(ENubyte *ENStack,ENint &_ENStackPointer);
 //bit or
 static void INTisINTbitorINT(ENubyte *ENStack,ENint &_ENStackPointer);
};

#endif
