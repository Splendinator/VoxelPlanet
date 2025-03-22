#include "pch.h"

#include "EditorTypePropertyEnum.h"

#include "AssetManager/AssetManager.h"
#include "EditorTypeEnum.h"
#include "Game.h"
#include "WritePropertyToFileUtils.h"

void EditorTypePropertyEnum::DrawImGUI()
{
	std::string currentValue = Game::GetAssetManager().GetEnumValueNameFromValue(enumName, value);
	if (ImGui::BeginCombo(name.c_str(), currentValue.c_str()))
	{
		EditorTypeEnum* pThisEnum = Game::GetAssetManager().FindEnumType(enumName);
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
	// "enum EColour myColour Red"
	std::string unused;
	file >> unused >> enumName >> name >> valueName;
	
	if (valueName == "")
	{
		DOMLOG_WARN("Can't find enum value", enumName, valueName, "so setting to default")
		valueName = Game::GetAssetManager().FindEnumType(enumName)->valueNamesToValues[0].valueName;
	}

	value = Game::GetAssetManager().GetEnumValueFromValueName(enumName, valueName);
}

void EditorTypePropertyEnum::WriteToFile(std::ofstream& file)
{
	// "enum EColour myColour Red"
	file << "enum " << enumName  << " " << name << " " << valueName << std::endl;
}

void EditorTypePropertyEnum::ForceSetValue(const ForceSetValueParams& params)
{
	valueName = params.newValue;
	value = Game::GetAssetManager().GetEnumValueFromValueName(enumName, params.newValue);
}
