#include "pch.h"

#include "HUD.h"

#include "ECS.h"
#include "HUDObjectBase.h"

void HUD::Init()
{
	if (pEcs)
	{
		hudObjectSharedInitParams.pEcs = pEcs;
		for(HUDObjectBase* pHudObject : pHudObjects)
		{
			if (pHudObject)
			{
				pHudObject->BaseInit(pEcs->GetPlayerEntityId(), hudObjectSharedInitParams, pDirectoryData);
			}
		}
	}
}

void HUD::UnInit()
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

