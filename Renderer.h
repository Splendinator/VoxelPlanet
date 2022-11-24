#pragma once

#include "DomMath/Mat4.h"
#include "DomUtils/DomUtils.h"
#include "DomUtils/Pointers.h"

class RendererObject;
class VectorArt;

// #TODO: Cull all the quads not on screen

namespace dmgf
{
	enum class ERenderObjectType
	{
		InGame,		// Rendered as an in-game object subject to camera movement
		HUD			// Rendered as a HUD object, stuck to the screen
	};

	// Initialise graphics -- Must be called after the window has been initialised.
	void Init();

	// Uninitialise graphics
	void UnInit();

	// Once per frame
	void Tick(float deltaTime);

	// Add object from an svg file. Objects from the same file will share the same vector art
	TransientPtr<RendererObject> AddObjectFromSVG(const char* pFileName, ERenderObjectType renderObjectType = ERenderObjectType::InGame);

	// Add object from vector art. This is used if you want to mess with the vector art mid-game (scale primitives etc.)
	TransientPtr<RendererObject> AddObjectFromVectorArt(const std::shared_ptr<VectorArt>& vectorArt, ERenderObjectType renderObjectType = ERenderObjectType::InGame);
	
	void RemoveObject(TransientPtr<RendererObject> pRendererObject);

	void SetCameraCenter(float x, float y);
	void SetCameraZoom(float zoom);

	float GetScreenWidth();
	float GetScreenHeight();
}
