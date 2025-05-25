#include "pch.h"

#include "EditorWindowChooseAssetType.h"

#include "Editor/ImGuiEditor.h"
#include "EditorWindowCreateAssetClass.h"
#include "EditorWindowCreateAssetDataComposite.h"

template <typename TEditorWindowClass>
void EditorWindowChooseAssetType::DrawAssetTypeButton(const std::string& buttonText)
{
	if (ImGui::Button(buttonText.c_str()))
	{
		pEditor->AddWindow(std::make_unique<TEditorWindowClass>(filePath));
		RequestClose();
	}
}

void EditorWindowChooseAssetType::Draw()
{
	DrawAssetTypeButton<EditorWindowCreateAssetClass>("Class");
	DrawAssetTypeButton<EditorWindowCreateAssetDataComposite>("Data Composite");
}
