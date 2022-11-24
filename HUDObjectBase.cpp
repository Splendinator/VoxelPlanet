#include "pch.h"

#include "HUDObjectBase.h"

void HUDObjectBase::BaseInit(EntityId player)
{
	controlledEntity = player;

	UICanvasInitParams initParams;
	initParams.sizeX = hudInitParams.sizeX;
	initParams.sizeY = hudInitParams.sizeY;
	initParams.filePath = hudInitParams.filePath;
	initParams.renderPriority = RenderPriority::UI;
	initParams.type = dmgf::ERenderObjectType::HUD;

	uiCanvas = std::make_unique<UICanvas>(initParams);

	const float absolutePositionX = hudInitParams.screenAnchorPoint.anchorX * dmgf::GetScreenWidth() - hudInitParams.hudAnchorPoint.anchorX * hudInitParams.sizeX;
	const float absolutePositionY = hudInitParams.screenAnchorPoint.anchorY * dmgf::GetScreenHeight() - hudInitParams.hudAnchorPoint.anchorY * hudInitParams.sizeY;
	uiCanvas->SetPosition(absolutePositionX, absolutePositionY);

	Init(*uiCanvas);

	BaseTick(std::numeric_limits<float>::epsilon()); // Tick once to initialize the HUD object
}

void HUDObjectBase::BaseUninit()
{
	uiCanvas->Uninit();

	Uninit();

	controlledEntity = INVALID_ENTITY_ID;
}

void HUDObjectBase::BaseTick(float deltaTime)
{
	if (controlledEntity != INVALID_ENTITY_ID)
	{
		Tick(hudInitParams.ecs, controlledEntity, deltaTime);
	}
}
