#pragma once

// Priority for drag and drop areas, Higher numbers take priority.
enum class EDragAndDropAreaPriority
{
	HUD,
	Menus,
	COUNT
};

struct DragAndDropAreaRegisterParams
{
	EDragAndDropAreaPriority priority = EDragAndDropAreaPriority::HUD;	
};