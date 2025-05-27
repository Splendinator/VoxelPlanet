#pragma once

#include "EditorWindowBase.h"

#include "Editor/Types/EditorTypeDataComposite.h"

#include "ImGui/ImGuiItemListWithSearchBar.h"

class EditorWindowInsertDataCompositeItem : public EditorWindowBase
{
public:

	EditorWindowInsertDataCompositeItem(std::weak_ptr<EditorAssetDataComposite> inDataCompositeAsset, int inInsertIndex = -1) : EditorWindowBase(), dataCompositeAsset(inDataCompositeAsset), insertIndex(inInsertIndex) {}
	
	//~ Begin EditorWindowBase Interface
	std::string GetWindowName() const override;
	//~ End EditorWindowBase Interface

protected:

	//~ Begin EditorWindowBase Interface
	void Open() override;
	void Draw() override;
	//~ End EditorWindowBase Interface
	
	std::weak_ptr<EditorAssetDataComposite> dataCompositeAsset;

	ImGuiItemListWithSearchBar propertyTypeList;

	int insertIndex = -1; // Index to insert the new property at. If -1, it will be added at the end of the list.
	
	char propertyNameBuffer[128] = {};

	bool bCreateVector = false; // Whether to create a vector of the property type (bCreateVector == true and PropertyType == "string" will create a vector of strings, not a single string property)
};
