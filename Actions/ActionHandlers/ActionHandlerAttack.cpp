#include "pch.h"

#include "ActionHandlerAttack.h"

#include "Ecs/Components.h"
#include "Ecs/ECS.h"

void ActionHandlerAttack::DoAction(ECS& ecs, EntityId e)
{
	// #TODO: Proper damage formula using like strength stats and defense armour shite and whatever and dodge and shit and whatever
	ecs.GetComponent<ComponentHealth>(enemy).health -= damage;
}

void ActionHandlerAttack::Reset()
{
	enemy = INVALID_ENTITY_ID;
	damage = 0;
}
