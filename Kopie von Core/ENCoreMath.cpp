//---------------------------------------------------------------------------
#include "ENCoreMath.h"
//---------------------------------------------------------------------------
ENVector ENCoreMath::MoveVec(ENVector pos,ENVector2 dir,ENfloat range)
{
 return pos+ENVecDir(range,dir);
}
