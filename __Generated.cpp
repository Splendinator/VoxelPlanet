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
#include "EditorTypePropertyInstancedAssetPtr.h"
#include "..\Roguelike\Core\GameInstance.h"
#include "..\Roguelike\Core\GameSystem.h"
#include "..\Roguelike\DirectoryData.h"
#include "..\Roguelike\ECS.h"
#include "..\Roguelike\HUD.h"
#include "..\Roguelike\HUDAnchorPoint.h"
#include "..\Roguelike\HUDObjectBase.h"
#include "..\Roguelike\HUDObjectHealth.h"
#include "..\Roguelike\ImGuiEditor.h"
#include "..\Roguelike\Input\InputAction.h"
#include "..\Roguelike\Input\InputContext.h"
#include "..\Roguelike\Input\InputKey.h"
#include "..\Roguelike\Input\InputSystem.h"
#include "..\Roguelike\TextRenderSystem\TextRenderSystem.h"
#include "..\Roguelike\UI\Menu\MenuScreenBase.h"
#include "..\Roguelike\UI\Menu\MenuSystem.h"
#include "..\Roguelike\WorldGenerator.h"

#pragma warning( disable : 4189 )

// MenuScreenBase
void MenuScreenBase::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	MenuScreenBase* pMenuScreenBase = static_cast<MenuScreenBase*>(pObject);
	pMenuScreenBase->x = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pMenuScreenBase->y = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
}

void* MenuScreenBase::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	MenuScreenBase* pMenuScreenBase = new MenuScreenBase;
	int propertyIndex = 0;
	MenuScreenBase::InitFromPropertiesSubset(pMenuScreenBase, properties, propertyIndex);
	return pMenuScreenBase;
}

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

// InputActionBase
void InputActionBase::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	InputActionBase* pInputActionBase = static_cast<InputActionBase*>(pObject);
	pInputActionBase->baseKey = static_cast<EInputKey>(static_cast<EditorTypePropertyEnum*>(properties[propertyIndex++])->GetValue());
	pInputActionBase->bCtrl = static_cast<EditorTypePropertyBool*>(properties[propertyIndex++])->GetValue();
	pInputActionBase->bShift = static_cast<EditorTypePropertyBool*>(properties[propertyIndex++])->GetValue();
	pInputActionBase->bAlt = static_cast<EditorTypePropertyBool*>(properties[propertyIndex++])->GetValue();
}

void* InputActionBase::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	InputActionBase* pInputActionBase = new InputActionBase;
	int propertyIndex = 0;
	InputActionBase::InitFromPropertiesSubset(pInputActionBase, properties, propertyIndex);
	return pInputActionBase;
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

// GameSystem
void GameSystem::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	GameSystem* pGameSystem = static_cast<GameSystem*>(pObject);
}

void* GameSystem::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	GameSystem* pGameSystem = new GameSystem;
	int propertyIndex = 0;
	GameSystem::InitFromPropertiesSubset(pGameSystem, properties, propertyIndex);
	return pGameSystem;
}

// GameInstance
void GameInstance::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	GameInstance* pGameInstance = static_cast<GameInstance*>(pObject);
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pGameInstance->pGameSystems.push_back(static_cast<GameSystem*>(static_cast<EditorTypePropertyClass*>(instancedProperty.get())->GetValue()));
		}
	}
}

void* GameInstance::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	GameInstance* pGameInstance = new GameInstance;
	int propertyIndex = 0;
	GameInstance::InitFromPropertiesSubset(pGameInstance, properties, propertyIndex);
	return pGameInstance;
}

// WorldGenerator
void WorldGenerator::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	WorldGenerator* pWorldGenerator = static_cast<WorldGenerator*>(pObject);
	GameSystem::InitFromPropertiesSubset(static_cast<GameSystem*>(pWorldGenerator), properties, propertyIndex);
	pWorldGenerator->pEcs = static_cast<ECS*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* WorldGenerator::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	WorldGenerator* pWorldGenerator = new WorldGenerator;
	int propertyIndex = 0;
	WorldGenerator::InitFromPropertiesSubset(pWorldGenerator, properties, propertyIndex);
	return pWorldGenerator;
}

// MenuSystem
void MenuSystem::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	MenuSystem* pMenuSystem = static_cast<MenuSystem*>(pObject);
	GameSystem::InitFromPropertiesSubset(static_cast<GameSystem*>(pMenuSystem), properties, propertyIndex);
	pMenuSystem->pInputSystem = static_cast<InputSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pMenuSystem->pMenuInputContext = static_cast<InputContext*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pMenuSystem->pOpenMenuAction = static_cast<InputActionBase*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pMenuSystem->pCloseMenuAction = static_cast<InputActionBase*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pMenuSystem->pBaseMenuScreen.SetAsset(static_cast<EditorTypePropertyInstancedAssetPtr*>(properties[propertyIndex++])->GetValue());
}

void* MenuSystem::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	MenuSystem* pMenuSystem = new MenuSystem;
	int propertyIndex = 0;
	MenuSystem::InitFromPropertiesSubset(pMenuSystem, properties, propertyIndex);
	return pMenuSystem;
}

