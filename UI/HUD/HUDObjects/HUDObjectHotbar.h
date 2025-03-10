#pragma once

#include "HUDObjectBase.h"

class DragAndDropManager;
class HotbarManager;
class UIObjectHotbarSlot;

EDITORCLASS()
class HUDObjectHotbar : public HUDObjectBase
{
	EDITORBODY()

protected:
	//~ Begin HUDObjectBase Interface
	void Init(UICanvas& canvas) override;
	void Uninit() override;
	void Tick(ECS& ecs, EntityId player, float deltaTime) override;
	//~ End HUDObjectBase Interface

	// Name of layer to dynamically spawn slot buttons into 
	EDITORPROPERTY()
	std::string slotSpawnAreaLayerName;

	// Name of the layer of the slot, located in the same file as the hotbar HUDObject
	EDITORPROPERTY()
	std::string slotLayerName;

	EDITORPROPERTY()
	DirectoryData* pDirectoryData = nullptr;

	EDITORPROPERTY()
	HotbarManager* pHotbarManager = nullptr;

	EDITORPROPERTY()
	DragAndDropManager* pDragAndDropManager = nullptr;

	std::vector<TransientPtr<UIObjectHotbarSlot>> spawnedHotbarSlots;
};
