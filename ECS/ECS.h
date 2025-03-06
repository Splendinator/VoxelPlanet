#pragma once

#include "Core/GameSystem.h"

#include "Components.h"
#include "ECS/Systems/ECSSystemBase.h"
#include "ECS/ECSTypes.h"
#include "Entity.h"
#include "ECS/Systems/Callbacks/ECSSystemCallbackBase.h"

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

class RPGSkillSystem;
// The entity component system.
// This class was designed to be very speedy, i.e data oriented design with minimal virtual functions, so it might seem a bit less intuitive to code for.
// The basic idea is that all components are just structs in an array, and entities are just indexes into those arrays.
// Then the ECSSystems just iterate though the arrays and find any entity that has the correct combination of components and ticks that.
// (e.g. the render system will iterate through all entities with a ComponentTransform and ComponentMesh and render the mesh at the location).
EDITORCLASS()
class ECS : public GameSystem
{
	EDITORBODY()

	// #TODO: We need to handle adding new component / removing components calling the appropriate functions like HandleEntityDeletion() and HandleEntityCreation() maybe
	// #TODO: Right now if we remove the rendering component we won't call ECSSystemRender::OnEntityDeleted() so it'll stick around forever (until the entity is re-used)
public:
	
	//~ Begin GameSystem Interface
	void Init() override;
	void Tick(float deltaTime);
	void UnInit();
	//~ End GameSystem Interface
	
	void RegisterSystemCallback(std::unique_ptr<ECSSystemCallbackBase>&& pSystemCallback);
	
	// Get the next unused entity (entity with no components). Useful if adding a new entity
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
	REGISTER_COMPONENT(ComponentClass);
	REGISTER_COMPONENT(ComponentProgression);
	REGISTER_COMPONENT(ComponentRace);

private:
	
	Entity& GetEntity(EntityId entityId) { return entities[entityId]; }

	// The systems initialise and tick in the order they are in this array so order matters.
	// e.g. ECSSystemRender will want to do at the end or you won't render the updated scene until next tick. 
	EDITORPROPERTY()
	std::vector<ECSSystemBase*> systems;

	// #TEMP: Remove this
	EDITORPROPERTY()
	RPGSkillSystem* pSkillSystem = nullptr;
	
	std::vector<std::unique_ptr<ECSSystemCallbackBase>> systemCallbacks;

	Entity entities[NUM_ENTITIES] = {};

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
	for (ECSSystemBase* pSystem : systems)
	{
		if (T* pReturnedSystem = dynamic_cast<T*>(pSystem))
		{
			return pReturnedSystem;
		}
	}
	return nullptr;
}
