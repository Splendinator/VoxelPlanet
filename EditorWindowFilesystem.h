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

	//~ Begin EditorWindowBase Interface
	virtual void Draw() override;
	//~ End EditorWindowBase Interface

protected:

	void DrawDirectory(const std::filesystem::path& path);

private:

	std::filesystem::path rootDirectory;

};
