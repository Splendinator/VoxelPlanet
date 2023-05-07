#pragma once

#include "EditorActionBase.h"

#include <filesystem>

/** EditorActionCreateNewFolder
*
* This class is used for creating a new folder as an undoable and redoable action.
*/
class EditorActionCreateNewFolder : public EditorActionBase
{
public:

	EditorActionCreateNewFolder(const std::filesystem::path& targetPath, const std::string& folderName) : targetPath(targetPath), folderName(folderName) {};

	//~ Begin EditorActionBase Interface
	void Undo() override;
	bool TryExecuteAction() override;
	std::string GetDescription() const override;
	//~ End EditorActionBase Interface

private:
	std::filesystem::path targetPath;
	std::string folderName;
};
