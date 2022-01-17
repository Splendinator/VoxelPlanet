#pragma once

#include "VectorPrimitiveShape.h"

/** VectorPrimitiveCircle
*
* This class is used to hold a vector art circle primitive
*/
class VectorPrimitiveCircle : public VectorPrimitiveShape
{
public:
	void Serialize(u32* pBuffer) override;

/// #TEMP: private:
	u32 x = 0;
	u32 y = 0;
	u32 rad = 0;
};

