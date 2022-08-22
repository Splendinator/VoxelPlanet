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

/// Temp headers, this stuff will be moved to the world gen system at some point I guess
#include "ActionDeciderPlayer.h"
#include "ECS.h"
#include "Renderer.h"
#include "RendererObject.h"
#include "SystemAction.h"
#include "SystemRender.h"
#include "WorldGenerator.h"

ECS ecs;
WorldGenerator worldGenerator(ecs);

EntityId playerEntity = 0;

void Game::Init()
{
	// Register systems to ECS (order matters)
	ecs.RegisterSystem(std::make_unique<SystemAction>()); // Control before render or player is 1 square behind for a frame
	ecs.RegisterSystem(std::make_unique<SystemRender>());

	// Create player entity
	{
		Entity& e = ecs.GetEntity(playerEntity);
		e.components.AddComponent(EComponents::ComponentMesh);
		e.components.AddComponent(EComponents::ComponentTransform);
		e.components.AddComponent(EComponents::ComponentAction);
		ecs.GetComponent<ComponentMesh>(playerEntity).pRendererObject = dmgf::AddObjectFromSVG("C:/Users/Dominic/Desktop/Player.svg");
		ecs.GetComponent<ComponentMesh>(playerEntity).pRendererObject->SetRenderPriority(0.0f);
		ecs.GetComponent<ComponentTransform>(playerEntity).x = 10000;
		ecs.GetComponent<ComponentTransform>(playerEntity).y = 10000;
		ecs.GetComponent<ComponentAction>(playerEntity).maxEnergy = 100;
		ecs.GetComponent<ComponentAction>(playerEntity).energy = 100;
		ecs.GetComponent<ComponentAction>(playerEntity).pActionDecider = new ActionDeciderPlayer;
	}

	worldGenerator.SetCenter(0, 0, /*bInit =*/true);
}

void Game::UnInit()
{
	worldGenerator.Uninitialise();
	ecs.Uninitialise();
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
	
	ComponentTransform& transform = ecs.GetComponent<ComponentTransform>(playerEntity);
	worldGenerator.SetCenter(transform.x, transform.y, false);
	dmgf::SetCameraCenter((transform.x - 8) * SystemRender::GRID_SIZE, (transform.y - 4) * SystemRender::GRID_SIZE); // #TODO: Need a way to center camera based on screen size + zoom level
}

void Game::tick(float deltaTime)
{
#ifdef DOMIMGUI
	CreateImGuiWindow(deltaTime);
#endif //~ #ifdef DOMIMGUI
	GameplayTick(deltaTime);
}
