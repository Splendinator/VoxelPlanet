#pragma once

#include "WorldGenerationTypes.h"
#include "WorldGenerationUtils.h"

#include "ECS/ECSTypes.h"

class WorldGenerationLogicBase;
class DirectoryData;
class ECS;
class WorldGenerationTileDefinition;

EDITORENUM()
enum class EWorldGenerationTile : u8
{
	Water,
	Grass,
	Dirt,
	Sand,
	Snow,
	COUNT
};

EDITORENUM()
enum class EWorldGenerationForegroundTile : u8
{
	None,
	Tree
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

	EDITORPROPERTY()
	WorldGenerationTileDefinition* pTreeTileDefinition;
};

// See WorldGenerator
EDITORCLASS(Singleton)
class WorldGenerationContinent
{
	EDITORBODY()
public:

	void Init();
	void UnInit(); 

	EntityId CreateTileEntity(EWorldGenerationLayer layer, Vec2i position) const;

	Vec2i GetPlayerSpawnPoint() const;

	RandSeed GetSeed() const { return seed; }

protected:

	EntityId CreateTileEntityInternal(EWorldGenerationTile tile, EWorldGenerationLayer layer, Vec2i position) const;

	EWorldGenerationTile& GetTileRef(Vec2i position) const;
	int GetTileIndex(Vec2i position) const { return (position.x * continentSize) + position.y; }
	
	EDITORPROPERTY()
	ECS* pEcs = nullptr;

	EDITORPROPERTY()
	DirectoryData* pDirectoryData = nullptr;

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

	// Enum hash map of tile enums to their definitions
	const WorldGenerationTileDefinition* tileMap[(int)EWorldGenerationTile::COUNT];
	
	// 2d array of all tiles [continentWidth, continentHeight]
	EWorldGenerationTile* pTiles;

	// #JANK: I can't be fucked to data drive trees, it's 11pm right now.
	EWorldGenerationForegroundTile* pForegroundTiles;
	
	RandSeed seed = {};

	
};
