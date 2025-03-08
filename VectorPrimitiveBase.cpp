#include "pch.h"
#include "VectorPrimitiveBase.h"

#include "VectorPrimitiveLayer.h"

void VectorPrimitiveBase::RefreshParentLayerBoundingBox()
{
	if (pParent)
	{
		pParent->RefreshBoundingBox();
	}
}
