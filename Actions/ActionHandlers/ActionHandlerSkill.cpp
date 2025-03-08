#include "pch.h"

#include "ActionHandlerSkill.h"

#include "RPGSystems/Skills/RPGSkillSystem.h"

void ActionHandlerSkill::DoAction(ECS& ecs, EntityId e)
{
	if (pSkillSystem)
	{
		// #TODO: The handling is done in ActionDeciderPlayer right now and I don't care to move it here
		//pSkillSystem->TryFireSkill(pSkill)
	}
}

void ActionHandlerSkill::Reset()
{
	pSkill = nullptr;
}
