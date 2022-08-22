#pragma once

#include "DomMath/Mat4.h"
#include "DomUtils/DomUtils.h"

class RendererObject;

// #TODO: Cull all the quads not on screen

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
	
	void RemoveObject(RendererObject* pRendererObject);

	void SetCameraCenter(float x, float y);
}
