#pragma once

#include "DomUtils/DomUtils.h"

class Camera;

namespace dmgf
{
	// Initialise graphics -- Must be called after the window has been initialised.
	void NewInit();

	// Uninitialise graphics
	void UnInit();

	// Once per frame
	void Tick(float deltaTime);

	Camera& GetCamera();
}