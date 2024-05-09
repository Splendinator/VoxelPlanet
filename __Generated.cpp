#include "pch.h"
#include "__Generated.h"
#include "EditorTypePropertyClass.h"
#include "EditorTypePropertyFloat.h"
#include "EditorTypePropertyInt.h"
#include "EditorTypePropertyBool.h"
#include "EditorTypePropertyString.h"
#include "EditorTypePropertyStruct.h"
#include "EditorTypePropertyVector.h"
#include "EditorTypePropertyEnum.h"
#include "..\Roguelike\DirectoryData.h"
#include "..\Roguelike\Game.h"
#include "..\Roguelike\TextRenderSystem\TextRenderSystem.h"

// TextRenderCharacterData
void TextRenderCharacterData::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	TextRenderCharacterData* pTextRenderCharacterData = static_cast<TextRenderCharacterData*>(pObject);
	pTextRenderCharacterData->fileName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pTextRenderCharacterData->character = static_cast<EditorTypePropertyInt*>(properties[propertyIndex++])->GetValue();
}

void* TextRenderCharacterData::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	TextRenderCharacterData* pTextRenderCharacterData = new TextRenderCharacterData;
	int propertyIndex = 0;
	TextRenderCharacterData::InitFromPropertiesSubset(pTextRenderCharacterData, properties, propertyIndex);
	return pTextRenderCharacterData;
}

// TextboxParams
void TextboxParams::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	TextboxParams* pTextboxParams = static_cast<TextboxParams*>(pObject);
	pTextboxParams->text = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pTextboxParams->xPos = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pTextboxParams->yPos = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pTextboxParams->fontSize = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
}

void* TextboxParams::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	TextboxParams* pTextboxParams = new TextboxParams;
	int propertyIndex = 0;
	TextboxParams::InitFromPropertiesSubset(pTextboxParams, properties, propertyIndex);
	return pTextboxParams;
}

// MyStruct
void MyStruct::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	MyStruct* pMyStruct = static_cast<MyStruct*>(pObject);
	pMyStruct->x = static_cast<EditorTypePropertyInt*>(properties[propertyIndex++])->GetValue();
}

void* MyStruct::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	MyStruct* pMyStruct = new MyStruct;
	int propertyIndex = 0;
	MyStruct::InitFromPropertiesSubset(pMyStruct, properties, propertyIndex);
	return pMyStruct;
}

// DirectoryData
void DirectoryData::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	DirectoryData* pDirectoryData = static_cast<DirectoryData*>(pObject);
	pDirectoryData->fonts = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
}

void* DirectoryData::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	DirectoryData* pDirectoryData = new DirectoryData;
	int propertyIndex = 0;
	DirectoryData::InitFromPropertiesSubset(pDirectoryData, properties, propertyIndex);
	return pDirectoryData;
}

// TextRenderSystem
void TextRenderSystem::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	TextRenderSystem* pTextRenderSystem = static_cast<TextRenderSystem*>(pObject);
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pTextRenderSystem->characterDatas.push_back(*static_cast<TextRenderCharacterData*>(static_cast<EditorTypePropertyStruct*>(instancedProperty.get())->GetValue()));
		}
	}
}

void* TextRenderSystem::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	TextRenderSystem* pTextRenderSystem = new TextRenderSystem;
	int propertyIndex = 0;
	TextRenderSystem::InitFromPropertiesSubset(pTextRenderSystem, properties, propertyIndex);
	return pTextRenderSystem;
}

// GameAssets
void GameAssets::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	GameAssets* pGameAssets = static_cast<GameAssets*>(pObject);
	pGameAssets->pTextRenderSystem = static_cast<TextRenderSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pGameAssets->pDirectoryData = static_cast<DirectoryData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pGameAssets->myEnum = static_cast<EMyEnum>(static_cast<EditorTypePropertyEnum*>(properties[propertyIndex++])->GetValue());
}

void* GameAssets::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	GameAssets* pGameAssets = new GameAssets;
	int propertyIndex = 0;
	GameAssets::InitFromPropertiesSubset(pGameAssets, properties, propertyIndex);
	return pGameAssets;
}

namespace __Generated
{
	std::unordered_map<std::string, void* (*)(const std::vector<EditorTypePropertyBase*>&)> stringToCreateObjectFunction
	{
		{"TextRenderCharacterData", &TextRenderCharacterData::InitFromProperties},
		{"TextboxParams", &TextboxParams::InitFromProperties},
		{"MyStruct", &MyStruct::InitFromProperties},
		{"DirectoryData", &DirectoryData::InitFromProperties},
		{"TextRenderSystem", &TextRenderSystem::InitFromProperties},
		{"GameAssets", &GameAssets::InitFromProperties},
	};
}
