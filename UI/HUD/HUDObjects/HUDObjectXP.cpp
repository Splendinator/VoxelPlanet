#include "pch.h"

#include "HUDObjectXP.h"

#include "ECS/Components.h"
#include "ECS/ECS.h"
#include "RPGSystems/RPGSystem.h"
#include "UI/UIObjects/UIObjectProgressBar.h"

void HUDObjectXP::Init(UICanvas& canvas)
{
	pXpProgressBar = canvas.AddExistingUIObject<UIObjectProgressBar>();
}

void HUDObjectXP::Uninit()
{
	pXpProgressBar = nullptr;
}

void HUDObjectXP::Tick(ECS& ecs, EntityId player, float deltaTime)
{
	if (pRpgSystem)
	{
		ComponentProgression& progressionComponent = ecs.GetComponent<ComponentProgression>(player);
		if (const RPGLevelProgressionData* pLevelProgressionData = pRpgSystem->GetLevelProgressionDataForLevel(progressionComponent.level))
		{
			if (pLevelProgressionData->requiredXp > 0)
			{
				pXpProgressBar->SetProgressFrac((float)progressionComponent.currentXp / (float)pLevelProgressionData->requiredXp);
				return;
			}
		}
		
		// Max level -- Show full
		pXpProgressBar->SetProgressFrac(1.0f);
	}
	else
	{
		// Invalid setup -- Show empty
		pXpProgressBar->SetProgressFrac(0.0f);	
	}
}
