#pragma once

namespace RenderPriority
{
	// Higher renders on top of lower

	// ** ERenderObjectType::InGame **
	const float background = 0.0f; // Background floor stuff like Grass etc.
	const float foreground = 0.1f; // Foreground objects on top of floor like Trees etc.
	const float unit = 0.2f; // Creatures in the world
	const float inGameUI = 0.3f; // In-game icons like Health bars above enemy head etc.
	const float spellcastUI = 0.4f; // Green/red transparent highlights when aiming a skill

	// ** ERenderObjectType::UI **	
	const float HUD = 0.8f;
	const float menu = 0.9f;
	const float dragAndDropIcon = 1.0f; // floating icon following mouse while drag and dropping
}