#pragma once

#include "EditorWindowBase.h"

#include <filesystem>

/** EditorWindowCreateAsset
*
* This class is used to display a window for creating a new asset,
* with error messages for invalid asset names or duplicate assets.
*/
class EditorWindowCreateAsset : public EditorWindowBase
{
public:

	EditorWindowCreateAsset(const std::filesystem::path& filePath) : targetFilePath(filePath) {}

protected:
	//~ Begin EditorWindowBase Interface
	virtual void Draw() override;
	std::string GetWindowName() const override { return "Create Asset"; }
	//~ End EditorWindowBase Interface

private:
	void CreateNewAssetWindow();
	std::string GetErrorMessage(const std::string& assetName);

	std::filesystem::path targetFilePath;

	std::string selectedClass;

	char assetNameBuffer[128] = {};
	char searchBuffer[128] = {};

};
