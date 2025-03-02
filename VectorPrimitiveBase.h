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
	
	// Recursive function to find the top hovered over primitive
	// Remember the input cursorPos will be in vector-art space, you most likely can't just plug in the raw mouse coordinates
	virtual const VectorPrimitiveBase* FindPrimitiveUnderCursor(Vec2i cursorPos) const { return nullptr; }

	// Returns whether the the passed in primitive is a child 
	virtual bool IsChildOfThis(const VectorPrimitiveBase* pPossibleChild) const { return false; }

	// Templated version of FindPrimitiveByLabel() for ease of use. Throws an error if the primitive is not found for ease of debugging vector art assets.
	template<typename TClass>
	TransientPtr<TClass> FindPrimitiveByLabel(const std::string& label);

	// This can be overwritten to find a primitive by its label. 
	// This is used to search recursively for the first non-nullptr primitive with the given label.
	// Note: This doesn't return the VectorPrimitiveLayer with the label but the primitive below it
	virtual VectorPrimitiveBase* FindPrimitiveByLabelInternal(const std::string& label) { return nullptr; }
	
	// This is the layer directly above this; this will be nullptr for the root layer in a file.
	VectorPrimitiveLayer* pParent = nullptr;

	// #TODO: This is just a hacky way for me to set UIIconLoader icons to the correct location for now.
	// #TODO: We will eventually want a way to add Transforms (scale, rotation, position) to each VectorArtPrimitiveLayer
	// #TODO: and it will recursively stack them together as you go down the layers, but this will need to be integrated into the rendering pipeline
	// This adds to the X and Y co-ordinates.
	virtual void DebugAddX(u32 deltaX) {}
	virtual void DebugAddY(u32 deltaY) {}
};

template<typename TClass>
TransientPtr<TClass> VectorPrimitiveBase::FindPrimitiveByLabel(const std::string& label)
{
	static_assert(std::is_base_of<VectorPrimitiveBase, TClass>::value, "TClass must inherit from VectorPrimitiveBase");

	TClass* pPrimitive = dynamic_cast<TClass*>(FindPrimitiveByLabelInternal(label));

	DOMLOG_ERROR_IF(pPrimitive == nullptr, "Failed to find primitive with label:", label);

	return pPrimitive;
}
