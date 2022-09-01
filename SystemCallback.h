#pragma once

#include "SystemCallbackBase.h"
#include "ECS.h"

/** SystemCallback
 * 
 * Templated system callback.
 * This will call a Tick() function in the provided TSystem and pass through a tuple of all of the components that said system is using
 * 
 * System callbacks should be set up in a system's initialise function
 */
template<typename TSystem, typename... T>
class SystemCallback : public SystemCallbackBase
{
public:
	SystemCallback(TSystem* pInSystem) : pSystem(pInSystem) {};
	SystemCallback(const SystemCallback& other) = default;
	SystemCallback(SystemCallback&& other) = default;
	~SystemCallback() = default;

	SystemCallback& operator=(SystemCallback& other) = default;
	SystemCallback& operator=(SystemCallback&& other) = default;


protected:

	template<typename THead, typename... TTail>
	void PopulateTuple(std::tuple<T*...>& tuple, ECS* pEcs, int entityIndex)
	{
		std::get<THead*>(tuple) = &pEcs->GetComponent<THead>(entityIndex);
		if constexpr (sizeof...(TTail) > 0)
		{
			PopulateTuple<TTail...>(tuple, pEcs, entityIndex);
		}
	};

	//~ Begin SystemCallbackBase Interface
	void DoCallback(ECS* pEcs, float deltaTime) override;
	void DoEntityDeletion(ECS* pEcs, EntityId entity) override;
	//~ End SystemCallbackBase Interface

	TSystem* pSystem = nullptr;
};

template<typename TSystem, typename... T>
void SystemCallback<TSystem, T...>::DoCallback(ECS* pEcs, float deltaTime)
{
	SystemTickParams params;
	params.deltaTime = deltaTime;
	params.pEcs = pEcs;

	for (EntityId i = 0; i < ECS::NUM_ENTITIES; ++i) // #TODO: This should probably cache a list of entities that updates if anything changes
	{
		if (pEcs->EntityHasComponents<T...>(i))
		{
			params.entityId = i;

			std::tuple<T*...> tuple;
			PopulateTuple<T...>(tuple, pEcs, i);

			pSystem->Tick(params, std::move(tuple));
		}
	}
}

template<typename TSystem, typename... T>
void SystemCallback<TSystem, T...>::DoEntityDeletion(ECS* pEcs, EntityId entity)
{
	if (pEcs->EntityHasComponents<T...>(entity))
	{
		SystemEntityDeletionParams params = {};
		params.pEcs = pEcs;
		params.entityId = entity;
		
		std::tuple<T*...> tuple;
		PopulateTuple<T...>(tuple, pEcs, entity);

		pSystem->OnEntityDeleted(params, std::move(tuple));
	}
}
