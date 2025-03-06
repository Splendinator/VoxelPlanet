#include "pch.h"

#include "ECS.h"

#include "Actions/ActionDeciders/ActionDeciderPlayer.h"
#include "RendererObject.h"
#include "WorldGenerator.h"

void ECS::RegisterSystemCallback(std::unique_ptr<ECSSystemCallbackBase>&& pSystemCallback)
{
	systemCallbacks.emplace_back(std::move(pSystemCallback));
}

void ECS::Init()
{
	for (ECSSystemBase* pSystem : systems)
	{
		pSystem->Initialise(this);
	}
	
	// Reserve bare bones player entity at entityId = 0
	// Other systems are responsible for setting up player components
	// #TODO: Most of these should be moved out of here and into the correct systems
	{
		Entity& e = GetEntity(playerEntity);
		e.components.AddComponent(EComponents::ComponentTransform);
		e.components.AddComponent(EComponents::ComponentAction);
		e.components.AddComponent(EComponents::ComponentFaction);
		e.components.AddComponent(EComponents::ComponentRigid);

		GetComponent<ComponentTransform>(playerEntity).x = WORLD_START_X;
		GetComponent<ComponentTransform>(playerEntity).y = WORLD_START_Y;

		// #TODO: Redo action deciders
		ActionDeciderPlayer* pPlayerActionDecider = new ActionDeciderPlayer;
		pPlayerActionDecider->pSkillSystem = pSkillSystem;
		GetComponent<ComponentAction>(playerEntity).pActionDecider = pPlayerActionDecider;
		GetComponent<ComponentAction>(playerEntity).maxEnergy = 100;
		GetComponent<ComponentAction>(playerEntity).energy = 100;
		
		GetComponent<ComponentFaction>(playerEntity).factionFlags = ComponentFaction::EFactionFlags::Player;
	}
}

void ECS::Tick(float deltaTime)
{
	// #TODO: Need a way to only tick a entities if the component has changed, maybe just a bool in the base class of all components?
	
	// System Pre-Tic
	ECSSystemTickParams params;
	params.pEcs = this;
	params.entityId = INVALID_ENTITY_ID;
	params.deltaTime = deltaTime;
	params.frame = frame;
	for (ECSSystemBase* pSystem : systems)
	{
		pSystem->PreTick(params);
	}

	// System Callbacks
	for (std::unique_ptr<ECSSystemCallbackBase>& callback : systemCallbacks)
	{
		callback->HandleCallbacks(this, deltaTime, frame);
	}

	++frame;
}

void ECS::UnInit()
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
	for (std::unique_ptr<ECSSystemCallbackBase>& callback : systemCallbacks)
	{
		callback->HandleEntityDeletion(this, entity);
	}
	GetEntity(entity).components.RemoveAllComponents();
}
