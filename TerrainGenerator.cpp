#include "TerrainGenerator.h"

#include "DomLog/DomLog.h"
#include "ChunkManager.h"
#include "ChunkUtils.h"
#include "PerlinNoise.h"

#include <thread>

TerrainGenerator::TerrainGenerator(ChunkManager* pInChunkManager)
{
	pChunkManager = pInChunkManager;
}

void TerrainGenerator::Initialise(u32 numThreads)
{
	bActive = true;

	threads.resize(numThreads);

	for (u32 i = 0; i < numThreads; ++i)
	{
		threads[i] = new std::thread(&TerrainGenerator::ThreadLoop, this);
		threads[i]->detach();
	}
}

void TerrainGenerator::Uninitialise()
{
	bActive = false;

	for (std::thread* thread : threads)
	{
		if (thread->joinable())
		{
			thread->join();
		}

		delete thread;
	}
}

int TerrainGenerator::GetHeightAt(int x, int y)
{
	const float xParam = abs(x) / 100.f;
	const float yParam = abs(y) / 100.f;
	return 64.0f + 32.0f * Noise::perlinNoise2D(xParam, yParam);
}

void TerrainGenerator::GenerateChunkTerrain(Chunk& chunk)
{
	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int y = 0; y < CHUNK_SIZE; ++y)
		{
			const int height = GetHeightAt(chunk.pos.x * CHUNK_SIZE + x, chunk.pos.y * CHUNK_SIZE + y);
			
			for (int i = 0; i < height; ++i)
			{
				chunk.GetBlockRef(x, i, y) = BlockType::Grass;
			}
		}
	}
}

void TerrainGenerator::ThreadLoop(TerrainGenerator* pGenerator)
{
	Vec2i outChunkPos;

	while (pGenerator->bActive)
	{
		if (pGenerator->pChunkManager->GetNextChunkToRender(outChunkPos))
		{
			Chunk* newChunk = new Chunk; ///TODO: Just reuse chunks instead of making a new one each time
			newChunk->pos = outChunkPos;

			pGenerator->GenerateChunkTerrain(*newChunk);

			pGenerator->pChunkManager->AddChunk(newChunk);
		}
	}
}
