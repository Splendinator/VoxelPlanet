#pragma once

#include "UIObjectIconLoader.h"

#include "UI/DragAndDrop/DragAndDropShared.h"
#include "UI/DragAndDrop/IDragAndDropArea.h"

class DirectoryData;

struct UIObjectDragAndDropSetupParams
{
	DirectoryData* pDirectoryData = nullptr;
	DragAndDropManager* pDragAndDropManager = nullptr;

	DragAndDropAreaRegisterParams areaParams = {};
};

// #TODO: Needs non-essential functionality moving to external systems via delegates 
class UIObjectDragAndDropArea : public UIObjectIconLoader, public IDragAndDropArea
{

public:
	
	// This needs to be called before any droppables can be rendered
	void Setup(UIObjectDragAndDropSetupParams params);
	
	// Fill this slot with the droppable
	void SetDroppable(IDragAndDroppable* pDroppable);

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
};
