#pragma once

class GameInstance;
class AssetManager;

namespace Game
{
	void Init();
	void UnInit();
	void Tick(float deltaTime);
	
	bool CanClose();

	// #TODO: This almost always crashes, good luck. I need to go through all systems and see which ones aren't uninitialising/initialising properly.
	void RestartGame();

	const GameInstance& GetGameInstance();
	AssetManager& GetAssetManager();
}

