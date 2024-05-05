#include "pch.h"

#include "EditorTypePropertyClass.h"

#include "Game.h"
#include "ImGuiEditor.h"

#include "WritePropertyToFileUtils.h"

void EditorTypePropertyClass::DrawImGUI()
{
	if (ImGui::InputText(name.c_str(), &inputBuffer[0], 128, ImGuiInputTextFlags_NoUndoRedo))
	{
		onPropertyChanged.Invoke({ this, assetName, inputBuffer});	
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

	if (unused != "nullptr")
	{
		assetName = unused;
		memcpy(inputBuffer, assetName.c_str(), assetName.size()); // Do we need a SetAssetName() that does this?
		
	}
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
	if (assetName != "")
	{
		return Game::Editor().FindObjectFromAsset<void>(assetName);
	}
	return nullptr;
}
