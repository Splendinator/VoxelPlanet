#pragma once

#include "ECSTypes.h"

class ActionHandlerBase;
class ECS;

/** ActionDeciderBase
*
* This class is used for deciding which action to take. Used with the action system
*/
class ActionDeciderBase
{
public:

	virtual ~ActionDeciderBase() = default; // Deleted by SystemAction

	// This should be overridden to return an action handler. 
	// This can return nullptr if no action is to be taken this frame and the action system will ask again next frame. (i.e waiting for player input)
	virtual ActionHandlerBase* DecideAction(ECS& ecs, EntityId actor) = 0;

protected:

	// Try moving to a location, else try attacking an entity at that location.
	ActionHandlerBase* TryMoveOrAttack(ECS& ecs, EntityId e, int deltaX, int deltaY);
};
