#include "pch.h"

#include "SystemEntityMap.h"

#include "Components.h"
#include "ECS.h"
#include "SystemCallback.h"

// EntityList
void EntityList::Reset()
{
	for (EntityId& entity : entities)
	{
		entity = INVALID_ENTITY_ID;
	}
}

void EntityList::AddEntity(EntityId newEntity)
{
	for (EntityId& entity : entities)
	{
		if (entity == INVALID_ENTITY_ID)
		{
			entity = newEntity;
			return;
		}
	}

	DOMLOG_ERROR("Failed to add entity, need to increase HASH_MAP_NUM_ENTITIES");
}

void EntityList::RemoveEntity(EntityId oldEntity)
{
	for (EntityId& entity : entities)
	{
		if (entity == oldEntity)
		{
			entity = INVALID_ENTITY_ID;
			return;
		}
	}

	DOMLOG_WARN("Removing entity that doesn't exist (", oldEntity, ")");
}

bool EntityList::Contains(EntityId entity) const
{
	for (EntityId thisEntity : entities)
	{
		if (thisEntity == entity)
		{
			return true;
		}
	}
	return false;
}

// SystemEntityMap
void SystemEntityMap::InitialiseInternal(ECS* pEcs)
{
	pEcs->RegisterSystemCallback(std::make_unique<SystemCallback<SystemEntityMap, ComponentTransform>>(this));

	for (int x = 0; x < HASH_MAP_SIZE; x++)
	{
		for (int y = 0; y < HASH_MAP_SIZE; y++)
		{
			entityMap[x][y].Reset();
		}
	}
}

const EntityList& SystemEntityMap::GetEntities(int x, int y) const
{
	return entityMap[GetHashX(x)][GetHashY(y)];
}

void SystemEntityMap::Tick(const SystemTickParams& params, const std::tuple<ComponentTransform*>& components)
{
	// #TODO: This needs to handle transform components moving
	
	const int hashX = GetHashX(std::get<ComponentTransform*>(components)->x);
	const int hashY = GetHashY(std::get<ComponentTransform*>(components)->y);
	
	EntityList& entityList = entityMap[hashX][hashY];
	if (!entityList.Contains(params.entityId))
	{
		entityList.AddEntity(params.entityId);
	}
}

void SystemEntityMap::OnEntityDeleted(const SystemEntityDeletionParams& params, const std::tuple<ComponentTransform*>& components)
{
	const int hashX = GetHashX(std::get<ComponentTransform*>(components)->x);
	const int hashY = GetHashY(std::get<ComponentTransform*>(components)->y);

	EntityList& entityList = entityMap[hashX][hashY];
	
	entityList.RemoveEntity(params.entityId);
}

int SystemEntityMap::GetHashX(int x) const
{
	return abs(x % HASH_MAP_SIZE);
}

int SystemEntityMap::GetHashY(int y) const
{
	return abs(y % HASH_MAP_SIZE);
}
