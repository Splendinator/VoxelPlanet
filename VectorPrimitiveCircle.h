#pragma once

#include "VectorPrimitiveShape.h"

/** VectorPrimitiveCircle
*
* This class is used to hold a vector art circle primitive
*/
class VectorPrimitiveCircle : public VectorPrimitiveShape
{
public:
	//~ Begin VectorPrimitiveBase Interface
	u32* Serialize(u32* pBuffer) override;
	std::istream& PopulateFromFile(std::istream& stream) override;
	//~ End VectorPrimitiveBase Interface

private:
	u32 x = 0;
	u32 y = 0;
	u32 rad = 0;
};

