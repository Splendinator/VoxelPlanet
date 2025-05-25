#pragma once

#include "EditorWindowBase.h"

#include <filesystem>

/** EditorWindowCreateNewFile
*
* This class is used to display a window for creating a new folder,
* with error messages for invalid folder names or duplicate folders.
*/
class EditorWindowCreateNewFolder : public EditorWindowBase
{
public:

	EditorWindowCreateNewFolder(const std::filesystem::path& filePath) : targetFilePath(filePath) {}

protected:
	//~ Begin EditorWindowBase Interface
	virtual void Draw() override;
	std::string GetWindowName() const override { return "Create Folder"; }
	//~ End EditorWindowBase Interface

private:
	void CreateNewFolderWindow();
	std::string GetErrorMessage(const std::string& fileName);

	std::filesystem::path targetFilePath;

	char folderNameBuffer[128] = {};
};
