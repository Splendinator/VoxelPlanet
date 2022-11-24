#include "pch.h"

#include "SystemCleanUp.h"

#include "ECS.h"
#include "SystemCallback.h"

void SystemCleanUp::InitialiseInternal(ECS* pEcs)
{
	pEcs->RegisterSystemCallback(std::make_unique<SystemCallback<SystemCleanUp, ComponentTransform>>(this));
}

void SystemCleanUp::Tick(const SystemTickParams& params, const std::tuple<ComponentTransform*>& components)
{
	ComponentTransform* pTransform = std::get<ComponentTransform*>(components);
	
	pTransform->lastX = pTransform->x;
	pTransform->lastY = pTransform->y;
}

void SystemCleanUp::OnEntityDeleted(const SystemEntityDeletionParams& params, const std::tuple<ComponentTransform*>& components)
{
	ComponentTransform* pTransform = std::get<ComponentTransform*>(components);

	pTransform->lastX = pTransform->lastY = -1;
}
