#include "pch.h"

#include "EditorTypePropertyInstancedAssetPtr.h"

#include "AssetManager/AssetManager.h"
#include "Editor/Assets/EditorAssetBase.h"
#include "Game.h"
#include "WritePropertyToFileUtils.h"

void EditorTypePropertyInstancedAssetPtr::DrawImGUI()
{
	if (ImGui::BeginCombo(name.c_str(), assetName.c_str()))
	{
		std::vector<std::weak_ptr<EditorAssetClass>> validAssets = Game::GetAssetManager().GatherAssetsOfClass(className, true, EClassMetadataFlags::Instanced);

		auto RenderSelectable = [this](const std::string& selectableAssetName)
		{
			bool bIsSelected = selectableAssetName == assetName;
			if (ImGui::Selectable(selectableAssetName.c_str(), &bIsSelected))
			{
				onPropertyChanged.Invoke({this, assetName, selectableAssetName});
			}
		};

		RenderSelectable("nullptr");
		for (auto validAsset : validAssets)
		{
			std::string selectableAssetName = validAsset.lock().get()->GetName();
			RenderSelectable(selectableAssetName);
		}

		ImGui::EndCombo();
	}
}

EditorTypePropertyBase* EditorTypePropertyInstancedAssetPtr::DeepCopy()
{
	return new EditorTypePropertyInstancedAssetPtr(name, className, assetName);
}

bool EditorTypePropertyInstancedAssetPtr::CanReadFromFile(std::ifstream& file) const
{
	std::string temp;

	file >> temp;
	if (temp != "instancedassetptr")
	{
		return false;
	}

	file >> temp;
	if (temp != className)
	{
		return false;
	}

	file >> temp;
	return temp == name;
}

void EditorTypePropertyInstancedAssetPtr::ReadFromFile(std::ifstream& file)
{
	// Read in "instancedassetptr MenuScreenBase pBaseMenuScreen PauseMenu"
	
	std::string unused;
	file >> unused >> className >> name >> assetName;
}

void EditorTypePropertyInstancedAssetPtr::WriteToFile(std::ofstream& file)
{
	PropertyFileUtils::WriteInstancedAssetPtrToFile(file, name, className, assetName);
}

void EditorTypePropertyInstancedAssetPtr::ForceSetValue(const ForceSetValueParams& params)
{
	assetName = params.newValue;
}

const std::string& EditorTypePropertyInstancedAssetPtr::GetValue() const
{
	return assetName;
}
