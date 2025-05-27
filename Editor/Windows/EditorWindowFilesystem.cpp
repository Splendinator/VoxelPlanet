#include "pch.h"

#include "EditorWindowFilesystem.h"

#include "AssetManager/AssetManager.h"
#include "Editor/Actions/EditorActionDeleteFile.h"
#include "Editor/ImGuiEditor.h"
#include "Editor/ImGuiEditorGlobals.h"
#include "EditorWindowChooseAssetType.h"
#include "EditorWindowCreateNewFolder.h"
#include "EditorWindowEditAsset.h"
#include "Game.h"

#include <filesystem>

namespace fs = std::filesystem;

void EditorWindowFilesystem::Draw()
{
	if (ImGui::Button("Reload Game"))
	{
		Game::RestartGame();
	}

	ImGui::Separator();

	const bool bSearching = DrawSearchBar();

	if (!bSearching)
	{
		
		
		DrawDirectory(rootDirectory);

		openFileLocationRequest = std::filesystem::directory_entry(); 
	}
}

void EditorWindowFilesystem::DrawDirectory(const std::filesystem::path& path)
{
	std::string dirName = path.filename().string();

	ImVec4 folderColour(0.7f, 0.7f, 0.7f, 1.0f);
	ImGui::PushStyleColor(ImGuiCol_Text, folderColour);

	if (openFileLocationRequest.exists())
	{
		const bool bShouldBeOpen = std::strstr(openFileLocationRequest.path().string().c_str(), path.string().c_str()) != nullptr; 
		ImGui::SetNextItemOpen(bShouldBeOpen);
	}
	
	if (ImGui::TreeNode(dirName.c_str()))
	{
		ImGui::PopStyleColor();

		ImGui::SameLine();
		if(ImGui::Button("Create Folder"))
		{
			pEditor->AddWindow(std::make_unique<EditorWindowCreateNewFolder>(path));
		}
		ImGui::SameLine();
		if (ImGui::Button("Create Asset"))
		{
			pEditor->AddWindow(std::make_unique<EditorWindowChooseAssetType>(path));
		}
		ImGui::SameLine();
		if (ImGui::Button("Open in Explorer"))
		{
			system(("explorer " + path.string()).c_str());
		}
		// #NOTE: We purposely don't have a delete folder button so you don't delete half the game's assets on accident. Just use file explorer for that shit

		// Draw directories first
		for (const auto& entry : fs::directory_iterator(path))
		{
			if (entry.is_directory())
			{
				DrawDirectory(entry.path());
			}
		}

		// Draw assets under
		for (const auto& entry : fs::directory_iterator(path))
		{
			if (entry.is_regular_file())
			{
				DrawAsset(entry, false);
			}
		}
		
		ImGui::TreePop();

		ImGui::Separator();
	}
	else
	{
		ImGui::PopStyleColor();
	}
}

bool EditorWindowFilesystem::DrawSearchBar()
{
	ImGui::PushItemWidth(-1);
	ImGui::InputTextWithHint("Search", "Search Assets", searchBuffer, sizeof(searchBuffer));
	ImGui::PopItemWidth();

	if (searchBuffer[0] == '\0')
	{
		return false;
	}

	// #NOTE: If this window starts lagging we need to cache the searchable assets once at the start, then when adding/removing assets.
	searchableAssets.clear();
	RefreshSearchableAssets(rootDirectory);
	
	char lowerCaseSearchBuffer[128] = {};
	memcpy(lowerCaseSearchBuffer, searchBuffer, sizeof(searchBuffer));
	dmut::ToLowerInline(&lowerCaseSearchBuffer[0]);
	
	for (const SearchableAssetData& searchableAsset : searchableAssets)
	{
		if (std::strstr(dmut::ToLower(searchableAsset.name).c_str(), lowerCaseSearchBuffer) != nullptr)
		{
			DrawAsset(searchableAsset.directoryEntry, true);
		}
	}

	return true;
}

void EditorWindowFilesystem::DrawAsset(const std::filesystem::directory_entry& entry, bool bShowOpenFileLocation)
{
	const std::string extension = entry.path().extension().string();
	if (extension == ImGuiEditorGlobals::assetExtension)
	{
		const std::string fileName = entry.path().filename().string();
		const std::string assetName = fileName.substr(0, fileName.size() - ImGuiEditorGlobals::assetExtension.size());
		
		ImGui::PushID(fileName.c_str());
		ImGui::Text("%s", fileName.c_str());
		ImGui::SameLine();
		
		if (ImGui::Button("Edit"))
		{
			std::weak_ptr<EditorAssetBase> pAsset = Game::GetAssetManager().FindAsset(assetName);
			if (!pAsset.expired())
			{
				pEditor->AddWindow(std::make_unique<EditorWindowEditAsset>(pAsset, entry.path()));
			}
			else
			{
				DOMLOG_ERROR("Asset not found / Asset file empty", assetName);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			pEditor->DoAction(std::make_shared<EditorActionDeleteFile>(entry.path(), assetName));
		}

		if (bShowOpenFileLocation)
		{
			ImGui::SameLine();
			if (ImGui::Button("Open file location"))
			{
				searchBuffer[0] = '\0';
				openFileLocationRequest = entry;
			}
		}
		
		ImGui::PopID();
	}
}

void EditorWindowFilesystem::RefreshSearchableAssets(const std::filesystem::path& path)
{
	for (const auto& entry : fs::directory_iterator(path))
	{
		if (entry.is_directory())
		{
			RefreshSearchableAssets(entry.path());
		}
		else if (entry.is_regular_file())
		{
			const std::string extension = entry.path().extension().string();
			if (extension == ImGuiEditorGlobals::assetExtension)
			{
				const std::string fileName = entry.path().filename().string();
				const std::string assetName = fileName.substr(0, fileName.size() - ImGuiEditorGlobals::assetExtension.size());
				searchableAssets.push_back({assetName, entry});
			}
		}
	}
}
