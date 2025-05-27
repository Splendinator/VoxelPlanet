#include "pch.h"

#include "VectorPrimitiveFactoryRectangle.h"

#include "VectorPrimitiveRectangle.h"

VectorPrimitiveBase* VectorPrimitiveFactoryRectangle::CreatePrimitive()
{
	return new VectorPrimitiveRectangle;
}
