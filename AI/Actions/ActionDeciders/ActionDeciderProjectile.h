#pragma once

#include "ActionDeciderBase.h"

class ActionHandlerProjectile;
class ECSSystemAction;
class ECSSystemEntityMap;
struct ComponentProjectile;

// ActionDecider that makes projectiles move in a straight line towards their target
EDITORCLASS()
class ActionDeciderProjectile : public ActionDeciderBase
{
	EDITORBODY()
public:
	//~ Begin ActionDeciderBase Interface
	ActionHandlerBase* DecideAction(ECS& ecs, EntityId entity) override;
	//~ End ActionDeciderBase Interface

protected:

	ActionHandlerBase* TryProjectileAction(ECS& ecs, EntityId entity, const ComponentProjectile& projectileComponent);
	
	EDITORPROPERTY()
	ECSSystemAction* pActionSystem = nullptr;

	EDITORPROPERTY()
	ECSSystemEntityMap* pEntityMap = nullptr;

	// If the distance between the entry and exit point of the projectile on a grid square is less than this squared then we skip the square (see PathGridIntersectionPoints)
	// (basically, if the projectile only barely scrapes a square then move to the next one, likely doing a diagonal move instead of orthogonal)
	EDITORPROPERTY()
	float skipSquareThresholdSq = 0.2f;

	// Action that happens when at the projectile's location when the projectile reaches its target or collides along the way.
	EDITORPROPERTY()
	ActionHandlerProjectile* pProjectileAction = nullptr;
};
