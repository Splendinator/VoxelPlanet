#include "pch.h"

#include "RPGSkillEffectModules.h"

#include "Ecs/ECS.h"
#include "ECS/Systems/ECSSystemEntityMap.h"

void RPGSkillEffectModuleDamage::ApplyEffect(const RPGSkillParams& params) const
{
	EntityList targetEntityList = params.entityMapSystem.GetEntities(params.targetPos.x, params.targetPos.y);

	RPGDamageParams damageParams;
	damageParams.attackerEntity = params.caster;
	damageParams.damageMagnitude = damage;
	
	for (EntityId entity : targetEntityList.entities)
	{
		if (params.ecs.EntityHasComponents<ComponentFaction, ComponentHealth>(entity))
		{
			damageParams.targetEntity = entity;
			params.rpgSystem.DealDamage(damageParams);		
		}
	}
}

