#pragma once

#include "VectorPrimitiveBase.h"

/** VectorPrimitiveLayer
*
* This class is used to represent a layer for vector art.
* It basically holds a list of other primitives and any transformations applied to this layer are applied to everything underneathe it too.
*/
class VectorPrimitiveLayer : public VectorPrimitiveBase
{
public:
	~VectorPrimitiveLayer();

	//~ Begin VectorPrimitiveBase Interface
	u32* Serialize(u32* pBuffer) override;
	std::istream& PopulateFromFile(std::istream& stream) override;
	VectorPrimitiveBase* FindPrimitiveByLabelInternal(const std::string& label) override;
	//~ End VectorPrimitiveBase Interface

private:

	std::string layerLabel; // This is the name of the layer in inkscape

	std::vector<VectorPrimitiveBase*> children;
};

