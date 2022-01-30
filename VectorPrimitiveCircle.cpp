#include "pch.h"

#include "VectorPrimitiveCircle.h"

#include "VectorPrimitiveTypes.h"
#include "DomImport/XMLUtils.h"

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
	VectorPrimitiveShape::PopulateFromFile(stream);

	// #TODO: Should these be floats instead?
	x = (u32)std::stof(dmim::GetNextAttribute(stream, "cx"));
	y = (u32)std::stof(dmim::GetNextAttribute(stream, "cy"));
	rad = (u32)std::stof(dmim::GetNextAttribute(stream, "r"));

	return stream;
}
