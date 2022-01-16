#pragma once

#pragma once

// This class is used to load resources into the game, class so that it can have a virtual LoadResources() function.
class ResourceLoader
{
public:
	// Loads resources into the game. Override this to load a different game
	virtual void LoadResources();

};

