#include "pch.h"

#include "EditorWindowFilesystem.h"

#include "EditorWindowCreateNewFolder.h"
#include "ImGuiEditor.h"
#include "ImGuiEditorGlobals.h"

#include <filesystem>

namespace fs = std::filesystem;

void EditorWindowFilesystem::Draw()
{
	ImGui::SetWindowPos({ 0,0 }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowSize({ 400,400 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Editor");

	DrawDirectory(rootDirectory);

	ImGui::End();
}

void EditorWindowFilesystem::DrawDirectory(const std::filesystem::path& path)
{
	std::string dirName = path.filename().string();
	if (ImGui::TreeNode(dirName.c_str()))
	{
		for (const auto& entry : fs::directory_iterator(path))
		{
			if (entry.is_regular_file())
			{
				const std::string extension = entry.path().extension().string();
				if (extension == ImGuiEditorGlobals::assetExtension)
				{
					/// #TEMP: Draw Asset
				}
			}
			else if (entry.is_directory())
			{
				DrawDirectory(entry.path());
			}
		}
		
		if(ImGui::Button("Create New Folder"))
		{
			pEditor->AddWindow(std::make_unique<EditorWindowCreateNewFolder>(path));
		}
		
		ImGui::TreePop();
	}
}
