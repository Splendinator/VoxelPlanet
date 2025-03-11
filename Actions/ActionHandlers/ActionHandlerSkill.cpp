#include "pch.h"

#include "ActionHandlerSkill.h"

#include "RPGSystems/Skills/RPGSkillSystem.h"

void ActionHandlerSkill::Setup(const RPGSkillData* pInSkill, Vec2i inAimLocation)
{
	pSkill = pInSkill;
	aimLocation = inAimLocation;
}

void ActionHandlerSkill::DoAction(ECS& ecs, EntityId e)
{
	if (pSkillSystem)
	{
		pSkillSystem->TryFireSkill(pSkill, e, aimLocation);
	}
}

void ActionHandlerSkill::Reset()
{
	pSkill = nullptr;
	aimLocation = {0,0};
}
