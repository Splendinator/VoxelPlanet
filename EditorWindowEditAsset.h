#pragma once

#include "EditorWindowBase.h"

class EditorAssetBase;
struct OnPropertyChangedParams;

/** EditorWindowEditAsset
*
* This class is used for rendering an editor window to edit an asset.
*/
class EditorWindowEditAsset : public EditorWindowBase
{
public:
	EditorWindowEditAsset(std::weak_ptr<EditorAssetBase>& pInAsset, const std::filesystem::path& inAssetPath) : pAsset(pInAsset), assetPath(inAssetPath) {}

protected:
	//~ Begin EditorWindowBase Interface
	virtual void Open() override;
	virtual void Close() override;
	virtual void Draw() override;
	std::string GetWindowName() const override { return "Edit Asset"; }
	//~ End EditorWindowBase Interface

private:

	void OnPropertyChanged(const OnPropertyChangedParams& params);

	DelegateClass<EditorWindowEditAsset, const OnPropertyChangedParams&> onPropertyChangedDelegate;
	
	std::weak_ptr<EditorAssetBase> pAsset;

	std::filesystem::path assetPath;
};