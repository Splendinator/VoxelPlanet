#pragma once

#include "DomMath/Mat4.h"
#include "DomUtils/DomUtils.h"

class RendererObject;
class VectorArt;

// #TODO: Cull all the quads not on screen

namespace dmgf
{
	// Initialise graphics -- Must be called after the window has been initialised.
	void Init();

	// Uninitialise graphics
	void UnInit();

	// Once per frame
	void Tick(float deltaTime);

	// Add object from an svg file. Objects from the same file will share the same vector art
	RendererObject* AddObjectFromSVG(const char* pFileName);

	// Add object from vector art. This is used if you want to mess with the vector art mid-game (scale primitives etc.)
	RendererObject* AddObjectFromVectorArt(const std::shared_ptr<VectorArt>& vectorArt);
	
	void RemoveObject(RendererObject* pRendererObject);

	void SetCameraCenter(float x, float y);
}
