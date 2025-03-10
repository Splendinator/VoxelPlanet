#include "pch.h"

#include "UIObjectHotbarSlot.h"

void UIObjectHotbarSlot::Init(VectorPrimitiveLayer* pRoot)
{
	// #TEMP: Dont make this derrive from thingy just make it a composite thing
	
	VectorPrimitiveLayer* pDragAndDropArea = pRoot->FindLayerByLabel("Icon");

	UIObjectDragAndDropArea::Init(pDragAndDropArea ? pDragAndDropArea : pRoot); // We're only using the "icon" sub-portion of this object as a drag and droppable
	
	// #TEMP: Bind to hotbar manager delegates
}

void UIObjectHotbarSlot::Uninit()
{
	// #TEMP: Unbind from hotbar manager delegates
	
	UIObjectDragAndDropArea::Uninit();
}

void UIObjectHotbarSlot::RecievedDragEvent(IDragAndDroppable*& pOutDroppable)
{
	UIObjectDragAndDropArea::RecievedDragEvent(pOutDroppable);

	SetDroppable(nullptr);
}

void UIObjectHotbarSlot::RecievedDropEvent(IDragAndDroppable& droppable)
{
	UIObjectDragAndDropArea::RecievedDropEvent(droppable);

	SetDroppable(&droppable);
}
