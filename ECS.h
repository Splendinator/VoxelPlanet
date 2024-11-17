#pragma once

#include "Core/GameSystem.h"

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

EDITORCLASS()
class ECS : public GameSystem
{
	EDITORBODY()

	/// #TODO: We need to handle adding new component / removing components calling the appropriate functions like HandleEntityDeletion() and HandleEntityCreation() maybe

public:
	
	//~ Begin GameSystem Interface
	void Init() override;
	void Tick(float deltaTime);
	void UnInit();
	//~ End GameSystem Interface

	void RegisterSystem(std::unique_ptr<SystemBase>&& pSystem);
	void RegisterSystemCallback(std::unique_ptr<SystemCallbackBase>&& pSystemCallback);
	
	// Get the next unused entity (entity with no components)
	EntityId GetNextFreeEntity();

	void DeleteEntity(EntityId entity);

	template<typename T> 
	T* GetSystem();

	template<typename T>
	T& GetComponent(EntityId entityId) { DOMASSERT(false, "This shouldn't be called! Are you missing a REGISTER_COMPONENT?"); }
	
	template<typename THead, typename... TTail>
	bool EntityHasComponents(EntityId entityId) const;

	template<typename T>
	T& AddComponent(EntityId entityId) { DOMASSERT(false, "This shouldn't be called! Are you missing a REGISTER_COMPONENT?"); }

	template<typename T>
	void ForEachEntity(const T& predicate) { for (EntityId e = 0; e < NUM_ENTITIES; ++e) { predicate(e); } }

	EntityId GetPlayerEntityId() const {return playerEntity;}

	REGISTER_COMPONENT(ComponentMesh);
	REGISTER_COMPONENT(ComponentTransform);
	REGISTER_COMPONENT(ComponentAction);
	REGISTER_COMPONENT(ComponentRigid);
	REGISTER_COMPONENT(ComponentHealth);
	REGISTER_COMPONENT(ComponentFaction);

private:

	Entity& GetEntity(EntityId entityId) { return entities[entityId]; }
	
	std::vector<std::unique_ptr<SystemBase>> systems;
	std::vector<std::unique_ptr<SystemCallbackBase>> systemCallbacks;

	Entity entities[NUM_ENTITIES];

	EntityId playerEntity = 0;

	int frame = 0; // The frame we are on, useful for debug or systems that want to do something different on the first frame. Is '0' for the first frame
};

template<typename THead, typename... TTail>
bool ECS::EntityHasComponents(EntityId entityId) const
{
	return EntityHasComponents<THead>(entityId) && EntityHasComponents<TTail...>(entityId);
}

template<typename T>
T* ECS::GetSystem()
{
	for (std::unique_ptr<SystemBase>& system : systems)
	{
		if (T* returnedSystem = dynamic_cast<T*>(system.get()))
		{
			return returnedSystem;
		}
	}
	return nullptr;
}
