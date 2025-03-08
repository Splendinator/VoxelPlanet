#pragma once

#include "Core/GameSystem.h"

#include "Attributes/RPGAttributesShared.h"
#include "ECS/ECSTypes.h"

#include "Races/RPGRaceData.h"

class DirectoryData;
class ECS;
class RPGAttributeBase;
class RPGClassData;
class RPGClassSpecialisationData;

EDITORSTRUCT()
struct RPGLevelProgressionData
{
	EDITORBODY()

	// XP to the next level
	EDITORPROPERTY()
	u32 requiredXp = 0;
};

struct RPGEntitySetupParams
{
	// Class Setup
	const RPGClassData* pClassData = nullptr;
	const RPGClassSpecialisationData* pSpecialisationData = nullptr;
	bool bUseClassMeshOverRaceMesh = false; // Whether to set the mesh to that of the class. If this is false it will default to the race's mesh. (We might end up combining race+class meshes later) 

	// Race Setup
	const RPGRaceData* pRaceData = nullptr;

	// Level Setup 
	int startLevel = 0; // Starts at 0, not 1.
};

struct RPGDamageParams
{
	EntityId attackerEntity;
	EntityId targetEntity;

	u32 damage;
};

// RPG system. Responsible for anything RPG, (levels, attributes, classes, races, skills, etc.).
// This class is mostly just a house for all the data assets, the bulk of the RPG logic will be in the ECS as it happens to entities 
EDITORCLASS()
class RPGSystem : public GameSystem
{
	EDITORBODY()
	
public:
	//~ Begin GameSystem Interface
	void Init() override;
	void RecalculateAttributesForEntity(EntityId entity);
#ifdef DOMIMGUI
	void DrawImGui(float deltaTime) override;
#endif
	//~ End GameSystem Interface
	
	void SetupRPGEntity(EntityId entity, RPGEntitySetupParams params);

	void DealDamage(RPGDamageParams params);
	
	// Returns nullptr if none can be found (usually means max level has been hit)
	const RPGLevelProgressionData* GetLevelProgressionDataForLevel(u32 level) const;

protected:

	EDITORPROPERTY()
	ECS* pEcs = nullptr;

	EDITORPROPERTY()
	const DirectoryData* pDirectoryData = nullptr;

	// All attributes in the game
	EDITORPROPERTY()
	std::vector<const RPGAttributeBase*> attributes;

	// Data on progression from one level to the next. The entry in the array is the level so there should be one entry per player level. (i.e if there's 50 levels you'd want 50 here)
	// It is theoretically possible for units to be outside of this range, but you can't level up with XP past the number of levels in this array and they'll stop giving skill points etc. past the max
	EDITORPROPERTY()
	std::vector<RPGLevelProgressionData> levelProgressionData;

	RPGAttributeCalculationSharedData attributeSharedData = {};
};
