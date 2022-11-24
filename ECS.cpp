#include "pch.h"

#include "ECS.h"

void ECS::RegisterSystem(std::unique_ptr<SystemBase>&& pSystem)
{
	pSystem->Initialise(this);
	systems.emplace_back(std::move(pSystem));
}
#pragma optimize( "", on )

void ECS::RegisterSystemCallback(std::unique_ptr<SystemCallbackBase>&& pSystemCallback)
{
	systemCallbacks.emplace_back(std::move(pSystemCallback));
}

void ECS::Tick(float deltaTime)
{
	// #TODO: Need a way to only tick a entities if the component has changed, maybe just a bool in the base class of all components?

	// System Pre-Tick
	SystemTickParams params;
	params.pEcs = this;
	params.entityId = INVALID_ENTITY_ID;
	params.deltaTime = deltaTime;
	params.frame = frame;
	for (std::unique_ptr<SystemBase>& pSystem : systems)
	{
		pSystem->PreTick(params);
	}

	// System Callbacks
	for (std::unique_ptr<SystemCallbackBase>& callback : systemCallbacks)
	{
		callback->HandleCallbacks(this, deltaTime, frame);
	}

	++frame;
}

void ECS::Uninitialise()
{
	systemCallbacks.clear();
	systems.clear();
	
	memset((void*)&systemCallbacks, 0, sizeof(systemCallbacks)); // #JANK: For some reason we get an error without this line here, maybe investigate later on if you can be arsed ever.
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


	DOMLOG_ERROR("Out of entitites! Increase NUM_ENTITIES!");
	return INVALID_ENTITY_ID;
}

void ECS::DeleteEntity(EntityId entity)
{
	for (std::unique_ptr<SystemCallbackBase>& callback : systemCallbacks)
	{
		callback->HandleEntityDeletion(this, entity);
	}
	GetEntity(entity).components.RemoveAllComponents();
}
