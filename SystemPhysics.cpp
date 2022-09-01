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

	const int effectiveX = GetEffectiveX(std::get<ComponentTransform*>(components)->x);
	const int effectiveY = GetEffectiveY(std::get<ComponentTransform*>(components)->y);

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
