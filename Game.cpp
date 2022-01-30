#include "pch.h"

#include "Game.h"

#include "../Graphics/Camera.h"
#include "DomLog/DomLog.h"
#include "DomMath/Constants.h"
#include "DomMath/Constexpr.h"
#include "DomMath/Math.h"
#include "DomMath/Vec3.h"
#include "DomUtils/DomUtils.h"
#include "DomWindow/DomWindow.h"
#include "ResourceLoader.h"

#include <imgui.h>
#include <time.h>

/// #TEMP: Temp for testing ECS
#include "ECS.h"
#include "Renderer.h"
ECS ecs;

void Game::Init()
{
	ecs.RegisterSystem(std::make_unique<SystemRender>());

	{
		Entity& e = ecs.GetEntity(0);
		e.components.AddComponent(EComponents::Mesh);
		e.components.AddComponent(EComponents::Transform);
		ecs.GetComponent<Mesh>(0).pRendererObject = dmgf::AddObjectFromSVG("C:/Users/Dominic/Desktop/Grass.svg");
		ecs.GetComponent<Transform>(0).x = 2;
		ecs.GetComponent<Transform>(0).y = 2;
	}
	{
		Entity& e = ecs.GetEntity(1);
		e.components.AddComponent(EComponents::Mesh);
		///e.components.AddComponent(EComponents::Transform);
	}
	{
		Entity& e = ecs.GetEntity(2);
		///e.components.AddComponent(EComponents::Mesh);
		e.components.AddComponent(EComponents::Transform);
	}
	{
		Entity& e = ecs.GetEntity(3);
		e.components.AddComponent(EComponents::Mesh);
		e.components.AddComponent(EComponents::Transform);
		ecs.GetComponent<Mesh>(3).pRendererObject = dmgf::AddObjectFromSVG("C:/Users/Dominic/Desktop/Grass.svg");
		ecs.GetComponent<Transform>(3).x = 5;
		ecs.GetComponent<Transform>(3).y = 5;
	}
}

void Game::UnInit()
{

}

#ifdef DOMIMGUI
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

void GameplayTick(float deltaTime)
{
	ecs.Tick(deltaTime);
}

void Game::tick(float deltaTime)
{
#ifdef DOMIMGUI
	CreateImGuiWindow(deltaTime);
#endif //~ #ifdef DOMIMGUI
	GameplayTick(deltaTime);
}
