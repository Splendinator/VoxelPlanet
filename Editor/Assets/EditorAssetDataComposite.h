#pragma once

#include "EditorAssetBase.h"

#include "Editor/Types/EditorTypeDataComposite.h"

// Data table asset. This is used to store a map of IDs to values and the value can be of any type.
// An example might be a data composite that contains all of the data for a fireball spell, and it contains "float ExplosionRadius", "int Damage", etc.
class EditorAssetDataComposite : public EditorAssetBase
{
public:

    EditorAssetDataComposite(const std::string& assetName, const std::filesystem::path& inAssetfilePath);

    //~ Begin EditorAssetBase Interface
    virtual void Draw() override;
    virtual void ReadFromFile(std::ifstream& file) override;
    virtual void WriteToFile(std::ofstream& file) override;
    //~ End EditorAssetBase Interface

protected:
    
    EditorTypeDataComposite editorTypeDataComposite;
};