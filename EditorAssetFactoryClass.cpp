#include "pch.h"

#include "EditorAssetFactoryClass.h"

#include "EditorAssetClass.h"
#include "EditorTypeClass.h"
#include "ImGuiEditor.h"


std::string EditorAssetFactoryClass::GetKeyword() const
{
	return "Object";
}

std::shared_ptr<EditorAssetBase> EditorAssetFactoryClass::CreateAsset(const std::filesystem::path& assetFilePath)
{
	// We need to find the class name so we can get the correct template type from the editor
	std::ifstream stream(assetFilePath);
	std::string className;

	stream >> className;
	DOMLOG_ERROR_IF(className != "Object", "Expecting Object");

	stream >> className >> className;
	DOMLOG_ERROR_IF(className != "Class", "Expecting Class");

	stream >> className;

	EditorTypeBase* pFoundType = editor.FindType(className);
	if (!pFoundType)
	{
		DOMLOG_ERROR("No type found for", className);
		return {};
	}

	EditorTypeClass* pEditorTypeClass = dynamic_cast<EditorTypeClass*>(pFoundType);
	if (!pEditorTypeClass)
	{
		DOMLOG_ERROR("Type found for", className, "but it's not a class");
		return {};
	}
	
	std::shared_ptr<EditorAssetClass> pNewAsset = std::make_shared<EditorAssetClass>("", pEditorTypeClass, assetFilePath); // Empty name as it'll get populated in ReadFile()
	
	stream.seekg(0);
	pNewAsset->ReadFromFile(stream);

	return pNewAsset;
}
