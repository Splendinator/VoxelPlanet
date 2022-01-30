#pragma once

#include "VectorPrimitiveLayer.h"

class VectorPrimitiveLayer;

/** VectorArt
*
* This class is used for managing all of the primitive factories and using them to read in a file.
* This class is not trivially copyable, use move semantics to move it around. 
*/
class VectorArt
{
public:
	
	VectorArt() = default;
	VectorArt(VectorArt&& other)
		: pRootLayer(std::move(other.pRootLayer))
		, pageWidth(other.pageWidth)
		, pageHeight(other.pageHeight)
	{}
	explicit VectorArt(const char* pFilePath);
	~VectorArt() = default;

	VectorArt& operator=(VectorArt&& other);
	
	// Deleted
	VectorArt& operator=(VectorArt& other) = delete;
	VectorArt(const VectorArt& other) = delete;
	
	void Serialize(u32* pBuffer);

private:
	std::unique_ptr<VectorPrimitiveLayer> pRootLayer;

	u32 pageWidth = 0;
	u32 pageHeight = 0;
};
