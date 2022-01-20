#pragma once

#include "VectorPrimitiveFactoryBase.h"

/** VectorPrimitiveFactoryRectangle
*
* This class is used for #TEMP
*/
class VectorPrimitiveFactoryRectangle : public VectorPrimitiveFactoryBase
{
public:
	//~ Begin VectorPrimitiveFactoryBase Interface
	virtual VectorPrimitiveBase* CreatePrimitive() override;
	//~ End VectorPrimitiveFactoryBase Interface
};
