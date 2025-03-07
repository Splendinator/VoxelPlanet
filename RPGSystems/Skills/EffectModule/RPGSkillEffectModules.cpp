#include "pch.h"

#include "RPGSkillEffectModules.h"

#include "ECS/ECS.h"
#include "ECS/Systems/ECSSystemEntityMap.h"

void RPGSkillEffectModuleTemp::ApplyEffect(const RPGSkillParams& params) const
{
	const EntityList& entityList = params.entityMapSystem.GetEntities(params.targetPos.x, params.targetPos.y);

	for (EntityId entity : entityList.entities)
	{
		if (params.ecs.EntityHasComponents<ComponentHealth>(entity))
		{
			params.ecs.GetComponent<ComponentHealth>(entity).health -= damage;
		}
		
	}
}
