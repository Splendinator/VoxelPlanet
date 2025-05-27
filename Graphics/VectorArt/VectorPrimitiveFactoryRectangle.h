#pragma once

#include "VectorPrimitiveFactoryBase.h"

/** VectorPrimitiveFactoryRectangle
*
* This class is used to create a VectorPrimitiveRectangle
*/
class VectorPrimitiveFactoryRectangle : public VectorPrimitiveFactoryBase
{
public:
	//~ Begin VectorPrimitiveFactoryBase Interface
	virtual VectorPrimitiveBase* CreatePrimitive() override;
	//~ End VectorPrimitiveFactoryBase Interface
};
