#pragma once

#include "WorldGenerationTypes.h"
#include "ECS/ECSTypes.h"

class ECS;
class WorldGenerationContinent;

// Represents a small chunk of the world 
class WorldGenerationChunk
{
public:

	WorldGenerationChunk(ECS* pEcs, WorldGenerationContinent* pContinent, int inX, int inY);

	static constexpr int CHUNK_SIZE = 6; // Should be even
	static constexpr int HALF_CHUNK_SIZE = CHUNK_SIZE / 2;

	void DeleteChunk(ECS* pEcs);

	int GetChunkX() const { return gridX; }
	int GetChunkY() const { return gridY; }

private:

	EntityId& GetEntityRef(EWorldGenerationLayer layer, int x, int y) { return tileEntities[x * (2 * CHUNK_SIZE) + y * 2 + (int)layer]; };

	int gridX;
	int gridY;

	// This array contains all the static tile entities in this chunk.
	// The chunk owns these entities and deleted them when it despawns. 
	EntityId tileEntities[(int)EWorldGenerationLayer::COUNT * CHUNK_SIZE * CHUNK_SIZE];
};