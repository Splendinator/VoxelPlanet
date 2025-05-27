#pragma once

#include "EditorWindowBase.h"

#include <filesystem>

/** EditorWindowFilesystem
*
* This class is used to show an ImGUI editor window that lets you browse the file system
*/
class EditorWindowFilesystem : public EditorWindowBase
{
public:

	EditorWindowFilesystem(const std::filesystem::path& inDirectory) : rootDirectory(inDirectory) {}

protected:
	//~ Begin EditorWindowBase Interface
	std::string GetWindowName() const override { return "Editor"; }
	void Draw() override;
	bool CanCloseOnRequest() const override { return false; }
	//~ End EditorWindowBase Interface

	void DrawDirectory(const std::filesystem::path& path);

	// Returns whether the search bar was rendered
	bool DrawSearchBar();
	
	void DrawAsset(const std::filesystem::directory_entry& entry, bool bShowOpenFileLocation);

	void RefreshSearchableAssets(const std::filesystem::path& path);
private:

	struct SearchableAssetData
	{
		std::string name;
		std::filesystem::directory_entry directoryEntry;
	};
	
	std::filesystem::path rootDirectory;

	std::vector<SearchableAssetData> searchableAssets;

	// User has requested to open a file at location. This will close all folders except the one containing this file.
	std::filesystem::directory_entry openFileLocationRequest; 
	
	char searchBuffer[128] = {};

};
