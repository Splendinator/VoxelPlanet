#include "pch.h"

#include "EditorTypePropertyBool.h"

#include "WritePropertyToFileUtils.h"

void EditorTypePropertyBool::DrawImGUI()
{
	std::string oldValue = value ? "true" : "false";
	if (ImGui::Checkbox(name.c_str(), &value)) // Using Checkbox for boolean input
	{
		std::string newValue = value ? "true" : "false";
		onPropertyChanged.Invoke({ this, oldValue, newValue });
	}
}

EditorTypePropertyBase* EditorTypePropertyBool::DeepCopy()
{
	return new EditorTypePropertyBool(name, value);
}

bool EditorTypePropertyBool::CanReadFromFile(std::ifstream& file) const
{
	// Read in "bool bBool true"
	std::string temp;

	file >> temp;
	if (temp != "bool")
	{
		return false;
	}

	file >> temp;
	return temp == name;
}

void EditorTypePropertyBool::ReadFromFile(std::ifstream& file)
{
	// Read in "bool bBool true"
	std::string unused;
	std::string boolValue;
	file >> unused >> name >> boolValue;
	value = (boolValue == "true");
}

void EditorTypePropertyBool::WriteToFile(std::ofstream& file)
{
	PropertyFileUtils::WriteBoolToFile(file, name, value);
}

void EditorTypePropertyBool::ForceSetValue(const std::string& newValue)
{
	value = (newValue == "true");
}
