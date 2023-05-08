#pragma once

#include "EditorAssetFactoryBase.h"

/** EditorAssetFactoryClass
*
* This class is used for creating "Object" type assets from an input file stream.
*/
class EditorAssetFactoryClass : public EditorAssetFactoryBase
{
public:
	EditorAssetFactoryClass(ImGuiEditor& inEditor) : EditorAssetFactoryBase(inEditor) {}

	//~ Begin EditorAssetFactoryBase Interface
	virtual std::string GetKeyword() const override;
	virtual std::shared_ptr<EditorAssetBase> CreateAsset(const std::filesystem::path& assetFilePath) override;
	//~ End EditorAssetFactoryBase Interface
};
