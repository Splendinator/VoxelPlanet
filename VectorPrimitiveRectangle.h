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

private:
	// Coorinates of top left
	u32 x;
	u32 y;

	u32 width;
	u32 height;
};

