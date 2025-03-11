#include "pch.h"

#include "UIObjectHotbarSlot.h"

#include "HotbarManager/IHotbarItem.h"
#include "UI/DragAndDrop/IDragAndDroppable.h"
#include "UIObjectDragAndDropArea.h"
#include "HotbarManager/HotbarManager.h"

void UIObjectHotbarSlot::Init(VectorPrimitiveLayer* pRoot)
{
	onDropEventDelegate.Bind(this, &UIObjectHotbarSlot::OnDropEvent);
	onDragEventDelegate.Bind(this, &UIObjectHotbarSlot::OnDragEvent);
	onSlotUpdatedDelegate.Bind(this, &UIObjectHotbarSlot::OnHotbarUpdated);
	
	pDragAndDropArea = AddUIObject<UIObjectDragAndDropArea>(pRoot, "Icon");

	if (pDragAndDropArea)
	{
		pDragAndDropArea->SetShouldDraggingClear(true);
		pDragAndDropArea->onDropEventDelegates.Add(onDropEventDelegate);
		pDragAndDropArea->onDragEventDelegates.Add(onDragEventDelegate);
	}
}

void UIObjectHotbarSlot::Uninit()
{
	pDragAndDropArea.Clear();
	if (pHotbarManager)
	{
		pHotbarManager->onHotbarSlotUpdatedDelegates.Remove(onSlotUpdatedDelegate);
	}
}

void UIObjectHotbarSlot::Setup(const UIObjectHotbarSlotSetupParams& params)
{
	if (pDragAndDropArea)
	{
		pDragAndDropArea->Setup(params.dragAndDropParams);
	}

	pHotbarManager = &params.hotbarManager;
	slotIndex = params.slotIndex;

	pHotbarManager->onHotbarSlotUpdatedDelegates.Add(onSlotUpdatedDelegate);
}

void UIObjectHotbarSlot::OnDropEvent(const UIObjectDragAndDropDelegateParams& params)
{
	if (pHotbarManager)
	{
		if (IHotbarItem* pHotbarItem = dynamic_cast<IHotbarItem*>(params.pDragAndDroppable))
		{
			pHotbarManager->SetHotbarItem(slotIndex, pHotbarItem);	
		}
	}
}

void UIObjectHotbarSlot::OnDragEvent(const UIObjectDragAndDropDelegateParams& params)
{
	if (pHotbarManager)
	{
		pHotbarManager->ClearHotbarItem(slotIndex);
	}
}

void UIObjectHotbarSlot::OnHotbarUpdated(const OnHotbarSlotUpdatedDelegateParams& params)
{
	if (pDragAndDropArea)
	{
		if (params.slotIndex == slotIndex)
		{
			if (params.pCurrentItem == nullptr)
			{
				pDragAndDropArea->SetDroppable(nullptr);
				return;
			}
			
			if (IDragAndDroppable* pDragAndDroppable = dynamic_cast<IDragAndDroppable*>(params.pCurrentItem))
			{
				pDragAndDropArea->SetDroppable(pDragAndDroppable);
			}
			else
			{
				DOMLOG_ERROR("Hotbar items must be IDragAndDroppable to work with the hotbar HUD")
			}
		}
	}
}
