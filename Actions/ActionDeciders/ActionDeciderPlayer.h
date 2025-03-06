#pragma once

#include "ActionDeciderBase.h"

class RPGSkillSystem;

/** ActionDeciderPlayer
*
* This action decider is to be used with player controller entities, it will decide what to do based on player input
*/
class ActionDeciderPlayer : public ActionDeciderBase
{
public:

	//~ Begin ActionDeciderBase Interface
	ActionHandlerBase* DecideAction(ECS& ecs, EntityId e) override;
	//~ End ActionDeciderBase Interface

	// #TEMP: Editor expose ActionDeciders and ActionResponses
	RPGSkillSystem* pSkillSystem = nullptr;
	bool bAimingSkill = false;
};
