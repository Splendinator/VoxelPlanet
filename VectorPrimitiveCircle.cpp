#include "pch.h"

#include "VectorPrimitiveCircle.h"

#include <DomMath/Vec2.h>

#include "VectorPrimitiveTypes.h"
#include "DomImport/XMLUtils.h"

u32* VectorPrimitiveCircle::Serialize(u32* pBuffer)
{
	*pBuffer = (u32)EPrimitiveType::Cirlce;
	++pBuffer;

	*pBuffer = (u32)x;
	++pBuffer;
	*pBuffer = (u32)y;
	++pBuffer;
	*pBuffer = rad;
	++pBuffer;

	return VectorPrimitiveShape::Serialize(pBuffer);
}

std::istream& VectorPrimitiveCircle::PopulateFromFile(std::istream& stream)
{
	VectorPrimitiveShape::PopulateFromFile(stream);

	// #JANK: Should these be floats instead?
	x = (i32)std::stof(dmim::GetNextAttribute(stream, "cx"));
	y = (i32)std::stof(dmim::GetNextAttribute(stream, "cy"));
	rad = (u32)std::stof(dmim::GetNextAttribute(stream, "r"));

	return stream;
}

const VectorPrimitiveBase* VectorPrimitiveCircle::FindPrimitiveUnderCursor(Vec2i cursorPos) const
{
	if (Vec2i::DistanceSq(Vec2i(x, y), cursorPos) <= (int)(rad * rad))
	{
		return this;
	}
	return nullptr;   
}

Box2f VectorPrimitiveCircle::GetBoundingBox() const
{
	return Box2f::InitFromCenterAndSize({(float)x, (float)y}, {(float)rad, (float)rad});
}

void VectorPrimitiveCircle::AdjustPositionWithinLayer(Vec2f delta)
{
	x += (i32)delta.x;
	y += (i32)delta.y;
}
