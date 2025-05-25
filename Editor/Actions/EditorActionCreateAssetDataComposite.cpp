#include "pch.h"

#include "EditorActionCreateAssetDataComposite.h"

#include "Editor/Assets/EditorAssetDataComposite.h"

std::shared_ptr<EditorAssetBase> EditorActionCreateAssetDataComposite::CreateAsset(const std::filesystem::path& assetFilePath)
{
    return std::make_shared<EditorAssetDataComposite>(assetName, assetFilePath);
}