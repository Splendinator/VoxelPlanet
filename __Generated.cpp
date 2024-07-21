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
#include "..\Roguelike\HUD.h"
#include "..\Roguelike\HUDAnchorPoint.h"
#include "..\Roguelike\HUDObjectBase.h"
#include "..\Roguelike\HUDObjectHealth.h"
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

// HUDObjectSharedInitParams
void HUDObjectSharedInitParams::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	HUDObjectSharedInitParams* pHUDObjectSharedInitParams = static_cast<HUDObjectSharedInitParams*>(pObject);
	pHUDObjectSharedInitParams->screenEdgePadding = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
}

void* HUDObjectSharedInitParams::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	HUDObjectSharedInitParams* pHUDObjectSharedInitParams = new HUDObjectSharedInitParams;
	int propertyIndex = 0;
	HUDObjectSharedInitParams::InitFromPropertiesSubset(pHUDObjectSharedInitParams, properties, propertyIndex);
	return pHUDObjectSharedInitParams;
}

// GameAssets
void GameAssets::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	GameAssets* pGameAssets = static_cast<GameAssets*>(pObject);
	pGameAssets->pTextRenderSystem = static_cast<TextRenderSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pGameAssets->pDirectoryData = static_cast<DirectoryData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pGameAssets->pHUD = static_cast<HUD*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* GameAssets::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	GameAssets* pGameAssets = new GameAssets;
	int propertyIndex = 0;
	GameAssets::InitFromPropertiesSubset(pGameAssets, properties, propertyIndex);
	return pGameAssets;
}

// DirectoryData
void DirectoryData::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	DirectoryData* pDirectoryData = static_cast<DirectoryData*>(pObject);
	pDirectoryData->fonts = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pDirectoryData->hudObjects = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
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

// HUDAnchorPoint
void HUDAnchorPoint::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	HUDAnchorPoint* pHUDAnchorPoint = static_cast<HUDAnchorPoint*>(pObject);
	pHUDAnchorPoint->anchorPoint = static_cast<EHUDAnchorPoint>(static_cast<EditorTypePropertyEnum*>(properties[propertyIndex++])->GetValue());
	pHUDAnchorPoint->anchorX = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pHUDAnchorPoint->anchorY = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
}

void* HUDAnchorPoint::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	HUDAnchorPoint* pHUDAnchorPoint = new HUDAnchorPoint;
	int propertyIndex = 0;
	HUDAnchorPoint::InitFromPropertiesSubset(pHUDAnchorPoint, properties, propertyIndex);
	return pHUDAnchorPoint;
}

// HUDObjectBase
void HUDObjectBase::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	HUDObjectBase* pHUDObjectBase = static_cast<HUDObjectBase*>(pObject);
	{
HUDAnchorPoint* temp = static_cast<HUDAnchorPoint*>(static_cast<EditorTypePropertyStruct*>(properties[propertyIndex++])->GetValue());
pHUDObjectBase->screenAnchorPoint = *temp;
delete temp;
}
	{
HUDAnchorPoint* temp = static_cast<HUDAnchorPoint*>(static_cast<EditorTypePropertyStruct*>(properties[propertyIndex++])->GetValue());
pHUDObjectBase->hudAnchorPoint = *temp;
delete temp;
}
	pHUDObjectBase->fileName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pHUDObjectBase->sizeX = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pHUDObjectBase->sizeY = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
}

void* HUDObjectBase::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	HUDObjectBase* pHUDObjectBase = new HUDObjectBase;
	int propertyIndex = 0;
	HUDObjectBase::InitFromPropertiesSubset(pHUDObjectBase, properties, propertyIndex);
	return pHUDObjectBase;
}

// HUD
void HUD::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	HUD* pHUD = static_cast<HUD*>(pObject);
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pHUD->pHudObjects.push_back(static_cast<HUDObjectBase*>(static_cast<EditorTypePropertyClass*>(instancedProperty.get())->GetValue()));
		}
	}
	{
HUDObjectSharedInitParams* temp = static_cast<HUDObjectSharedInitParams*>(static_cast<EditorTypePropertyStruct*>(properties[propertyIndex++])->GetValue());
pHUD->hudObjectSharedInitParams = *temp;
delete temp;
}
}

void* HUD::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	HUD* pHUD = new HUD;
	int propertyIndex = 0;
	HUD::InitFromPropertiesSubset(pHUD, properties, propertyIndex);
	return pHUD;
}

// HUDObjectHealth
void HUDObjectHealth::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	HUDObjectHealth* pHUDObjectHealth = static_cast<HUDObjectHealth*>(pObject);
	HUDObjectBase::InitFromPropertiesSubset(static_cast<HUDObjectBase*>(pHUDObjectHealth), properties, propertyIndex);
}

void* HUDObjectHealth::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	HUDObjectHealth* pHUDObjectHealth = new HUDObjectHealth;
	int propertyIndex = 0;
	HUDObjectHealth::InitFromPropertiesSubset(pHUDObjectHealth, properties, propertyIndex);
	return pHUDObjectHealth;
}

namespace __Generated
{
	std::unordered_map<std::string, void* (*)(const std::vector<EditorTypePropertyBase*>&)> stringToCreateObjectFunction
	{
		{"TextRenderCharacterData", &TextRenderCharacterData::InitFromProperties},
		{"TextboxParams", &TextboxParams::InitFromProperties},
		{"HUDObjectSharedInitParams", &HUDObjectSharedInitParams::InitFromProperties},
		{"GameAssets", &GameAssets::InitFromProperties},
		{"DirectoryData", &DirectoryData::InitFromProperties},
		{"TextRenderSystem", &TextRenderSystem::InitFromProperties},
		{"HUDAnchorPoint", &HUDAnchorPoint::InitFromProperties},
		{"HUDObjectBase", &HUDObjectBase::InitFromProperties},
		{"HUD", &HUD::InitFromProperties},
		{"HUDObjectHealth", &HUDObjectHealth::InitFromProperties},
	};
}
