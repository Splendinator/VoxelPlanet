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
	EditorTypeBase* pEditorType = editor.FindType(className);
	if (!pEditorType)
	{
		DOMLOG_ERROR("Class type not found");
		return false;
	}

	EditorTypeClass* pEditorTypeClass = dynamic_cast<EditorTypeClass*>(pEditorType);
	if (!pEditorTypeClass)
	{
		DOMLOG_ERROR("Type found but it isn't a class");
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
	pCreatedAsset = pNewAsset;
	editor.AddAsset(pNewAsset);

	return true;
}

std::string EditorActionCreateAsset::GetDescription() const
{
	return "Create Asset: " + assetName;
}