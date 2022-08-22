#pragma once

#include "ECSTypes.h"

class ECS;

/** ActionHandlerBase
*
* This class is the base class used for handling actions with the action system
* 
* It should do something (move an entity, attack, cast a spell, etc.)
* 
* These classes are singletons
*/
class ActionHandlerBase
{
public:
	
	// Override this to do the action
	virtual void DoAction(ECS& ecs, EntityId e) = 0;

	// Override this to reset the parameters. Needed to clean up singletons after use in the action system
	virtual void Reset() = 0;
};
