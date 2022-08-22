#include "pch.h"

#include "ECS.h"

void ECS::RegisterSystem(std::unique_ptr<SystemBase>&& pSystem)
{
	pSystem->Initialise(this);
	systems.emplace_back(std::move(pSystem));
}

void ECS::RegisterSystemCallback(std::unique_ptr<SystemCallbackBase>&& pSystemCallback)
{
	systemCallbacks.emplace_back(std::move(pSystemCallback));
}

void ECS::Tick(float deltaTime)
{
	for (std::unique_ptr<SystemCallbackBase>& callback : systemCallbacks)
	{
		callback->HandleCallbacks(this, deltaTime);
	}
}

void ECS::Uninitialise()
{
	systemCallbacks.clear();
	memset((void*)&systemCallbacks, 0, sizeof(systemCallbacks)); // #TODO: For some reason we get an error without this line here, maybe investigate later on if you can be arsed ever.

	systems.clear();
}

EntityId ECS::GetNextFreeEntity()
{
	// #TODO: This can be optimized by caching the previously fetched entity and starting from there next time
	for (EntityId entity = 0; entity < NUM_ENTITIES; ++entity)
	{
		if (!GetEntity(entity).components.HasAnyComponents())
		{
			return entity;
		}
	}

	DOMLOG_WARN("Out of entitites! Increase NUM_ENTITIES!");
	return ECS::INVALID_ENTITY_ID;
}