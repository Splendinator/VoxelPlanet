#pragma once

#include "VectorPrimitiveFactoryBase.h"

/** VectorPrimitiveFactoryLayer
*
* This class is used for VectorPrimitiveLayer
*/
class VectorPrimitiveFactoryLayer : public VectorPrimitiveFactoryBase
{
public:

	//~ Begin VectorPrimitiveFactoryBase Interface
	virtual VectorPrimitiveBase* CreatePrimitive() override;
	//~ End VectorPrimitiveFactoryBase Interface

};
