#pragma once

#include "Editor/Types/EditorTypePropertiesBase.h"

/** EditorTypeDataComposite
*
* This class is ***NOT*** used like other data types.
* It's just used internally in EditorAssetDataComposite because a lot of the functionality is useful.
*/
class EditorTypeDataComposite : public EditorTypePropertiesBase
{
public:
    EditorTypeDataComposite() = default;

    //~ Begin EditorTypeBase Interface
    void DrawImGUI() override;
    EditorTypeBase* DeepCopy() override;
    virtual void ReadFromFile(std::ifstream& file);
    virtual void WriteToFile(std::ofstream& file);
    //~ End EditorTypeBase Interface
};