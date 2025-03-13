#include "pch.h"

#include "WorldGenerationChunk.h"
#include "ECS/ECS.h"
#include "WorldGenerator.h"
#include "WorldGenerationContinent.h"

#include "Camera/CameraSystem.h"

#include "Core/GameInstance.h"

#include "DomWindow/DomWindow.h"

// #TEMP: Optimisation
#pragma optimize("", off)
void WorldGenerator::Init()
{
	if (pContinent)
	{
		pContinent->Init();

		Vec2i spawnPoint = pContinent->GetPlayerSpawnPoint();
		SetCenter(spawnPoint, /*bInit =*/true);

		if (pEcs)
		{
			ComponentTransform& transform = pEcs->GetComponent<ComponentTransform>(pEcs->GetPlayerEntityId());
			transform.x = spawnPoint.x;
			transform.y = spawnPoint.y;
		}
	}
	
}

void WorldGenerator::Tick(float deltaTime)
{
	// Update center to generate around where the player moves  
	if (pEcs && pContinent)
	{
		ComponentTransform& transform = pEcs->GetComponent<ComponentTransform>(pEcs->GetPlayerEntityId());
		SetCenter({transform.x, transform.y}, false);
	}
}

void WorldGenerator::SetCenter(Vec2i center, bool bInit)
{
	// Effective coords that we will use as the center coords
	const int effectiveX = center.x;
	const int effectiveY = center.y;

	// Min/max grid positions of the chunks
	const int minChunkX = effectiveX / WorldGenerationChunk::CHUNK_SIZE - NUM_CHUNKS;
	const int maxChunkX = minChunkX + (CHUNK_SQUARE_SIZE - 1);
	const int minChunkY = effectiveY / WorldGenerationChunk::CHUNK_SIZE - NUM_CHUNKS;
	const int maxChunkY = minChunkY + (CHUNK_SQUARE_SIZE - 1);

	// Delete chunks that are now out of range
	bool bDeletedChunks = bInit;
	if (!bInit)
	{
		for (WorldGenerationChunk*& pChunk : pChunks)
		{
			const int chunkX = pChunk->GetChunkX();
			const int chunkY = pChunk->GetChunkY();

			if (chunkX > maxChunkX ||
				chunkX < minChunkX ||
				chunkY > maxChunkY ||
				chunkY < minChunkY)
			{
				pChunk->DeleteChunk(pEcs);
				delete pChunk;
				pChunk = nullptr;
				bDeletedChunks = true;
			}
		}
	}

	if (bDeletedChunks)
	{
		// Create new chunks
		WorldGenerationChunk* pNewChunks[CHUNK_SQUARE_SIZE * CHUNK_SQUARE_SIZE] = {};
		
		// Gets the new index in the pNewChunks for a given X and Y
		auto GetNewIndex = [=](int x, int y)
		{
			return (x - minChunkX) * CHUNK_SQUARE_SIZE + (y - minChunkY);
		};

		// Populate from current chunks
		for (WorldGenerationChunk* pChunk : pChunks)
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
					pNewChunks[newIndex] = new WorldGenerationChunk(pEcs, pContinent, x, y);
				}
			}
		}

		memcpy(pChunks, pNewChunks, sizeof(pChunks));
	}
}

void WorldGenerator::UnInit()
{
	for (WorldGenerationChunk*& pChunk : pChunks)
	{
		if (pChunk)
		{
			pChunk->DeleteChunk(pEcs);
			delete pChunk;
			pChunk = nullptr;
		}
	}
}

#ifdef DOMIMGUI
void WorldGenerator::DrawImGui(float deltaTime)
{
	ImGui::Text("Seed: %llu", pContinent->GetSeed().seed);
	
	ImGui::Text("Click to teleport!");
	if (CameraSystem* pCameraSystem = Game::GetGameInstance().FindGameSystemSlow<CameraSystem>())
	{
		// Click to teleport to help explore world generation quickly
		if (dmwi::isPressed(dmwi::Button::LMB))
		{
			if (ECS* pDebugEcs = Game::GetGameInstance().FindGameSystemSlow<ECS>())
			{
				ComponentTransform& transformComponent = pDebugEcs->GetComponent<ComponentTransform>(pDebugEcs->GetPlayerEntityId());
				Vec2i posUnderMouse = pCameraSystem->GetWorldGridCoordinateUnderMouse();
				transformComponent.x = posUnderMouse.x;
				transformComponent.y = posUnderMouse.y;
			}
		}
	}
}
#endif
#pragma optimize("", on)
