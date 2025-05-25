#pragma once

#include "EditorWindowBase.h"

// Editor window to select which asset type to create, a more specific window will be opened after this one depending on the type of asset selected
class EditorWindowChooseAssetType : public EditorWindowBase
{
public:
	EditorWindowChooseAssetType(const std::filesystem::path& inFilePath) : filePath(inFilePath) {}

	//~ Begin EditorWindowBase Interface
	std::string GetWindowName() const override { return "Choose Asset Type"; }
	//~ End EditorWindowBase Interface

protected:
	//~ Begin EditorWindowBase Interface
	void Draw() override;
	//~ End EditorWindowBase Interface

private:

	template <typename T>
	void DrawAssetTypeButton(const std::string& buttonText);

	// Folder that the new asset will be created in
	std::filesystem::path filePath;
};
