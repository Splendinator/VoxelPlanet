#include "pch.h"

#include "RPGSkillEffectModules.h"

#include "ECS/ECS.h"
#include "ECS/Systems/ECSSystemEntityMap.h"
#include "RPGSystems/RPGSystem.h"

void RPGSkillEffectModuleTemp::ApplyEffect(const RPGSkillParams& params) const
{
	const EntityList& entityList = params.entityMapSystem.GetEntities(params.targetPos.x, params.targetPos.y);

	for (EntityId entity : entityList.entities)
	{
		if (params.ecs.EntityHasComponents<ComponentHealth>(entity))
		{
			RPGDamageParams damageParams = {};
			damageParams.attackerEntity = params.caster;
			damageParams.targetEntity = entity;
			damageParams.damage = damage;
			
			params.rpgSystem.DealDamage(damageParams);
		}
		
	}
}
