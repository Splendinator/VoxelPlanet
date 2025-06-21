#pragma once

#include "ECS/ECSTypes.h"

class ECS;

/** ActionHandlerBase
*
* This class is the base class used for handling actions with the action system
* 
* It should do something (move an entity, attack, cast a spell, etc.)
* 
* These classes are singletons
*/
EDITORCLASS(Abstract, Singleton)
class ActionHandlerBase
{
	EDITORBODY()
public:
	
	// Override this to do the action
	virtual void DoAction(ECS& ecs, EntityId e) { PUREVIRTUAL() };

	// Override this to reset the parameters. Needed to clean up singletons after use in the action system
	virtual void Reset() { PUREVIRTUAL() };

	// If we do most actions we need to tick all the other ECS systems so they can update (e.g. moving needs ECSSystemPhysics to update the entity's position or it won't collide with anything)
	EDITORPROPERTY()
	bool bCanThisHappenOnTheSameFrameAsOtherActions = false;
};
