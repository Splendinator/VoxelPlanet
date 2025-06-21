#pragma once

#include "ECS/ECSTypes.h"

class ActionHandlerAttack;
class ActionHandlerBase;
class ActionHandlerMove;
class ECS;

/** ActionDeciderBase
*
* This class is used for deciding which action to take. Used with the action system
*/
EDITORCLASS(Abstract, Singleton)
class ActionDeciderBase
{
	EDITORBODY()
public:

	virtual ~ActionDeciderBase() = default; // Deleted by ECSSystemAction

	// This should be overridden to return an action handler. 
	// This can return nullptr if no action is to be taken this frame and the action system will ask again next frame. (i.e waiting for player input)
	virtual ActionHandlerBase* DecideAction(ECS& ecs, EntityId actor) { PUREVIRTUAL() return nullptr; }

protected:

	// Try moving to a location, else try attacking an entity at that location.
	ActionHandlerBase* TryMoveOrAttack(ECS& ecs, EntityId e, int deltaX, int deltaY);

	ActionHandlerBase* TryMove(ECS& ecs, EntityId e, int deltaX, int deltaY);
	
	// #TEMP: We need to move the move and attack out of the base since debuffs and projectiles are using ActionDeciders and don't need this shit
	EDITORPROPERTY()
	ActionHandlerAttack* pAttackAction = nullptr;
	EDITORPROPERTY()
	ActionHandlerMove* pMoveAction = nullptr;
};
