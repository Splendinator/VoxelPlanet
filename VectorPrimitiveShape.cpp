#include "pch.h"

#include "VectorPrimitiveShape.h"

void VectorPrimitiveShape::Serialize(u32* pBuffer)
{
	*pBuffer = red;
	++pBuffer;
	
	*pBuffer = green;
	++pBuffer;
	
	*pBuffer = blue;
	++pBuffer;
}
