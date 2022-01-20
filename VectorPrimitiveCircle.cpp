#include "pch.h"

#include "VectorPrimitiveCircle.h"

#include "VectorPrimitiveTypes.h"

u32* VectorPrimitiveCircle::Serialize(u32* pBuffer)
{
	*pBuffer = (u32)EPrimitiveType::Cirlce;
	++pBuffer;

	*pBuffer = x;
	++pBuffer;
	*pBuffer = y;
	++pBuffer;
	*pBuffer = rad;
	++pBuffer;

	return VectorPrimitiveShape::Serialize(pBuffer);
}

std::istream& VectorPrimitiveCircle::PopulateFromFile(std::istream& stream)
{
	return stream;
}
