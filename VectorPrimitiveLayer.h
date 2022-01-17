#pragma once

#include "VectorPrimitiveBase.h"

/** VectorPrimitiveLayer
*
* This class is used to represent a layer for vector art.
* It basically holds a list of other primitives and any transformations applied to this layer are applied to everything underneathe it too.
*/
class VectorPrimitiveLayer : public VectorPrimitiveBase
{
public:
	void Serialize(u32* pBuffer) override;

/// #TEMP: private:
};

