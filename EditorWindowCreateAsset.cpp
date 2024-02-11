#include "pch.h"

#include "EditorWindowCreateAsset.h"

#include "EditorActionCreateAsset.h"
#include "ImGuiEditor.h"
#include "ImGuiEditorGlobals.h"

namespace fs = std::filesystem;

void EditorWindowCreateAsset::Draw()
{
	CreateNewAssetWindow();
}

void EditorWindowCreateAsset::CreateNewAssetWindow()
{
	ImGui::Text("%s", targetFilePath.string().c_str());
	
	// Search for the class
	{
		std::vector<std::string> allTypes = pEditor->GetAllClassTypes();

		ImGui::PushItemWidth(-1);
		ImGui::InputTextWithHint("Search", "Search Classes", searchBuffer, sizeof(searchBuffer));
		ImGui::PopItemWidth();

		ImGui::BeginChild("Classes", ImVec2(0, 200), true);
		for (const std::string& type : allTypes)
		{
			if (searchBuffer[0] == '\0' || std::strstr(type.c_str(), searchBuffer) != nullptr)
			{
				bool bSelected = selectedClass == type;
				if (ImGui::Selectable(type.c_str(), bSelected))
				{
					selectedClass = type;
				}
			}
		}
		ImGui::EndChild();
	}

	ImGui::Text("");

	// Add a text input field to specify the asset name
	ImGui::InputText("Asset Name", assetNameBuffer, sizeof(assetNameBuffer));

	// Check whether it's valid
	std::string newAssetName(assetNameBuffer);
	std::string errorMessage = GetErrorMessage(newAssetName);

	// Show a red warning text if there's an error
	if (!errorMessage.empty())
	{
		ImVec4 redColour(1.0f, 0.0f, 0.0f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, redColour);
		ImGui::Text("%s", errorMessage.c_str());
		ImGui::PopStyleColor();
	}
	else if (ImGui::Button("Create Asset")) 
	{
		pEditor->DoAction(std::make_unique<EditorActionCreateAsset>(targetFilePath, selectedClass, newAssetName, *pEditor));
		RequestClose();
	}
}

std::string EditorWindowCreateAsset::GetErrorMessage(const std::string& assetName)
{
	if (selectedClass == "")
	{
		return "No class selected";
	}
	if (assetName.empty())
	{
		return "Invalid asset name";
	}
	if (fs::exists((targetFilePath / assetName).string() + ImGuiEditorGlobals::assetExtension))
	{
		return "Asset already exists";
	}
	return "";
}
