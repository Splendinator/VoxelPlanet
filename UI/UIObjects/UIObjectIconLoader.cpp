#include "pch.h"

#include "UI/UICanvas.h"
#include "UIObjectIconLoader.h"
#include "VectorArt.h"
#include "VectorPrimitiveRectangle.h"

void UIObjectIconLoader::LoadUIIcon(const std::string& file, const std::string& primitiveName /*= ""*/)
{
	pLoadedVectorArt.reset(new VectorArt(file.c_str()));
	
	if (primitiveName.empty())
	{
		pReplaceWithThis = pLoadedVectorArt->GetRootLayer();
	}
	else
	{
		// #JANK: Should this use pParent instead of getting the layer directly?
		pReplaceWithThis = pLoadedVectorArt->FindLayerByLabel(primitiveName);
	}
	
	pReplaceThis->SetChildren(pReplaceWithThis->GetChildren());
	pReplaceThis->SetPositionOffset({(float)pRectangleForSize->GetX(), (float)pRectangleForSize->GetY()});
	pReplaceThis->SetScale({2.0f, 2.0f});
}

void UIObjectIconLoader::Init(VectorPrimitiveBase* pRoot)
{
	UIObjectBase::Init(pRoot);

	pRectangleForSize = dynamic_cast<VectorPrimitiveRectangle*>(pRoot);
	DOMLOG_ERROR_IF(pRectangleForSize == nullptr, "We expect a parent. I don't even know how this can be null")
	
	pReplaceThis = pRoot->pParent;
	DOMLOG_ERROR_IF(pReplaceThis == nullptr, "We expect a parent. I don't even know how this can be null")
}

void UIObjectIconLoader::Uninit()
{
	UIObjectBase::Uninit();
}