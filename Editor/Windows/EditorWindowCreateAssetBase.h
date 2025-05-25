#pragma once

#include "EditorWindowBase.h"

// Base class for windows that create a new asset.
class EditorWindowCreateAssetBase : public EditorWindowBase
{
public:
	EditorWindowCreateAssetBase(const std::filesystem::path& filePath) : targetFilePath(filePath) {}
	

protected:
	
	// Text to let you know where the asset is being created.
	void RenderFilePathText() const;

	// Input field for the asset name.
	void RenderAssetNameInput();
	
	// Returns whether there was an error. Renders an error message if there's one to show.
	[[nodiscard]] bool RenderErrorMessageIfOneExists();
	
	virtual std::string GetErrorMessage() const;
	
	char assetNameBuffer[128] = {};
	
	// Filepath that the new asset should go in
	std::filesystem::path targetFilePath;
	
};
