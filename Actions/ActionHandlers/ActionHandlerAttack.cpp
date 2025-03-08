#include "pch.h"

#include "ActionHandlerAttack.h"

#include "RPGSystems/RPGSystem.h"

void ActionHandlerAttack::DoAction(ECS& ecs, EntityId e)
{
	if (pRpgSystem)
	{
		RPGDamageParams damageParams = {};
		damageParams.damage = 20; // #TODO: Calculate this from some sort of basic attack damage attribute
		damageParams.attackerEntity = e;
		damageParams.targetEntity = target;

		pRpgSystem->DealDamage(damageParams);
	}
}

void ActionHandlerAttack::Reset()
{
	target = INVALID_ENTITY_ID;
}
