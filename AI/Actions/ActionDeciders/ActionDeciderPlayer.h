#pragma once

#include "ActionDeciderBase.h"

class ActionHandlerSkill;
class ActionHandlerWait;
class HotbarManager;

/** ActionDeciderPlayer
*
* This action decider is to be used with player controller entities, it will decide what to do based on player input
*/
EDITORCLASS()
class ActionDeciderPlayer : public ActionDeciderBase
{
	EDITORBODY()
public:

	//~ Begin ActionDeciderBase Interface
	ActionHandlerBase* DecideAction(ECS& ecs, EntityId e) override;
	//~ End ActionDeciderBase Interface

	// Used to query the hotbar for possible player actions
	EDITORPROPERTY()
	HotbarManager* pHotbarManager = nullptr;

	// Singleton actions
	EDITORPROPERTY()
	ActionHandlerWait* pWaitAction = nullptr;
};
