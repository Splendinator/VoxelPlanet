#pragma once

#include "WorldGenerationTypes.h"
#include "WorldGenerationUtils.h"

#include "DomUtils/HeapAlloc.h"

#include "ECS/ECSTypes.h"

class ActionDeciderAI;
class RPGSystem;
class DirectoryData;
class ECS;
class RPGRaceData;
class WorldGenerationLogicBase;
class WorldGenerationTileDefinition;

// #TODO: We need to have a serious think about enemies and what system should handle spawning them / despawning them / remembering how much HP they had, etc.
// #TODO: Maybe certain things like enemies should be generated at runtime instead the first time you spawn a chunk so we don't have to lagspike for ages generating every single tile of the continent.
// #TODO: We should try and make the world generation process more modular instead of the monolithic Init() function that we have now.

EDITORENUM()
enum class EWorldGenerationTile : u8
{
	None,
	Water,
	Grass,
	Dirt,
	Sand,
	Snow,
	Tree,
	COUNT
};

EDITORSTRUCT()
struct WorldGenerationTileEntry
{
	EDITORBODY()
	
	EDITORPROPERTY()
	EWorldGenerationTile type = EWorldGenerationTile::Grass;

	EDITORPROPERTY()
	WorldGenerationTileDefinition* pDefinition;
};

EDITORSTRUCT()
struct WorldGenerationShorelineParams
{
	EDITORBODY()

	// The first step of generating a shoreline is to generate a circle X distance from touching the edge of the bounding box of the continent. 
	EDITORPROPERTY()
	float baseDistanceFromEdge = 0.0f;

	// see pShorelineDistance comment
	EDITORPROPERTY()
	float maxShorelineAngle = 1.f;
	
	// The second step of generating a shoreline is to run the atan2() of the angle of the tile through this logic to decide how far the shoreline extends in that direction.
	// Basically, the X value passed in will be a number between 0.0f and maxShorelineAngle depending on how far around the island we are, 0.0f is at the bottom most point
	EDITORPROPERTY()
	WorldGenerationLogicBase* pShorelineDistanceDeltaLogic = nullptr;
};

EDITORSTRUCT()
struct WorldGenerationSandParams
{
	EDITORBODY()

	// The base number of tiles away from ocean before we generate sand  
	EDITORPROPERTY()
	float baseDistanceFromOcean = 0.0f;

	EDITORPROPERTY()
	WorldGenerationLogicBase* pSandDistanceFromShoreDeltaLogic = nullptr;
};

EDITORSTRUCT()
struct WorldGenerationTreeParams
{
	EDITORBODY()
	
	EDITORPROPERTY()
	float distanceFromOceanTreesBegin;

	EDITORPROPERTY()
	float distanceFromOceanTreesEnd;

	EDITORPROPERTY()
	float treePercentageAtMinimumDistance;

	EDITORPROPERTY()
	float treePercentageAtMaximumDistance;
};

EDITORSTRUCT()
struct WorldGenerationEnemyParams
{
	EDITORBODY()

	// Minimum distance from player spawn before enemies can spawn
	EDITORPROPERTY()
	float minDistanceFromPlayerSpawnSq;

	// [0.0, 1.0]
	EDITORPROPERTY()
	float enemySpawnChanceAlpha;
	
	EDITORPROPERTY()
	RPGRaceData* pBanditRaceData = nullptr;

	EDITORPROPERTY()
	ActionDeciderAI* pEnemyActionDecider = nullptr;
};

// See WorldGenerator
EDITORCLASS(Singleton)
class WorldGenerationContinent
{
	struct EnemySpawnData
	{
		RPGRaceData* pRaceData = nullptr;
		int level = 0;
	};
	
	EDITORBODY()
public:

	void Init();
	void UnInit(); 

	EntityId CreateTileEntity(EWorldGenerationLayer layer, Vec2i position) const;

	Vec2i GetPlayerSpawnPoint() const;

	RandSeed GetSeed() const { return seed; }

protected:

	EntityId CreateTileEntityInternal(EWorldGenerationTile tile, EWorldGenerationLayer layer, Vec2i position) const;
	void CreateEnemyEntityInternal(EnemySpawnData& spawnData, Vec2i position) const;

	EWorldGenerationTile& GetBackgroundTileRef(Vec2i position) const { return GetTileRef(position, EWorldGenerationLayer::Background); }
	EWorldGenerationTile& GetForegroundTileRef(Vec2i position) const { return GetTileRef(position, EWorldGenerationLayer::Foreground); }
	EWorldGenerationTile& GetTileRef(Vec2i position, EWorldGenerationLayer layer) const;
	EnemySpawnData& GetEnemyDataRef(Vec2i position) const { return pEnemySpawnData[(position.x * continentSize) + position.y]; }
	
	int GetBackgroundTileIndex(Vec2i position) const { return GetTileIndex(position, EWorldGenerationLayer::Background); }
	int GetForegroundTileIndex(Vec2i position) const { return GetTileIndex(position, EWorldGenerationLayer::Foreground); }
	int GetTileIndex(Vec2i position, EWorldGenerationLayer layer) const { return (position.x * continentSize) + position.y + (int)layer * continentSize * continentSize; }
	
	EDITORPROPERTY()
	ECS* pEcs = nullptr;

	EDITORPROPERTY()
	DirectoryData* pDirectoryData = nullptr;

	EDITORPROPERTY()
	RPGSystem* pRPGSystem = nullptr;

	// Size of the whole map continent in tiles.
	EDITORPROPERTY()
	int continentSize = 0;

	// Whether to use a random seed, if this is false use DebugSeed
	EDITORPROPERTY()
	bool bRandomSeed = true;
	EDITORPROPERTY()
	u64 debugSeed = 0;
	
	EDITORPROPERTY()
	std::vector<WorldGenerationTileEntry> tileData;

	// Used to generate the shoreline
	EDITORPROPERTY()
	WorldGenerationShorelineParams shorelineParams;

	// Used to generate sand around shoreline
	EDITORPROPERTY()
	WorldGenerationSandParams sandParams;

	// Used to generate trees inland
	EDITORPROPERTY()
	WorldGenerationTreeParams treeParams;

	// Used to generate enemies
	EDITORPROPERTY()
	WorldGenerationEnemyParams enemyParams;

	// Enum hash map of tile enums to their definitions
	const WorldGenerationTileDefinition* tileMap[(int)EWorldGenerationTile::COUNT] = {};
	
	// 2d array of all tiles [continentWidth, continentHeight]
	HeapAllocSize<EWorldGenerationTile> pTiles;
	
	// 2d array of all enemies to spawn
	HeapAllocSize<EnemySpawnData> pEnemySpawnData;
	
	RandSeed seed = {};

	
};
