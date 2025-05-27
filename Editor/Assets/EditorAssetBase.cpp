#include "pch.h"

#include "EditorAssetBase.h"

void EditorAssetBase::SaveAsset()
{
	std::ofstream outFile(assetFilePath);
	if (!outFile.is_open())
	{
		DOMLOG_ERROR("Cannot open asset path:", assetFilePath.string());
		return;
	}

	WriteToFile(outFile);
}

void EditorAssetBase::OnPropertyChanged(const OnPropertyChangedData& params)
{
	onPropertyChanged.Invoke(params);
}
