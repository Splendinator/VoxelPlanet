#pragma once

#include "Editor/Types/EditorTypePropertiesBase.h"

class EditorAssetDataComposite;

/** EditorTypeDataComposite
*
* This class is ***NOT*** used like other data types.
* It's just used internally in EditorAssetDataComposite because a lot of the functionality is useful.
*/
class EditorTypeDataComposite : public EditorTypePropertiesBase
{
public:
    EditorTypeDataComposite() = default;

	void SetOwningAsset(std::weak_ptr<EditorAssetDataComposite> pAsset) { pOwningAsset = pAsset; }
	void ResetOwningAsset() { pOwningAsset.reset(); }
	
    //~ Begin EditorTypeBase Interface
    void DrawImGUI() override;
    EditorTypeBase* DeepCopy() override;
    virtual void ReadFromFile(std::ifstream& file);
    virtual void WriteToFile(std::ofstream& file);
    //~ End EditorTypeBase Interface

private:

	std::weak_ptr<EditorAssetDataComposite> pOwningAsset;
};