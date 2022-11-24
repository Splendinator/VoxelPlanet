#pragma once

#include "SystemBase.h"

#include "ECSTypes.h"

struct ComponentTransform;

struct EntityList
{
	static constexpr int HASH_MAP_NUM_ENTITIES = 4; // Max entities that can be in a single grid space

	void Reset();
	void AddEntity(EntityId newEntity);
	void TryRemoveEntity(EntityId oldEntity);
	bool Contains(EntityId entity) const;

	EntityId entities[HASH_MAP_NUM_ENTITIES];
};

/** SystemEntityMap
*
* This class is used for storing entities in a hash map based off their positions for an easy way to find entities in a specific square
*/
class SystemEntityMap : public SystemBase
{
	static constexpr int HASH_MAP_SIZE = 128; // Size of the hash map

public:
	
	// Get entities at a specific location
	const EntityList& GetEntities(int x, int y) const;

	void Tick(const SystemTickParams& params, const std::tuple<ComponentTransform*>& components);
	void OnEntityDeleted(const SystemEntityDeletionParams& params, const std::tuple<ComponentTransform*>& components);

protected:

	//~ Begin SystemBase Interface
	void InitialiseInternal(ECS* pEcs) override;
	//~ End SystemBase Interface
	 
private:
	
	// Used to get coordinates into the hash map
	int GetHashX(int x) const;
	int GetHashY(int y) const;

	EntityList entityMap[HASH_MAP_SIZE][HASH_MAP_SIZE];
};
