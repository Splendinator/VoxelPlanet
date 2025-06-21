#pragma once

#include "ActionDeciderBase.h"

class ActionHandlerProjectile;
class ECSSystemAction;
class ECSSystemEntityMap;
struct ComponentProjectile;

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

	// If the distance between the entry and exit point of the projectile is less than this squared then we skip the square
	// (basically, if the projectile only barely scrapes a square then move to the next one, likely doing a diagonal move instead of orthogonal)
	EDITORPROPERTY()
	float skipSquareThresholdSq = 0.2f;

	EDITORPROPERTY()
	ActionHandlerProjectile* pProjectileAction = nullptr;
};
