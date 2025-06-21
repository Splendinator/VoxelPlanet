#include "pch.h"

#include "ActionDeciderBase.h"

#include "AI/Actions/ActionHandlers/ActionHandlerAttack.h"
#include "AI/Actions/ActionHandlers/ActionHandlerMove.h"
#include "ECS/ECS.h"
#include "ECS/ECSTypes.h"
#include "ECS/Systems/ECSSystemEntityMap.h"
#include "ECS/Systems/ECSSystemPhysics.h"

ActionHandlerBase* ActionDeciderBase::TryMoveOrAttack(ECS& ecs, EntityId e, int deltaX, int deltaY)
{
	if (ActionHandlerBase* pAction = TryMove(ecs, e, deltaX, deltaY))
	{
		return pAction;
	}

	if (ECSSystemEntityMap* pSystemEntityMap = ecs.GetSystem<ECSSystemEntityMap>())
	{
		const int currentX = ecs.GetComponent<ComponentTransform>(e).x;
		const int currentY = ecs.GetComponent<ComponentTransform>(e).y;
		const int targetX = currentX + deltaX;
		const int targetY = currentY + deltaY;
		
		const EntityList& entityList = pSystemEntityMap->GetEntities(targetX, targetY);
		for (EntityId entity : entityList.entities)
		{
			if (ecs.EntityHasComponents<ComponentHealth>(entity))
			{
				if (pAttackAction)
				{
					ActionHandlerAttack& actionHandlerAttack = *pAttackAction;
					actionHandlerAttack.target = entity;
					return &actionHandlerAttack;
				}
			}
		}
	}
	
	return nullptr;
}

ActionHandlerBase* ActionDeciderBase::TryMove(ECS& ecs, EntityId e, int deltaX, int deltaY)
{
	if (ECSSystemPhysics* pSystemPhysics = ecs.GetSystem<ECSSystemPhysics>())
	{
		const int currentX = ecs.GetComponent<ComponentTransform>(e).x;
		const int currentY = ecs.GetComponent<ComponentTransform>(e).y;
		const int targetX = currentX + deltaX;
		const int targetY = currentY + deltaY;

		if (pSystemPhysics->CanMoveTo(targetX, targetY))
		{
			if (pMoveAction)
			{
				ActionHandlerMove& actionHandlerMove = *pMoveAction;
				actionHandlerMove.xOffset = deltaX;
				actionHandlerMove.yOffset = deltaY;
				return &actionHandlerMove;
			}
		}
	}
	
	return nullptr;
}
