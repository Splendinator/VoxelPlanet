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

	*pBuffer = (u32)positionOffset.x;
	++pBuffer;

	*pBuffer = (u32)positionOffset.y;
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

	// .svg files save all primitives in absolute space (relative to the top left of the document) but it's far more useful for us to use
	// layer space (relative to the top left of the bounding box of the layer) so we translate to that by refreshing the bounding box with a {0,0,0,0} lastBoundingBox. 
	RefreshBoundingBox();
	
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
	cursorPos = cursorPos - Vec2i((int)positionOffset.x, (int)positionOffset.y);
	
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

	RefreshBoundingBox();
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

Box2f VectorPrimitiveLayer::GetBoundingBox() const
{
	if (children.size() == 0)
	{
		return {};
	}
	
	float leftMost = std::numeric_limits<float>::max();
	float rightMost = std::numeric_limits<float>::min();
	float topMost = std::numeric_limits<float>::max();
	float bottomMost = std::numeric_limits<float>::min();

	for (VectorPrimitiveBase* pChild : children)
	{
		Box2f childBoundingBox = pChild->GetBoundingBox();

		const float childLeft = childBoundingBox.GetLeft();
		const float childRight = childBoundingBox.GetRight();
		const float childTop = childBoundingBox.GetTop();
		const float childBottom = childBoundingBox.GetBottom();

		leftMost = std::min(childLeft, leftMost);
		rightMost = std::max(childRight, rightMost);
		topMost = std::min(childTop, topMost);
		bottomMost = std::max(childBottom, bottomMost);
	}

	return Box2f::InitFromLeftRightTopBottom(leftMost, rightMost, topMost, bottomMost);
}

void VectorPrimitiveLayer::AdjustPositionWithinLayer(Vec2f delta)
{
	for (VectorPrimitiveBase* pChild : children)
	{
		pChild->AdjustPositionWithinLayer(delta);
	}
}

void VectorPrimitiveLayer::RefreshBoundingBox()
{
	Box2f boundingBox = GetBoundingBox();
	SetPositionOffset( { boundingBox.GetLeft(), boundingBox.GetTop() });

	const float xDelta = lastBoundingBox.GetLeft() - boundingBox.GetLeft();
	const float yDelta = lastBoundingBox.GetTop() - boundingBox.GetTop();
	
	for (VectorPrimitiveBase* pChild : children)
	{
		pChild->AdjustPositionWithinLayer({xDelta, yDelta});
	}

	lastBoundingBox = boundingBox;
}
