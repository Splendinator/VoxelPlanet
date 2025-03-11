#pragma once

#include "UIObjectBase.h"

#include "HotbarManager/HotbarManager.h"

class HotbarManager;
class UIObjectDragAndDropArea;
struct UIObjectDragAndDropDelegateParams;
struct UIObjectDragAndDropSetupParams;

struct UIObjectHotbarSlotSetupParams
{
	const UIObjectDragAndDropSetupParams& dragAndDropParams;
	HotbarManager& hotbarManager;
	int slotIndex = 0;
};

class UIObjectHotbarSlot : public UIObjectBase
{
public:

	void Setup(const UIObjectHotbarSlotSetupParams& params);
	
protected:
	//~ Begin UIObjectBase Interface
	void Init(VectorPrimitiveLayer* pRoot) override;
	void Uninit() override;
	//~ End UIObjectBase Interface

	TransientPtr<UIObjectDragAndDropArea> pDragAndDropArea;

private:

	void OnDragEvent(const UIObjectDragAndDropDelegateParams& params);
	void OnDropEvent(const UIObjectDragAndDropDelegateParams& params);
	void OnHotbarUpdated(const OnHotbarSlotUpdatedDelegateParams& params);

	DelegateClass<UIObjectHotbarSlot, const UIObjectDragAndDropDelegateParams&> onDragEventDelegate;
	DelegateClass<UIObjectHotbarSlot, const UIObjectDragAndDropDelegateParams&> onDropEventDelegate;
	DelegateClass<UIObjectHotbarSlot, const OnHotbarSlotUpdatedDelegateParams&> onSlotUpdatedDelegate;

	TransientPtr<HotbarManager> pHotbarManager = nullptr;
	
	int slotIndex = 0; // Index in the hotbar manager that this slot represents
	
};
