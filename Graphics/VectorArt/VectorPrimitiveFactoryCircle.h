#pragma once

#include "VectorPrimitiveFactoryBase.h"

/** VectorPrimitiveFactoryCircle
*
* This class is used to create a VectorPrimitiveCircle
*/
class VectorPrimitiveFactoryCircle : public VectorPrimitiveFactoryBase
{
public:
	//~ Begin VectorPrimitiveFactoryBase Interface
	virtual VectorPrimitiveBase* CreatePrimitive() override;
	//~ End VectorPrimitiveFactoryBase Interface
};
