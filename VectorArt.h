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

	template<typename TClass>
	TClass* FindPrimitiveByLabel(const std::string& label);

	// Page dimensions are used to figure out size of primitives relative to the quad they are on.
	// i.e if page width is 1000 units wide and you scale a primitive to be 500 units wide, it will be half the width of the quad.
	u32 GetPageWidth() { return pageWidth; }
	u32 GetPageHeight() { return pageHeight; }

private:

	VectorPrimitiveBase* FindPrimitiveByLabelInternal(const std::string& label);

	std::unique_ptr<VectorPrimitiveLayer> pRootLayer;

	u32 pageWidth = 0;
	u32 pageHeight = 0;
};

template<typename TClass>
TClass* VectorArt::FindPrimitiveByLabel(const std::string& label)
{
	if (VectorPrimitiveBase* pPrimitive = FindPrimitiveByLabelInternal(label))
	{
		TClass* pReturnedPrimitive = dynamic_cast<TClass*>(pPrimitive);
		DOMLOG_WARN_IF(pReturnedPrimitive == nullptr, "Could not cast primitive '", label, "'to type:", typeid(TClass).name());
		return pReturnedPrimitive;
	}

	DOMLOG_WARN("No label '", label, "' found.");

	return nullptr;
}
