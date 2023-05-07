#pragma once

#include "EditorAssetBase.h"

class EditorTypeBase;

/** EditorAssetClass
*
* This class is used for handling assets that have an associated EditorTypeClass.
*/
class EditorAssetClass : public EditorAssetBase
{
public:
	EditorAssetClass(const std::string& assetName, EditorTypeBase* pEditorType);
	virtual ~EditorAssetClass();

	//~ Begin EditorAssetBase Interface
	virtual void Draw() override;
	//~ End EditorAssetBase Interface

	// Unique label for this asset type. Used for identifying the asset type in the file.
	std::string GetLabel();

private:

	// Deep copy of the type, feel free to change this and do whatever
	EditorTypeBase* pEditorTypeInstance;
};