#include "pch.h"

#include "EditorActionDeleteFile.h"

#include <fstream>
#include "DomLog/DomLog.h"

void EditorActionDeleteFile::Undo()
{
	std::ofstream outFile(targetFile);
	if (!outFile)
	{
		DOMLOG_ERROR("File cannot be re-created after deleting", targetFile);
	}

	outFile.write(deletedFileContents.RawPtr(), deletedFileContents.GetSize());

	deletedFileContents.DeAlloc();
}

bool EditorActionDeleteFile::TryExecuteAction()
{
	if (!std::filesystem::exists(targetFile))
	{
		DOMLOG_WARN("Trying to delete asset that doesn't exist", targetFile)
		return false;
	}

	std::ifstream fileStream(targetFile);

	if (!fileStream.is_open())
	{
		DOMLOG_WARN("Can't open file", targetFile, ". Won't delete")
		return false;
	}

	// #JANK: seekg(0, end) is adding weird characters at the end when you Undo() but because it doesn't mess up our parser I simply cannot bring myself to care. (They get removed when you resave) 
	fileStream.seekg(0, std::ios_base::end);
	const size_t fileSize = fileStream.tellg();
	deletedFileContents.Alloc(fileSize);
	
	fileStream.seekg(0, std::ios_base::beg);
	fileStream.read(deletedFileContents.RawPtr(), fileSize);
	fileStream.close();
	
	int errorCode = std::remove(targetFile.string().c_str());

	if (errorCode != 0)
	{
		DOMLOG_WARN("Can't delete file", targetFile, ". Maybe it's in use?")
		deletedFileContents.DeAlloc();
		return false;
	}
	
	return true;
}

std::string EditorActionDeleteFile::GetDescription() const
{
	return "Delete Asset: " + targetFile.string();
}
