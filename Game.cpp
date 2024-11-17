#include "pch.h"

#include "Game.h"

#include "DomWindow/DomWindow.h"
#include "Core/GameInstance.h"
#include "ImGuiEditor.h"
#include <imgui.h>
#include <time.h>

#ifdef DOMIMGUI
ImGuiEditor imGuiEditor;
#endif //~ DOMIMGUI

GameInstance* pGameInstance = nullptr;

void Game::Init()
{
#ifdef DOMIMGUI
	imGuiEditor.Init();
#endif //~ DOMIMGUI

	// Set random seed based off time
	srand((unsigned int)time(NULL));
	
	pGameInstance = imGuiEditor.FindObjectFromAsset<GameInstance>("GameInstance");
	if (pGameInstance)
	{
		pGameInstance->InitGameSystems();
	}
}
#pragma optimize("", on)

void Game::UnInit()
{
	if (pGameInstance)
	{
		pGameInstance->UnInitGameSystems();
	}

#ifdef DOMIMGUI
	imGuiEditor.Uninit();
#endif //~ DOMIMGUI
}

void GameplayTick(float deltaTime)
{
	if (pGameInstance)
	{
		pGameInstance->TickGameSystems(deltaTime);
	}
}

#ifdef DOMIMGUI
// #TEMP: This is the FPS counter, will need removing / moving to an EditorWindow
void CreateImGuiWindow(float deltaTime)
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
#ifdef DOMIMGUI
	CreateImGuiWindow(deltaTime);
	imGuiEditor.Tick(deltaTime);
#endif //~ #ifdef DOMIMGUI
	GameplayTick(deltaTime);
}

bool Game::CanClose()
{
	return dmwi::isHeld(dmwi::Button::SHIFT) && dmwi::isPressed(dmwi::Button::ESC);
}

#ifdef DOMIMGUI
ImGuiEditor& Game::Editor() { return imGuiEditor; };
#endif
