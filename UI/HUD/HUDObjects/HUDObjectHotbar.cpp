#include "pch.h"

#include "HUDObjectHotbar.h"

#include "Core/DirectoryData.h"
#include "HotbarManager/HotbarManager.h"
#include "UI/DragAndDrop/DragAndDropShared.h"
#include "UI/UIObjects/UIObjectDragAndDropArea.h"
#include "UI/UIObjects/UIObjectHotbarSlot.h"
#include "UI/UIObjects/UIObjectSubObjectSpawner.h"

void HUDObjectHotbar::Init(UICanvas& canvas)
{
	UIObjectSubObjectSpawner* pHotbarSlotSpawner = canvas.AddExistingUIObject<UIObjectSubObjectSpawner>(slotSpawnAreaLayerName);

	if (pHotbarManager && pDirectoryData && pDragAndDropManager)
	{
		UIObjectSubObjectSpawnerSpawnParams spawnParams = {};
		spawnParams.file = DirectoryData::ConcatenateSVGFilePathChecked(pDirectoryData->hudObjects, GetOwnFileName()); // Slots are in the same file as the HudObject
		spawnParams.layerName = slotLayerName;
		spawnParams.numObjects = (u32)pHotbarManager->GetHotbarSlots().size();
		spawnParams.spawnDirection = EUIObjectSubObjectSpawnerSpawnDirection::Horizontal;
		spawnParams.spawnLayout = EUIObjectSubObjectSpawnerSpawnLayout::Center;
		
		spawnedHotbarSlots = pHotbarSlotSpawner->SpawnSubobjectsAs<UIObjectHotbarSlot>(spawnParams);

		UIObjectDragAndDropSetupParams dragAndDropParams = {pDirectoryData, pDragAndDropManager};
		dragAndDropParams.areaParams.priority = EDragAndDropAreaPriority::HUD;

		UIObjectHotbarSlotSetupParams hotbarSlotParams = {dragAndDropParams, *pHotbarManager};
		hotbarSlotParams.slotIndex = 0;
		
		for (int i = 0; i < spawnedHotbarSlots.size(); ++i)
		{
			hotbarSlotParams.slotIndex = i;
			spawnedHotbarSlots[i]->Setup(hotbarSlotParams);
		}
	}
}

void HUDObjectHotbar::Uninit()
{

}

void HUDObjectHotbar::Tick(ECS& ecs, EntityId player, float deltaTime)
{

}
