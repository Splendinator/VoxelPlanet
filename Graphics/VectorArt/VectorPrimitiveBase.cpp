#include "pch.h"
#include "VectorPrimitiveBase.h"

#include "VectorPrimitiveLayer.h"

VectorArtSpaceToLayerSpaceTransform VectorPrimitiveBase::GetVectorArtSpaceToLayerSpaceTransform() const
{
	return pParent->GetVectorArtSpaceToLayerSpaceTransform();
}
