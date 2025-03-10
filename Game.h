#pragma once

class GameInstance;
class AssetManager;

namespace Game
{
	void Init();
	void UnInit();
	void Tick(float deltaTime);
	
	bool CanClose();
	
	void RestartGame();

	const GameInstance& GetGameInstance();
	AssetManager& GetAssetManager();
}

