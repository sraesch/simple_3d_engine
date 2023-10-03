#ifndef    _EN_SYSFUNCS_H_
#define    _EN_SYSFUNCS_H_

#include "../EngineSystem/EngineSystem.h"

class ENScriptSysFuncs
{
 public:
   static ENbool Init();
 private:
   static void SysVEC2(ENubyte *Data,void *res);
   static void SysVEC3(ENubyte *Data,void *res);
   static void SysVEC4(ENubyte *Data,void *res);
   static void SysMAT2(ENubyte *Data,void *res);
   static void SysMAT3(ENubyte *Data,void *res);
   static void SysMAT4(ENubyte *Data,void *res);
   static void SysVEC2_LEN(ENubyte *Data,void *res);
   static void SysVEC3_LEN(ENubyte *Data,void *res);
   static void SysVEC4_LEN(ENubyte *Data,void *res);
   static void SysVEC2_NORMALIZE(ENubyte *Data,void *res);
   static void SysVEC3_NORMALIZE(ENubyte *Data,void *res);
   static void SysVEC4_NORMALIZE(ENubyte *Data,void *res);
   static void SysVEC2_DOT(ENubyte *Data,void *res);
   static void SysVEC3_DOT(ENubyte *Data,void *res);
   static void SysVEC4_DOT(ENubyte *Data,void *res);
   static void SysVEC3_ANGLE(ENubyte *Data,void *res);

   static void SysRANDOM(ENubyte *Data,void *res);

   static void SysSIN(ENubyte *Data,void *res);
   static void SysCOS(ENubyte *Data,void *res);
   static void SysTAN(ENubyte *Data,void *res);
   static void SysASIN(ENubyte *Data,void *res);
   static void SysACOS(ENubyte *Data,void *res);
   static void SysATAN(ENubyte *Data,void *res);
};

#endif