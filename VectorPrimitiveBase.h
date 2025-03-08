#pragma once

#include <DomMath/Vec2.h>

#include "DomMath/Types.h"
#include "DomUtils/Pointers.h"

class VectorPrimitiveLayer;

/** VectorPrimitiveBase
*
* This class is used to represent a single primitive (circle, square, layer etc.) of a piece of vector art.
*/
class VectorPrimitiveBase
{
public:
	virtual ~VectorPrimitiveBase() = default;

	// This should be overridden to serialize the primitive to the input buffer.
	// Serialized vector art is decoded and rendered by VectorFrag.frag so write this function in such a way that it can be replicated inversely in GLSL (c code)
	// #JANK: GLSL makes it very awkward to use anything less than u32 (cant use char, can't use pointers) so I'm just using ints for now. This can be rewritten to use bytes here
	virtual u32* Serialize(u32* pBuffer) = 0;

	// This should be overridden to read in the contents of this vector primitive from an SVG file (xml file).
	// This should start reading from right after the associated tag. i.e reading in a circle will start after "<circle"
	virtual std::istream& PopulateFromFile(std::istream& stream) = 0;

	// Should returns the 2d bounding box of this shape
	virtual Box2f GetBoundingBox() const = 0;

	// Children should override this to change their X and Y co-ordinates.
	// This is just used to adjust primitives within a layer to keep them relative to the top left of the layer's bounding box.
	// **Do not use this** to move all primitives in a layer. Use VectorPrimitiveLayer::SetPositionOffset()
	virtual void AdjustPositionWithinLayer(Vec2f delta) = 0;
	
	// Recursive function to find the top hovered over primitive
	// Remember the input cursorPos will be in vector-art space, you most likely can't just plug in the raw mouse coordinates
	virtual const VectorPrimitiveBase* FindPrimitiveUnderCursor(Vec2i cursorPos) const { return nullptr; }

	// Returns whether the the passed in primitive is a child 
	virtual bool IsChildOfThis(const VectorPrimitiveBase* pPossibleChild) const { return false; }

	// Recursively goes through the layers until it finds one with the correct label
	virtual VectorPrimitiveLayer* FindLayerByLabel(const std::string& label) { return nullptr; }
	
	// This is the layer directly above this; this will be nullptr for the root layer in a file.
	VectorPrimitiveLayer* pParent = nullptr;

protected:
	
	// Child classes will need to call this whenever they change shape/size to keep the layer bounding box accurate
	void RefreshParentLayerBoundingBox();
};
