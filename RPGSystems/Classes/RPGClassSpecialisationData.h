#pragma once

// See RPGClassData
// A specialisation is a something you pick after you've leveled up for a while
EDITORCLASS(Singleton)
class RPGClassSpecialisationData
{
	EDITORBODY()
public:

	// Display name of this class
	EDITORPROPERTY()
	std::string displayName;
};