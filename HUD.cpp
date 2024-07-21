#include "pch.h"

#include "HUD.h"

#include "FilePaths.h"
#include "HUDObjectHealth.h"

void HUD::Initialise(ECS& ecs, EntityId player)
{
	hudObjectSharedInitParams.pEcs = &ecs;
	for(HUDObjectBase* pHudObject : pHudObjects)
	{
		if (pHudObject)
		{
			pHudObject->BaseInit(player, hudObjectSharedInitParams);
		}
	}
}

void HUD::Uninitialise()
{
	for(HUDObjectBase* pHudObject : pHudObjects)
	{
		if (pHudObject)
		{
			pHudObject->BaseUninit();
		}
	}
}

void HUD::Tick(float deltaTime)
{
	for(HUDObjectBase* pHudObject : pHudObjects)
	{
		if (pHudObject)
		{
			pHudObject->BaseTick(deltaTime);
		}
	}
}

