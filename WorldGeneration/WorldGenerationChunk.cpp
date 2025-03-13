#include "pch.h"

#include "WorldGenerationChunk.h"

#include "WorldGenerationContinent.h"
#include "ECS/ECS.h"

WorldGenerationChunk::WorldGenerationChunk(ECS* pEcs, WorldGenerationContinent* pContinent, int inX, int inY) : gridX(inX), gridY(inY)
{
	if (pEcs && pContinent)
	{
		for (int x = 0; x < CHUNK_SIZE; ++x)
		{
			for (int y = 0; y < CHUNK_SIZE; ++y)
			{
				for (int layer = 0; layer < (int)EWorldGenerationLayer::COUNT; ++layer)
				{
					GetEntityRef((EWorldGenerationLayer)layer, x, y) = pContinent->CreateTileEntity((EWorldGenerationLayer)layer, {x + gridX * CHUNK_SIZE, y + gridY * CHUNK_SIZE});
				}
			}
		}
	}
}

void WorldGenerationChunk::DeleteChunk(ECS* pEcs)
{
	for (EntityId entity : tileEntities)
	{
		pEcs->DeleteEntity(entity);
	}
}