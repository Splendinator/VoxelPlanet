#include "pch.h"

#include "ActionDeciderBase.h"

#include "Actions/ActionHandlers/ActionHandlerAttack.h"
#include "Actions/ActionHandlers/ActionHandlerMove.h"
#include "ECS/ECS.h"
#include "ECS/ECSTypes.h"
#include "ECS/Systems/ECSSystemEntityMap.h"
#include "ECS/Systems/ECSSystemPhysics.h"

ActionHandlerBase* ActionDeciderBase::TryMoveOrAttack(ECS& ecs, EntityId e, int deltaX, int deltaY)
{
	if (ECSSystemPhysics* pSystemPhysics = ecs.GetSystem<ECSSystemPhysics>())
	{
		// Get entities current location
		const int currentX = ecs.GetComponent<ComponentTransform>(e).x;
		const int currentY = ecs.GetComponent<ComponentTransform>(e).y;
		const int targetX = currentX + deltaX;
		const int targetY = currentY + deltaY;

		if (pSystemPhysics->CanMoveTo(targetX, targetY))
		{
			ActionHandlerMove& actionHandlerMove = ActionHandlerMove::GetSingleton();
			actionHandlerMove.xOffset = deltaX;
			actionHandlerMove.yOffset = deltaY;
			return &actionHandlerMove;
		}
		else if (ECSSystemEntityMap* pSystemEntityMap = ecs.GetSystem<ECSSystemEntityMap>())
		{
			const EntityList& entityList = pSystemEntityMap->GetEntities(targetX, targetY);
			for (EntityId entity : entityList.entities)
			{
				if (ecs.EntityHasComponents<ComponentHealth>(entity))
				{
					ActionHandlerAttack& actionHandlerAttack = ActionHandlerAttack::GetSingleton();
					actionHandlerAttack.enemy = entity;
					actionHandlerAttack.damage = 20; // #TODO: Get damage from proper formula
					return &actionHandlerAttack;
				}
			}
		}
	}

	return nullptr;
}
