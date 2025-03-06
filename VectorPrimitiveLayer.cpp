#include "pch.h"

#include "VectorPrimitiveLayer.h"

#include "DomImport/XMLUtils.h"
#include "VectorPrimitiveFactoryCircle.h"
#include "VectorPrimitiveFactoryLayer.h"
#include "VectorPrimitiveFactoryRectangle.h"
#include "VectorPrimitiveTypes.h"

VectorPrimitiveLayer::~VectorPrimitiveLayer()
{
	for (VectorPrimitiveBase* child : children)
	{
		delete child;
	}
}

u32* VectorPrimitiveLayer::Serialize(u32* pBuffer)
{
	// Push translation data that recursively effects any objects that are children of this layer
	*pBuffer = (u32)EPrimitiveType::PushLayer;
	++pBuffer;
	
	*pBuffer = (u32)(255 * opacity);
	++pBuffer;

	*pBuffer = positionOffset.x;
	++pBuffer;

	*pBuffer = positionOffset.y;
	++pBuffer;

	// Draw children
	for (VectorPrimitiveBase* child : children)
	{
		pBuffer = child->Serialize(pBuffer);
	}

	// Pop layer translations
	*pBuffer = (u32)EPrimitiveType::PopLayer;
	++pBuffer;

	return pBuffer;
}
#pragma optimize("", on)

std::istream& VectorPrimitiveLayer::PopulateFromFile(std::istream& stream)
{
	layerLabel = dmim::GetNextAttribute(stream, "inkscape:label");
	std::string style = dmim::GetNextAttribute(stream, "style");
	std::string opacityString = dmim::GetSubAttribute(style, "opacity");

	if (!opacityString.empty())
	{
		opacity = std::stof(opacityString);
	}
	
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
			pNewPrimitive->pParent = this;
			children.push_back(pNewPrimitive);
			pNewPrimitive->PopulateFromFile(stream);
		}
	}
	
	return stream;
}

VectorPrimitiveBase* VectorPrimitiveLayer::FindPrimitiveByLabelInternal(const std::string& label)
{
	// Label found
	if (label == layerLabel)
	{
		if (children.size() > 1)
		{
			// If the layer has multiple primitives we return it, otherwise we treat the primitive itself as having the name and return that
			// #NOTE: I'm not sure if this is intuitive to use?
			return this;
		}
		return children[0];
	}
	
	// Search children recursively
	for (VectorPrimitiveBase* child : children)
	{
		if (VectorPrimitiveBase* pFound = child->FindPrimitiveByLabelInternal(label))
		{
			return pFound;
		}
	}

	return nullptr;
}

const VectorPrimitiveBase* VectorPrimitiveLayer::FindPrimitiveUnderCursor(Vec2i cursorPos) const
{
	// Adjust by inverse positionOffset to keep everything accurate
	cursorPos = cursorPos - positionOffset;
	
	// Iterate backwards (highest layer first)
	for (int i = (int)children.size() - 1; i >= 0; i--)
	{
		const VectorPrimitiveBase* child = children[i];
		if (const VectorPrimitiveBase* pPrimitiveUnderCursor = child->FindPrimitiveUnderCursor(cursorPos))
		{
			return pPrimitiveUnderCursor;
		}
	}

	return nullptr;
}

void VectorPrimitiveLayer::SetChildren(const std::vector<VectorPrimitiveBase*> newChildren)
{
	for (VectorPrimitiveBase* child : children)
	{
		delete child;
	}
	children = newChildren;
}

bool VectorPrimitiveLayer::IsChildOfThis(const VectorPrimitiveBase* pPossibleChild) const
{
	for (VectorPrimitiveBase* pChild : children)
	{
		if (pChild == pPossibleChild || pChild->IsChildOfThis(pPossibleChild))
		{
			return true;
		}
	}

	return false;
}
