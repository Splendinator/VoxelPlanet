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
	//~ End VectorPrimitiveBase Interface

	void SetWidth(u32 inWidth) { width = inWidth; };
	void SetHeight(u32 inHeight) { height = inHeight; };
	void SetX(u32 inX) { x = inX; };
	void SetY(u32 inY) { y = inY; };

private:
	// Coorinates of top left
	u32 x;
	u32 y;

	u32 width;
	u32 height;
};

