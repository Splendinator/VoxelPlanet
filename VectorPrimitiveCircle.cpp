#include "pch.h"

#include "VectorPrimitiveCircle.h"

#include "VectorPrimitiveTypes.h"

void VectorPrimitiveCircle::Serialize(u32* pBuffer)
{
	*pBuffer = (u32)EPrimitiveType::Cirlce;
	++pBuffer;

	*pBuffer = x;
	++pBuffer;
	*pBuffer = y;
	++pBuffer;
	*pBuffer = rad;
	++pBuffer;

	VectorPrimitiveShape::Serialize(pBuffer);
}
