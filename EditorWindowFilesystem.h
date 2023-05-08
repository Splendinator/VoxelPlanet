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

private:

	std::filesystem::path rootDirectory;

};
