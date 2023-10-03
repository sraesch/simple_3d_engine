//---------------------------------------------------------------------------
#include "XMLClass.h"
//---------------------------------------------------------------------------
ENXMLBRUSH::ENXMLBRUSH()
          : ENArray(sizeof(ENXMLPLANE))
{
}

char *ENXMLBRUSH::ReadBrush(char *ptr)
{
}

char *ENXMLBRUSH::JumpOverSpace(char *ptr)
{
 while((*ptr)==10||(*ptr)==13||(*ptr)==32)
   ptr++;

 return ptr;   
}
