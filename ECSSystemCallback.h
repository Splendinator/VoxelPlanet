#pragma once

#include "ECSSystemCallbackBase.h"
#include "ECS.h"

/** ECSSystemCallback
 * 
 * Templated system callback.
 * This will call a Tick() function in the provided TSystem and pass through a tuple of all of the components that said system is using
 * 
 * System callbacks should be set up in a system's initialise function
 */
template<typename TSystem, typename... T>
class ECSSystemCallback : public ECSSystemCallbackBase
{
public:
	ECSSystemCallback(TSystem* pInSystem) : pSystem(pInSystem) {};
	ECSSystemCallback(const ECSSystemCallback& other) = default;
	ECSSystemCallback(ECSSystemCallback&& other) = default;
	~ECSSystemCallback() = default;

	ECSSystemCallback& operator=(ECSSystemCallback& other) = default;
	ECSSystemCallback& operator=(ECSSystemCallback&& other) = default;


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

	//~ Begin ECSSystemCallbackBase Interface
	void DoCallback(ECS* pEcs, float deltaTime, int frame) override;
	void DoEntityDeletion(ECS* pEcs, EntityId entity) override;
	//~ End ECSSystemCallbackBase Interface

	TSystem* pSystem = nullptr;
};

template<typename TSystem, typename... T>
void ECSSystemCallback<TSystem, T...>::DoCallback(ECS* pEcs, float deltaTime, int frame)
{
	ECSSystemTickParams params;
	params.deltaTime = deltaTime;
	params.pEcs = pEcs;
	params.frame = frame;

	std::tuple<T*...> components;
	
	for (EntityId i = 0; i < NUM_ENTITIES; ++i) // #TODO: This should probably cache a list of entities that updates if anything changes
	{
		if (pEcs->EntityHasComponents<T...>(i))
		{
			params.entityId = i;
			
			PopulateTuple<T...>(components, pEcs, i);

			pSystem->Tick(params, components);
		}
	}
}

template<typename TSystem, typename... T>
void ECSSystemCallback<TSystem, T...>::DoEntityDeletion(ECS* pEcs, EntityId entity)
{
	if (pEcs->EntityHasComponents<T...>(entity))
	{
		ECSSystemEntityDeletionParams params = {};
		params.pEcs = pEcs;
		params.entityId = entity;
		
		std::tuple<T*...> components;
		PopulateTuple<T...>(components, pEcs, entity);

		pSystem->OnEntityDeleted(params, components);
	}
}
