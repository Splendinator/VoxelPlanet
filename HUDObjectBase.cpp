#include "pch.h"

#include "HUDObjectBase.h"

#include "DirectoryData.h"
#include "Game.h"
#include "HUD.h"

void HUDObjectBase::BaseInit(EntityId player, const HUDObjectSharedInitParams& hudInitParams)
{
	controlledEntity = player;

	pEcs = hudInitParams.pEcs;
	
	UICanvasInitParams initParams;
	initParams.sizeX = sizeX;
	initParams.sizeY = sizeY;
	initParams.filePath = Game::GetGameAssets().pDirectoryData->hudObjects + fileName + ".svg";
	initParams.renderPriority = RenderPriority::UI;
	initParams.type = dmgf::ERenderObjectType::HUD;

	uiCanvas = std::make_unique<UICanvas>(initParams);

	Vec2f anchorPoint = screenAnchorPoint.GetAnchorPoint(hudInitParams.screenEdgePadding);
	const float absolutePositionX = anchorPoint.x * dmgf::GetScreenWidth() - anchorPoint.x * sizeX;
	const float absolutePositionY = anchorPoint.y * dmgf::GetScreenHeight() - anchorPoint.y * sizeY;
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
		Tick(*pEcs, controlledEntity, deltaTime);
	}
}
