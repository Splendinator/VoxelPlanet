#pragma once

#include "UI/DragAndDrop/IDragAndDroppable.h"

class RPGSkillEffectModuleBase;
class RPGSkillAimModuleBase;

// Data asset describing a single spell
EDITORCLASS(Singleton)
class RPGSkillData : public IDragAndDroppable
{
	EDITORBODY()
public:
	bool IsValid() const { return pAimModule && pEffectModule; }

	// Name of the layer of the icon that shows this skill
	EDITORPROPERTY()
	std::string iconLayerName;
	
	EDITORPROPERTY()
	const RPGSkillAimModuleBase* pAimModule = nullptr;

	EDITORPROPERTY()
	const RPGSkillEffectModuleBase* pEffectModule = nullptr;

protected:
	
	//~ Begin IDragAndDroppable Interface
	std::string GetDragAndDropIconFile(DirectoryData& directoryData) const override;
	std::string GetOptionalDragAndDropIconLayer() const override { return iconLayerName; }
	//~ End IDragAndDroppable Interface
};
