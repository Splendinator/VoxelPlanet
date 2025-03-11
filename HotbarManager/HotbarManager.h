#pragma once

#include "Core/GameSystem.h"

#include "StatefulHotbarActions.h"

class ActionDeciderBase;
struct OnInputActionDelegateParams;
class IHotbarItem;
class InputActionBase;

struct OnHotbarSlotUpdatedDelegateParams
{
	int slotIndex = 0;
	IHotbarItem* pLastItem = nullptr;
	IHotbarItem* pCurrentItem = nullptr;
};

EDITORSTRUCT()
struct HotbarSlot
{
	EDITORBODY()

	HotbarSlot() = default;
	~HotbarSlot() = default;
	
	// Needs override due to unique_ptr
	HotbarSlot(const HotbarSlot& other) { pInputAction = other.pInputAction; pCurrentlyAssignedItem = other.pCurrentlyAssignedItem; }
	
	// Input action to trigger the item in this slot
	EDITORPROPERTY()
	InputActionBase* pInputAction = nullptr;
	
	IHotbarItem* pCurrentlyAssignedItem = nullptr;

	// If this is set this hotbar slot's item created a stateful action on use. Only one stateful action can be active in the manager at once.
	std::unique_ptr<StatefulHotbarActionBase> pCurrentStatefulAction;
};

// Manages the gameplay side of the hotbar.
// Anything with an IHotbarItem interface can be assigned to a slot and used when it's the player's turn.
EDITORCLASS(Singleton)
class HotbarManager : public GameSystem
{
	EDITORBODY()
public:

	void SetHotbarItem(int slotIndex, IHotbarItem* pItem);
	void ClearHotbarItem(int slotIndex);
	
	const std::vector<HotbarSlot>& GetHotbarSlots() const { return hotbarSlots; }

	DelegateList<const OnHotbarSlotUpdatedDelegateParams&> onHotbarSlotUpdatedDelegates;

	TransientPtr<ActionHandlerBase> GetRequestedPlayerAction() const { return pRequestedPlayerAction; }
	void ClearRequestedPlayerAction() { pRequestedPlayerAction = nullptr; }

protected:

	DelegateClass<HotbarManager, const OnInputActionDelegateParams&> onHotbarInputActionDelegate;
	
	void OnHotbarInputAction(const OnInputActionDelegateParams& params);

	void SetRequestedPlayerAction(TransientPtr<ActionHandlerBase> pAction);
	void CancelActiveStatefulHotbarInputAction();
	
	//~ Begin GameSystem Interface
	void Init() override;
	//~ End GameSystem Interface
	
	// All hotbar slots. The index in this array will be their position on the HUDObject
	EDITORPROPERTY()
	std::vector<HotbarSlot> hotbarSlots;
	
	TransientPtr<ActionHandlerBase> pRequestedPlayerAction = nullptr;
};
