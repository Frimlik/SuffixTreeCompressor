
#include "Leaf.h"

Leaf::Leaf() : VertexBase()
{
}

Leaf::~Leaf()
{
}

//void Leaf::WriteVertex(int level, int labelStart, int *labelEnd, Buffer * buffer)
//{
//	VertexBase::WriteVertex(level, labelStart + this->offsetInBuffer, *labelEnd, buffer);
//}

int Leaf::UpdateOffset()
{
	return this->offsetInBuffer;
}
