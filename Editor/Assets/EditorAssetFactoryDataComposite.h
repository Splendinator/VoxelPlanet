#pragma once

#include "EditorAssetFactoryBase.h"

/** EditorAssetFactoryDataComposite
*
* This class is used for creating EditorAssetDataComposite type assets from an input file stream.
*/
class EditorAssetFactoryDataComposite : public EditorAssetFactoryBase
{
public:
    EditorAssetFactoryDataComposite(AssetManager& inAssetManager) : EditorAssetFactoryBase(inAssetManager) {}

    //~ Begin EditorAssetFactoryBase Interface
    virtual std::string GetKeyword() const override;
    virtual std::shared_ptr<EditorAssetBase> CreateAsset(const std::filesystem::path& assetFilePath) override;
    //~ End EditorAssetFactoryBase Interface
};