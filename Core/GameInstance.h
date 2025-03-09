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

	// Find a game system by dynamic casting all game systems.
	// This is a lot slower than using an EDITORPROPERTY() to the singleton system but in cases where
	// setting up the data would be a massive hassle and/or performance isn't a concern this can be used.  
	template<typename TGameSystem>
	TGameSystem* FindGameSystemSlow() const;
	
private:

	// Vector of game systems, the systems will initialise in the order that they are in the array.
	// so if, for example, the world generator needs an ECS in order to spawn entities into the world then the world generator should go after the ECS.
	EDITORPROPERTY()
	std::vector<GameSystem*> pGameSystems;

	bool bShowingDebugImGui = false;
};

template <typename TGameSystem>
TGameSystem* GameInstance::FindGameSystemSlow() const
{
	static_assert(std::is_base_of<GameSystem, TGameSystem>::value, "Must be a game system");
	
	for (GameSystem* pGameSystem : pGameSystems)
	{
		if (TGameSystem* pCastedGameSystem = dynamic_cast<TGameSystem*>(pGameSystem))
		{
			return pCastedGameSystem;
		}
	}

	return nullptr;
}