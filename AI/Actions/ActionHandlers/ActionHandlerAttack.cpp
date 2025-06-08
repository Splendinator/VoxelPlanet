#include "pch.h"

#include "ActionHandlerAttack.h"

#include "RPGSystems/RPGSystem.h"

void ActionHandlerAttack::DoAction(ECS& ecs, EntityId e)
{
	if (pRpgSystem)
	{
		RPGDamageParams damageParams = {};
		damageParams.damageMagnitude.damageType = EDamageType::Physical;
		damageParams.damageMagnitude.scaling = 1.0f;
		damageParams.attackerEntity = e;
		damageParams.targetEntity = target;

		pRpgSystem->DealDamage(damageParams);
	}
}

void ActionHandlerAttack::Reset()
{
	target = INVALID_ENTITY_ID;
}
