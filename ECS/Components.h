#pragma once

#include "DomMath/Types.h"
#include "DomUtils/Pointers.h"

class ActionDeciderBase;
class RPGClassData;
class RPGClassSpecialisationData;
class RPGRaceData;
class RendererObject;

// Enum entries in here must match the name of the struct
// When adding a new entry here you must also add it to the ECS
enum class EComponents
{
	ComponentMesh,
	ComponentTransform,
	ComponentAction,
	ComponentRigid,
	ComponentHealth,
	ComponentFaction,
	ComponentClass,
	ComponentProgression,
	ComponentRace,
	ComponentProjectile,
};

// Component to render something
struct ComponentMesh
{
	TransientPtr<RendererObject> pRendererObject = nullptr;
};

// Component to give something a location
struct ComponentTransform
{
	int x;
	int y;
	int lastX = -1;
	int lastY = -1;
};

// Component to take an action based off energy
struct ComponentAction
{
	u16 maxEnergy = 0; // How often to take a turn, something with 50 max energy will take an action twice as often as something with 100 max energy
	u16 energy = 0;
	TransientPtr<ActionDeciderBase> pActionDecider = nullptr;
};

// Component to block movement onto its tile, just used for the bitflag
struct ComponentRigid {};

struct ComponentHealth
{
	int health = 0;
	int maxHealth = 0;
};

struct ComponentFaction
{
	// #TODO: More factions
	enum class EFactionFlags : u8
	{
		None = 0,
		Player = 1 << 0,
		Enemy = 1 << 1,
	};
	
	EFactionFlags factionFlags = EFactionFlags::None;
};

// Allows RPG characters to have a class
struct ComponentClass
{
	const RPGClassData* pClassData = nullptr;
	const RPGClassSpecialisationData* pSpecialisationData = nullptr;
};

// Allows RPG characters to have a level
struct ComponentProgression
{
	u32 level = 0; // This is an index and starts at 0.
	u32 currentXp = 0;
};

// RPG Race
struct ComponentRace
{
	const RPGRaceData* pRaceData = nullptr;
};

struct ComponentProjectile
{
	int startX = 0;
	int startY = 0;
	int targetX = 0;
	int targetY = 0;
};