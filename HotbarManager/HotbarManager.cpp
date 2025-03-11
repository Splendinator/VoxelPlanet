#include "pch.h"

#include "HotbarManager.h"

#include "IHotbarItem.h"

#include "Input/InputAction.h"

void HotbarManager::Init()
{
	onHotbarInputActionDelegate.Bind(this, &HotbarManager::OnHotbarInputAction);
	
	for (HotbarSlot& slot : hotbarSlots)
	{
		if (slot.pInputAction)
		{
			slot.pInputAction->onActionActivatedDelegates.Add(onHotbarInputActionDelegate);
		}
	}
}

void HotbarManager::SetHotbarItem(int slotIndex, IHotbarItem* pItem)
{
	if (slotIndex < 0 || slotIndex >= hotbarSlots.size())
	{
		DOMLOG_ERROR("Invalid slot index");
		return;
	}

	if (pItem != hotbarSlots[slotIndex].pCurrentlyAssignedItem)
	{
		IHotbarItem* pLastItem = hotbarSlots[slotIndex].pCurrentlyAssignedItem;
		hotbarSlots[slotIndex].pCurrentlyAssignedItem = pItem;
		
		onHotbarSlotUpdatedDelegates.Invoke({slotIndex, pLastItem, pItem});
	}
}

void HotbarManager::ClearHotbarItem(int slotIndex)
{
	SetHotbarItem(slotIndex, nullptr);
}

void HotbarManager::OnHotbarInputAction(const OnInputActionDelegateParams& params)
{
	for (HotbarSlot& slot : hotbarSlots)
	{
		if (slot.pInputAction == params.pInputAction && slot.pCurrentlyAssignedItem)
		{
			// 1. If there's an active stateful action on this slot already, just cancel it.
			if (slot.pCurrentStatefulAction)
			{
				slot.pCurrentStatefulAction->ToggleOffStatefulAction();
				slot.pCurrentStatefulAction.release();
				return;
			}

			OnHotbarItemUsedParams onHotbarItemUsedParams = {};
			slot.pCurrentlyAssignedItem->OnHotbarItemUsed(onHotbarItemUsedParams);

			// 2. If the slot outputs a simple action use that
			if (onHotbarItemUsedParams.pOutPlayerActionHandler)
			{
				SetRequestedPlayerAction(onHotbarItemUsedParams.pOutPlayerActionHandler);
				return;
			}

			// 3. If the slot outputs a stateful action use that
			if (onHotbarItemUsedParams.pOutStatefulPlayerAction)
			{
				CancelActiveStatefulHotbarInputAction(); // Cancel old stateful action -- We can only have 1 active at once

				slot.pCurrentStatefulAction = std::move(onHotbarItemUsedParams.pOutStatefulPlayerAction);
				slot.pCurrentStatefulAction->onPlayerActionDecided.Bind(this, &HotbarManager::SetRequestedPlayerAction);
				slot.pCurrentStatefulAction->onRequestCancelStatefulAction.Bind(this, &HotbarManager::CancelActiveStatefulHotbarInputAction);
				slot.pCurrentStatefulAction->ToggleOnStatefulAction();
				
				return;
			}

			DOMLOG_ERROR("No action found?")
		}
	}
}

void HotbarManager::SetRequestedPlayerAction(TransientPtr<ActionHandlerBase> pAction)
{
	CancelActiveStatefulHotbarInputAction();
	
	pRequestedPlayerAction = pAction;
}

void HotbarManager::CancelActiveStatefulHotbarInputAction()
{
	for (HotbarSlot& slot : hotbarSlots)
	{
		if (slot.pCurrentStatefulAction)
		{
			slot.pCurrentStatefulAction->ToggleOffStatefulAction();
			slot.pCurrentStatefulAction.release();
		}
	}
}
