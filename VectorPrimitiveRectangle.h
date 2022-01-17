#pragma once

#include "VectorPrimitiveShape.h"

/** VectorPrimitiveRectangle
*
* This class is used to hold a vector art rectangle primitive
*/
class VectorPrimitiveRectangle : public VectorPrimitiveShape
{
public:
	void Serialize(u32* pBuffer) override;

/// #TEMP: private:
};

