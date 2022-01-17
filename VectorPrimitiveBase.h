#pragma once

#include "DomMath/Types.h"

/** VectorPrimitiveBase
*
* This class is used to represent a single primitive (circle, square, layer etc.) of a piece of vector art.
*/
class VectorPrimitiveBase
{
public:

	// This should be overridden to serialize the primitive to the input buffer.
	// Serialized vector art is decoded and rendered by VectorFrag.frag so write this function in such a way that it can be replicated inversely in GLSL (c code)
	// #TODO: GLSL makes it very awkward to use anything less than u32 (cant use char, can't use pointers) so I'm just using ints for now. This can be rewritten to use bytes here
	// 
	virtual void Serialize(u32* pBuffer) = 0;
};

