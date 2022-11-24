#pragma once

#include "ECSTypes.h"

#include "DomUtils/HeapAlloc.h"

class ECS;
class HUDObjectBase;

/** HUD
*
* This class is responsible for showing the HUD.
* 
*/
class HUD
{
public:

	HUD(ECS& inEcs);

	void Initialise(EntityId player);
	void Uninitialise();
	void Tick(float deltaTime);

private:

	ECS& ecs;
	dmut::HeapAllocSize<HUDObjectBase*> pHudObjects;
};