#include "pch.h"

#include "EditorAssetFactoryDataComposite.h"

#include "EditorAssetDataComposite.h"

std::string EditorAssetFactoryDataComposite::GetKeyword() const
{
	return "DataComposite";
}

std::shared_ptr<EditorAssetBase> EditorAssetFactoryDataComposite::CreateAsset(const std::filesystem::path& assetFilePath)
{
	std::shared_ptr<EditorAssetDataComposite> pNewAsset = std::make_shared<EditorAssetDataComposite>("", assetFilePath);

	std::ifstream stream(assetFilePath);
	stream.seekg(0);
	pNewAsset->ReadFromFile(stream);

	return pNewAsset;
}
