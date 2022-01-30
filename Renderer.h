#pragma once

#include "DomMath/Mat4.h"
#include "DomUtils/DomUtils.h"

class RendererObject;

namespace dmgf
{
	// Initialise graphics -- Must be called after the window has been initialised.
	void Init();

	// Uninitialise graphics
	void UnInit();

	// Once per frame
	void Tick(float deltaTime);

	// Add object from an svg file
	RendererObject* AddObjectFromSVG(const char* pFileName);
}
