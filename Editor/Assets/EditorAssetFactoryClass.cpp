#include "pch.h"

#include "EditorAssetFactoryClass.h"

#include "AssetManager/AssetManager.h"
#include "EditorAssetClass.h"
#include "Editor/Types/EditorTypeClass.h"

std::string EditorAssetFactoryClass::GetKeyword() const
{
	return "Asset";
}

std::shared_ptr<EditorAssetBase> EditorAssetFactoryClass::CreateAsset(const std::filesystem::path& assetFilePath)
{
	// We need to find the class name so we can get the correct template type from the assetManager
	std::ifstream stream(assetFilePath);
	std::string className;

	stream >> className;
	DOMLOG_ERROR_IF(className != "Asset", "Expecting Object");

	stream >> className >> className;
	DOMLOG_ERROR_IF(className != "Class", "Expecting Class");

	stream >> className;
	
	EditorTypeClass* pEditorTypeClass = assetManager.FindClassTemplateType(className);
	if (!pEditorTypeClass)
	{
		DOMLOG_ERROR("No class found for", className);
		return {};
	}
	
	std::shared_ptr<EditorAssetClass> pNewAsset = std::make_shared<EditorAssetClass>("", pEditorTypeClass, assetFilePath); // Empty name as it'll get populated in ReadFile()
	
	stream.seekg(0);
	pNewAsset->ReadFromFile(stream);

	return pNewAsset;
}
