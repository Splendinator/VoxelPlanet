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

void EntityList::TryRemoveEntity(EntityId oldEntity)
{
	for (EntityId& entity : entities)
	{
		if (entity == oldEntity)
		{
			entity = INVALID_ENTITY_ID;
			return;
		}
	}
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
	ComponentTransform* pTransformComponent = std::get<ComponentTransform*>(components);
	
	if (pTransformComponent->x != pTransformComponent->lastX || pTransformComponent->y != pTransformComponent->lastY)
	{
		const int oldHashX = GetHashX(pTransformComponent->lastX);
		const int oldHashY = GetHashY(pTransformComponent->lastY);
		const int newHashX = GetHashX(pTransformComponent->x);
		const int newHashY = GetHashY(pTransformComponent->y);
		
		entityMap[oldHashX][oldHashY].TryRemoveEntity(params.entityId);
		entityMap[newHashX][newHashY].AddEntity(params.entityId);
	}	
}

void SystemEntityMap::OnEntityDeleted(const SystemEntityDeletionParams& params, const std::tuple<ComponentTransform*>& components)
{
	ComponentTransform* pTransformComponent = std::get<ComponentTransform*>(components);

	const int oldHashX = GetHashX(pTransformComponent->lastX);
	const int oldHashY = GetHashY(pTransformComponent->lastY);
	const int newHashX = GetHashX(pTransformComponent->x);
	const int newHashY = GetHashY(pTransformComponent->y);

	entityMap[oldHashX][oldHashY].TryRemoveEntity(params.entityId);
	entityMap[newHashX][newHashY].TryRemoveEntity(params.entityId);
}

int SystemEntityMap::GetHashX(int x) const
{
	return abs(x % HASH_MAP_SIZE);
}

int SystemEntityMap::GetHashY(int y) const
{
	return abs(y % HASH_MAP_SIZE);
}
