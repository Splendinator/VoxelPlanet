#include "pch.h"

#include "ActionHandlerSkill.h"

void ActionHandlerSkill::DoAction(ECS& ecs, EntityId e)
{
	// #TEMP: Use skill
}

void ActionHandlerSkill::Reset()
{
	pSkill = nullptr;
}
