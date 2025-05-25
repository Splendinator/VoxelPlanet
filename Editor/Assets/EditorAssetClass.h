#pragma once

#include "EditorAssetBase.h"

class EditorTypeClass;
class EditorTypePropertyBase;

/** EditorAssetClass
*
* This class is used for handling assets that represent a single EDITORCLASS() and lets you change the EDITORPROPERTIES() 
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
	//~ End EditorAssetBase Interface

	EditorTypeClass* GetClassEditorType() const { return pEditorTypeInstance; }
	
	const std::vector<EditorTypePropertyBase*>& GetProperties();

private:

	// Deep copy of the type, feel free to change this and do whatever
	EditorTypeClass* pEditorTypeInstance;
};