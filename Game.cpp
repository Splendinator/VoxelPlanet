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
#include "HUD.h"
#include "ResourceLoader.h"
#include "WorldGenerator.h"

#include <imgui.h>
#include <time.h>

/// Temp headers, this stuff will be moved to the world gen system at some point I guess
#include "ActionDeciderPlayer.h"
#include "Components.h"
#include "ECS.h"
#include "ImGuiEditor.h"
#include "FilePaths.h"
#include "RenderPriorities.h"
#include "Renderer.h"
#include "RendererObject.h"
#include "SystemAction.h"
#include "SystemCleanUp.h"
#include "SystemDamage.h"
#include "SystemEntityMap.h"
#include "SystemNameslate.h"
#include "SystemPhysics.h"
#include "SystemRender.h"
#include "VectorArt.h"
#include "VectorPrimitiveRectangle.h"
#include "ActionDeciderAI.h"

#ifdef DOMIMGUI
ImGuiEditor imGuiEditor;
#endif //~ DOMIMGUI

ECS ecs;
WorldGenerator worldGenerator(ecs);
HUD hud(ecs);


EntityId playerEntity = 0;

constexpr int WORLD_START_X = 10000;
constexpr int WORLD_START_Y = 10000;

void Game::Init()
{
#ifdef DOMIMGUI
	imGuiEditor.Init();
#endif //~ DOMIMGUI
	

	// Register systems to ECS (order matters)
	ecs.RegisterSystem(std::make_unique<SystemAction>());
	ecs.RegisterSystem(std::make_unique<SystemEntityMap>());
	ecs.RegisterSystem(std::make_unique<SystemPhysics>());
	ecs.RegisterSystem(std::make_unique<SystemDamage>());
	ecs.RegisterSystem(std::make_unique<SystemNameslate>());
	ecs.RegisterSystem(std::make_unique<SystemRender>());
	ecs.RegisterSystem(std::make_unique<SystemCleanUp>());

	// Create player entity
	{
		Entity& e = ecs.GetEntity(playerEntity);
		e.components.AddComponent(EComponents::ComponentMesh);
		e.components.AddComponent(EComponents::ComponentTransform);
		e.components.AddComponent(EComponents::ComponentAction);
		e.components.AddComponent(EComponents::ComponentHealth);
		e.components.AddComponent(EComponents::ComponentFaction);
		e.components.AddComponent(EComponents::ComponentRigid);
		ecs.GetComponent<ComponentMesh>(playerEntity).pRendererObject = dmgf::AddObjectFromSVG(FilePath::VectorArt::player);
		ecs.GetComponent<ComponentMesh>(playerEntity).pRendererObject->SetRenderPriority(RenderPriority::unit);
		ecs.GetComponent<ComponentTransform>(playerEntity).x = WORLD_START_X;
		ecs.GetComponent<ComponentTransform>(playerEntity).y = WORLD_START_Y;
		ecs.GetComponent<ComponentAction>(playerEntity).maxEnergy = 100;
		ecs.GetComponent<ComponentAction>(playerEntity).energy = 100;
		ecs.GetComponent<ComponentAction>(playerEntity).pActionDecider = new ActionDeciderPlayer;
		ecs.GetComponent<ComponentHealth>(playerEntity).health = 1000;
		ecs.GetComponent<ComponentHealth>(playerEntity).maxHealth = 1000;
		ecs.GetComponent<ComponentFaction>(playerEntity).factionFlags = ComponentFaction::EFactionFlags::Player;
	}

	// Set random seed based off time
	srand((unsigned int)time(NULL));
	worldGenerator.SetCenter(WORLD_START_X, WORLD_START_Y, /*bInit =*/true);

	hud.Initialise(playerEntity);
}  

void Game::UnInit()
{
	hud.Uninitialise();
	worldGenerator.Uninitialise();
	ecs.Uninitialise();

#ifdef DOMIMGUI
	imGuiEditor.Uninit();
#endif //~ DOMIMGUI
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
	dmgf::SetCameraCenter(transform.x * SystemRender::GRID_SIZE + SystemRender::GRID_SIZE * 0.5f, transform.y * SystemRender::GRID_SIZE + SystemRender::GRID_SIZE * 0.5f);
	hud.Tick(deltaTime);

	// Zoom
	{
		static float zoom = 64.f / SystemRender::GRID_SIZE;
		static float zoomSpeed = 64.f / SystemRender::GRID_SIZE;
		if (dmwi::isHeld(dmwi::Button::PLUS))
		{
			zoom += zoomSpeed * deltaTime;
		}
		if (dmwi::isHeld(dmwi::Button::SUB))
		{
			zoom -= zoomSpeed * deltaTime;
		}
		dmgf::SetCameraZoom(zoom);
	}
}

void Game::tick(float deltaTime)
{
#ifdef DOMIMGUI
	CreateImGuiWindow(deltaTime);
	imGuiEditor.Tick();
#endif //~ #ifdef DOMIMGUI
	GameplayTick(deltaTime);

}
