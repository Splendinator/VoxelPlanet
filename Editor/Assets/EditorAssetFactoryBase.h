#pragma once

class AssetManager;
class EditorAssetBase;

/** EditorAssetFactoryBase
*
* This class is the base interface for creating different types of assetManager assets from an input file stream.
*/
class EditorAssetFactoryBase
{
public:
	EditorAssetFactoryBase(AssetManager& inAssetManager) : assetManager(inAssetManager) {}
	virtual ~EditorAssetFactoryBase() {}

	virtual std::string GetKeyword() const = 0;
	virtual std::shared_ptr<EditorAssetBase> CreateAsset(const std::filesystem::path& assetFilePath) = 0;

protected:

	AssetManager& assetManager;
};