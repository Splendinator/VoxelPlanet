#include "pch.h"

#include "EditorTypePropertyClass.h"

#include "EditorAssetBase.h"
#include "Game.h"
#include "ImGuiEditor.h"

#include "WritePropertyToFileUtils.h"

void EditorTypePropertyClass::DrawImGUI()
{
	if (ImGui::BeginCombo(name.c_str(), assetName.c_str()))
	{
		std::vector<std::weak_ptr<EditorAssetBase>> validAssets = Game::Editor().GatherAssetsOfClass(className);

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
			std::string selectableAssetName = validAsset.lock().get()->GetName().c_str();
			RenderSelectable(selectableAssetName);
		}

		ImGui::EndCombo();
	}
}

EditorTypePropertyBase* EditorTypePropertyClass::DeepCopy()
{
	return new EditorTypePropertyClass(name, className, assetName);
}

bool EditorTypePropertyClass::CanReadFromFile(std::ifstream& file) const
{
	std::string temp;

	file >> temp;
	if (temp != "class")
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

void EditorTypePropertyClass::ReadFromFile(std::ifstream& file)
{
	// Read in "class SingleFloat pNext nullptr"
	
	std::string unused;
	file >> unused >> className >> name >> unused;

	assetName = unused;
}

void EditorTypePropertyClass::WriteToFile(std::ofstream& file)
{
	PropertyFileUtils::WriteClassToFile(file, name, className, assetName);
}

void EditorTypePropertyClass::ForceSetValue(const std::string& newValue)
{
	assetName = newValue;
}

void* EditorTypePropertyClass::GetValue() const
{
	if (assetName != "nullptr" && assetName != "")
	{
		return Game::Editor().FindObjectFromAsset<void>(assetName);
	}
	return nullptr;
}
