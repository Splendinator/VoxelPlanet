#include "pch.h"

#include "ActionDeciderProjectile.h"

#include "ECS/ECS.h"

ActionHandlerBase* ActionDeciderProjectile::DecideAction(ECS& ecs, EntityId entity)
{
	DOMLOG_WARN_IF(!ecs.EntityHasComponents<ComponentProjectile>(entity))

	// #TEMP: const ComponentProjectile& projectileComponent = ecs.GetComponent<ComponentProjectile>(entity);

	 return nullptr;
}
