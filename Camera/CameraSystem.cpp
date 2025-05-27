#include "pch.h"

#include "CameraSystem.h"

#include "ECS/Components.h"
#include "ECS/ECS.h"
#include "ECS/Systems/ECSSystemRender.h"
#include "Graphics/Renderer.h"
#include "DomWindow/DomWindow.h"

void CameraSystem::Init()
{
	
}

void CameraSystem::Tick(float deltaTime)
{
	// Camera follow player
	if (pEcs)
	{
		ComponentTransform& transform = pEcs->GetComponent<ComponentTransform>(pEcs->GetPlayerEntityId());
		cameraCenter = {(transform.x * ECSSystemRender::GRID_SIZE) + (ECSSystemRender::GRID_SIZE * 0.5f), (transform.y * ECSSystemRender::GRID_SIZE) + (ECSSystemRender::GRID_SIZE * 0.5f) };
		dmgf::SetCameraCenter(cameraCenter.x, cameraCenter.y);
	}

	// Zoom
	// #TODO: Need to use InputActions for zoom
	// #TODO: Needs upper/lower limit before shipping
	{
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

void CameraSystem::UnInit()
{
	
}

Vec2i CameraSystem::GetWorldGridCoordinateUnderMouse() const
{
	const float screenWidth = dmgf::GetScreenWidth();
	const float screenHeight = dmgf::GetScreenHeight();

	const Vec2f cameraTopLeft = cameraCenter - Vec2f((screenWidth * 0.5f) / zoom, (screenHeight * 0.5f) / zoom);

	const float mouseX = (float)dmwi::GetMouseX();
	const float mouseY = (float)dmwi::GetMouseY();
	
	const Vec2f mousePosInWorldSpace = (cameraTopLeft + (Vec2f(mouseX, mouseY) / zoom)) / ECSSystemRender::GRID_SIZE;

	return {(int)(mousePosInWorldSpace.x), (int)(mousePosInWorldSpace.y)};
}