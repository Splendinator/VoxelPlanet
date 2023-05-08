#include "pch.h"

#include "ActionDeciderAI.h"

#include "ActionHandlerWait.h"
#include "Components.h"
#include "ECS.h"
#include "ECSTypes.h"

#include <limits>

ActionHandlerBase* ActionDeciderAI::DecideAction(ECS& ecs, EntityId actor)
{
	if (ecs.EntityHasComponents<ComponentTransform, ComponentFaction>(actor))
	{
		ComponentTransform& actorTransform = ecs.GetComponent<ComponentTransform>(actor);
		ComponentFaction& actorFaction = ecs.GetComponent<ComponentFaction>(actor);

		const int actorX = actorTransform.x;
		const int actorY = actorTransform.y;

		struct ClosestEnemyData
		{
			int enemyDistance = std::numeric_limits<int>::max();
			int enemyX;
			int enemyY;
			bool bValid = false;
		} closestEnemyData;

		// Find closest enemy
		ecs.ForEachEntity([&](EntityId enemy)
			{
				if (ecs.EntityHasComponents<ComponentFaction, ComponentTransform>(enemy))
				{
					ComponentFaction& enemyFaction = ecs.GetComponent<ComponentFaction>(enemy);
				
					if (IsWarringFactions(enemyFaction, actorFaction))
					{
						ComponentTransform& enemyTransform = ecs.GetComponent<ComponentTransform>(enemy);
						const int enemyX = enemyTransform.x;
						const int enemyY = enemyTransform.y;

						const int distance = abs(actorX - enemyX) + abs(actorY - enemyY);
						if (distance < closestEnemyData.enemyDistance)
						{
							closestEnemyData.enemyDistance = distance;
							closestEnemyData.enemyX = enemyX;
							closestEnemyData.enemyY = enemyY;
							closestEnemyData.bValid = true;
						}
					}
				}
			});

		if (closestEnemyData.bValid)
		{
			int deltaX = closestEnemyData.enemyX - actorX;
			int deltaY = closestEnemyData.enemyY - actorY;

			if (deltaX != 0) { deltaX = deltaX / abs(deltaX); }
			if (deltaY != 0) { deltaY = deltaY / abs(deltaY); }

			if (ActionHandlerBase* pActionHandler = TryMoveOrAttack(ecs, actor, deltaX, deltaY))
			{
				return pActionHandler;
			}
		}
	}

	return &ActionHandlerWait::GetSingleton();
}

bool ActionDeciderAI::IsWarringFactions(const ComponentFaction& faction, const ComponentFaction& otherFaction) const
{
	// #TODO: We'll have a whole proper system for this later with rep points and all sorts of crazy shit
	return faction.factionFlags != otherFaction.factionFlags;
}
