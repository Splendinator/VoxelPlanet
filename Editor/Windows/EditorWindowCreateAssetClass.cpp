#include "pch.h"

#include "EditorWindowCreateAssetClass.h"

#include "AssetManager/AssetManager.h"
#include "Editor/Actions/EditorActionCreateAssetClass.h"
#include "Game.h"
#include "Editor/ImGuiEditor.h"

namespace fs = std::filesystem;

void EditorWindowCreateAssetClass::Open()
{
	searchBar.SetItems(Game::GetAssetManager().GetAllClassTemplateNames(/*bIgnoreAbstract=*/true));
}

void EditorWindowCreateAssetClass::Draw()
{
	RenderFilePathText();
	
	searchBar.Draw();
	selectedClass = searchBar.GetSelectedItem();
	
	ImGui::Text("");
	
	RenderAssetNameInput();

	const bool bError = RenderErrorMessageIfOneExists();

	if (!bError)
	{
		if (ImGui::Button("Create Asset")) 
		{
			pEditor->DoAction(std::make_unique<EditorActionCreateAssetClass>(targetFilePath, selectedClass, std::string(assetNameBuffer)));
			RequestClose();
		}
	}
}

std::string EditorWindowCreateAssetClass::GetErrorMessage() const
{
	if (selectedClass == "")
	{
		return "No class selected";
	}
	
	return EditorWindowCreateAssetBase::GetErrorMessage();
}
