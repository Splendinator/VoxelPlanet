#pragma once

#include "ActionDeciderBase.h"

EDITORCLASS()
class ActionDeciderProjectile : public ActionDeciderBase
{
	EDITORBODY()
public:
	//~ Begin ActionDeciderBase Interface
	ActionHandlerBase* DecideAction(ECS& ecs, EntityId entity) override;
	//~ End ActionDeciderBase Interface
};
