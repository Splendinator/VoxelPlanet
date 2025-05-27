#include "pch.h"

#include "DirectoryData.h"

std::string DirectoryData::ConcatenateFilePathChecked(std::string path, std::string fileName, std::string extension)
{
	DOMLOG_ERROR_IF(path.empty(), "File path is empty");
	DOMLOG_ERROR_IF(fileName.empty(), "File name is empty");
	DOMLOG_ERROR_IF(extension.empty(), "File extension is empty");
	
	std::string fullPath = path + fileName + extension;
	
	DOMLOG_ERROR_IF(!std::filesystem::exists(fullPath), "Invalid file:", fullPath);
	
	return fullPath;
}
