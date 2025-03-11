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

	// 1. Layer space -> Vector art space 
	VectorArtSpaceToLayerSpaceTransform transform = pRoot->GetVectorArtSpaceToLayerSpaceTransform();
	dragAndDropAreaBoundingBox = {};
	dragAndDropAreaBoundingBox.width = iconSize.width;
	dragAndDropAreaBoundingBox.height = iconSize.height;

	dragAndDropAreaBoundingBox.left += transform.position.x;
	dragAndDropAreaBoundingBox.top += transform.position.y;
	dragAndDropAreaBoundingBox.width *= transform.scale.x;
	dragAndDropAreaBoundingBox.height *= transform.scale.y;

	// 2. Vector art space -> Screen space
	const Vec2f screenSpacePosition = pParentCanvas->GetScreenSpacePosition();
	const Vec2f screenSpaceScale = pParentCanvas->GetScreenSpaceScale();
	
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

	if (bShouldDraggingClear)
	{
		SetDroppable(nullptr);	
	}

	onDragEventDelegates.Invoke({this, pOutDroppable});
}

void UIObjectDragAndDropArea::RecievedDropEvent(IDragAndDroppable& droppable)
{
	onDropEventDelegates.Invoke({this, &droppable});
}
