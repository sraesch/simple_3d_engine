#include "EngineMedia.h"

ENVertexArray::ENVertexArray()
              : ENArray(sizeof(ENVector))
{
}

ENVertexArray::~ENVertexArray()
{}

void ENVertexArray::SetGLVertexPointer()
{
 glVertexPointer(3,GL_FLOAT,0,Data);
}
