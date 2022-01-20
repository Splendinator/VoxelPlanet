#pragma once
#include "VectorPrimitiveBase.h"
class VectorPrimitiveShape : public VectorPrimitiveBase
{
public:
	//~ Begin VectorPrimitiveBase Interface
	u32* Serialize(u32* pBuffer) override;
	std::istream& PopulateFromFile(std::istream& stream) override;
	//~ End VectorPrimitiveBase Interface

/// #TEMP: private: 

	// Colour
	u32 red = 0;
	u32 green = 0;
	u32 blue = 0;
};

