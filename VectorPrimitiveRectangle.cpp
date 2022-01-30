#include "pch.h"
#include "VectorPrimitiveRectangle.h"

#include "DomImport/XMLUtils.h"
#include "VectorPrimitiveTypes.h"

u32* VectorPrimitiveRectangle::Serialize(u32* pBuffer)
{
	*pBuffer = (u32)EPrimitiveType::Rectangle;
	++pBuffer;

	*pBuffer = x;
	++pBuffer;
	*pBuffer = y;
	++pBuffer;
	*pBuffer = width;
	++pBuffer;
	*pBuffer = height;
	++pBuffer;

	return VectorPrimitiveShape::Serialize(pBuffer);
}

std::istream& VectorPrimitiveRectangle::PopulateFromFile(std::istream& stream)
{
	VectorPrimitiveShape::PopulateFromFile(stream);

	// #TODO: Should these be floats instead?
	width = (u32)std::stof(dmim::GetNextAttribute(stream, "width"));
	height = (u32)std::stof(dmim::GetNextAttribute(stream, "height"));
	x = (u32)std::stof(dmim::GetNextAttribute(stream, "x"));
	y = (u32)std::stof(dmim::GetNextAttribute(stream, "y"));

	return stream;
}
