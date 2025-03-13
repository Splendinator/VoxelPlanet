#include "pch.h"

#include "WorldGenerationContinent.h"

#include "DirectoryData.h"
#include "ECS/ECS.h"
#include "ECS/Systems/ECSSystemRender.h"
#include "RenderPriorities.h"
#include "Renderer.h"
#include "RendererObject.h"
#include "WorldGenerationLogic.h"
#include "WorldGenerationTileDefinition.h"
#include "WorldGenerationUtils.h"

// We can't spawn at 0,0 or it bugs out so we just do an extra 100 in each direction
constexpr int CONTINENT_ORIGIN_X = 100;
constexpr int CONTINENT_ORIGIN_Y = 100;

void WorldGenerationContinent::Init()
{
	// Get seed of this continent
	if (bRandomSeed)
	{
		for (int i = 0; i < 8; ++i)
		{
			seed.byte[i] = rand() % 255;
		}
	}
	else
	{
		seed.seed = debugSeed;
	}

	// Populate tile map (to tie enums to their definitions)
	for (WorldGenerationTileEntry& entry : tileData)
	{
		tileMap[(int)entry.type] = entry.pDefinition;
	}
	
	// Reserve space for all background tiles on the island
	pTiles = new EWorldGenerationTile[continentSize * continentSize];

	// Generate shoreline
	const int halfContinentSize = (int)(continentSize * 0.5f);
	const float baseShorelineRadius = halfContinentSize - shorelineParams.baseDistanceFromEdge; 
	for (int x = 0; x < continentSize; ++x)
	{
		for (int y = 0; y < continentSize; ++y)
		{
			const float tileDistanceFromCenter = Vec2i::Distance(Vec2i(x,y), Vec2i(halfContinentSize, halfContinentSize));

			const float atan2X = (float)x - (float)halfContinentSize;
			const float atan2Y = (float)y - (float)halfContinentSize;
			float shorelineAngle = std::atan2(atan2X, atan2Y); // Get atan angle (-PI, +PI] 
			const float adjustedShorelineAngle = ((shorelineAngle + dmma::piFloat) / dmma::twoPiFloat) * shorelineParams.maxShorelineAngle; // Adjust angle to be [0.0f, maxShorelineAngle)

			float finalShorelineRadius = baseShorelineRadius;
			if (shorelineParams.pShorelineDistanceDeltaLogic)
			{
				RandSeed shorelineSeed = seed;
				
				finalShorelineRadius += shorelineParams.pShorelineDistanceDeltaLogic->BaseDoLogic(shorelineSeed, adjustedShorelineAngle, 0.0f);
			}
			 
			if (tileDistanceFromCenter < finalShorelineRadius)
			{
				GetTileRef({x,y}) = EWorldGenerationTile::Grass;
			}
			else
			{
				GetTileRef({x,y}) = EWorldGenerationTile::Water;
			}
		}
	}
}

void WorldGenerationContinent::UnInit()
{
	delete[] pTiles;
}

EntityId WorldGenerationContinent::CreateTileEntity(EWorldGenerationLayer layer, Vec2i position) const
{
	position.x -= CONTINENT_ORIGIN_X;
	position.y -= CONTINENT_ORIGIN_Y;
	
	if (layer == EWorldGenerationLayer::Foreground)
	{
		// #TEMP: Do foreground 
		return INVALID_ENTITY_ID;
	}
	
	if (position.x < 0 || position.x >= continentSize || position.y < 0 || position.y >= continentSize)
	{
		// Out of bounds, spawn water
		return CreateTileEntity(EWorldGenerationTile::Water, layer, position);
	}

	return CreateTileEntity(GetTileRef(position), layer, position);
}

Vec2i WorldGenerationContinent::GetPlayerSpawnPoint() const
{
	// Walk diagonally upwards from the bottom left of the continent until we hit land, then spawn there
	for (int xCoordinate = 0; xCoordinate < continentSize; ++xCoordinate)
	{
		const int yCoordinate = continentSize-xCoordinate-1;
		
		if (GetTileRef({xCoordinate, yCoordinate}) == EWorldGenerationTile::Grass)
		{
			return {CONTINENT_ORIGIN_X + xCoordinate, CONTINENT_ORIGIN_Y + yCoordinate};
		}
	}

	DOMLOG_WARN("Couldn't find suitable spawn for player, falling back to origin")
	return {CONTINENT_ORIGIN_X, CONTINENT_ORIGIN_Y + CONTINENT_ORIGIN_Y - 1};
}

EntityId WorldGenerationContinent::CreateTileEntity(EWorldGenerationTile tile, EWorldGenerationLayer layer, Vec2i position) const
{
	DOMLOG_ERROR_IF(tile >= EWorldGenerationTile::COUNT)
	
	const WorldGenerationTileDefinition* pTileDefinition = tileMap[(int)tile];
	if (pEcs && pDirectoryData && pTileDefinition)
	{
		EntityId newTile = pEcs->GetNextFreeEntity();

		// Transform
		ComponentTransform& transformComponent = pEcs->AddComponent<ComponentTransform>(newTile);
		transformComponent.x = position.x + CONTINENT_ORIGIN_X;
		transformComponent.y = position.y + CONTINENT_ORIGIN_Y;

		// Mesh
		ComponentMesh& meshComponent = pEcs->AddComponent<ComponentMesh>(newTile);
		meshComponent.pRendererObject = dmgf::AddObjectFromSVG(DirectoryData::ConcatenateSVGFilePathChecked(pDirectoryData->worldGenerationTiles, pTileDefinition->fileName).c_str());
		ECSSystemRender::SetupRenderObjectOnGrid(meshComponent.pRendererObject);

		constexpr float renderPriorities[(int)EWorldGenerationLayer::COUNT]
		{
			/*EWorldGenerationLayer::Background*/ RenderPriority::background,
			/*EWorldGenerationLayer::Foreground*/ RenderPriority::foreground,
		};
		meshComponent.pRendererObject->SetRenderPriority(renderPriorities[(int)layer]);

		// Rigid
		if (pTileDefinition->bRigidBody)
		{
			pEcs->AddComponent<ComponentRigid>(newTile);
		}
		
		return newTile;
	}

	return INVALID_ENTITY_ID;
}

EWorldGenerationTile& WorldGenerationContinent::GetTileRef(Vec2i position) const
{
	DOMLOG_ERROR_IF(position.x < 0 || position.x >= continentSize || position.y < 0 || position.y >= continentSize)
	return pTiles[position.x * continentSize + position.y];
}
