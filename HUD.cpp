#include "pch.h"

#include "HUD.h"

#include "FilePaths.h"
#include "HUDObjectHealth.h"

HUD::HUD(ECS& inEcs) : ecs(inEcs)
{
	constexpr int NUM_HUD_OBJECTS = 1;

	const float screenEdgePadding = 0.05f; // Padding around the edge of the screen that HUDObjects will not be rendered in

	pHudObjects.Alloc(NUM_HUD_OBJECTS);

	// Health bar
	{
		HUDInitParams initParams(ecs);
		initParams.screenAnchorPoint = HUDAnchorPoint(EHUDAnchorPoint::TopLeft, screenEdgePadding);
		initParams.hudAnchorPoint = HUDAnchorPoint(EHUDAnchorPoint::TopLeft);
		initParams.sizeX = 400.0f;
		initParams.sizeY = 400.f * 0.08f;
		initParams.filePath = FilePath::VectorArt::healthBar;

		pHudObjects[0] = new HUDObjectHealth(initParams);
	}
}

void HUD::Initialise(EntityId player)
{
	for (int i = 0; i < pHudObjects.GetSize(); ++i)
	{
		pHudObjects[i]->BaseInit(player);
	}
}

void HUD::Uninitialise()
{
	for (int i = 0; i < pHudObjects.GetSize(); ++i)
	{
		pHudObjects[i]->BaseUninit();
	}
}

void HUD::Tick(float deltaTime)
{
	for (int i = 0; i < pHudObjects.GetSize(); ++i)
	{
		pHudObjects[i]->BaseTick(deltaTime);
	}
}

