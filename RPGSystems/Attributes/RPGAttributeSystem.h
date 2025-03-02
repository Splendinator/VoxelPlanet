#pragma once

#include "RPGAttributesShared.h"

#include "Core/GameSystem.h"

class ECS;
class RPGAttributeBase;

// System responsible for RPG Attributes. In this game RPG attributes are any deteministic numbers that are modified by things like gear, level, class, other attributes, etc.
// e.g. MaxHealth, Strength, ManaRegen, etc.
EDITORCLASS()
class RPGAttributeSystem : public GameSystem
{
	EDITORBODY()

protected:

	//~ Begin GameSystem Interface
	void Init() override;
#ifdef DOMIMGUI
	void DrawImGui(float deltaTime) override;
#endif
	//~ End GameSystem Interface
	
	EDITORPROPERTY()
	ECS* pEcs = nullptr;
	
	// List of all attributes in the game
	EDITORPROPERTY()
	std::vector<const RPGAttributeBase*> attributes;

	RPGAttributeCalculationSharedData sharedData;
};
