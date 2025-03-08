#pragma once

#include "ActionDeciderBase.h"

class ActionHandlerWait;
class ActionHandlerSkill;
class RPGSkillSystem;

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

	EDITORPROPERTY()
	RPGSkillSystem* pSkillSystem = nullptr;

	// Singleton actions
	EDITORPROPERTY()
	ActionHandlerWait* pWaitAction = nullptr;
	EDITORPROPERTY()
	ActionHandlerSkill* pSkillAction = nullptr;
	
	bool bAimingSkill = false;
};
