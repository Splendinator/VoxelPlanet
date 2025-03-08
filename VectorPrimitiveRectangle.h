#pragma once

#include "VectorPrimitiveShape.h"

/** VectorPrimitiveRectangle
*
* This class is used to hold a vector art rectangle primitive
*/
class VectorPrimitiveRectangle : public VectorPrimitiveShape
{
public:
	//~ Begin VectorPrimitiveBase Interface
	u32* Serialize(u32* pBuffer) override;
	std::istream& PopulateFromFile(std::istream& stream) override;
	const VectorPrimitiveBase* FindPrimitiveUnderCursor(Vec2i cursorPos) const override;
	Box2f GetBoundingBox() const override;
	void AdjustPositionWithinLayer(Vec2f delta) override;
	//~ End VectorPrimitiveBase Interface

	void SetWidth(u32 inWidth) { width = inWidth; RefreshParentLayerBoundingBox(); }
	void SetHeight(u32 inHeight) { height = inHeight; RefreshParentLayerBoundingBox(); }
	void SetX(i32 inX) { x = inX; RefreshParentLayerBoundingBox(); }
	void SetY(i32 inY) { y = inY; RefreshParentLayerBoundingBox(); }

	u32 GetWidth() const { return width; }
	u32 GetHeight() const { return height; }
	i32 GetX() const { return x; }
	i32 GetY() const { return y; }

private:
	// Coorinates of top left
	i32 x;
	i32 y;

	u32 width;
	u32 height;
};

