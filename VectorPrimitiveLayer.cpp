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
	
	*pBuffer = *reinterpret_cast<u32*>(&opacity);
	++pBuffer;

	*pBuffer = (u32)positionOffset.x;
	++pBuffer;

	*pBuffer = (u32)positionOffset.y;
	++pBuffer;

	*pBuffer = *reinterpret_cast<u32*>(&scale.x);
	++pBuffer;

	*pBuffer = *reinterpret_cast<u32*>(&scale.y);
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
	// layer space (relative to the top left of the bounding box of the layer) so we translate to that by refreshing the bounding box with a {0,0,0,0} currentBoundingBox. 
	RefreshBoundingBox();
	
	return stream;
}

VectorPrimitiveLayer* VectorPrimitiveLayer::FindLayerByLabel(const std::string& label)
{
	// Label found
	if (label == layerLabel)
	{
		return this;
	}
	
	// Search children recursively
	for (VectorPrimitiveBase* child : children)
	{
		if (VectorPrimitiveLayer* pFound = child->FindLayerByLabel(label))
		{
			return pFound;
		}
	}
	
	return nullptr;
}

const VectorPrimitiveBase* VectorPrimitiveLayer::FindPrimitiveUnderCursor(Vec2i cursorPos) const
{
	// Adjust by inverse positionOffset and scaleOffset to keep everything accurate
	cursorPos -= Vec2i((int)positionOffset.x, (int)positionOffset.y);
	cursorPos.x = (i32)(cursorPos.x / scale.x);
	cursorPos.y = (i32)(cursorPos.y / scale.y);
	
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

void VectorPrimitiveLayer::AdjustPositionWithinLayer(Vec2f delta)
{
	positionOffset.x += delta.x;
	positionOffset.y += delta.y;
}

void VectorPrimitiveLayer::StealChildrenFromLayer(VectorPrimitiveLayer* pOtherLayer)
{
	if (pOtherLayer == nullptr)
	{
		return;
	}
	
	for (VectorPrimitiveBase* child : children)
	{
		delete child;
	}

	children = std::move(pOtherLayer->children);
	
	// Steal size of bounding box from other layer, but keep our own position/scale etc.
	currentBoundingBox.SetWidth(pOtherLayer->currentBoundingBox.GetWidth());
	currentBoundingBox.SetHeight(pOtherLayer->currentBoundingBox.GetHeight());
}

void VectorPrimitiveLayer::RefreshBoundingBox()
{
	if (children.size() == 0)
	{
		return;
	}

	// 1. Calculate new bounding box
	float leftMost = std::numeric_limits<float>::max();
	float rightMost = std::numeric_limits<float>::lowest();
	float topMost = std::numeric_limits<float>::max();
	float bottomMost = std::numeric_limits<float>::lowest();

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

	// std::max() is used here to cull anything that ends up off the left or top of the canvas
	const Box2f newBoundingBox = Box2f::InitFromLeftRightTopBottom(std::max(leftMost, 0.0f), rightMost, std::max(topMost, 0.0f), bottomMost);

	// 2. Move all children to be relative to the top left of our new bounding box.
	SetPositionOffset( { newBoundingBox.GetLeft(), newBoundingBox.GetTop() });
	
	const float xDelta = currentBoundingBox.GetLeft() - newBoundingBox.GetLeft();
	const float yDelta = currentBoundingBox.GetTop() - newBoundingBox.GetTop();
	
	for (VectorPrimitiveBase* pChild : children)
	{
		pChild->AdjustPositionWithinLayer({xDelta, yDelta});
	}

	currentBoundingBox = newBoundingBox;
}
