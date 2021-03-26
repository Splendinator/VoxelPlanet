#include "pch.h"

#include "Game.h"

#include "DomGraphics.h"

#include "DomLog/DomLog.h"
#include "DomMath/Vec3.h"
#include "DomMath/Constants.h"
#include "DomMath/Constexpr.h"
#include "DomMath/Math.h"
#include "DomUtils/DomUtils.h"
#include "DomWindow/DomWindow.h"
#include "../Graphics/Camera.h"

#include <imgui.h>
#include <time.h>

void Game::Init()
{
	
}

void Game::UnInit()
{

}

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

void GameplayTick(float deltaTime)
{

}

void Game::tick(float deltaTime)
{
	CreateImGuiWindow(deltaTime);
	GameplayTick(deltaTime);
}
