#pragma once

class GameSystem;

// The data driven instance of the game.
// A single instance of this is loaded when the game boots up and will be responsible for bootloading everything else in the game
EDITORCLASS()
class GameInstance
{
	EDITORBODY()

public:

	void InitGameSystems();
	void TickGameSystems(float deltaTime);
	void UnInitGameSystems();

#ifdef DOMIMGUI
	void TickImGui(float deltaTime);
#endif

private:

	// Vector of game systems, the systems will initialise in the order that they are in the array.
	// so if, for example, the world generator needs an ECS in order to spawn entities into the world then the world generator should go after the ECS.
	EDITORPROPERTY()
	std::vector<GameSystem*> pGameSystems;

	bool bShowingDebugImGui = false;
};