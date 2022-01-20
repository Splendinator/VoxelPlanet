#include "pch.h"

#include "VectorPrimitiveShape.h"

u32* VectorPrimitiveShape::Serialize(u32* pBuffer)
{
	*pBuffer = red;
	++pBuffer;
	
	*pBuffer = green;
	++pBuffer;
	
	*pBuffer = blue;
	++pBuffer;

	return pBuffer;
}

std::istream& VectorPrimitiveShape::PopulateFromFile(std::istream& stream)
{
	return stream;
}