// TextRenderSystem
void TextRenderSystem::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	TextRenderSystem* pTextRenderSystem = static_cast<TextRenderSystem*>(pObject);
	GameSystem::InitFromPropertiesSubset(static_cast<GameSystem*>(pTextRenderSystem), properties, propertyIndex);
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pTextRenderSystem->characterDatas.push_back(*static_cast<TextRenderCharacterData*>(static_cast<EditorTypePropertyStruct*>(instancedProperty.get())->GetValue()));
		}
	}
	pTextRenderSystem->pDirectoryData = static_cast<DirectoryData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* TextRenderSystem::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	TextRenderSystem* pTextRenderSystem = new TextRenderSystem;
	int propertyIndex = 0;
	TextRenderSystem::InitFromPropertiesSubset(pTextRenderSystem, properties, propertyIndex);
	return pTextRenderSystem;
}

// InputSystem
void InputSystem::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	InputSystem* pInputSystem = static_cast<InputSystem*>(pObject);
	GameSystem::InitFromPropertiesSubset(static_cast<GameSystem*>(pInputSystem), properties, propertyIndex);
	pInputSystem->pDefaultInputContext = static_cast<InputContext*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* InputSystem::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	InputSystem* pInputSystem = new InputSystem;
	int propertyIndex = 0;
	InputSystem::InitFromPropertiesSubset(pInputSystem, properties, propertyIndex);
	return pInputSystem;
}

// InputContext
void InputContext::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	InputContext* pInputContext = static_cast<InputContext*>(pObject);
	pInputContext->priority = static_cast<EditorTypePropertyInt*>(properties[propertyIndex++])->GetValue();
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pInputContext->pInputActions.push_back(static_cast<InputActionBase*>(static_cast<EditorTypePropertyClass*>(instancedProperty.get())->GetValue()));
		}
	}
}

void* InputContext::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	InputContext* pInputContext = new InputContext;
	int propertyIndex = 0;
	InputContext::InitFromPropertiesSubset(pInputContext, properties, propertyIndex);
	return pInputContext;
}

// InputActionPress
void InputActionPress::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	InputActionPress* pInputActionPress = static_cast<InputActionPress*>(pObject);
	InputActionBase::InitFromPropertiesSubset(static_cast<InputActionBase*>(pInputActionPress), properties, propertyIndex);
}

void* InputActionPress::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	InputActionPress* pInputActionPress = new InputActionPress;
	int propertyIndex = 0;
	InputActionPress::InitFromPropertiesSubset(pInputActionPress, properties, propertyIndex);
	return pInputActionPress;
}

// ImGuiEditor
void ImGuiEditor::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ImGuiEditor* pImGuiEditor = static_cast<ImGuiEditor*>(pObject);
	GameSystem::InitFromPropertiesSubset(static_cast<GameSystem*>(pImGuiEditor), properties, propertyIndex);
	pImGuiEditor->pInputSystem = static_cast<InputSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pImGuiEditor->pEditorInputContext = static_cast<InputContext*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* ImGuiEditor::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ImGuiEditor* pImGuiEditor = new ImGuiEditor;
	int propertyIndex = 0;
	ImGuiEditor::InitFromPropertiesSubset(pImGuiEditor, properties, propertyIndex);
	return pImGuiEditor;
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

// ECS
void ECS::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ECS* pECS = static_cast<ECS*>(pObject);
	GameSystem::InitFromPropertiesSubset(static_cast<GameSystem*>(pECS), properties, propertyIndex);
}

void* ECS::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ECS* pECS = new ECS;
	int propertyIndex = 0;
	ECS::InitFromPropertiesSubset(pECS, properties, propertyIndex);
	return pECS;
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
	GameSystem::InitFromPropertiesSubset(static_cast<GameSystem*>(pHUD), properties, propertyIndex);
	pHUD->pDirectoryData = static_cast<DirectoryData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pHUD->pEcs = static_cast<ECS*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
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
		{"MenuScreenBase", &MenuScreenBase::InitFromProperties},
		{"TextRenderCharacterData", &TextRenderCharacterData::InitFromProperties},
		{"TextboxParams", &TextboxParams::InitFromProperties},
		{"InputActionBase", &InputActionBase::InitFromProperties},
		{"HUDObjectSharedInitParams", &HUDObjectSharedInitParams::InitFromProperties},
		{"DirectoryData", &DirectoryData::InitFromProperties},
		{"GameSystem", &GameSystem::InitFromProperties},
		{"GameInstance", &GameInstance::InitFromProperties},
		{"WorldGenerator", &WorldGenerator::InitFromProperties},
		{"MenuSystem", &MenuSystem::InitFromProperties},
		{"TextRenderSystem", &TextRenderSystem::InitFromProperties},
		{"InputSystem", &InputSystem::InitFromProperties},
		{"InputContext", &InputContext::InitFromProperties},
		{"InputActionPress", &InputActionPress::InitFromProperties},
		{"ImGuiEditor", &ImGuiEditor::InitFromProperties},
		{"HUDAnchorPoint", &HUDAnchorPoint::InitFromProperties},
		{"ECS", &ECS::InitFromProperties},
		{"HUDObjectBase", &HUDObjectBase::InitFromProperties},
		{"HUD", &HUD::InitFromProperties},
		{"HUDObjectHealth", &HUDObjectHealth::InitFromProperties},
	};
}
