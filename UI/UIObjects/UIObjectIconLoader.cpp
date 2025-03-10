#include "pch.h"

#include "UIObjectIconLoader.h"
#include "VectorArt.h"

void UIObjectIconLoader::LoadUIIcon(const std::string& file, const std::string& primitiveName /*= ""*/)
{
	std::unique_ptr<VectorArt> pLoadedVectorArt(new VectorArt(file.c_str()));

	TransientPtr<VectorPrimitiveLayer> pReplaceWithThis = nullptr;
	if (primitiveName.empty())
	{
		pReplaceWithThis = pLoadedVectorArt->GetRootLayer();
	}
	else
	{
		pReplaceWithThis = pLoadedVectorArt->FindLayerByLabel(primitiveName);
	}
	
	pReplaceThis->StealChildrenFromLayer(pReplaceWithThis);

	// We want to scale the new icon to the same size as the placeholder one
	Box2f newIconBoundingBox = pReplaceThis->GetBoundingBox();
	DOMLOG_ERROR_IF(newIconBoundingBox.GetWidth() == 0.0f || newIconBoundingBox.GetHeight() == 0.0f)

	const float scaleX = iconSize.GetWidth() / newIconBoundingBox.GetWidth();
	const float scaleY = iconSize.GetHeight() / newIconBoundingBox.GetHeight();
	pReplaceThis->SetScale({scaleX, scaleY});
}

void UIObjectIconLoader::Init(VectorPrimitiveLayer* pRoot)
{
	UIObjectBase::Init(pRoot);
	 
	pReplaceThis = pRoot;

	iconSize = pReplaceThis->GetBoundingBox();
}

void UIObjectIconLoader::Uninit()
{
	UIObjectBase::Uninit();
}