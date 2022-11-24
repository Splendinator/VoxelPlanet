#pragma once

#include "DomMath/Types.h"
#include "DomUtils/Pointers.h"

class ActionDeciderBase;
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
	short maxEnergy = 0; // How often to take a turn, something with 50 max energy will take an action twice as often as something with 100 energy
	short energy = 0;
	ActionDeciderBase* pActionDecider = nullptr; // These are deleted by the SystemAction when the entity is deleted
};

// Component to block movement onto its tile
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