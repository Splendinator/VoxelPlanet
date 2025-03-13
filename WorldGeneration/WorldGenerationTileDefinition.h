#pragma once

// Contains enough data to generate a single terrain tile
EDITORCLASS(Singleton)
class WorldGenerationTileDefinition
{
	EDITORBODY()
public:
	
	// Name of this file in the DirectorData::worldGenerationTiles folder
	EDITORPROPERTY()
	std::string fileName;

	// Whether this has collision
	EDITORPROPERTY()
	bool bRigidBody = false;
};
