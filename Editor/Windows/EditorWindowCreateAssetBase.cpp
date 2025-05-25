#include "pch.h"

#include "EditorWindowCreateAssetBase.h"

#include "Editor/ImGuiEditorGlobals.h"

void EditorWindowCreateAssetBase::RenderFilePathText() const
{
	ImGui::Text("%s", targetFilePath.string().c_str());
}

void EditorWindowCreateAssetBase::RenderAssetNameInput()
{
	ImGui::InputText("Asset Name", assetNameBuffer, sizeof(assetNameBuffer));
}

bool EditorWindowCreateAssetBase::RenderErrorMessageIfOneExists()
{
	std::string errorMessage = GetErrorMessage();

	const bool bError = !errorMessage.empty();
	
	if (bError)
	{
		ImVec4 redColour(1.0f, 0.0f, 0.0f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, redColour);
		ImGui::Text("%s", errorMessage.c_str());
		ImGui::PopStyleColor();
	}

	return bError;
}

std::string EditorWindowCreateAssetBase::GetErrorMessage() const
{
	std::string assetName(assetNameBuffer); 
	
	if (assetName.empty())
	{
		return "Invalid asset name";
	}
	if (std::filesystem::exists((targetFilePath / assetName).string() + ImGuiEditorGlobals::assetExtension))
	{
		return "Asset already exists";
	}
	return "";
}
