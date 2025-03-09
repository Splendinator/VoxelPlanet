#pragma once

class DragAndDropManager;
class IDragAndDroppable;
struct DragAndDropAreaRegisterParams;

// Interface used by anything wanting to act as an area that can be dragged to/from
// This is designed to be used by the buttons on the UI that let you drag to/from, e.g. the hotbar slots.
// To use this just register it with DragAndDropManager::RegisterArea()
class IDragAndDropArea
{
public:

	// This is called when the player starts dragging with the mouse over this area.
	// It should populate outDroppable with the droppable associated with this area (skill on hotbar etc.)  
	virtual void RecievedDragEvent(IDragAndDroppable*& pOutDroppable) {};

	// This is called when the player drops a draggable over this area
	virtual void RecievedDropEvent(IDragAndDroppable& droppable) {};
	
	// Derrived classes should set this up with the correct size 
	Box2f dragAndDropAreaBoundingBox = {};
};
