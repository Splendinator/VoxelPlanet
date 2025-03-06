#include "pch.h"

#include "ActionHandlerMove.h"
#include "Ecs/ECS.h"

void ActionHandlerMove::DoAction(ECS& ecs, EntityId e)
{
	ComponentTransform& transformComponent = ecs.GetComponent<ComponentTransform>(e);
	transformComponent.x += xOffset;
	transformComponent.y += yOffset;
}
