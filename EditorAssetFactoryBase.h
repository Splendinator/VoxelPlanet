#pragma once

class EditorAssetBase;
class ImGuiEditor;

/** EditorAssetFactoryBase
*
* This class is the base interface for creating different types of editor assets from an input file stream.
*/
class EditorAssetFactoryBase
{
public:
	EditorAssetFactoryBase(ImGuiEditor& inEditor) : editor(inEditor) {}
	virtual ~EditorAssetFactoryBase() {}

	virtual std::string GetKeyword() const = 0;
	virtual std::shared_ptr<EditorAssetBase> CreateAsset(const std::filesystem::path& assetFilePath) = 0;

protected:

	ImGuiEditor& editor;
};