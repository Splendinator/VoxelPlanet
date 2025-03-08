#include "pch.h"
#include "VectorPrimitiveRectangle.h"

#include "DomImport/XMLUtils.h"
#include "VectorPrimitiveTypes.h"

u32* VectorPrimitiveRectangle::Serialize(u32* pBuffer)
{
	*pBuffer = (u32)EPrimitiveType::Rectangle;
	++pBuffer;

	*pBuffer = (u32)x;
	++pBuffer;
	*pBuffer = (u32)y;
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

	// #JANK: Should these be floats instead?
	width = (u32)std::stof(dmim::GetNextAttribute(stream, "width"));
	height = (u32)std::stof(dmim::GetNextAttribute(stream, "height"));
	x = (i32)std::stof(dmim::GetNextAttribute(stream, "x"));
	y = (i32)std::stof(dmim::GetNextAttribute(stream, "y"));

	return stream;
}

const VectorPrimitiveBase* VectorPrimitiveRectangle::FindPrimitiveUnderCursor(Vec2i cursorPos) const
{
	if (cursorPos.x >= (int)x && cursorPos.x < (int)(x + width) && cursorPos.y >= (int)y && cursorPos.y < (int)(y + height))
	{
		return this;
	}
	return nullptr;
}

Box2f VectorPrimitiveRectangle::GetBoundingBox() const
{
	Box2f boundingBox;
	boundingBox.SetTopLeft({(float)x, (float)y});
	boundingBox.SetSize({(float)width, (float)height});

	return boundingBox;
}

void VectorPrimitiveRectangle::AdjustPositionWithinLayer(Vec2f delta)
{
	x += (i32)delta.x;
	y += (i32)delta.y;
}
