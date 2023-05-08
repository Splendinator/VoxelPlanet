#include "pch.h"

#include "EditorWindowEditAsset.h"

#include "EditorActionChangeProperty.h"
#include "EditorAssetBase.h"
#include "EditorTypePropertyBase.h"
#include "ImGuiEditor.h"

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
	if (pAsset.expired())
	{
		// In case of deletion mid-edit
		RequestClose();
	}
	else
	{
		pAsset.lock()->Draw();
	}
}

void EditorWindowEditAsset::OnPropertyChanged(const OnPropertyChangedParams& params)
{
	pEditor->DoAction(std::make_shared<EditorActionChangeProperty>(params.pProperty, pAsset, params.oldValue, params.newValue, assetPath));
}

