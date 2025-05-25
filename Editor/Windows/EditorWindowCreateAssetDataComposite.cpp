#include "pch.h"

#include "EditorWindowCreateAssetDataComposite.h"

#include "Editor/Actions/EditorActionCreateAssetDataComposite.h"
#include "Editor/ImGuiEditor.h"

void EditorWindowCreateAssetDataComposite::Draw()
{
    RenderFilePathText();
    RenderAssetNameInput();

    const bool bError = RenderErrorMessageIfOneExists();

    if (!bError)
    {
        if (ImGui::Button("Create Asset"))
        {
            pEditor->DoAction(std::make_unique<EditorActionCreateAssetDataComposite>(targetFilePath, std::string(assetNameBuffer)));
            RequestClose();
        }
    }
}

std::string EditorWindowCreateAssetDataComposite::GetErrorMessage() const
{
    return EditorWindowCreateAssetBase::GetErrorMessage();
}