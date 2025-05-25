#include "pch.h"

#include "EditorActionCreateNewFolder.h"

void EditorActionCreateNewFolder::Undo()
{
	std::filesystem::remove(targetPath / folderName);
}

bool EditorActionCreateNewFolder::TryExecuteAction()
{
	std::filesystem::path newFolderPath = targetPath / folderName;
	if (!std::filesystem::exists(newFolderPath) && !folderName.empty())
	{
		std::filesystem::create_directory(newFolderPath);
		return true;
	}
	return false;
}

std::string EditorActionCreateNewFolder::GetDescription() const
{
	return "Created folder: " + folderName;
}
