//---------------------------------------------------------------------------
#ifndef XMLClassH
#define XMLClassH
//---------------------------------------------------------------------------
#include "EngineSystem.h"

struct ENXMLPLANE
{
 ENVector Pos,Normal;
};

class ENXMLBRUSH : public ENArray
{
 public:
   ENXMLBRUSH();
   char *ReadBrush(char *ptr);
 private:
   char *JumpOverSpace(char *ptr);
};

#endif
