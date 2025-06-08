#include "pch.h"

#include "ActionDeciderProjectile.h"

#include "ECS/ECS.h"

// #TEMP: Incomplete function
#pragma warning( disable : 4189 )

ActionHandlerBase* ActionDeciderProjectile::DecideAction(ECS& ecs, EntityId entity)
{
	DOMLOG_WARN_IF(!(ecs.EntityHasComponents<ComponentTransform, ComponentProjectile>(entity)), "needs components");
	
	if (pActionSystem == nullptr)
	{
		DOMLOG_WARN("Needs action system");
		return nullptr;
	}

	const ComponentProjectile& projectileComponent = ecs.GetComponent<ComponentProjectile>(entity);
	const ComponentAction& actionComponent = ecs.GetComponent<ComponentAction>(entity);
	const ComponentTransform& transformComponent = ecs.GetComponent<ComponentTransform>(entity);

	const Vec2i startPos = {projectileComponent.startX, projectileComponent.startY};
	const Vec2i endPos = {projectileComponent.targetX, projectileComponent.targetY};

	const float totalLength = Vec2i::Distance(startPos, endPos);
	
	
	return nullptr;
}
