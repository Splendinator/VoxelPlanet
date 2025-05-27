#include "pch.h"

#include "EditorWindowEditAsset.h"

#include "Editor/Actions/EditorActionChangeProperty.h"
#include "Editor/Assets/EditorAssetBase.h"
#include "Editor/ImGuiEditor.h"

void EditorWindowEditAsset::Open()
{
	std::shared_ptr<EditorAssetBase> pAssetPtr = pAsset.lock();
	pAssetPtr->onPropertyChanged.Add(onPropertyChangedDelegate);
	onPropertyChangedDelegate.Bind(this, &EditorWindowEditAsset::OnPropertyChanged);
}

void EditorWindowEditAsset::Close()
{
	if (!pAsset.expired())
	{
		std::shared_ptr<EditorAssetBase> pAssetPtr = pAsset.lock();
		pAssetPtr->onPropertyChanged.Remove(onPropertyChangedDelegate);
	}
}

void EditorWindowEditAsset::Draw()
{
	if (pAsset.expired() || !pAsset.lock()->IsInAssetManager())
	{
		// In case of deletion mid-edit
		RequestClose();
	}
	else
	{
		if (ImGui::Button("Open File in Explorer"))
		{
			system(("explorer " + pAsset.lock()->GetAssetFilePath().string()).c_str());
		}

		pAsset.lock()->Draw();
	}
}

void EditorWindowEditAsset::OnPropertyChanged(const OnPropertyChangedData& params)
{
	pEditor->DoAction(std::make_shared<EditorActionChangeProperty>(pAsset, params, assetPath));
}

