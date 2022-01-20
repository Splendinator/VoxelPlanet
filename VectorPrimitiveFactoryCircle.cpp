#include "pch.h"

#include "VectorPrimitiveFactoryCircle.h"

#include "VectorPrimitiveCircle.h"

VectorPrimitiveBase* VectorPrimitiveFactoryCircle::CreatePrimitive()
{
	return new VectorPrimitiveCircle;
}
