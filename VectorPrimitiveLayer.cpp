#include "pch.h"

#include "VectorPrimitiveLayer.h"

#include "DomImport/XMLUtils.h"
#include "VectorPrimitiveFactoryCircle.h"
#include "VectorPrimitiveFactoryLayer.h"

u32* VectorPrimitiveLayer::Serialize(u32* pBuffer)
{
	for (VectorPrimitiveBase* child : children)
	{
		pBuffer = child->Serialize(pBuffer);
	}

	return pBuffer;
}

std::istream& VectorPrimitiveLayer::PopulateFromFile(std::istream& stream)
{
	struct FactoryEntry
	{
		std::string tag;
		VectorPrimitiveFactoryBase* pFactory = nullptr;
	};

	static VectorPrimitiveFactoryLayer factoryLayer;
	static VectorPrimitiveFactoryCircle factoryCircle;

	static FactoryEntry factories[]
	{
		{"g", &factoryLayer},
		{"ellipse", &factoryCircle},
	};

	auto FindFactory = [](std::string tag) -> VectorPrimitiveFactoryBase*
	{
		for (FactoryEntry& factory : factories)
		{
			if (factory.tag == tag)
			{
				return factory.pFactory;
			}
		}
		return nullptr;
	};

	std::string readTag;
	
	while (readTag != "/g>")
	{
		readTag = dmim::GetNextTag(stream);
		if (VectorPrimitiveFactoryBase* pFactory = FindFactory(readTag))
		{
			VectorPrimitiveBase* pNewPrimitive = pFactory->CreatePrimitive();
			children.push_back(pNewPrimitive);
			pNewPrimitive->PopulateFromFile(stream);
		}
	}

	return stream;
}
