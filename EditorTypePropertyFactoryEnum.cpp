#include "pch.h"

#include "EditorTypePropertyFactoryEnum.h"

#include "AssetManager/AssetManager.h"
#include "EditorTypePropertyEnum.h"
#include "Game.h"

EditorTypePropertyBase* EditorTypePropertyFactoryEnum::CreateType(std::ifstream& stream)
{
	// "enum EMyEnum myEnum EMyEnum::Five"

	std::string enumName;
	std::string propertyName;
	std::string enumValue;

	stream >> enumName >> enumName >> propertyName >> enumValue;

	return new EditorTypePropertyEnum(propertyName, enumName, Game::GetAssetManager().GetEnumValueFromValueName(enumName, enumValue));
}

std::string EditorTypePropertyFactoryEnum::GetLabel()
{
	return "enum";
}
