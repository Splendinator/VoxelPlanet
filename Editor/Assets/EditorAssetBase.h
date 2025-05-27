#pragma once

class EditorTypeBase;
struct OnPropertyChangedData;

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
	
	void BaseOnAddedToAssetManager() { bIsInAssetManager = true; OnAddedToAssetManager(); }
	void BaseOnRemovedFromAssetManager() { bIsInAssetManager = false; OnRemovedFromAssetManager(); }

	const std::string& GetName() { return name; }
	const std::filesystem::path& GetAssetFilePath() { return assetFilePath; }

	// Sometimes assets can be removed from the asset manager but still exists as a valid shared_ptr (e.g. deleting the file with EditorActionDeleteFile)
	// So this can be used to check if the asset is currently in the asset manager.
	bool IsInAssetManager() const {return bIsInAssetManager; }

	void SaveAsset();
	
	// Fired when a property of this asset changes while editing it.
	DelegateList<const OnPropertyChangedData&> onPropertyChanged;

protected:

	virtual void OnAddedToAssetManager() {};
	virtual void OnRemovedFromAssetManager() {};
	
	DelegateClass<EditorAssetBase, const OnPropertyChangedData&> onPropertyChangedDelegate;

	bool bIsInAssetManager = false;
	
	std::string name; // Name of the asset (e.g for Health.asset this will be "Health")

	std::filesystem::path assetFilePath;

private: 

	void OnPropertyChanged(const OnPropertyChangedData& params);
};
