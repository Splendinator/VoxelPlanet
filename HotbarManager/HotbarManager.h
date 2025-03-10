#pragma once

class IHotbarItem;
class InputActionBase;

struct OnHotbarSlotUpdatedDelegateParams
{
	int index = 0;
	IHotbarItem* pLastItem = nullptr;
	IHotbarItem* pCurrentItem = nullptr;
};

EDITORSTRUCT()
struct HotbarSlot
{
	EDITORBODY()

	// Input action to trigger the item in this slot
	EDITORPROPERTY()
	InputActionBase* pInputAction = nullptr;
	
	IHotbarItem* pCurrentlyAssignedItem = nullptr;
};

// Manages the gameplay side of the hotbar.
// Anything with an IHotbarItem interface can be assigned to a slot and used when it's the player's turn.
EDITORCLASS(Singleton)
class HotbarManager
{
	EDITORBODY()
public:

	void SetHotbarItem(int index, IHotbarItem* pItem);
	void ClearHotbarItem(int index);
	
	const std::vector<HotbarSlot>& GetHotbarSlots() const { return hotbarSlots; }

	DelegateList<OnHotbarSlotUpdatedDelegateParams> onHotbarSlotUpdatedDelegates;

protected:

	// All hotbar slots. The index in this array will be their position on the HUDObject
	EDITORPROPERTY()
	std::vector<HotbarSlot> hotbarSlots;
};
