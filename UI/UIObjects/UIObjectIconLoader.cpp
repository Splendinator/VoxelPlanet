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
	
	pReplaceThis->StealChildrenFromLayer(pReplaceWithThis);
	pReplaceThis->SetScale({2.0f, 2.0f});
	
	//DOMLOG_ERROR_IF(newIconBoundingBox.GetWidth() == 0.0f || newIconBoundingBox.GetHeight() == 0.0f)
	//const float scaleX = iconSize.GetWidth() / newIconBoundingBox.GetWidth();
	//const float scaleY = iconSize.GetHeight() / newIconBoundingBox.GetHeight();
	//pReplaceThis->SetScale({scaleX, scaleY});
}

void UIObjectIconLoader::Init(VectorPrimitiveBase* pRoot)
{
	UIObjectBase::Init(pRoot);
	 
	if (VectorPrimitiveLayer* pLayer = dynamic_cast<VectorPrimitiveLayer*>(pRoot))
	{
		pReplaceThis = pLayer;
	}
	else
	{
		pReplaceThis = pRoot->pParent;
	}
	
	DOMLOG_ERROR_IF(pReplaceThis == nullptr, "We expect a parent. I don't even know how this can be null")

	iconSize = pReplaceThis->GetBoundingBox();
}

void UIObjectIconLoader::Uninit()
{
	UIObjectBase::Uninit();
}