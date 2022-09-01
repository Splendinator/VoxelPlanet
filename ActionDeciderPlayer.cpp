#include "pch.h"

#include "ActionDeciderPlayer.h"

#include "ActionHandlerAttack.h"
#include "ActionHandlerMove.h"
#include "DomWindow/DomWindow.h"
#include "ECS.h"
#include "SystemEntityMap.h"
#include "SystemPhysics.h"

ActionHandlerBase* ActionDeciderPlayer::DecideAction(ECS& ecs, EntityId e)
{
	SystemPhysics* pSystemPhysics = ecs.GetSystem<SystemPhysics>();
	SystemEntityMap* pSystemEntityMap = ecs.GetSystem<SystemEntityMap>();

	// Movement
	if (pSystemPhysics)
	{
		// Get entities current location
		const int currentX = ecs.GetComponent<ComponentTransform>(e).x;
		const int currentY = ecs.GetComponent<ComponentTransform>(e).y;
		
		if (dmwi::isPressed(dmwi::W))
		{
			if (pSystemPhysics->CanMoveTo(currentX, currentY - 1))
			{
				ActionHandlerMove& actionHandlerMove = ActionHandlerMove::GetSingleton();
				actionHandlerMove.yOffset = -1;
				return &actionHandlerMove;
			}
			else if(pSystemEntityMap)
			{
				const EntityList& entityList = pSystemEntityMap->GetEntities(currentX, currentY - 1);
				for(EntityId entity : entityList.entities)
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
		else if (dmwi::isPressed(dmwi::A))
		{
			if (pSystemPhysics->CanMoveTo(currentX - 1, currentY))
			{
				ActionHandlerMove& actionHandlerMove = ActionHandlerMove::GetSingleton();
				actionHandlerMove.xOffset = -1;
				return &actionHandlerMove;
			}
			else if (pSystemEntityMap)
			{
				const EntityList& entityList = pSystemEntityMap->GetEntities(currentX - 1, currentY);
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
		else if (dmwi::isPressed(dmwi::S))
		{
			if (pSystemPhysics->CanMoveTo(currentX, currentY + 1))
			{
				ActionHandlerMove& actionHandlerMove = ActionHandlerMove::GetSingleton();
				actionHandlerMove.yOffset = 1;
				return &actionHandlerMove;
			}
			else if (pSystemEntityMap)
			{
				const EntityList& entityList = pSystemEntityMap->GetEntities(currentX, currentY + 1);
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
		else if (dmwi::isPressed(dmwi::D))
		{
			if (pSystemPhysics->CanMoveTo(currentX + 1, currentY))
			{
				ActionHandlerMove& actionHandlerMove = ActionHandlerMove::GetSingleton();
				actionHandlerMove.xOffset = 1;
				return &actionHandlerMove;
			}
			else if (pSystemEntityMap)
			{
				const EntityList& entityList = pSystemEntityMap->GetEntities(currentX + 1, currentY);
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
	}

	return nullptr;
}
