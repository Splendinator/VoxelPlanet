#include "pch.h"

#include "EditorTypePropertyString.h"

#include "WritePropertyToFileUtils.h"

void EditorTypePropertyString::DrawImGUI()
{
	constexpr int BUFFER_SIZE = 512;
	char buffer[BUFFER_SIZE];

	strncpy_s(buffer, value.c_str(), sizeof(buffer));
	
	buffer[BUFFER_SIZE - 1] = '\0'; // Ensure null termination in case of overflow

	if (ImGui::InputText(name.c_str(), buffer, BUFFER_SIZE, ImGuiInputTextFlags_NoUndoRedo)) // We have our own undo/redo stack
	{
		std::string oldValue = value;
		value = buffer; // Update the string with new input
		onPropertyChanged.Invoke({ this, oldValue, value });
	}
}

EditorTypePropertyBase* EditorTypePropertyString::DeepCopy()
{
	return new EditorTypePropertyString(name, value);
}

bool EditorTypePropertyString::CanReadFromFile(std::ifstream& file) const
{
	// Read in 'string myString "dog"'
	std::string temp;

	file >> temp;
	if (temp != "string")
	{
		return false;
	}

	file >> temp;
	return temp == name;
}

void EditorTypePropertyString::ReadFromFile(std::ifstream& file)
{
	// Read in 'string myString "dog"'
	std::string unused;
	file >> unused >> name;
	
	std::getline(file, value, '"');		// discard all up to and including the '"'
	std::getline(file, value, '"');		// get everything before the final '"' 
}

void EditorTypePropertyString::WriteToFile(std::ofstream& file)
{
	PropertyFileUtils::WriteStringToFile(file, name, "\"" + value + "\"");
}

void EditorTypePropertyString::ForceSetValue(const std::string& newValue)
{
	value = newValue;
}
