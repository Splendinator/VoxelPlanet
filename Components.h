#pragma once

class ActionDeciderBase;
class RendererObject;

// Enum entries in here must match the name of the struct
// When adding a new entry here you must also add it to the ECS
enum class EComponents
{
	ComponentMesh,
	ComponentTransform,
	ComponentAction,
};

// Component to render something
struct ComponentMesh
{
	RendererObject* pRendererObject = nullptr;
};

// Component to give something a location
struct ComponentTransform
{
	int x;
	int y;
};

// Component to take an action based off energy
struct ComponentAction
{
	short maxEnergy = 0; // How often to take a turn, something with 50 max energy will take an action twice as often as something with 100 energy
	short energy = 0;
	ActionDeciderBase* pActionDecider = nullptr;
};