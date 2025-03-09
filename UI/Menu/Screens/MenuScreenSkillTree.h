#pragma once

#include "MenuScreenBase.h"

class DragAndDropManager;
class ECS;
class RPGSkillSystem;
class UIObjectDragAndDropArea;

EDITORSTRUCT()
struct SkillTreeMenuSkillSlotData
{
	EDITORBODY()

	// Name of the button in the menu file
	EDITORPROPERTY()
	std::string buttonLayerName;
	
	TransientPtr<UIObjectDragAndDropArea> dragAndDropArea;
};

EDITORCLASS()
class MenuScreenSkillTree : public MenuScreenBase
{
	EDITORBODY()

protected:

	//~ Begin MenuScreenBase Interface
	void Init(UICanvas* pCanvas) override;
	void UnInit() override;
	//~ End MenuScreenBase Interface

	EDITORPROPERTY()
	std::vector<SkillTreeMenuSkillSlotData> skillSlotDatas;

	EDITORPROPERTY()
	DragAndDropManager* pDragAndDropManager = nullptr;
	
	EDITORPROPERTY()
	ECS* pEcs = nullptr;
};
