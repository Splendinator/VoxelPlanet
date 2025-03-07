#pragma once

#include "Core/GameSystem.h"

class ECS;

EDITORCLASS()
class CameraSystem : public GameSystem
{
	EDITORBODY()

public:

	// Get the world-grid coordinate space under the mouse
	// This can be passed in to ECSSystemEntityMap to get entities under the mouse
	Vec2i GetWorldGridCoordinateUnderMouse() const;
	
protected:
	
	//~ Begin GameSystem Interface
	void Init() override;
	void Tick(float deltaTime) override;
	void UnInit() override;
	//~ End GameSystem Interface

	EDITORPROPERTY()
	ECS* pEcs = nullptr;

	// How zoomed out the camera is by default (relative to ECSSystemRender::GRID_SIZE)
	EDITORPROPERTY()
	float zoom = 64.f;

	// Speed per second that we zoom in/out when hitting numpad +/- 
	EDITORPROPERTY()
	float zoomSpeed = 64.f;

	Vec2f cameraCenter;
};
