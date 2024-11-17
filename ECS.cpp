#include "pch.h"

#include "ECS.h"

#include "ActionDeciderPlayer.h"
#include "FilePaths.h"
#include "RendererObject.h"
#include "SystemAction.h"
#include "SystemCleanUp.h"
#include "SystemDamage.h"
#include "SystemEntityMap.h"
#include "SystemNameslate.h"
#include "SystemPhysics.h"
#include "SystemRender.h"
#include "WorldGenerator.h"

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

void ECS::Init()
{
	// Register systems to ECS (order matters)
	RegisterSystem(std::make_unique<SystemAction>());
	RegisterSystem(std::make_unique<SystemEntityMap>());
	RegisterSystem(std::make_unique<SystemPhysics>());
	RegisterSystem(std::make_unique<SystemDamage>());
	RegisterSystem(std::make_unique<SystemNameslate>());
	RegisterSystem(std::make_unique<SystemRender>());
	RegisterSystem(std::make_unique<SystemCleanUp>());
	
	// Create player entity
	{
		Entity& e = GetEntity(playerEntity);
		e.components.AddComponent(EComponents::ComponentMesh);
		e.components.AddComponent(EComponents::ComponentTransform);
		e.components.AddComponent(EComponents::ComponentAction);
		e.components.AddComponent(EComponents::ComponentHealth);
		e.components.AddComponent(EComponents::ComponentFaction);
		e.components.AddComponent(EComponents::ComponentRigid);
		GetComponent<ComponentMesh>(playerEntity).pRendererObject = dmgf::AddObjectFromSVG(FilePath::VectorArt::player);
		GetComponent<ComponentMesh>(playerEntity).pRendererObject->SetRenderPriority(RenderPriority::unit);
		GetComponent<ComponentTransform>(playerEntity).x = WORLD_START_X;
		GetComponent<ComponentTransform>(playerEntity).y = WORLD_START_Y;
		GetComponent<ComponentAction>(playerEntity).maxEnergy = 100;
		GetComponent<ComponentAction>(playerEntity).energy = 100;
		GetComponent<ComponentAction>(playerEntity).pActionDecider = new ActionDeciderPlayer;
		GetComponent<ComponentHealth>(playerEntity).health = 100;
		GetComponent<ComponentHealth>(playerEntity).maxHealth = 100;
		GetComponent<ComponentFaction>(playerEntity).factionFlags = ComponentFaction::EFactionFlags::Player;
	}
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
	for (std::unique_ptr<SystemCallbackBase>& callback : systemCallbacks)
	{
		callback->HandleEntityDeletion(this, entity);
	}
	GetEntity(entity).components.RemoveAllComponents();
}
