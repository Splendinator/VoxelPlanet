#pragma once

// #TEMP: These need to be replaced with RPGLevelScalingNumbers
EDITORSTRUCT()
struct RPGRaceInitialAttributeValues
{
	EDITORBODY()

	EDITORPROPERTY()
	int maxHealth = 100;

	EDITORPROPERTY()
	int spellDamage = 10;
};

EDITORSTRUCT()
struct RPGRacePerLevelAttributeValues
{
	EDITORBODY()

	EDITORPROPERTY()
	float maxHealthMultiplierPerLevel = 1.1f;

	EDITORPROPERTY()
	float spellDamageMultiplierPerLevel = 1.06f;
};

EDITORCLASS(Singleton)
class RPGRaceData
{
	EDITORBODY()
public:

	// Player facing display name
	EDITORPROPERTY()
	std::string displayName;
	
	// Initial attributes of this race (at level 1)
	EDITORPROPERTY()
	RPGRaceInitialAttributeValues initialAttributes;

	// Attributes to add/multiply to the base per level of the creature with this race
	EDITORPROPERTY()
	RPGRacePerLevelAttributeValues perLevelAttributes;
	
	// Vector art file name used if a creature of this race is spawned
	EDITORPROPERTY()
	std::string vectorArtFileName;
};
