#include "pch.h"

#include "VectorPrimitiveShape.h"

#include "DomImport/XMLUtils.h"
#include "DomUtils/DomUtils.h"

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
	// colour in format #12ABFF
	std::string styleString = dmim::GetNextAttribute(stream, "style");
	std::string colourString = dmim::GetSubAttribute(styleString, "fill");

	red = dmut::HexCharToInt(colourString[1]) * 16 + dmut::HexCharToInt(colourString[2]);
	green = dmut::HexCharToInt(colourString[3]) * 16 + dmut::HexCharToInt(colourString[4]);
	blue = dmut::HexCharToInt(colourString[5]) * 16 + dmut::HexCharToInt(colourString[6]);

	return stream;
}
