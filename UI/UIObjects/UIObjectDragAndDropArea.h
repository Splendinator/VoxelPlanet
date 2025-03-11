#pragma once

#include "UIObjectIconLoader.h"

#include "UI/DragAndDrop/DragAndDropShared.h"
#include "UI/DragAndDrop/IDragAndDropArea.h"

class UIObjectDragAndDropArea;
class DirectoryData;

struct UIObjectDragAndDropSetupParams
{
	DirectoryData* pDirectoryData = nullptr;
	DragAndDropManager* pDragAndDropManager = nullptr;

	DragAndDropAreaRegisterParams areaParams = {};
};

struct UIObjectDragAndDropDelegateParams
{
	UIObjectDragAndDropArea* pDragAndDropArea = nullptr;
	IDragAndDroppable* pDragAndDroppable = nullptr;
};

class UIObjectDragAndDropArea : public UIObjectIconLoader, public IDragAndDropArea
{

public:
	
	// This needs to be called before any droppables can be rendered
	void Setup(UIObjectDragAndDropSetupParams params);
	
	// Fill this slot with the droppable
	void SetDroppable(IDragAndDroppable* pDroppable);
	
	void SetShouldDraggingClear(bool bInShouldDraggingClear) { bShouldDraggingClear = bInShouldDraggingClear; }
	
	DelegateList<const UIObjectDragAndDropDelegateParams&> onDragEventDelegates;
	DelegateList<const UIObjectDragAndDropDelegateParams&> onDropEventDelegates;

protected:
	//~ Begin UIObjectBase Interface
	void Init(VectorPrimitiveLayer* pRoot) override;
	void Uninit() override;
	//~ End UIObjectBase Interface

	//~ Begin IDragAndDropArea Interface
	void RecievedDragEvent(IDragAndDroppable*& pOutDroppable) override;
	void RecievedDropEvent(IDragAndDroppable& droppable) override;
	//~ End IDragAndDropArea Interface

	TransientPtr<IDragAndDroppable> pShownDragAndDroppable = nullptr;
	TransientPtr<DirectoryData> pDirectoryData = nullptr;
	TransientPtr<DragAndDropManager> pDragAndDropManager = nullptr;

	bool bShouldDraggingClear = false; // When you drag the item out of this area should it clear it?
};
