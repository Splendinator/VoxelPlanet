#pragma once

#include <DomMath/Types.h>

#include "Core/GameSystem.h"

// #TEMP: I DON'T THINK WE NEED THIS AS WE CAN USE THE InputContexts, DELETE LATER?

// Priority flags, only the highest priority system should be capturing mouse input 
enum class EMouseCapturePriorityFlags : u8
{
	// Game -- Used to aim abilities by clicking on the map etc.
	Game = 1 << 0,

	// Menu -- Used to click buttons in the menu screens etc.
	Menu = 1 << 1,
};

// This is used as an API to the mouse cursor position.
// The only use right now is to decide which system gets to read mouse inputs (game, menu, etc.)
class MouseSystem : public GameSystem
{
public:

	EMouseCapturePriorityFlags PriorityFlags;
};
