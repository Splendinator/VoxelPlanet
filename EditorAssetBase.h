#pragma once

class EditorTypeBase;
struct OnPropertyChangedParams;

/** EditorAssetBase
*
* This class is the base class for all editor assets (Classes, Textures, Models, etc.). Assets are usually instances of types
*/
class EditorAssetBase
{
public:
	EditorAssetBase(const std::string& assetName, const std::filesystem::path& inAssetfilePath) : name(assetName), assetFilePath(inAssetfilePath) { onPropertyChangedDelegate.Bind(this, &EditorAssetBase::OnPropertyChanged); }
	virtual ~EditorAssetBase() {};

	// Draw the asset using ImGUI
	virtual void Draw() = 0;

	// Read and write to a .asset file
	virtual void ReadFromFile(std::ifstream& file) = 0;
	virtual void WriteToFile(std::ofstream& file) = 0;

	const std::string& GetName() { return name; }
	const std::filesystem::path& GetAssetFilePath() { return assetFilePath; }

	virtual EditorTypeBase* GetEditorType() const = 0;

	// Fired when a property of this asset changes while editing it.
	DelegateList<const OnPropertyChangedParams&> onPropertyChanged;

protected:
	DelegateClass<EditorAssetBase, const OnPropertyChangedParams&> onPropertyChangedDelegate;

	std::string name; // Name of the asset (e.g for Health.asset this will be "Health")

	std::filesystem::path assetFilePath;

private: 

	void OnPropertyChanged(const OnPropertyChangedParams& params);
};
