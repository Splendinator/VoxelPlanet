#pragma once
#include "VectorPrimitiveBase.h"
class VectorPrimitiveShape : public VectorPrimitiveBase
{
public:
	void Serialize(u32* pBuffer) override;

/// #TEMP: private: 

	// Colour
	u32 red = 0;
	u32 green = 0;
	u32 blue = 0;
};

