#include "pch.h"

#include "EditorActionCreateAssetBase.h"

#include "Editor/ImGuiEditorGlobals.h"

void EditorActionCreateAssetBase::Undo()
{
	if (!pCreatedAsset.expired())
	{
		std::filesystem::path assetFilePath = targetPath / (assetName + ImGuiEditorGlobals::assetExtension);
		std::filesystem::remove(assetFilePath);
		
		Game::GetAssetManager().RemoveAsset(pCreatedAsset.lock());
		pCreatedAsset.reset();
	}
}

bool EditorActionCreateAssetBase::TryExecuteAction()
{
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

	std::shared_ptr<EditorAssetBase> pNewAsset = CreateAsset(assetFilePath);

	if (!pNewAsset)
	{
		DOMLOG_ERROR("Failed to create new asset");
		return false;
	}
	
	// Write to file with default values
	std::ofstream fileStream(assetFilePath);
	pNewAsset->WriteToFile(fileStream);

	pCreatedAsset = pNewAsset;
	Game::GetAssetManager().AddAsset(pNewAsset);

	return true;
}


std::string EditorActionCreateAssetBase::GetDescription() const
{
	return "Create Asset: " + assetName;
}
