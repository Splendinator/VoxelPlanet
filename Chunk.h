#pragma once

#include "ECS.h"

class ChunkGeneratorBase;

enum EWorldGenerationLayer
{
	Background, // eg. Grass
	Foreground, // eg. Tree
	COUNT
};

class Chunk
{
public:

	Chunk(ECS* pEcs, int inX, int inY);
	~Chunk();

	static constexpr int CHUNK_SIZE = 6; // Should be even
	static constexpr int HALF_CHUNK_SIZE = CHUNK_SIZE / 2;

	void DeleteChunk(ECS* pEcs);

	int GetChunkX() const { return gridX; }
	int GetChunkY() const { return gridY; }

private:

	EntityId& GetEntity(EWorldGenerationLayer layer, int x, int y) { return entities[x * (2 * CHUNK_SIZE) + y * 2 + (int)layer]; };

	int gridX;
	int gridY;

	// #TODO: Strong pointer probably better here
	ChunkGeneratorBase* pChunkGenerator = nullptr;

	EntityId entities[EWorldGenerationLayer::COUNT * CHUNK_SIZE * CHUNK_SIZE];
};