#include "pch.h"

#include "EditorTypePropertyClass.h"

#include "Game.h"
#include "ImGuiEditor.h"

#include "WritePropertyToFileUtils.h"

void EditorTypePropertyClass::DrawImGUI()
{
	static char inputBuffer[128];
	if (ImGui::InputText("AssetName", &inputBuffer[0], 128, ImGuiInputTextFlags_NoUndoRedo))
	{
		onPropertyChanged.Invoke({ this, assetName, inputBuffer});	
	}
}

EditorTypePropertyBase* EditorTypePropertyClass::DeepCopy()
{
	return new EditorTypePropertyClass(name, className, assetName);
}

void EditorTypePropertyClass::ReadFromFile(std::ifstream& file)
{
	// Read in "object SingleFloat pNext nullptr"
	
	std::string unused;
	file >> unused >> className >> name >> unused;

	if (unused != "nullptr")
	{
		assetName = unused;
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
		return Game::Editor().FindObject<void>(assetName);
	}
	return nullptr;
}
