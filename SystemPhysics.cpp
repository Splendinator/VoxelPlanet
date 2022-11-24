#include "pch.h"

#include "SystemPhysics.h"

#include "Components.h"
#include "ECS.h"
#include "SystemCallback.h"

SystemPhysics::~SystemPhysics()
{
	DOMLOG_PRINT("Destructing Physics");
}

bool SystemPhysics::CanMoveTo(int x, int y) const
{
	const int effectiveX = GetEffectiveX(x);
	const int effectiveY = GetEffectiveY(y);

	return !bGrid[effectiveX][effectiveY];
}

void SystemPhysics::Tick(const SystemTickParams& params, const std::tuple<ComponentTransform*, ComponentRigid*>& components)
{
	// #TODO: This needs to handle transform components moving

	ComponentTransform* pTransform = std::get<ComponentTransform*>(components);

	const int effectiveX = GetEffectiveX(pTransform->x);
	const int effectiveY = GetEffectiveY(pTransform->y);
	const int lastEffectiveX = GetEffectiveX(pTransform->lastX);
	const int lastEffectiveY = GetEffectiveY(pTransform->lastY);

	// #JANK: I think this will fail if two things move in the same tick and the second thing moves into the first thing's old space but that shouldn't happen unless we do multiple actions in a single tick
	bGrid[lastEffectiveX][lastEffectiveY] = false;
	bGrid[effectiveX][effectiveY] = true;
}

void SystemPhysics::OnEntityDeleted(const SystemEntityDeletionParams& params, const std::tuple<ComponentTransform*, ComponentRigid*>& components)
{
	const int effectiveX = GetEffectiveX(std::get<ComponentTransform*>(components)->x);
	const int effectiveY = GetEffectiveY(std::get<ComponentTransform*>(components)->y);
	
	bGrid[effectiveX][effectiveY] = false;
}

void SystemPhysics::InitialiseInternal(ECS* pEcs)
{
	pEcs->RegisterSystemCallback(std::make_unique<SystemCallback<SystemPhysics, ComponentTransform, ComponentRigid>>(this));
}

int SystemPhysics::GetEffectiveX(int x) const
{
	return abs(x % GRID_SIZE);
}

int SystemPhysics::GetEffectiveY(int y) const
{
	return abs(y % GRID_SIZE);
}
