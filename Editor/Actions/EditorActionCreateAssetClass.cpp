#include "pch.h"

#include "EditorActionCreateAssetClass.h"

#include "AssetManager/AssetManager.h"
#include "Editor/Assets/EditorAssetClass.h"
#include "Editor/Types/EditorTypeClass.h"
#include "Game.h"

bool EditorActionCreateAssetClass::TryExecuteAction()
{
	EditorTypeClass* pEditorTypeClass = Game::GetAssetManager().FindClassTemplateType(className);
	if (!pEditorTypeClass)
	{
		DOMLOG_ERROR("Class", className, "not found");
		return false;
	}
	
	return EditorActionCreateAssetBase::TryExecuteAction();
}

std::shared_ptr<EditorAssetBase> EditorActionCreateAssetClass::CreateAsset(const std::filesystem::path& assetFilePath)
{
	EditorTypeClass* pEditorTypeClass = Game::GetAssetManager().FindClassTemplateType(className);
	return std::make_shared<EditorAssetClass>(assetName, pEditorTypeClass, assetFilePath);
}
