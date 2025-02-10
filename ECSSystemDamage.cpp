#include "pch.h"

#include "ECSSystemDamage.h"

#include "Components.h"
#include "ECSSystemCallback.h"

void ECSSystemDamage::InitialiseInternal(ECS* pEcs)
{
	pEcs->RegisterSystemCallback(std::make_unique<ECSSystemCallback<ECSSystemDamage, ComponentHealth>>(this));
}

void ECSSystemDamage::Tick(const ECSSystemTickParams& params, const std::tuple<ComponentHealth*>& components)
{
	ComponentHealth* pHealth = std::get<ComponentHealth*>(components);
	if (pHealth->health <= 0)
	{
		params.pEcs->DeleteEntity(params.entityId);
	}
}
