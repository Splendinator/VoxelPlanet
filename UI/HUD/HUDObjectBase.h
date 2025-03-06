#pragma once

#include "ECS/ECSTypes.h"
#include "HUDAnchorPoint.h"
#include "UI/UICanvas.h"

class DirectoryData;
class ECS;
struct HUDObjectSharedInitParams;

/** HUDObjectBase
*
* Base HUDobject. HUDObjects render UI to the screen.
*/
EDITORCLASS(Abstract)
class HUDObjectBase
{
	EDITORBODY()

public:
	
	void BaseInit(EntityId player, const HUDObjectSharedInitParams& hudInitParams, DirectoryData* pDirectoryData);
	void BaseUninit();
	void BaseTick(float deltaTime);

protected:

	virtual void Init(UICanvas& canvas) {}; // Should populate the canvas with UIObjects
	virtual void Uninit() {};
	virtual void Tick(ECS& ecs, EntityId player, float deltaTime) {} // Should populate UIObjects with the player's data

private:
	
	// Anchor points that the UI object uses to attach to the screen
	// i.e screenAnchorPoint(Center), hudAnchorPoint(TopLeft) will put the top left of the UI object in the center of the screen.
	EDITORPROPERTY()
	HUDAnchorPoint screenAnchorPoint;
	EDITORPROPERTY()
	HUDAnchorPoint hudAnchorPoint; 

	// Name of the HUDObject file. Don't include ".svg"
	EDITORPROPERTY()
	std::string fileName;

	// Size of the UI object in pixels
	EDITORPROPERTY()
	float sizeX = 0.0f;
	EDITORPROPERTY()
	float sizeY = 0.0f;
	
	EntityId controlledEntity = INVALID_ENTITY_ID; // Currently controlled player entity
	
	std::unique_ptr<UICanvas> uiCanvas;
	
	ECS* pEcs = nullptr;
};