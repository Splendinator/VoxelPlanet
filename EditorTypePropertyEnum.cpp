#include "pch.h"

#include "EditorTypePropertyEnum.h"

#include "EditorTypeEnum.h"
#include "Game.h"
#include "ImGuiEditor.h"
#include "WritePropertyToFileUtils.h"


void EditorTypePropertyEnum::DrawImGUI()
{
	std::string currentValue = Game::Editor().GetEnumValueNameFromValue(enumName, value);
	if (ImGui::BeginCombo(name.c_str(), currentValue.c_str()))
	{
		EditorTypeEnum* pThisEnum = Game::Editor().FindEnumType(enumName);
		DOMASSERT(pThisEnum);

		auto RenderSelectable = [this, currentValue](const std::string& enumValueName)
		{
			bool bIsSelected = enumValueName == currentValue;
			if (ImGui::Selectable(enumValueName.c_str(), &bIsSelected))
			{
				onPropertyChanged.Invoke({this, currentValue, enumValueName});
			}
		};
		
		for (EnumValueNameToValue& nameToValue : pThisEnum->valueNamesToValues)
		{
			RenderSelectable(nameToValue.valueName);
		}

		ImGui::EndCombo();
	}
}

EditorTypePropertyBase* EditorTypePropertyEnum::DeepCopy()
{
	return new EditorTypePropertyEnum(name, enumName, value);
}

bool EditorTypePropertyEnum::CanReadFromFile(std::ifstream& file) const
{
	// "enum EMyEnum myEnum 4"
	std::string temp;

	file >> temp;
	if (temp != "enum")
	{
		return false;
	}

	file >> temp;
	if (temp != enumName)
	{
		return false;
	}

	file >> temp;
	return temp == name;
}

void EditorTypePropertyEnum::ReadFromFile(std::ifstream& file)
{
	// "enum EMyEnum myEnum 4"
	std::string unused;
	file >> unused >> enumName >> name >> value;

	if (Game::Editor().GetEnumValueNameFromValue(enumName, value) == "")
	{
		DOMLOG_WARN("Can't find enum value", enumName, value, "so setting to default")
		value = Game::Editor().FindEnumType(enumName)->valueNamesToValues[0].value;
	}
}

void EditorTypePropertyEnum::WriteToFile(std::ofstream& file)
{
	// "enum EMyEnum myEnum 4"
	file << "enum " << enumName  << " " << name << " " << value << std::endl;
}

void EditorTypePropertyEnum::ForceSetValue(const std::string& newValue)
{
	value = Game::Editor().GetEnumValueFromValueName(enumName, newValue);
}
