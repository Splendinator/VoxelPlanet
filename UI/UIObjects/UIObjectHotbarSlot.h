#pragma once

#include "UIObjectDragAndDropArea.h"

class UIObjectHotbarSlot : public UIObjectDragAndDropArea
{
protected:
	//~ Begin UIObjectBase Interface
	void Init(VectorPrimitiveLayer* pRoot) override;
	void Uninit() override;
	//~ End UIObjectBase Interface

	//~ Begin IDragAndDropArea Interface
	void RecievedDragEvent(IDragAndDroppable*& pOutDroppable) override;
	void RecievedDropEvent(IDragAndDroppable& droppable) override;
	//~ End IDragAndDropArea Interface

public:
	
};
