#pragma once

#include "ECSTypes.h"

#include "DomUtils/HeapAlloc.h"

class ECS;
class HUDObjectBase;

EDITORSTRUCT()
struct HUDObjectSharedInitParams
{
	EDITORBODY()
	
	// Padding percent around the edge of the screen that HUDObjects will not be rendered in
	EDITORPROPERTY()
	float screenEdgePadding = 0.05f;

	ECS* pEcs = nullptr;
};

/** HUD
*
* This class is responsible for showing the HUD.
* 
*/
EDITORCLASS()
class HUD
{
	EDITORBODY()
	
public:

	void Initialise(ECS& ecs, EntityId player);
	void Uninitialise();
	void Tick(float deltaTime);

private:
	
	EDITORPROPERTY()
	std::vector<HUDObjectBase*> pHudObjects;

	// Params shared by all HUD objects
	EDITORPROPERTY()
	HUDObjectSharedInitParams hudObjectSharedInitParams;
};