#pragma once

#include "EditorActionCreateAssetBase.h"

// Action to create an EditorAssetDataComposite
class EditorActionCreateAssetDataComposite : public EditorActionCreateAssetBase
{
public:
    EditorActionCreateAssetDataComposite(const std::filesystem::path& inTargetPath, const std::string& inAssetName)
        : EditorActionCreateAssetBase(inTargetPath, inAssetName) {}


protected:

    //~ Begin EditorActionCreateAssetBase Interface
    std::shared_ptr<EditorAssetBase> CreateAsset(const std::filesystem::path& assetFilePath) override;
    //~ End EditorActionCreateAssetBase Interface
};