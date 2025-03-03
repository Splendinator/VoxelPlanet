#pragma once

#include "ECSTypes.h"

#include "Core/GameSystem.h"

class DirectoryData;
class ECS;
class RPGClassData;

EDITORENUM()
enum class ERPGClass
{
	None,
	Warrior,
	Ranger,
	Mage
};

EDITORENUM()
enum class ERPGClassSpecialisation
{
	None, // No specialisation picked yet
};

EDITORSTRUCT()
struct RPGClassEntry
{
	EDITORBODY()

	EDITORPROPERTY()
	ERPGClass rpgClass = ERPGClass::None;

	EDITORPROPERTY()
	const RPGClassData* pClassData;
};

// System managing RPG classes
EDITORCLASS()
class RPGClassSystem : public GameSystem
{
	EDITORBODY()

public:

	// Setup the entities class, with optional bSetMesh (Monsters don't want to inherit the classes mesh, but the player does)
	void SetEntityClass(EntityId entity, ERPGClass rpgClass, bool bSetMesh) const;
	void SetEntitySpecialisation(EntityId entity, ERPGClassSpecialisation rpgSpecialisation) const;

	const RPGClassData* FindClassData(ERPGClass rpgClass) const;
	
protected:

	//~ Begin GameSystem Interface
	void Init() override;
	//~ End GameSystem Interface
	
	
	EDITORPROPERTY()
	ECS* pEcs = nullptr;

	EDITORPROPERTY()
	DirectoryData* pDirectoryData = nullptr;
	
	// Map of class enum IDs to their data
	EDITORPROPERTY()
	std::vector<RPGClassEntry> classes;
	
};
