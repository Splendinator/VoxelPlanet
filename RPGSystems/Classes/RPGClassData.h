#pragma once

class RPGClassSpecialisationData;

// In this game a class is a basic class you have for the first 5 levels or so, it determines certain things like your starting resource (mana, stamina, etc.)
// and the first few skills you get. The philosophy is that a lot of specialisations will want to share basic utility spells (e.g. most melee characters will want a charge attack to gap close)
// so we have the basic utility spells in the class, then the specialisation you pick can build upon that foundation.
EDITORCLASS(Singleton)
class RPGClassData
{
	EDITORBODY()
public:

	// Display name of this class
	EDITORPROPERTY()
	std::string displayName;

	// Name of the vector art file of this class (no .svg needed)
	EDITORPROPERTY()
	std::string vectorArtFileName;
	
	// All possible specialisations for this class
	EDITORPROPERTY()
	std::vector<const RPGClassSpecialisationData*> specialisations;
};