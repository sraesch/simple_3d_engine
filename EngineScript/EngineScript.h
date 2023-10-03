#ifndef         _EN_ENGINESCRIPT_H_
#define         _EN_ENGINESCRIPT_H_

#include "ENScriptBuilder.h"
#include "ENOperatorFuncs.h"
#include "ENSysFuncs.h"
#include "ENScriptExecute.h"

struct ENScriptText
{
 char Text[1024];    
 ENVector2 Pos,Size; 
 ENint     Mode;     
 ENint     Layer;    
 ENint     ColorR,ColorG,ColorB,ColorA;
 ENint     Visible,Mirror;
 ENint     Src;
};

struct ENScriptPanel
{ 
 ENVector2 Pos,Size;
 ENint     Mode;
 ENint     Layer; 
 ENint     ColorR,ColorG,ColorB,ColorA;
 ENint     Visible,MirrorX,MirrorY;
 ENint     Src;
};

struct ENScriptLight
{
 ENint  r,g,b;
 ENVector  pos;
 ENVector2 dir;
 ENfloat spotcutoff;
 ENfloat range;
 ENint  ID;
};

struct ENScriptCursor
{
 ENVector2 Size;
 ENint    Mode;
 ENint    MirrorX,MirrorY;
 ENint    Colors[16];
};

struct ENScriptObject
{ 
 ENVector       Pos,Angle,Scale;
 ENfloat        CurrentFrame;
 ENint          CurrentSkin; 

 ENint          Func;
 ENint          Visible,Passable;
 ENint          ViewX,ViewY;
 ENint          Mode;
 ENint          Red,Green,Blue,Alpha;
 ENint          Lighting,CullFace,DepthBuffer;
 ENint          Shadow,StaticHull;
 ENint          LocalInts[9];
 ENfloat        LocalFloats[9];
 ENint          Hull;
 void           *Addr;
};

#endif