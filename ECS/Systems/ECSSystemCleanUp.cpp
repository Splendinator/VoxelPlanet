#include "pch.h"

#include "ECSSystemCleanUp.h"

#include "ECS/ECS.h"
#include "ECS/Systems/Callbacks/ECSSystemCallback.h"

void ECSSystemCleanUp::InitialiseInternal(ECS* pEcs)
{
	pEcs->RegisterSystemCallback(std::make_unique<ECSSystemCallback<ECSSystemCleanUp, ComponentTransform>>(this));
}

void ECSSystemCleanUp::Tick(const ECSSystemTickParams& params, const std::tuple<ComponentTransform*>& components)
{
	ComponentTransform* pTransform = std::get<ComponentTransform*>(components);
	
	pTransform->lastX = pTransform->x;
	pTransform->lastY = pTransform->y;
}

void ECSSystemCleanUp::OnEntityDeleted(const ECSSystemEntityDeletionParams& params, const std::tuple<ComponentTransform*>& components)
{
	ComponentTransform* pTransform = std::get<ComponentTransform*>(components);

	pTransform->lastX = pTransform->lastY = -1;
}
