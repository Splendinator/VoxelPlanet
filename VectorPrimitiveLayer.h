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
	Box2f GetBoundingBox() const override;
	void AdjustPositionWithinLayer(Vec2f delta) override;
	//~ End VectorPrimitiveBase Interface

	const std::vector<VectorPrimitiveBase*>& GetChildren() const { return children; }
	void SetChildren(const std::vector<VectorPrimitiveBase*> newChildren);

	void SetPositionOffset(Vec2f inPositionOffset) { positionOffset = inPositionOffset; }

	// This will refresh the layer's bounding box such that the top-left most child primitive has co-ordinates (0,0).
	// Certain things like moving a primitive within a layer require the layer's bounding box to be refreshed for it to be accurate
	// If the bounding box is inaccurate it can mess with certain renderer operations like scaling the layer
	void RefreshBoundingBox();
	
private:

	// This is the name of the layer in inkscape
	std::string layerLabel; 

	// Opacity of all things inside this layer
	float opacity = 1.0f; 
	
	// Position offset of all things inside this layer
	Vec2f positionOffset = {};

	// This is the previous bounding box, this is used when recalculating the bounding box to figure out the delta.
	Box2f lastBoundingBox;
	
	// #NOTE: Rotation + scale?

	// #TODO: This should be a unique ptr
	std::vector<VectorPrimitiveBase*> children;
};

