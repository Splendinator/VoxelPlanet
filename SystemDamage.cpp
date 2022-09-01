#include "pch.h"

#include "SystemDamage.h"

#include "Components.h"
#include "SystemCallback.h"

void SystemDamage::InitialiseInternal(ECS* pEcs)
{
	pEcs->RegisterSystemCallback(std::make_unique<SystemCallback<SystemDamage, ComponentHealth>>(this));
}

void SystemDamage::Tick(const SystemTickParams& params, const std::tuple<ComponentHealth*>& components)
{
	ComponentHealth* pHealth = std::get<ComponentHealth*>(components);
	if (pHealth->health <= 0)
	{
		params.pEcs->DeleteEntity(params.entityId);
	}
}
