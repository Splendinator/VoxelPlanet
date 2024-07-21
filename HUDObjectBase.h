#pragma once

#include "ECSTypes.h"
#include "HUDAnchorPoint.h"
#include "UICanvas.h"

class ECS;
struct HUDObjectSharedInitParams;

/** HUDObjectBase
*
* Base HUDobject. HUDObjects render HUD to the screen.
*/
EDITORCLASS()
class HUDObjectBase
{
	EDITORBODY()

public:
	
	void BaseInit(EntityId player, const HUDObjectSharedInitParams& hudInitParams);
	void BaseUninit();
	void BaseTick(float deltaTime);

protected:

	virtual void Init(UICanvas& canvas) {}; // Should populate the canvas with UIObjects
	virtual void Uninit() {};
	virtual void Tick(ECS& ecs, EntityId player, float deltaTime) {} // Should populate UIObjects with the player's data

private:
	
	// Anchor points that the HUD object uses to attach to the screen
	// i.e screenAnchorPoint(Center), hudAnchorPoint(TopLeft) will put the top left of the HUD object in the center of the screen.
	EDITORPROPERTY()
	HUDAnchorPoint screenAnchorPoint;
	EDITORPROPERTY()
	HUDAnchorPoint hudAnchorPoint; 

	// Name of the HUDObject file. Don't include ".svg"
	EDITORPROPERTY()
	std::string fileName;

	// Size of the HUD object in pixels
	EDITORPROPERTY()
	float sizeX = 0.0f;
	EDITORPROPERTY()
	float sizeY = 0.0f;
	
	EntityId controlledEntity = INVALID_ENTITY_ID; // Currently controlled player entity
	
	std::unique_ptr<UICanvas> uiCanvas;
	
	ECS* pEcs = nullptr;
};