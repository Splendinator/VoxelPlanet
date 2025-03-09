#include "pch.h"

#include "UIObjectDragAndDropArea.h"

#include "UI/UICanvas.h"
#include "UI/DragAndDrop/DragAndDropManager.h"
#include "UI/DragAndDrop/IDragAndDroppable.h"

void UIObjectDragAndDropArea::Setup(UIObjectDragAndDropSetupParams params)
{
	DOMLOG_ERROR_IF(params.pDirectoryData == nullptr || params.pDragAndDropManager == nullptr, "Invalid params");

	pDirectoryData = params.pDirectoryData;
	pDragAndDropManager = params.pDragAndDropManager;

	if (pDragAndDropManager)
	{
		pDragAndDropManager->RegisterArea(*this, params.areaParams);
	}
}

void UIObjectDragAndDropArea::SetDroppable(IDragAndDroppable* pDroppable)
{
	if (!pDirectoryData)
	{
		DOMLOG_ERROR("No director data, did you forget to call Setup()");
		return;
	}
	
	pReplaceThis->SetVisible(pDroppable != nullptr);
	if (pDroppable)
	{
		LoadUIIcon(pDroppable->GetDragAndDropIconFile(*pDirectoryData), pDroppable->GetOptionalDragAndDropIconLayer());
	}

	pShownDragAndDroppable = pDroppable;
}

void UIObjectDragAndDropArea::Init(VectorPrimitiveLayer* pRoot)
{
	UIObjectIconLoader::Init(pRoot);

	pReplaceThis->SetVisible(false);

	// Set up dragAndDropAreaBoundingBox to a screen space bounding box around the icon.
	// #TODO: This probably doesn't work with non-uniform VectorArtPrimitiveLayer scaling or if you're not on the root layer.
	// #TODO: We need a way to recursively go down the layers from the root to get the cumulative translation so we can do Layer Space -> Vector Art Space -> Screen Space
	const Vec2f screenSpacePosition = pParentCanvas->GetScreenSpacePosition();
	const Vec2f screenSpaceScale = pParentCanvas->GetScreenSpaceScale();
	dragAndDropAreaBoundingBox = iconSize;

	dragAndDropAreaBoundingBox.left *= screenSpaceScale.x;
	dragAndDropAreaBoundingBox.top *= screenSpaceScale.y;
	dragAndDropAreaBoundingBox.width *= screenSpaceScale.x;
	dragAndDropAreaBoundingBox.height *= screenSpaceScale.y;
	
	dragAndDropAreaBoundingBox.left += screenSpacePosition.x;
	dragAndDropAreaBoundingBox.top += screenSpacePosition.y;
	
}

void UIObjectDragAndDropArea::Uninit()
{
	if (pDragAndDropManager)
	{
		pDragAndDropManager->UnregisterArea(*this);
	}
	
	UIObjectIconLoader::Uninit();
}

void UIObjectDragAndDropArea::RecievedDragEvent(IDragAndDroppable*& pOutDroppable)
{
	pOutDroppable = pShownDragAndDroppable.Get();
}

void UIObjectDragAndDropArea::RecievedDropEvent(IDragAndDroppable& droppable)
{
	
}
