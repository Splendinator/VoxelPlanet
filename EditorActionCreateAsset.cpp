#include "pch.h"

#include "EditorActionCreateAsset.h"

#include "EditorAssetClass.h"
#include "ImGuiEditor.h"
#include "ImGuiEditorGlobals.h"


void EditorActionCreateAsset::Undo()
{
	if (pCreatedAsset)
	{
		std::filesystem::path assetFilePath = targetPath / (assetName + ImGuiEditorGlobals::assetExtension);
		std::filesystem::remove(assetFilePath);
		
		editor.RemoveAsset(pCreatedAsset);
		pCreatedAsset = nullptr;
	}
}

bool EditorActionCreateAsset::TryExecuteAction()
{
	EditorTypeBase* pEditorType = editor.FindType(className);
	if (!pEditorType)
	{
		DOMLOG_ERROR("Class type not found");
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

	pCreatedAsset = new EditorAssetClass(assetName, pEditorType);
	editor.AddAsset(pCreatedAsset);

	return true;
}

std::string EditorActionCreateAsset::GetDescription() const
{
	return "Create Asset: " + assetName;
}