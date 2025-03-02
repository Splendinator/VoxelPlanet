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
		pReplaceWithThis = pLoadedVectorArt->FindPrimitiveByLabel<VectorPrimitiveBase>(primitiveName)->pParent;
	}

	// #TODO: Right now this assumes the icons are loaded in from 0,0 in their respective files.
	// #TODO: I'm not sure of the best way to fix this as the .svg file saves absolute co-ordinates.
	// #TODO: Likely we need to correct for this with bounding boxes when we add stacking layer transforms (i.e the TODO in VectorPrimitiveBase)   
	pReplaceThis->SetChildren(pReplaceWithThis->GetChildren());
	pReplaceThis->DebugAddX(pRectangleForSize->GetX());
	pReplaceThis->DebugAddY(pRectangleForSize->GetY());
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