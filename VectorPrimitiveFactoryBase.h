#pragma once

class VectorPrimitiveBase;

/** VectorPrimitiveFactoryBase
*
* This is the base class of vector factories. Vector factories are used to create a VectorPrimitiveBase (see VectorPrimitiveFactorManager for usage)
* The primitives returned from CreatePrimitive are on the heap and will need deleting after use.
*/
class VectorPrimitiveFactoryBase
{
public:
	virtual VectorPrimitiveBase* CreatePrimitive() = 0;
};
