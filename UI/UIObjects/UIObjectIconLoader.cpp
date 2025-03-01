#include "pch.h"

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

	pReplaceThis->SetChildren(pReplaceWithThis->GetChildren());
}

void UIObjectIconLoader::Init(VectorPrimitiveBase* pRoot)
{
	UIObjectBase::Init(pRoot);

	pReplaceThis = pRoot->pParent;
	DOMLOG_ERROR_IF(pReplaceThis == nullptr, "We expect a parent. I don't even know how this can be null")

	// #TEMP: We need a GetBoundingBox() in VectorPrimitiveBase. We then need to transform the new icon's bounding box into the existing bounding box, similar to what we've done in UICanvas::FindPrimitiveUnderCursor() 
}

void UIObjectIconLoader::Uninit()
{
	
	UIObjectBase::Uninit();
}