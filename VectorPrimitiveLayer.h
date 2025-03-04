#pragma once

#include "VectorPrimitiveBase.h"

/** VectorPrimitiveLayer
*
* This class is used to represent a layer for vector art.
* It basically holds a list of other primitives and any transformations applied to this layer are applied to everything underneath it too.
*/
class VectorPrimitiveLayer : public VectorPrimitiveBase
{
public:
	~VectorPrimitiveLayer() override;

	//~ Begin VectorPrimitiveBase Interface
	u32* Serialize(u32* pBuffer) override;
	std::istream& PopulateFromFile(std::istream& stream) override;
	VectorPrimitiveBase* FindPrimitiveByLabelInternal(const std::string& label) override;
	const VectorPrimitiveBase* FindPrimitiveUnderCursor(Vec2i cursorPos) const override;
	bool IsChildOfThis(const VectorPrimitiveBase* pPossibleChild) const override;
	//~ End VectorPrimitiveBase Interface

	const std::vector<VectorPrimitiveBase*>& GetChildren() const { return children; }
	void SetChildren(const std::vector<VectorPrimitiveBase*> newChildren);

	void SetPositionOffset(Vec2i inPositionOffset) { positionOffset = inPositionOffset; }
	
private:

	// This is the name of the layer in inkscape
	std::string layerLabel; 

	// Opacity of all things inside this layer
	float opacity = 1.0f; 

	// #TODO: Right now all primitives use aboslute co-ordinates when read in from a .svg.
	// #TODO: We instead need to use this and force relative co-ordinates. To do this we need to calcualte a bounding box around the layer.   
	// Position offset of all things inside this layer
	Vec2i positionOffset = {};
	
	// #NOTE: Rotation + scale?

	// #TODO: This should be a unique ptr
	std::vector<VectorPrimitiveBase*> children;
};

