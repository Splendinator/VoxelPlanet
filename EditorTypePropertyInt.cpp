#include "pch.h"

#include "EditorTypePropertyInt.h"

#include "WritePropertyToFileUtils.h"

void EditorTypePropertyInt::DrawImGUI()
{
	std::string oldValue = std::to_string(value);
	if (ImGui::InputInt(name.c_str(), &value, 0, 0, ImGuiInputTextFlags_NoUndoRedo)) // We have our own undo/redo stack{
	{
		onPropertyChanged.Invoke({ this, oldValue, std::to_string(value) });
	}
}

EditorTypePropertyBase* EditorTypePropertyInt::DeepCopy()
{
	return new EditorTypePropertyInt(name, value);
}

bool EditorTypePropertyInt::CanReadFromFile(std::ifstream& file) const
{
	// Read in "int myInt 2"
	std::string temp;

	file >> temp;
	if (temp != "int")
	{
		return false;
	}

	file >> temp;
	return temp == name;
}

void EditorTypePropertyInt::ReadFromFile(std::ifstream& file)
{
	// Read in "int myInt 2"
	std::string unused;
	file >> unused >> name >> value;
}

void EditorTypePropertyInt::WriteToFile(std::ofstream& file)
{
	PropertyFileUtils::WriteIntToFile(file, name, value);
}

void EditorTypePropertyInt::ForceSetValue(const std::string& newValue)
{
	value = std::stoi(newValue);
}
