#include "pch.h"

#include "EditorTypeFactoryEnum.h"

#include "EditorTypeEnum.h"
#include "ImGuiEditorGlobals.h"

EditorTypeBase* EditorTypeFactoryEnum::CreateType(std::ifstream& stream)
{
	/**
	* Enum EMyEnum
	* Red 3
	* Blue 2
	*/
	EditorTypeEnum* pEditorTypeEnum = new EditorTypeEnum();
	stream >> pEditorTypeEnum->name;

	std::string enumValueName;
	stream >> enumValueName;

	while(enumValueName != ImGuiEditorGlobals::delimiterString)
	{
		std::string enumValue;
		stream >> enumValue;

		EnumValueNameToValue value;
		value.valueName = enumValueName;
		value.value = std::stoi(enumValue);
		
		pEditorTypeEnum->valueNamesToValues.push_back(value);

		stream >> enumValueName;
	}
	
	return pEditorTypeEnum;
}

std::string EditorTypeFactoryEnum::GetKeyword() const
{
	return "Enum";
}
