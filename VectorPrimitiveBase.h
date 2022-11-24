#pragma once

#include "DomMath/Types.h"
#include "DomUtils/Pointers.h"

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
	// #TODO: GLSL makes it very awkward to use anything less than u32 (cant use char, can't use pointers) so I'm just using ints for now. This can be rewritten to use bytes here
	virtual u32* Serialize(u32* pBuffer) = 0;

	// This should be overridden to read in the contents of this vector primitive from an SVG file (xml file).
	// This should start reading from right after the associated tag. i.e reading in a circle will start after "<circle"
	virtual std::istream& PopulateFromFile(std::istream& stream) = 0;
	
	// This can be overwritten to find a primitive by its label. 
	// This is used to search recursively for the first non-nullptr primitive with the given label.
	virtual VectorPrimitiveBase* FindPrimitiveByLabelInternal(const std::string& label) { return nullptr; };

	// Templated version of FindPrimitiveByLabel() for ease of use. Throws an error if the primitive is not found for ease of debugging vector art assets.
	template<typename TClass>
	TransientPtr<TClass> FindPrimitiveByLabel(const std::string& label);
};

template<typename TClass>
TransientPtr<TClass> VectorPrimitiveBase::FindPrimitiveByLabel(const std::string& label)
{
	static_assert(std::is_base_of<VectorPrimitiveBase, TClass>::value, "TClass must inherit from VectorPrimitiveBase");

	TClass* pPrimitive = dynamic_cast<TClass*>(FindPrimitiveByLabelInternal(label));

	DOMLOG_ERROR_IF(pPrimitive == nullptr, "Failed to find primitive with label:", label);

	return pPrimitive;
}
