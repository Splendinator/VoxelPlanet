#pragma once

#include "DomMath/Types.h"

#include <vector>

class Chunk;
class ChunkManager;

namespace std
{
	class thread;
}

// Generates chunks and sends them to the chunk manager
class TerrainGenerator
{
public:
	
	TerrainGenerator(ChunkManager* pInChunkManager);

	void Initialise(u32 numThreads);
	void Uninitialise();

	// This is the function that the threads below will be in.
	static void ThreadLoop(TerrainGenerator* pGenerator);

private:

	void GenerateChunkTerrain(Chunk& chunk);

	int GetHeightAt(int x, int y);

	ChunkManager* pChunkManager = nullptr;

	std::vector<std::thread*> threads;

	// Threads listen to this to decide whether to contine generating or not
	bool bActive = false;
};

