#include "pch.h"

#include "ActionDeciderProjectile.h"

#include "AI/Actions/ActionHandlers/ActionHandlerBase.h"
#include "AI/Actions/ActionHandlers/ActionHandlerProjectile.h"
#include "AI/Paths/PathUtils.h"
#include "ECS/ECS.h"
#include "ECS/Systems/ECSSystemEntityMap.h"

ActionHandlerBase* ActionDeciderProjectile::DecideAction(ECS& ecs, EntityId entity)
{
	DOMLOG_WARN_IF(!(ecs.EntityHasComponents<ComponentTransform, ComponentProjectile>(entity)), "needs components");
	
	if (pActionSystem == nullptr)
	{
		DOMLOG_WARN("Needs action system");
		return nullptr;
	}

	const ComponentProjectile& projectileComponent = ecs.GetComponent<ComponentProjectile>(entity);
	ComponentTransform& transformComponent = ecs.GetComponent<ComponentTransform>(entity);
	ComponentAction& actionComponent = ecs.GetComponent<ComponentAction>(entity);

	// It's possible that something has moved onto the projectile during its turn, so we check if something is on top of the projectile.
	// If so, we doing the projectile's action
	// #TODO: This would be better if it reacted to the entity moving onto the square instead of checking here but realistically unless a projectile is moving very slowly or the entity is moving very quickly this should be fine.
	if (pEntityMap)
	{
		const EntityList& entityList = pEntityMap->GetEntities(transformComponent.x, transformComponent.y);

		for (EntityId entityId : entityList.entities)
		{
			if (entityId == INVALID_ENTITY_ID)
			{
				break;
			}
			
			if (entityId == ecs.GetPlayerEntityId())
			{
				// We spawn the projectile on the player, but don't want to collide with self
				continue;
			}
			
			if (ecs.EntityHasComponents<ComponentRigid>(entityId))
			{
				return TryProjectileAction(ecs, entity, projectileComponent);		
			}
		}
	}
	

	const Vec2i startPos = {projectileComponent.startX, projectileComponent.startY};
	const Vec2i endPos = {projectileComponent.targetX, projectileComponent.targetY};
	
	Vec2i newTransform = {transformComponent.x, transformComponent.y};
	float distancedTraveledThisTurn = 0.0f;

	PathGridIntersectionPoints intersectionPoints = PathUtils::GetGridIntersectionPointOfLine(startPos, endPos, {transformComponent.x, transformComponent.y});

	auto MoveToNextSquare = [&]()
	{
		newTransform.x += (int)(bool)(intersectionPoints.exitDirection & EIntersectionExitDirectionFlags::Right);
		newTransform.x -= (int)(bool)(intersectionPoints.exitDirection & EIntersectionExitDirectionFlags::Left);
		newTransform.y += (int)(bool)(intersectionPoints.exitDirection & EIntersectionExitDirectionFlags::Bottom);
		newTransform.y -= (int)(bool)(intersectionPoints.exitDirection & EIntersectionExitDirectionFlags::Top);
		intersectionPoints = PathUtils::GetGridIntersectionPointOfLine(startPos, endPos, newTransform);
		distancedTraveledThisTurn += Vec2f::Distance(intersectionPoints.entryPoint, intersectionPoints.exitPoint);	
	};
	
	if (Vec2f::DistanceSq(intersectionPoints.entryPoint, intersectionPoints.exitPoint) < skipSquareThresholdSq)
	{
		// If we are here we barely scraped the square, so skip it and move to the next one (this should  mean we are effectively doing a diagonal move instead of an orthogonal one)
		MoveToNextSquare();
	}
	
	MoveToNextSquare();

	// Make the turn time proportional to the distance traveled this turn
	// #TODO: This means that diagonal moves with take longer, do we want the same for enemies/player?
	actionComponent.maxEnergy = (u16)(distancedTraveledThisTurn * (100.f / projectileComponent.speed));
	
	if (newTransform.x == projectileComponent.targetX && newTransform.y == projectileComponent.targetY)
	{
		// If here we've reached the end of the path
		transformComponent.x = newTransform.x;
		transformComponent.y = newTransform.y;
		return TryProjectileAction(ecs, entity, projectileComponent);
	}
	
	const Vec2i delta = newTransform - Vec2i{transformComponent.x, transformComponent.y};
	if (ActionHandlerBase* pAction = TryMove(ecs, entity, delta.x, delta.y))
	{
		return pAction;
	}

	// If here we've hit something
	transformComponent.x = newTransform.x;
	transformComponent.y = newTransform.y;
	return TryProjectileAction(ecs, entity, projectileComponent);
}

ActionHandlerBase* ActionDeciderProjectile::TryProjectileAction(ECS& ecs, EntityId entity, const ComponentProjectile& projectileComponent)
{
	if (pProjectileAction)
	{
		pProjectileAction->Setup(projectileComponent.pProjectileEffect, projectileComponent.caster);
		return pProjectileAction;
	}

	DOMLOG_ERROR("No projectile action set up");
	ecs.DeleteEntity(entity);

	return nullptr;
}
