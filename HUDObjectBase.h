#pragma once

#include "ECSTypes.h"
#include "HUDAnchorPoint.h"
#include "UICanvas.h"

class ECS;

struct HUDInitParams
{
	HUDInitParams(ECS& inEcs) : ecs(inEcs) {}
	HUDInitParams(const HUDInitParams&) = default;

	// Anchor point on the screen
	HUDAnchorPoint screenAnchorPoint; 
	
	// Anchor point of the HUD object that will attach to the screen anchor point.
	// i.e screenAnchorPoint(Center), hudAnchorPoint(TopLeft) will put the top left of the HUD object in the center of the screen.
	HUDAnchorPoint hudAnchorPoint; 

	ECS& ecs;
	const char* filePath = nullptr;
	float sizeX = 0.0f, sizeY = 0.0f; // Size of the HUD object in pixels
};

/** HUDObjectBase
*
* Base HUDobject. HUDObjects render HUD to the screen.
*/
class HUDObjectBase
{
public:

	HUDObjectBase(const HUDInitParams& initParams) : hudInitParams(initParams) {}

	void BaseInit(EntityId player);
	void BaseUninit();
	void BaseTick(float deltaTime);

protected:

	virtual void Init(UICanvas& canvas) {}; // Should populate the canvas with UIObjects
	virtual void Uninit() {};
	virtual void Tick(ECS& ecs, EntityId player, float deltaTime) {}; // Should populate UIObjects with the player's data

private:

	EntityId controlledEntity = INVALID_ENTITY_ID; // Currently controlled player entity

	std::unique_ptr<UICanvas> uiCanvas;

	HUDInitParams hudInitParams;
};