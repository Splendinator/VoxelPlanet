#pragma once

class AssetManager;

namespace Game
{
	void Init();
	void UnInit();
	void Tick(float deltaTime);
	
	bool CanClose();
	
	AssetManager& GetAssetManager();
}

