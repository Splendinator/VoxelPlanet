#include "pch.h"

#include "VectorPrimitiveLayer.h"

#include "DomImport/XMLUtils.h"
#include "VectorPrimitiveFactoryCircle.h"
#include "VectorPrimitiveFactoryLayer.h"
#include "VectorPrimitiveFactoryRectangle.h"

VectorPrimitiveLayer::~VectorPrimitiveLayer()
{
	for (VectorPrimitiveBase* child : children)
	{
		delete child;
	}
}

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
	layerLabel = dmim::GetNextAttribute(stream, "inkscape:label");

	struct FactoryEntry
	{
		std::string tag;
		VectorPrimitiveFactoryBase* pFactory = nullptr;
	};

	static VectorPrimitiveFactoryLayer factoryLayer;
	static VectorPrimitiveFactoryCircle factoryCircle;
	static VectorPrimitiveFactoryRectangle factoryRectangle;

	static FactoryEntry factories[]
	{
		{"g", &factoryLayer},
		{"circle", &factoryCircle},
		{"rect", &factoryRectangle},
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

VectorPrimitiveBase* VectorPrimitiveLayer::FindPrimitiveByLabel(const std::string& label)
{
	// Label found
	if (label == layerLabel)
	{
		DOMLOG_ERROR_IF(children.size() > 1, "Found correct label but there's more than one child. Returning first");
		return children[0];
	}
	
	// Search children recursively
	for (VectorPrimitiveBase* child : children)
	{
		if (VectorPrimitiveBase* pFound = child->FindPrimitiveByLabel(label))
		{
			return pFound;
		}
	}

	return nullptr;
}
