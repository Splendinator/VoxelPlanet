#pragma once

#include "EditorAssetBase.h"

class EditorTypeClass;
class EditorTypePropertyBase;

// #TODO: Do we even have multiple asset types? If not combine this with EditorAssetBase

/** EditorAssetClass
*
* This class is used for handling assets that have an associated EditorTypeClass.
*/
class EditorAssetClass : public EditorAssetBase
{
public:
	EditorAssetClass(const std::string& assetName, EditorTypeClass* pEditorType, const std::filesystem::path& assetFilePath);
	virtual ~EditorAssetClass();

	//~ Begin EditorAssetBase Interface
	virtual void Draw() override;
	virtual void ReadFromFile(std::ifstream& file) override;
	virtual void WriteToFile(std::ofstream& file) override;
	virtual EditorTypeBase* GetEditorType() const;
	//~ End EditorAssetBase Interface

	const std::vector<EditorTypePropertyBase*>& GetProperties();
	
	// Unique label for this asset type. Used for identifying the asset type in the file.
	std::string GetLabel();

private:

	// Deep copy of the type, feel free to change this and do whatever
	EditorTypeClass* pEditorTypeInstance;
};