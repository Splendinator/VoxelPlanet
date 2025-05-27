#include "pch.h"

#include "VectorPrimitiveFactoryLayer.h"

#include "VectorPrimitiveLayer.h"

VectorPrimitiveBase* VectorPrimitiveFactoryLayer::CreatePrimitive()
{
	return new VectorPrimitiveLayer;
}
