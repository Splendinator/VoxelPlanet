#include "pch.h"

#include "EditorWindowFilesystem.h"

#include "EditorWindowCreateAsset.h"
#include "EditorWindowCreateNewFolder.h"
#include "EditorWindowEditAsset.h"
#include "ImGuiEditor.h"
#include "ImGuiEditorGlobals.h"

#include <filesystem>

namespace fs = std::filesystem;

void EditorWindowFilesystem::Draw()
{
	DrawDirectory(rootDirectory);
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
					std::string assetName = entry.path().filename().string();
					ImGui::PushID(assetName.c_str());
					ImGui::Text("%s", assetName.c_str());
					ImGui::SameLine();
					if (ImGui::Button("Edit"))
					{
						assetName = assetName.substr(0, assetName.size() - ImGuiEditorGlobals::assetExtension.size()); // Get rid of extension
						std::weak_ptr<EditorAssetBase> pAsset = pEditor->FindAsset(assetName);
						if (!pAsset.expired())
						{
							pEditor->AddWindow(std::make_unique<EditorWindowEditAsset>(pAsset, entry.path()));
						}
						else
						{
							DOMLOG_ERROR("Asset not found / Asset file empty", assetName);
						}
					}
					ImGui::PopID();
				}
			}
			else if (entry.is_directory())
			{
				DrawDirectory(entry.path());
			}
		}
		
		if(ImGui::Button("Create Folder"))
		{
			pEditor->AddWindow(std::make_unique<EditorWindowCreateNewFolder>(path));
		}
		ImGui::SameLine();
		if (ImGui::Button("Create Asset"))
		{
			pEditor->AddWindow(std::make_unique<EditorWindowCreateAsset>(path));
		}
		
		ImGui::TreePop();
	}
}
