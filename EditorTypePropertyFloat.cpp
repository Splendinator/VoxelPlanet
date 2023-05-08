#include "pch.h"

#include "EditorTypePropertyFloat.h"

void EditorTypePropertyFloat::DrawImGUI()
{
	std::string oldValue = std::to_string(value);
	if (ImGui::InputFloat(name.c_str(), &value, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_NoUndoRedo)) // We have our own undo/redo stack
	{
		onPropertyChanged.Invoke({ this, oldValue, std::to_string(value) });
	}
}

EditorTypePropertyBase* EditorTypePropertyFloat::DeepCopy()
{
	return new EditorTypePropertyFloat(name, value);
}

void EditorTypePropertyFloat::ReadFromFile(std::ifstream& file)
{
	// Read in "float damage 2.0f"
	std::string unused;
	file >> unused >> name >> value;
}

void EditorTypePropertyFloat::WriteToFile(std::ofstream& file)
{
	file << "float " << name << " " << value << std::endl;
}

void EditorTypePropertyFloat::ForceSetValue(const std::string& newValue)
{
	value = std::stof(newValue);
}

