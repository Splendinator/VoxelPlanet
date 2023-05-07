#include "pch.h"

#include "EditorWindowCreateNewFolder.h"

#include "EditorActionCreateNewFolder.h"
#include "ImGuiEditor.h"

namespace fs = std::filesystem;

void EditorWindowCreateNewFolder::Draw()
{
	ImGui::Begin("Create New Folder");
	CreateNewFolderWindow();
	ImGui::End();
}

void EditorWindowCreateNewFolder::CreateNewFolderWindow()
{
	ImGui::Text("%s", targetFilePath.string().c_str());

	// Add a text input field to specify the folder name
	ImGui::InputText("Folder Name", folderNameBuffer, sizeof(folderNameBuffer));

	// Check whether it's valid
	std::string newFolderName(folderNameBuffer);
	std::string errorMessage = GetErrorMessage(newFolderName);

	// Show a red warning text if there's an error
	if (!errorMessage.empty())
	{
		ImVec4 redColour(1.0f, 0.0f, 0.0f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, redColour);
		ImGui::Text("%s", errorMessage.c_str());
		ImGui::PopStyleColor();
	}
	else if (ImGui::Button("Create New Folder"))
	{
		pEditor->DoAction(std::make_unique<EditorActionCreateNewFolder>(targetFilePath, newFolderName));
		Close();
	}
}

std::string EditorWindowCreateNewFolder::GetErrorMessage(const std::string& fileName)
{
	if (fileName.empty())
	{
		return "Invalid folder name";
	}
	if (fs::exists(targetFilePath / fileName))
	{
		return "Folder already exists";
	}
	return "";
}
