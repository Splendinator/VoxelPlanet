#include "pch.h"

#include "WorldGenerationContinent.h"

#include "AI/Actions/ActionDeciders/ActionDeciderAI.h"
#include "Core/DirectoryData.h"
#include "DomUtils/HeapAlloc.h"
#include "ECS/ECS.h"
#include "ECS/Systems/ECSSystemRender.h"
#include "RPGSystems/RPGSystem.h"
#include "Graphics/RenderPriorities.h"
#include "Graphics/Renderer.h"
#include "Graphics/RendererObject.h"
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

	const int numTilesToReserve = continentSize * continentSize * (int)EWorldGenerationLayer::COUNT;
	// Reserve space for all background tiles on the island
	pTiles.Alloc(numTilesToReserve);
	for (EWorldGenerationTile& tile : pTiles)
	{
		tile = EWorldGenerationTile::None;
	}

	const int halfContinentSize = (int)(continentSize * 0.5f);

	// 1. Generate shoreline.
	// This algorithm works by generating a circle around the center point, then the radius of that circle changes as you go around based off 1D noise.
	// We also calculate the "distance from ocean" and store it in tileDistanceFromOcean while we're here for use in later stages.
	HeapAlloc<float> tileDistanceFromOcean;
	tileDistanceFromOcean.Alloc((size_t)(continentSize * continentSize));
	memset(tileDistanceFromOcean.RawPtr(), -1, sizeof(float) * continentSize * continentSize);
	{
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
					GetBackgroundTileRef({x,y}) = EWorldGenerationTile::Grass;

					// Calculate distance from shore, adding noise 
					float distanceFromShore = finalShorelineRadius - tileDistanceFromCenter;
					if (sandParams.pSandDistanceFromShoreDeltaLogic)
					{
						RandSeed sandSeed = seed;
						WorldGenerationUtils::MutateSeed(sandSeed);
						distanceFromShore += sandParams.pSandDistanceFromShoreDeltaLogic->BaseDoLogic(sandSeed, adjustedShorelineAngle, 0.0f);
					}
					tileDistanceFromOcean[GetBackgroundTileIndex({x,y})] = std::max(1.0f, distanceFromShore);
				}
				else
				{
					GetBackgroundTileRef({x,y}) = EWorldGenerationTile::Water;
					tileDistanceFromOcean[GetBackgroundTileIndex({x,y})] = 0.0f;
				}
			}
		}
	}
	
	// 2. Generate sand around edge and trees in-land 
	{
		RandSeed treeSeed = seed;
		for (int x = 0; x < continentSize; ++x)
		{
			for (int y = 0; y < continentSize; ++y)
			{
				// Sand
				if (tileDistanceFromOcean[GetBackgroundTileIndex({x,y})] > 0.0f && tileDistanceFromOcean[GetBackgroundTileIndex({x,y})] <= sandParams.baseDistanceFromOcean)
				{
					GetBackgroundTileRef({x,y}) = EWorldGenerationTile::Sand;
				}

				// Trees
				if (tileDistanceFromOcean[GetBackgroundTileIndex({x,y})] > treeParams.distanceFromOceanTreesBegin)
				{
					float treeAlpha = (tileDistanceFromOcean[GetBackgroundTileIndex({x,y})] - treeParams.distanceFromOceanTreesBegin) / (treeParams.distanceFromOceanTreesEnd - treeParams.distanceFromOceanTreesBegin);
					treeAlpha = std::min(1.0f, treeAlpha);

					const float treePercentage = treeAlpha * (treeParams.treePercentageAtMaximumDistance - treeParams.treePercentageAtMinimumDistance) + treeParams.treePercentageAtMinimumDistance;

					WorldGenerationUtils::MutateSeed(treeSeed);
					if (WorldGenerationUtils::RandFloat(treeSeed) <= treePercentage)
					{
						GetForegroundTileRef({x,y}) = EWorldGenerationTile::Tree;
					}
				}
			}
		}
	}

	// 3. Generate enemies
	{
		pEnemySpawnData.Alloc((size_t)(continentSize * continentSize));
		memset((void*)pEnemySpawnData.RawPtr(), 0, pEnemySpawnData.GetSize() * sizeof(pEnemySpawnData[0]));
		
		Vec2i playerSpawnPoint = GetPlayerSpawnPoint();
		playerSpawnPoint -= {CONTINENT_ORIGIN_X, CONTINENT_ORIGIN_Y}; // Player spawn point in "continent space"
		
		RandSeed enemySeed = seed;
		for (int x = 0; x < continentSize; ++x)
		{
			for (int y = 0; y < continentSize; ++y)
			{
				if (GetBackgroundTileRef({x,y}) == EWorldGenerationTile::Water || GetForegroundTileRef({x,y}) == EWorldGenerationTile::Tree)
				{
					// #TODO: This should be figured out dynamically with WorldGenerationTileData::bRigidBody
					// Can't spawn on unwalkable terrain
					continue;
				}

				const float distanceFromSpawn = Vec2i::Distance(playerSpawnPoint, Vec2i(x,y));

				if (distanceFromSpawn < enemyParams.minDistanceFromPlayerSpawn)
				{
					// Too close to player spawn point
					 continue;
				}

				const float distanceFromOcean = tileDistanceFromOcean[GetBackgroundTileIndex({x,y})];

				WorldGenerationUtils::MutateSeed(enemySeed);
				if (WorldGenerationUtils::RandFloat(enemySeed) <= enemyParams.enemySpawnChanceAlpha)
				{
					EnemySpawnData& enemyData = GetEnemyDataRef({x,y});
					enemyData.pRaceData = (distanceFromOcean < enemyParams.crabSpawnDistance) ? enemyParams.pCrabRaceData : enemyParams.pBanditRaceData;
					enemyData.level = (int)(std::min(1.0f,(distanceFromSpawn / enemyParams.maxLevelDistanceFromSpawn)) * (float)enemyParams.maxLevel);
				}
			}
		}
	}
}

