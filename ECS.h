#pragma once

#include "Components.h"
#include "Entity.h"
#include "SystemBase.h"
#include "SystemCallbackBase.h"
#include "ECSTypes.h"

#define REGISTER_COMPONENT(ComponentName) \
private:\
ComponentName m##ComponentName[NUM_ENTITIES];\
public:\
template<>\
ComponentName& GetComponent<ComponentName>(int entityId) { return m##ComponentName[entityId]; }\
template<>\
bool EntityHasComponents<ComponentName>(int entityId) const { return entities[entityId].components.HasComponent(EComponents::##ComponentName); }\
template<>\
ComponentName& AddComponent<ComponentName>(int entityId) { entities[entityId].components.AddComponent(EComponents::##ComponentName); return GetComponent<ComponentName>(entityId); }

class ECS
{

public:
	static constexpr int NUM_ENTITIES = 4096;
	static constexpr int INVALID_ENTITY_ID = -1;

	void RegisterSystem(std::unique_ptr<SystemBase>&& pSystem);
	void RegisterSystemCallback(std::unique_ptr<SystemCallbackBase>&& pSystemCallback);

	void Tick(float deltaTime);

	void Uninitialise();

	Entity& GetEntity(EntityId entityId) { return entities[entityId]; }

	// Get the next unused entity (entity with no components)
	EntityId GetNextFreeEntity();

	void DeleteEntity(EntityId entity) { GetEntity(entity).components.RemoveAllComponents(); };

	template<typename T>
	T& GetComponent(EntityId entityId) { DOMASSERT(false, "This shouldn't be called! Are you missing a REGISTER_COMPONENT?"); }
	
	template<typename THead, typename... TTail>
	bool EntityHasComponents(EntityId entityId) const;

	template<typename T>
	T& AddComponent(EntityId entityId) { DOMASSERT(false, "This shouldn't be called! Are you missing a REGISTER_COMPONENT?"); }

	REGISTER_COMPONENT(ComponentMesh);
	REGISTER_COMPONENT(ComponentTransform);
	REGISTER_COMPONENT(ComponentAction);

private:

	std::vector<std::unique_ptr<SystemBase>> systems;
	std::vector<std::unique_ptr<SystemCallbackBase>> systemCallbacks;

	Entity entities[NUM_ENTITIES];
};

template<typename THead, typename... TTail>
bool ECS::EntityHasComponents(EntityId entityId) const
{
	return EntityHasComponents<THead>(entityId) && EntityHasComponents<TTail...>(entityId);
}
