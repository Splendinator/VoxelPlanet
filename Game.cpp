#include "pch.h"

#include "Game.h"

#include "AssetManager/AssetManager.h"
#include "Core/GameInstance.h"
#include "DomWindow/DomWindow.h"
#include <imgui.h>
#include <time.h>

AssetManager assetManager;
GameInstance* pGameInstance = nullptr;

bool bRestartRequested = false;

void Game::Init()
{
	assetManager.Init();

	// Set random seed based off time
	srand((unsigned int)time(NULL));
	
	pGameInstance = assetManager.LoadObjectFromAssetName<GameInstance>("GameInstance");
	if (pGameInstance)
	{
		pGameInstance->InitGameSystems();
	}
}

void Game::UnInit()
{
	if (pGameInstance)
	{
		pGameInstance->UnInitGameSystems();
	}
	
	assetManager.UnInit();
}

void GameplayTick(float deltaTime)
{
	if (pGameInstance)
	{
		pGameInstance->TickGameSystems(deltaTime);

#ifdef DOMIMGUI
		pGameInstance->TickImGui(deltaTime);
#endif
	}
}

#ifdef DOMIMGUI
// #TEMP: This is the FPS counter, will need removing / moving to an EditorWindow.
void TickImGuiFramerateWindow(float deltaTime)
{
	static int numFrames = 0;
	static float totalTime = 0.0f;
	static int fps = 0;

	totalTime += deltaTime;
	++numFrames;

	if (totalTime > 1.0f)
	{
		fps = numFrames;
		numFrames = 0;
		totalTime -= 1.0f;
	}

	ImGui::SetWindowPos({ 0,0 }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowSize({ 200,200 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("FPS");
	ImGui::Text("Frame rate: %d", fps); 
	ImGui::End();
}
#endif //~ #ifdef DOMIMGUI

void Game::Tick(float deltaTime)
{
	SCOPED_PERFORMANCE_MARKER(GameTick);
	
#ifdef DOMIMGUI
	TickImGuiFramerateWindow(deltaTime);
#endif //~ #ifdef DOMIMGUI
	
	GameplayTick(deltaTime);

	if (bRestartRequested)
	{
		UnInit();
		Init();
		bRestartRequested = false;
	}
}

bool Game::CanClose()
{
	return dmwi::isHeld(dmwi::Button::SHIFT) && dmwi::isPressed(dmwi::Button::ESC);
}

void Game::RestartGame()
{
	bRestartRequested = true;
}

const GameInstance& Game::GetGameInstance() { DOMASSERT(pGameInstance) return *pGameInstance; }

AssetManager& Game::GetAssetManager() { return assetManager; };
