#pragma once

#include "EditorWindowCreateAssetBase.h"

#include <filesystem>

#include "ImGui/ImGuiItemListWithSearchBar.h"


/** EditorWindowCreateAssetClass
*
* This class is used to display a window for creating a new EditorAssetClass,
* with error messages for invalid asset names or duplicate assets.
*/
class EditorWindowCreateAssetClass : public EditorWindowCreateAssetBase
{
public:
	EditorWindowCreateAssetClass(const std::filesystem::path& filePath) : EditorWindowCreateAssetBase(filePath) { }

protected:
	//~ Begin EditorWindowBase Interface
	virtual void Open() override;
	virtual void Draw() override;
	std::string GetWindowName() const override { return "Create Class Asset"; }
	//~ End EditorWindowBase Interface

private:

	//~ Begin EditorWindowCreateAssetBase Interface
	std::string GetErrorMessage() const override;
	//~ End EditorWindowCreateAssetBase Interface
	
	std::string selectedClass;

	ImGuiItemListWithSearchBar searchBar;
	
};