void WorldGenerationContinent::UnInit()
{
	delete sandParams.pSandDistanceFromShoreDeltaLogic;
	delete shorelineParams.pShorelineDistanceDeltaLogic;
}

EntityId WorldGenerationContinent::CreateTileEntity(EWorldGenerationLayer layer, Vec2i position) const
{
	position.x -= CONTINENT_ORIGIN_X;
	position.y -= CONTINENT_ORIGIN_Y;
	
	if (position.x < 0 || position.x >= continentSize || position.y < 0 || position.y >= continentSize)
	{
		// Out of bounds, spawn water
		return layer == EWorldGenerationLayer::Background ? CreateTileEntityInternal(EWorldGenerationTile::Water, layer, position) : INVALID_ENTITY_ID;
	}

	// Spawn enemies in foreground
	// #TODO: Hacky as fuck, needs a real enemy spawn manager system
	if (layer == EWorldGenerationLayer::Foreground)
	{
		EnemySpawnData& enemySpawnData = GetEnemyDataRef(position);
		if (enemySpawnData.pRaceData)
		{
			CreateEnemyEntityInternal(enemySpawnData, position);
			return INVALID_ENTITY_ID; // Don't pass a entity id of the enemy, as we don't want to chunk to own it.
		}
	}
	
	return CreateTileEntityInternal(GetTileRef(position, layer), layer, position);
}

Vec2i WorldGenerationContinent::GetPlayerSpawnPoint() const
{
	// Walk diagonally upwards from the bottom left of the continent until we hit land, then spawn there
	for (int xCoordinate = 0; xCoordinate < continentSize; ++xCoordinate)
	{
		const int yCoordinate = continentSize-xCoordinate-1;
		
		if (GetTileRef({xCoordinate, yCoordinate}, EWorldGenerationLayer::Background) == EWorldGenerationTile::Grass)
		{
			return {CONTINENT_ORIGIN_X + xCoordinate, CONTINENT_ORIGIN_Y + yCoordinate};
		}
	}

	DOMLOG_WARN("Couldn't find suitable spawn for player, falling back to origin")
	return {CONTINENT_ORIGIN_X, CONTINENT_ORIGIN_Y + continentSize - 1};
}

EntityId WorldGenerationContinent::CreateTileEntityInternal(EWorldGenerationTile tile, EWorldGenerationLayer layer, Vec2i position) const
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

void WorldGenerationContinent::CreateEnemyEntityInternal(EnemySpawnData& spawnData, Vec2i position) const
{
	if (pEcs && pDirectoryData && pRPGSystem)
	{
		EntityId enemyEntity = pEcs->GetNextFreeEntity();

		// Transform
		ComponentTransform& transform = pEcs->AddComponent<ComponentTransform>(enemyEntity);
		transform.x = position.x + CONTINENT_ORIGIN_X;
		transform.y = position.y + CONTINENT_ORIGIN_Y;

		// Action
		ComponentAction& action = pEcs->AddComponent<ComponentAction>(enemyEntity);
		action.pActionDecider = enemyParams.pEnemyActionDecider;

		// Faction
		ComponentFaction& faction = pEcs->AddComponent<ComponentFaction>(enemyEntity);
		faction.factionFlags = ComponentFaction::EFactionFlags::Enemy;

		RPGEntitySetupParams params = {};
		params.pRaceData = spawnData.pRaceData;
		params.startLevel = spawnData.level;
		pRPGSystem->SetupRPGEntity(enemyEntity, params);
	}
}

EWorldGenerationTile& WorldGenerationContinent::GetTileRef(Vec2i position, EWorldGenerationLayer layer) const
{
	DOMLOG_ERROR_IF(position.x < 0 || position.x >= continentSize || position.y < 0 || position.y >= continentSize)
	return pTiles[GetTileIndex(position, layer)];
}
