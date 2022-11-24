#include "pch.h"

#include "Chunk.h"
#include "Renderer.h"
#include "WorldGenerator.h"

void WorldGenerator::SetCenter(int newX, int newY, bool bInit)
{
	// #JANK: Negative numbers are a bit fucked, right now we spawn at (20000, 20000) to avoid this

	// Effective coords that we will use as the center coords
	const int effectiveX = newX;
	const int effectiveY = newY;

	// Min/max grid positions of the chunks
	const int minChunkX = effectiveX / Chunk::CHUNK_SIZE - NUM_CHUNKS;
	const int maxChunkX = minChunkX + (CHUNK_SQUARE_SIZE - 1);
	const int minChunkY = effectiveY / Chunk::CHUNK_SIZE - NUM_CHUNKS;
	const int maxChunkY = minChunkY + (CHUNK_SQUARE_SIZE - 1);

	// Delete old chunks
	bool bDeletedChunks = bInit;
	if (!bInit)
	{
		for (Chunk*& pChunk : pChunks)
		{
			const int chunkX = pChunk->GetChunkX();
			const int chunkY = pChunk->GetChunkY();

			if (chunkX > maxChunkX ||
				chunkX < minChunkX ||
				chunkY > maxChunkY ||
				chunkY < minChunkY)
			{
				pChunk->DeleteChunk(&ecs);
				delete pChunk;
				pChunk = nullptr;
				bDeletedChunks = true;
			}
		}
	}

	if (bDeletedChunks)
	{
		// Create new chunks
		Chunk* pNewChunks[CHUNK_SQUARE_SIZE * CHUNK_SQUARE_SIZE] = {};
		
		// Gets the new index in the pNewChunks for a given X and Y
		auto GetNewIndex = [=](int x, int y)
		{
			return (x - minChunkX) * CHUNK_SQUARE_SIZE + (y - minChunkY);
		};

		// Populate from current chunks
		for (Chunk* pChunk : pChunks)
		{
			if (pChunk)
			{
				pNewChunks[GetNewIndex(pChunk->GetChunkX(), pChunk->GetChunkY())] = pChunk;
			}
		}

		// Create new chunks in remaining chunks
		for (int x = minChunkX; x <= maxChunkX; ++x)
		{
			for (int y = minChunkY; y <= maxChunkY; ++y)
			{
				const int newIndex = GetNewIndex(x, y);
				if (pNewChunks[newIndex] == nullptr)
				{
					pNewChunks[newIndex] = new Chunk(&ecs, x, y);
				}
			}
		}

		memcpy(pChunks, pNewChunks, sizeof(pChunks));
	}
}

void WorldGenerator::Uninitialise()
{
	for (Chunk*& pChunk : pChunks)
	{
		pChunk->DeleteChunk(&ecs);
		delete pChunk;
		pChunk = nullptr;
	}
}
