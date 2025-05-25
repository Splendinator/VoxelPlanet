#pragma once

#include "EditorWindowCreateAssetBase.h"

/** EditorWindowCreateAssetDataComposite
*
* Window to create a new EditorAssetDataComposite
*/
class EditorWindowCreateAssetDataComposite : public EditorWindowCreateAssetBase
{
public:
    EditorWindowCreateAssetDataComposite(const std::filesystem::path& filePath) : EditorWindowCreateAssetBase(filePath) {}

protected:
    //~ Begin EditorWindowBase Interface
    virtual void Draw() override;
    std::string GetWindowName() const override { return "Create Data Composite Asset"; }
    //~ End EditorWindowBase Interface

private:
    //~ Begin EditorWindowCreateAssetBase Interface
    std::string GetErrorMessage() const override;
    //~ End EditorWindowCreateAssetBase Interface
};