#include "pch.h"

#include "EditorActionCreateAsset.h"

#include "EditorAssetClass.h"
#include "EditorTypeClass.h"
#include "ImGuiEditor.h"
#include "ImGuiEditorGlobals.h"

void EditorActionCreateAsset::Undo()
{
	if (!pCreatedAsset.expired())
	{
		std::filesystem::path assetFilePath = targetPath / (assetName + ImGuiEditorGlobals::assetExtension);
		std::filesystem::remove(assetFilePath);
		
		editor.RemoveAsset(pCreatedAsset.lock());
		pCreatedAsset.reset();
	}
}

bool EditorActionCreateAsset::TryExecuteAction()
{
	EditorTypeClass* pEditorTypeClass = editor.FindClassTemplateType(className);
	if (!pEditorTypeClass)
	{
		DOMLOG_ERROR("Class", className, "not found");
		return false;
	}

	std::filesystem::path assetFilePath = targetPath / (assetName + ImGuiEditorGlobals::assetExtension);
	if (std::filesystem::exists(assetFilePath))
	{
		DOMLOG_ERROR("File already exists");
		return false;
	}

	std::ofstream outFile(assetFilePath);
	if (!outFile)
	{
		DOMLOG_ERROR("File not created");
		return false;
	}
	outFile.close();

	std::shared_ptr<EditorAssetClass> pNewAsset = std::make_shared<EditorAssetClass>(assetName, pEditorTypeClass, assetFilePath);

	// Write to file with default values
	std::ofstream fileStream(assetFilePath);
	pNewAsset->WriteToFile(fileStream);

	pCreatedAsset = pNewAsset;
	editor.AddAsset(pNewAsset);

	return true;
}

std::string EditorActionCreateAsset::GetDescription() const
{
	return "Create Asset: " + assetName;
}