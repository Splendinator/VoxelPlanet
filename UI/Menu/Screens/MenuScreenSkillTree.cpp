#include "pch.h"

#include "MenuScreenSkillTree.h"

#include "ECS/ECS.h"
#include "RPGSystems/Classes/RPGClassData.h"
#include "RPGSystems/Skills/RPGSkillData.h"
#include "UI/UIObjects/UIObjectDragAndDropArea.h"

void MenuScreenSkillTree::Init(UICanvas* pCanvas)
{
	DOMLOG_ERROR_IF(pDragAndDropManager == nullptr);
	DOMLOG_ERROR_IF(pEcs == nullptr);

	// Initialise drag and drop UIObjects
	UIObjectDragAndDropSetupParams dragAndDropAreaSetupParams = {};
	dragAndDropAreaSetupParams.areaParams.priority = EDragAndDropAreaPriority::Menus;
	dragAndDropAreaSetupParams.pDirectoryData = pDirectoryData;
	dragAndDropAreaSetupParams.pDragAndDropManager = pDragAndDropManager;
	
	for (int index = 0; index < skillSlotDatas.size(); ++index)
	{
		SkillTreeMenuSkillSlotData& skillSlotData = skillSlotDatas[index];
		
		skillSlotData.dragAndDropArea = pCanvas->AddExistingUIObject<UIObjectDragAndDropArea>(skillSlotData.buttonLayerName);
		skillSlotData.dragAndDropArea->Setup(dragAndDropAreaSetupParams);
	}

	// Setup initial skills based off player level + class
	// #TODO: This will be in an actual skill tree manager at some point that will have data driven unlock conditions for all the skills.
	if (pEcs && pDragAndDropManager)
	{
		EntityId playerId = pEcs->GetPlayerEntityId();

		if (pEcs->EntityHasComponents<ComponentClass, ComponentProgression>(playerId))
		{
			ComponentProgression& progression = pEcs->GetComponent<ComponentProgression>(playerId);
			ComponentClass& classComponent = pEcs->GetComponent<ComponentClass>(playerId);

			for (u32 index = 0; index < skillSlotDatas.size(); ++index)
			{
				SkillTreeMenuSkillSlotData& skillSlotData = skillSlotDatas[index];
				if (progression.level < index)
				{
					// Not high enough level to use this skill 
					break;
				}

				if (classComponent.pClassData)
				{
					if (classComponent.pClassData->classSkills.size() <= index)
					{
						// Not enough skills to unlock
						break;
					}

					skillSlotData.dragAndDropArea->SetDroppable(classComponent.pClassData->classSkills[index]);
				}
			}
		}
	}
}

void MenuScreenSkillTree::UnInit()
{
	
}
