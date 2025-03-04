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
	void DebugAddX(u32 deltaX) override;
	void DebugAddY(u32 deltaY) override;
	bool IsChildOfThis(const VectorPrimitiveBase* pPossibleChild) const override;
	//~ End VectorPrimitiveBase Interface

	const std::vector<VectorPrimitiveBase*>& GetChildren() const { return children; }
	void SetChildren(const std::vector<VectorPrimitiveBase*> newChildren);

private:

	std::string layerLabel; // This is the name of the layer in inkscape
	float opacity = 1.0f; // Opacity of this layer

	// #TODO: This should be a unique ptr
	std::vector<VectorPrimitiveBase*> children;
};

