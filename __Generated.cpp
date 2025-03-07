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
#include "..\Roguelike\Camera\CameraSystem.h"
#include "..\Roguelike\Core\GameInstance.h"
#include "..\Roguelike\Core\GameSystem.h"
#include "..\Roguelike\DirectoryData.h"
#include "..\Roguelike\ECS\ECS.h"
#include "..\Roguelike\ECS\Systems\ECSSystemAction.h"
#include "..\Roguelike\ECS\Systems\ECSSystemBase.h"
#include "..\Roguelike\ECS\Systems\ECSSystemCleanUp.h"
#include "..\Roguelike\ECS\Systems\ECSSystemDamage.h"
#include "..\Roguelike\ECS\Systems\ECSSystemEntityMap.h"
#include "..\Roguelike\ECS\Systems\ECSSystemNameslate.h"
#include "..\Roguelike\ECS\Systems\ECSSystemPhysics.h"
#include "..\Roguelike\ECS\Systems\ECSSystemRender.h"
#include "..\Roguelike\ImGuiEditor.h"
#include "..\Roguelike\Input\InputAction.h"
#include "..\Roguelike\Input\InputContext.h"
#include "..\Roguelike\Input\InputKey.h"
#include "..\Roguelike\Input\InputSystem.h"
#include "..\Roguelike\RPGSystems\Attributes\RPGAttributeModifiers.h"
#include "..\Roguelike\RPGSystems\Attributes\RPGAttributes.h"
#include "..\Roguelike\RPGSystems\Classes\RPGClassData.h"
#include "..\Roguelike\RPGSystems\Classes\RPGClassSpecialisationData.h"
#include "..\Roguelike\RPGSystems\Races\RPGRaceData.h"
#include "..\Roguelike\RPGSystems\RPGSystem.h"
#include "..\Roguelike\RPGSystems\Skills\AimModule\RPGSkillAimModules.h"
#include "..\Roguelike\RPGSystems\Skills\EffectModule\RPGSkillEffectModules.h"
#include "..\Roguelike\RPGSystems\Skills\RPGSkillData.h"
#include "..\Roguelike\RPGSystems\Skills\RPGSkillsShared.h"
#include "..\Roguelike\RPGSystems\Skills\RPGSkillSystem.h"
#include "..\Roguelike\TextRenderSystem\TextRenderSystem.h"
#include "..\Roguelike\UI\HUD\HUD.h"
#include "..\Roguelike\UI\HUD\HUDAnchorPoint.h"
#include "..\Roguelike\UI\HUD\HUDObjectBase.h"
#include "..\Roguelike\UI\HUD\HUDObjectHealth.h"
#include "..\Roguelike\UI\Menu\MenuSystem.h"
#include "..\Roguelike\UI\Menu\Screens\MenuScreenBase.h"
#include "..\Roguelike\UI\Menu\Screens\MenuScreenClassSelect.h"
#include "..\Roguelike\UI\Menu\Screens\MenuScreenMain.h"
#include "..\Roguelike\WorldGenerator.h"

#pragma warning( disable : 4189 )

// MenuScreenClassSelectEntry
void MenuScreenClassSelectEntry::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	MenuScreenClassSelectEntry* pMenuScreenClassSelectEntry = static_cast<MenuScreenClassSelectEntry*>(pObject);
	pMenuScreenClassSelectEntry->classIconLayer = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pMenuScreenClassSelectEntry->loaderLayer = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pMenuScreenClassSelectEntry->pRpgClass = static_cast<RPGClassData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* MenuScreenClassSelectEntry::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	MenuScreenClassSelectEntry* pMenuScreenClassSelectEntry = new MenuScreenClassSelectEntry;
	int propertyIndex = 0;
	MenuScreenClassSelectEntry::_InitFromPropertiesSubset(pMenuScreenClassSelectEntry, properties, propertyIndex);
	return pMenuScreenClassSelectEntry;
}

void* MenuScreenClassSelectEntry::_CreateEmptyObject()
{
	return new MenuScreenClassSelectEntry;
}

// MenuScreenBase
void MenuScreenBase::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	MenuScreenBase* pMenuScreenBase = static_cast<MenuScreenBase*>(pObject);
	pMenuScreenBase->pDirectoryData = static_cast<DirectoryData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pMenuScreenBase->fileName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
}

void* MenuScreenBase::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	MenuScreenBase* pMenuScreenBase = new MenuScreenBase;
	int propertyIndex = 0;
	MenuScreenBase::_InitFromPropertiesSubset(pMenuScreenBase, properties, propertyIndex);
	return pMenuScreenBase;
}

void* MenuScreenBase::_CreateEmptyObject()
{
	return new MenuScreenBase;
}

// HUDObjectSharedInitParams
void HUDObjectSharedInitParams::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	HUDObjectSharedInitParams* pHUDObjectSharedInitParams = static_cast<HUDObjectSharedInitParams*>(pObject);
	pHUDObjectSharedInitParams->screenEdgePadding = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
}

void* HUDObjectSharedInitParams::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	HUDObjectSharedInitParams* pHUDObjectSharedInitParams = new HUDObjectSharedInitParams;
	int propertyIndex = 0;
	HUDObjectSharedInitParams::_InitFromPropertiesSubset(pHUDObjectSharedInitParams, properties, propertyIndex);
	return pHUDObjectSharedInitParams;
}

void* HUDObjectSharedInitParams::_CreateEmptyObject()
{
	return new HUDObjectSharedInitParams;
}

// TextRenderCharacterData
void TextRenderCharacterData::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	TextRenderCharacterData* pTextRenderCharacterData = static_cast<TextRenderCharacterData*>(pObject);
	pTextRenderCharacterData->fileName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pTextRenderCharacterData->character = static_cast<EditorTypePropertyInt*>(properties[propertyIndex++])->GetValue();
}

void* TextRenderCharacterData::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	TextRenderCharacterData* pTextRenderCharacterData = new TextRenderCharacterData;
	int propertyIndex = 0;
	TextRenderCharacterData::_InitFromPropertiesSubset(pTextRenderCharacterData, properties, propertyIndex);
	return pTextRenderCharacterData;
}

void* TextRenderCharacterData::_CreateEmptyObject()
{
	return new TextRenderCharacterData;
}

// TextboxParams
void TextboxParams::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	TextboxParams* pTextboxParams = static_cast<TextboxParams*>(pObject);
	pTextboxParams->text = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pTextboxParams->xPos = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pTextboxParams->yPos = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pTextboxParams->fontSize = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
}

void* TextboxParams::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	TextboxParams* pTextboxParams = new TextboxParams;
	int propertyIndex = 0;
	TextboxParams::_InitFromPropertiesSubset(pTextboxParams, properties, propertyIndex);
	return pTextboxParams;
}

void* TextboxParams::_CreateEmptyObject()
{
	return new TextboxParams;
}

// RPGSkillData
void RPGSkillData::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGSkillData* pRPGSkillData = static_cast<RPGSkillData*>(pObject);
	pRPGSkillData->pAimModule = static_cast<RPGSkillAimModuleBase*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pRPGSkillData->pEffectModule = static_cast<RPGSkillEffectModuleBase*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* RPGSkillData::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGSkillData* pRPGSkillData = new RPGSkillData;
	int propertyIndex = 0;
	RPGSkillData::_InitFromPropertiesSubset(pRPGSkillData, properties, propertyIndex);
	return pRPGSkillData;
}

void* RPGSkillData::_CreateEmptyObject()
{
	return new RPGSkillData;
}

// RPGSkillEffectModuleBase
void RPGSkillEffectModuleBase::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGSkillEffectModuleBase* pRPGSkillEffectModuleBase = static_cast<RPGSkillEffectModuleBase*>(pObject);
}

void* RPGSkillEffectModuleBase::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGSkillEffectModuleBase* pRPGSkillEffectModuleBase = new RPGSkillEffectModuleBase;
	int propertyIndex = 0;
	RPGSkillEffectModuleBase::_InitFromPropertiesSubset(pRPGSkillEffectModuleBase, properties, propertyIndex);
	return pRPGSkillEffectModuleBase;
}

void* RPGSkillEffectModuleBase::_CreateEmptyObject()
{
	return new RPGSkillEffectModuleBase;
}

// RPGSkillAimModuleBase
void RPGSkillAimModuleBase::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGSkillAimModuleBase* pRPGSkillAimModuleBase = static_cast<RPGSkillAimModuleBase*>(pObject);
}

void* RPGSkillAimModuleBase::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGSkillAimModuleBase* pRPGSkillAimModuleBase = new RPGSkillAimModuleBase;
	int propertyIndex = 0;
	RPGSkillAimModuleBase::_InitFromPropertiesSubset(pRPGSkillAimModuleBase, properties, propertyIndex);
	return pRPGSkillAimModuleBase;
}

void* RPGSkillAimModuleBase::_CreateEmptyObject()
{
	return new RPGSkillAimModuleBase;
}

// RPGLevelProgressionData
void RPGLevelProgressionData::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGLevelProgressionData* pRPGLevelProgressionData = static_cast<RPGLevelProgressionData*>(pObject);
	pRPGLevelProgressionData->requiredXp = static_cast<EditorTypePropertyInt*>(properties[propertyIndex++])->GetValue();
}

void* RPGLevelProgressionData::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGLevelProgressionData* pRPGLevelProgressionData = new RPGLevelProgressionData;
	int propertyIndex = 0;
	RPGLevelProgressionData::_InitFromPropertiesSubset(pRPGLevelProgressionData, properties, propertyIndex);
	return pRPGLevelProgressionData;
}

void* RPGLevelProgressionData::_CreateEmptyObject()
{
	return new RPGLevelProgressionData;
}

// RPGRacePerLevelAttributeValues
void RPGRacePerLevelAttributeValues::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGRacePerLevelAttributeValues* pRPGRacePerLevelAttributeValues = static_cast<RPGRacePerLevelAttributeValues*>(pObject);
	pRPGRacePerLevelAttributeValues->maxHealthMultiplierPerLevel = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
}

void* RPGRacePerLevelAttributeValues::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGRacePerLevelAttributeValues* pRPGRacePerLevelAttributeValues = new RPGRacePerLevelAttributeValues;
	int propertyIndex = 0;
	RPGRacePerLevelAttributeValues::_InitFromPropertiesSubset(pRPGRacePerLevelAttributeValues, properties, propertyIndex);
	return pRPGRacePerLevelAttributeValues;
}

void* RPGRacePerLevelAttributeValues::_CreateEmptyObject()
{
	return new RPGRacePerLevelAttributeValues;
}

// RPGRaceInitialAttributeValues
void RPGRaceInitialAttributeValues::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGRaceInitialAttributeValues* pRPGRaceInitialAttributeValues = static_cast<RPGRaceInitialAttributeValues*>(pObject);
	pRPGRaceInitialAttributeValues->maxHealth = static_cast<EditorTypePropertyInt*>(properties[propertyIndex++])->GetValue();
}

void* RPGRaceInitialAttributeValues::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGRaceInitialAttributeValues* pRPGRaceInitialAttributeValues = new RPGRaceInitialAttributeValues;
	int propertyIndex = 0;
	RPGRaceInitialAttributeValues::_InitFromPropertiesSubset(pRPGRaceInitialAttributeValues, properties, propertyIndex);
	return pRPGRaceInitialAttributeValues;
}

void* RPGRaceInitialAttributeValues::_CreateEmptyObject()
{
	return new RPGRaceInitialAttributeValues;
}

// RPGClassSpecialisationData
void RPGClassSpecialisationData::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGClassSpecialisationData* pRPGClassSpecialisationData = static_cast<RPGClassSpecialisationData*>(pObject);
	pRPGClassSpecialisationData->displayName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
}

void* RPGClassSpecialisationData::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGClassSpecialisationData* pRPGClassSpecialisationData = new RPGClassSpecialisationData;
	int propertyIndex = 0;
	RPGClassSpecialisationData::_InitFromPropertiesSubset(pRPGClassSpecialisationData, properties, propertyIndex);
	return pRPGClassSpecialisationData;
}

void* RPGClassSpecialisationData::_CreateEmptyObject()
{
	return new RPGClassSpecialisationData;
}

// RPGClassData
void RPGClassData::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGClassData* pRPGClassData = static_cast<RPGClassData*>(pObject);
	pRPGClassData->displayName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pRPGClassData->vectorArtFileName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pRPGClassData->specialisations.push_back(static_cast<RPGClassSpecialisationData*>(static_cast<EditorTypePropertyClass*>(instancedProperty.get())->GetValue()));
		}
	}
}

void* RPGClassData::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGClassData* pRPGClassData = new RPGClassData;
	int propertyIndex = 0;
	RPGClassData::_InitFromPropertiesSubset(pRPGClassData, properties, propertyIndex);
	return pRPGClassData;
}

void* RPGClassData::_CreateEmptyObject()
{
	return new RPGClassData;
}

// RPGAttributeModifierBase
void RPGAttributeModifierBase::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGAttributeModifierBase* pRPGAttributeModifierBase = static_cast<RPGAttributeModifierBase*>(pObject);
}

void* RPGAttributeModifierBase::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGAttributeModifierBase* pRPGAttributeModifierBase = new RPGAttributeModifierBase;
	int propertyIndex = 0;
	RPGAttributeModifierBase::_InitFromPropertiesSubset(pRPGAttributeModifierBase, properties, propertyIndex);
	return pRPGAttributeModifierBase;
}

void* RPGAttributeModifierBase::_CreateEmptyObject()
{
	return new RPGAttributeModifierBase;
}

// InputActionBase
void InputActionBase::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	InputActionBase* pInputActionBase = static_cast<InputActionBase*>(pObject);
	pInputActionBase->baseKey = static_cast<EInputKey>(static_cast<EditorTypePropertyEnum*>(properties[propertyIndex++])->GetValue());
	pInputActionBase->bCtrl = static_cast<EditorTypePropertyBool*>(properties[propertyIndex++])->GetValue();
	pInputActionBase->bShift = static_cast<EditorTypePropertyBool*>(properties[propertyIndex++])->GetValue();
	pInputActionBase->bAlt = static_cast<EditorTypePropertyBool*>(properties[propertyIndex++])->GetValue();
}

void* InputActionBase::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	InputActionBase* pInputActionBase = new InputActionBase;
	int propertyIndex = 0;
	InputActionBase::_InitFromPropertiesSubset(pInputActionBase, properties, propertyIndex);
	return pInputActionBase;
}

void* InputActionBase::_CreateEmptyObject()
{
	return new InputActionBase;
}

// ECSSystemBase
void ECSSystemBase::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ECSSystemBase* pECSSystemBase = static_cast<ECSSystemBase*>(pObject);
}

void* ECSSystemBase::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ECSSystemBase* pECSSystemBase = new ECSSystemBase;
	int propertyIndex = 0;
	ECSSystemBase::_InitFromPropertiesSubset(pECSSystemBase, properties, propertyIndex);
	return pECSSystemBase;
}

void* ECSSystemBase::_CreateEmptyObject()
{
	return new ECSSystemBase;
}

// ECSSystemAction
void ECSSystemAction::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ECSSystemAction* pECSSystemAction = static_cast<ECSSystemAction*>(pObject);
	ECSSystemBase::_InitFromPropertiesSubset(static_cast<ECSSystemBase*>(pECSSystemAction), properties, propertyIndex);
}

void* ECSSystemAction::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ECSSystemAction* pECSSystemAction = new ECSSystemAction;
	int propertyIndex = 0;
	ECSSystemAction::_InitFromPropertiesSubset(pECSSystemAction, properties, propertyIndex);
	return pECSSystemAction;
}

void* ECSSystemAction::_CreateEmptyObject()
{
	return new ECSSystemAction;
}

// DirectoryData
void DirectoryData::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	DirectoryData* pDirectoryData = static_cast<DirectoryData*>(pObject);
	pDirectoryData->fonts = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pDirectoryData->hudObjects = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pDirectoryData->menus = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pDirectoryData->sharedUI = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pDirectoryData->rpgClassVisuals = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pDirectoryData->rpgRaceVisuals = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pDirectoryData->rpgSkillVisuals = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
}

void* DirectoryData::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	DirectoryData* pDirectoryData = new DirectoryData;
	int propertyIndex = 0;
	DirectoryData::_InitFromPropertiesSubset(pDirectoryData, properties, propertyIndex);
	return pDirectoryData;
}

void* DirectoryData::_CreateEmptyObject()
{
	return new DirectoryData;
}

// GameSystem
void GameSystem::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	GameSystem* pGameSystem = static_cast<GameSystem*>(pObject);
}

void* GameSystem::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	GameSystem* pGameSystem = new GameSystem;
	int propertyIndex = 0;
	GameSystem::_InitFromPropertiesSubset(pGameSystem, properties, propertyIndex);
	return pGameSystem;
}

void* GameSystem::_CreateEmptyObject()
{
	return new GameSystem;
}

// GameInstance
void GameInstance::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
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

void* GameInstance::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	GameInstance* pGameInstance = new GameInstance;
	int propertyIndex = 0;
	GameInstance::_InitFromPropertiesSubset(pGameInstance, properties, propertyIndex);
	return pGameInstance;
}

void* GameInstance::_CreateEmptyObject()
{
	return new GameInstance;
}

// CameraSystem
void CameraSystem::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	CameraSystem* pCameraSystem = static_cast<CameraSystem*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pCameraSystem), properties, propertyIndex);
	pCameraSystem->pEcs = static_cast<ECS*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pCameraSystem->zoom = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pCameraSystem->zoomSpeed = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
}

void* CameraSystem::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	CameraSystem* pCameraSystem = new CameraSystem;
	int propertyIndex = 0;
	CameraSystem::_InitFromPropertiesSubset(pCameraSystem, properties, propertyIndex);
	return pCameraSystem;
}

void* CameraSystem::_CreateEmptyObject()
{
	return new CameraSystem;
}

// WorldGenerator
void WorldGenerator::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	WorldGenerator* pWorldGenerator = static_cast<WorldGenerator*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pWorldGenerator), properties, propertyIndex);
	pWorldGenerator->pEcs = static_cast<ECS*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* WorldGenerator::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	WorldGenerator* pWorldGenerator = new WorldGenerator;
	int propertyIndex = 0;
	WorldGenerator::_InitFromPropertiesSubset(pWorldGenerator, properties, propertyIndex);
	return pWorldGenerator;
}

void* WorldGenerator::_CreateEmptyObject()
{
	return new WorldGenerator;
}

// MenuScreenMain
void MenuScreenMain::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	MenuScreenMain* pMenuScreenMain = static_cast<MenuScreenMain*>(pObject);
	MenuScreenBase::_InitFromPropertiesSubset(static_cast<MenuScreenBase*>(pMenuScreenMain), properties, propertyIndex);
}

void* MenuScreenMain::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	MenuScreenMain* pMenuScreenMain = new MenuScreenMain;
	int propertyIndex = 0;
	MenuScreenMain::_InitFromPropertiesSubset(pMenuScreenMain, properties, propertyIndex);
	return pMenuScreenMain;
}

void* MenuScreenMain::_CreateEmptyObject()
{
	return new MenuScreenMain;
}

// MenuScreenClassSelect
void MenuScreenClassSelect::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	MenuScreenClassSelect* pMenuScreenClassSelect = static_cast<MenuScreenClassSelect*>(pObject);
	MenuScreenBase::_InitFromPropertiesSubset(static_cast<MenuScreenBase*>(pMenuScreenClassSelect), properties, propertyIndex);
	pMenuScreenClassSelect->pRpgSystem = static_cast<RPGSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pMenuScreenClassSelect->pEcs = static_cast<ECS*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pMenuScreenClassSelect->pPlayerRace = static_cast<RPGRaceData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pMenuScreenClassSelect->classIconFileName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pMenuScreenClassSelect->classEntries.push_back(*static_cast<MenuScreenClassSelectEntry*>(static_cast<EditorTypePropertyStruct*>(instancedProperty.get())->GetValue()));
		}
	}
}

void* MenuScreenClassSelect::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	MenuScreenClassSelect* pMenuScreenClassSelect = new MenuScreenClassSelect;
	int propertyIndex = 0;
	MenuScreenClassSelect::_InitFromPropertiesSubset(pMenuScreenClassSelect, properties, propertyIndex);
	return pMenuScreenClassSelect;
}

void* MenuScreenClassSelect::_CreateEmptyObject()
{
	return new MenuScreenClassSelect;
}

// MenuSystem
void MenuSystem::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	MenuSystem* pMenuSystem = static_cast<MenuSystem*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pMenuSystem), properties, propertyIndex);
	pMenuSystem->pInputSystem = static_cast<InputSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pMenuSystem->pMenuInputContext = static_cast<InputContext*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pMenuSystem->pOpenMenuAction = static_cast<InputActionBase*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pMenuSystem->pCloseMenuAction = static_cast<InputActionBase*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pMenuSystem->pBaseMenuScreen.SetAsset(static_cast<EditorTypePropertyInstancedAssetPtr*>(properties[propertyIndex++])->GetValue());
	pMenuSystem->pGameStartMenuScreen.SetAsset(static_cast<EditorTypePropertyInstancedAssetPtr*>(properties[propertyIndex++])->GetValue());
}

void* MenuSystem::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	MenuSystem* pMenuSystem = new MenuSystem;
	int propertyIndex = 0;
	MenuSystem::_InitFromPropertiesSubset(pMenuSystem, properties, propertyIndex);
	return pMenuSystem;
}

void* MenuSystem::_CreateEmptyObject()
{
	return new MenuSystem;
}

// HUDAnchorPoint
void HUDAnchorPoint::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	HUDAnchorPoint* pHUDAnchorPoint = static_cast<HUDAnchorPoint*>(pObject);
	pHUDAnchorPoint->anchorPoint = static_cast<EHUDAnchorPoint>(static_cast<EditorTypePropertyEnum*>(properties[propertyIndex++])->GetValue());
	pHUDAnchorPoint->anchorX = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pHUDAnchorPoint->anchorY = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
}

void* HUDAnchorPoint::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	HUDAnchorPoint* pHUDAnchorPoint = new HUDAnchorPoint;
	int propertyIndex = 0;
	HUDAnchorPoint::_InitFromPropertiesSubset(pHUDAnchorPoint, properties, propertyIndex);
	return pHUDAnchorPoint;
}

void* HUDAnchorPoint::_CreateEmptyObject()
{
	return new HUDAnchorPoint;
}

// TextRenderSystem
void TextRenderSystem::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	TextRenderSystem* pTextRenderSystem = static_cast<TextRenderSystem*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pTextRenderSystem), properties, propertyIndex);
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pTextRenderSystem->characterDatas.push_back(*static_cast<TextRenderCharacterData*>(static_cast<EditorTypePropertyStruct*>(instancedProperty.get())->GetValue()));
		}
	}
	pTextRenderSystem->pDirectoryData = static_cast<DirectoryData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* TextRenderSystem::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	TextRenderSystem* pTextRenderSystem = new TextRenderSystem;
	int propertyIndex = 0;
	TextRenderSystem::_InitFromPropertiesSubset(pTextRenderSystem, properties, propertyIndex);
	return pTextRenderSystem;
}

void* TextRenderSystem::_CreateEmptyObject()
{
	return new TextRenderSystem;
}

// RPGSkillHighlightEntry
void RPGSkillHighlightEntry::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGSkillHighlightEntry* pRPGSkillHighlightEntry = static_cast<RPGSkillHighlightEntry*>(pObject);
	pRPGSkillHighlightEntry->highlight = static_cast<ERPGSkillHighlightType>(static_cast<EditorTypePropertyEnum*>(properties[propertyIndex++])->GetValue());
	pRPGSkillHighlightEntry->fileName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
}

void* RPGSkillHighlightEntry::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGSkillHighlightEntry* pRPGSkillHighlightEntry = new RPGSkillHighlightEntry;
	int propertyIndex = 0;
	RPGSkillHighlightEntry::_InitFromPropertiesSubset(pRPGSkillHighlightEntry, properties, propertyIndex);
	return pRPGSkillHighlightEntry;
}

void* RPGSkillHighlightEntry::_CreateEmptyObject()
{
	return new RPGSkillHighlightEntry;
}

// RPGSkillEffectModuleTemp
void RPGSkillEffectModuleTemp::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGSkillEffectModuleTemp* pRPGSkillEffectModuleTemp = static_cast<RPGSkillEffectModuleTemp*>(pObject);
	RPGSkillEffectModuleBase::_InitFromPropertiesSubset(static_cast<RPGSkillEffectModuleBase*>(pRPGSkillEffectModuleTemp), properties, propertyIndex);
	pRPGSkillEffectModuleTemp->damage = static_cast<EditorTypePropertyInt*>(properties[propertyIndex++])->GetValue();
}

void* RPGSkillEffectModuleTemp::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGSkillEffectModuleTemp* pRPGSkillEffectModuleTemp = new RPGSkillEffectModuleTemp;
	int propertyIndex = 0;
	RPGSkillEffectModuleTemp::_InitFromPropertiesSubset(pRPGSkillEffectModuleTemp, properties, propertyIndex);
	return pRPGSkillEffectModuleTemp;
}

void* RPGSkillEffectModuleTemp::_CreateEmptyObject()
{
	return new RPGSkillEffectModuleTemp;
}

// RPGSkillAimModuleLine
void RPGSkillAimModuleLine::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGSkillAimModuleLine* pRPGSkillAimModuleLine = static_cast<RPGSkillAimModuleLine*>(pObject);
	RPGSkillAimModuleBase::_InitFromPropertiesSubset(static_cast<RPGSkillAimModuleBase*>(pRPGSkillAimModuleLine), properties, propertyIndex);
	pRPGSkillAimModuleLine->maxRange = static_cast<EditorTypePropertyInt*>(properties[propertyIndex++])->GetValue();
	pRPGSkillAimModuleLine->areaRadius = static_cast<EditorTypePropertyInt*>(properties[propertyIndex++])->GetValue();
}

void* RPGSkillAimModuleLine::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGSkillAimModuleLine* pRPGSkillAimModuleLine = new RPGSkillAimModuleLine;
	int propertyIndex = 0;
	RPGSkillAimModuleLine::_InitFromPropertiesSubset(pRPGSkillAimModuleLine, properties, propertyIndex);
	return pRPGSkillAimModuleLine;
}

void* RPGSkillAimModuleLine::_CreateEmptyObject()
{
	return new RPGSkillAimModuleLine;
}

// RPGRaceData
void RPGRaceData::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGRaceData* pRPGRaceData = static_cast<RPGRaceData*>(pObject);
	pRPGRaceData->displayName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	{
		RPGRaceInitialAttributeValues* temp = static_cast<RPGRaceInitialAttributeValues*>(static_cast<EditorTypePropertyStruct*>(properties[propertyIndex++])->GetValue());
		pRPGRaceData->initialAttributes = *temp;
		delete temp;
	}
	{
		RPGRacePerLevelAttributeValues* temp = static_cast<RPGRacePerLevelAttributeValues*>(static_cast<EditorTypePropertyStruct*>(properties[propertyIndex++])->GetValue());
		pRPGRaceData->perLevelAttributes = *temp;
		delete temp;
	}
	pRPGRaceData->vectorArtFileName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
}

void* RPGRaceData::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGRaceData* pRPGRaceData = new RPGRaceData;
	int propertyIndex = 0;
	RPGRaceData::_InitFromPropertiesSubset(pRPGRaceData, properties, propertyIndex);
	return pRPGRaceData;
}

void* RPGRaceData::_CreateEmptyObject()
{
	return new RPGRaceData;
}

// RPGAttributeBase
void RPGAttributeBase::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGAttributeBase* pRPGAttributeBase = static_cast<RPGAttributeBase*>(pObject);
	pRPGAttributeBase->displayName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pRPGAttributeBase->pModifiers.push_back(static_cast<RPGAttributeModifierBase*>(static_cast<EditorTypePropertyClass*>(instancedProperty.get())->GetValue()));
		}
	}
}

void* RPGAttributeBase::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGAttributeBase* pRPGAttributeBase = new RPGAttributeBase;
	int propertyIndex = 0;
	RPGAttributeBase::_InitFromPropertiesSubset(pRPGAttributeBase, properties, propertyIndex);
	return pRPGAttributeBase;
}

void* RPGAttributeBase::_CreateEmptyObject()
{
	return new RPGAttributeBase;
}

// InputSystem
void InputSystem::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	InputSystem* pInputSystem = static_cast<InputSystem*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pInputSystem), properties, propertyIndex);
	pInputSystem->pDefaultInputContext = static_cast<InputContext*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* InputSystem::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	InputSystem* pInputSystem = new InputSystem;
	int propertyIndex = 0;
	InputSystem::_InitFromPropertiesSubset(pInputSystem, properties, propertyIndex);
	return pInputSystem;
}

void* InputSystem::_CreateEmptyObject()
{
	return new InputSystem;
}

// InputContext
void InputContext::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
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

void* InputContext::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	InputContext* pInputContext = new InputContext;
	int propertyIndex = 0;
	InputContext::_InitFromPropertiesSubset(pInputContext, properties, propertyIndex);
	return pInputContext;
}

void* InputContext::_CreateEmptyObject()
{
	return new InputContext;
}

// InputActionPress
void InputActionPress::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	InputActionPress* pInputActionPress = static_cast<InputActionPress*>(pObject);
	InputActionBase::_InitFromPropertiesSubset(static_cast<InputActionBase*>(pInputActionPress), properties, propertyIndex);
}

void* InputActionPress::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	InputActionPress* pInputActionPress = new InputActionPress;
	int propertyIndex = 0;
	InputActionPress::_InitFromPropertiesSubset(pInputActionPress, properties, propertyIndex);
	return pInputActionPress;
}

void* InputActionPress::_CreateEmptyObject()
{
	return new InputActionPress;
}

// ImGuiEditor
void ImGuiEditor::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ImGuiEditor* pImGuiEditor = static_cast<ImGuiEditor*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pImGuiEditor), properties, propertyIndex);
	pImGuiEditor->pInputSystem = static_cast<InputSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pImGuiEditor->pEditorInputContext = static_cast<InputContext*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* ImGuiEditor::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ImGuiEditor* pImGuiEditor = new ImGuiEditor;
	int propertyIndex = 0;
	ImGuiEditor::_InitFromPropertiesSubset(pImGuiEditor, properties, propertyIndex);
	return pImGuiEditor;
}

void* ImGuiEditor::_CreateEmptyObject()
{
	return new ImGuiEditor;
}

// ECSSystemRender
void ECSSystemRender::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ECSSystemRender* pECSSystemRender = static_cast<ECSSystemRender*>(pObject);
	ECSSystemBase::_InitFromPropertiesSubset(static_cast<ECSSystemBase*>(pECSSystemRender), properties, propertyIndex);
}

void* ECSSystemRender::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ECSSystemRender* pECSSystemRender = new ECSSystemRender;
	int propertyIndex = 0;
	ECSSystemRender::_InitFromPropertiesSubset(pECSSystemRender, properties, propertyIndex);
	return pECSSystemRender;
}

void* ECSSystemRender::_CreateEmptyObject()
{
	return new ECSSystemRender;
}

// ECSSystemPhysics
void ECSSystemPhysics::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ECSSystemPhysics* pECSSystemPhysics = static_cast<ECSSystemPhysics*>(pObject);
	ECSSystemBase::_InitFromPropertiesSubset(static_cast<ECSSystemBase*>(pECSSystemPhysics), properties, propertyIndex);
}

void* ECSSystemPhysics::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ECSSystemPhysics* pECSSystemPhysics = new ECSSystemPhysics;
	int propertyIndex = 0;
	ECSSystemPhysics::_InitFromPropertiesSubset(pECSSystemPhysics, properties, propertyIndex);
	return pECSSystemPhysics;
}

void* ECSSystemPhysics::_CreateEmptyObject()
{
	return new ECSSystemPhysics;
}

// ECSSystemNameslate
void ECSSystemNameslate::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ECSSystemNameslate* pECSSystemNameslate = static_cast<ECSSystemNameslate*>(pObject);
	ECSSystemBase::_InitFromPropertiesSubset(static_cast<ECSSystemBase*>(pECSSystemNameslate), properties, propertyIndex);
}

void* ECSSystemNameslate::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ECSSystemNameslate* pECSSystemNameslate = new ECSSystemNameslate;
	int propertyIndex = 0;
	ECSSystemNameslate::_InitFromPropertiesSubset(pECSSystemNameslate, properties, propertyIndex);
	return pECSSystemNameslate;
}

void* ECSSystemNameslate::_CreateEmptyObject()
{
	return new ECSSystemNameslate;
}

// ECSSystemEntityMap
void ECSSystemEntityMap::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ECSSystemEntityMap* pECSSystemEntityMap = static_cast<ECSSystemEntityMap*>(pObject);
	ECSSystemBase::_InitFromPropertiesSubset(static_cast<ECSSystemBase*>(pECSSystemEntityMap), properties, propertyIndex);
}

void* ECSSystemEntityMap::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ECSSystemEntityMap* pECSSystemEntityMap = new ECSSystemEntityMap;
	int propertyIndex = 0;
	ECSSystemEntityMap::_InitFromPropertiesSubset(pECSSystemEntityMap, properties, propertyIndex);
	return pECSSystemEntityMap;
}

void* ECSSystemEntityMap::_CreateEmptyObject()
{
	return new ECSSystemEntityMap;
}

// ECSSystemDamage
void ECSSystemDamage::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ECSSystemDamage* pECSSystemDamage = static_cast<ECSSystemDamage*>(pObject);
	ECSSystemBase::_InitFromPropertiesSubset(static_cast<ECSSystemBase*>(pECSSystemDamage), properties, propertyIndex);
}

void* ECSSystemDamage::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ECSSystemDamage* pECSSystemDamage = new ECSSystemDamage;
	int propertyIndex = 0;
	ECSSystemDamage::_InitFromPropertiesSubset(pECSSystemDamage, properties, propertyIndex);
	return pECSSystemDamage;
}

void* ECSSystemDamage::_CreateEmptyObject()
{
	return new ECSSystemDamage;
}

// ECSSystemCleanUp
void ECSSystemCleanUp::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ECSSystemCleanUp* pECSSystemCleanUp = static_cast<ECSSystemCleanUp*>(pObject);
	ECSSystemBase::_InitFromPropertiesSubset(static_cast<ECSSystemBase*>(pECSSystemCleanUp), properties, propertyIndex);
}

void* ECSSystemCleanUp::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ECSSystemCleanUp* pECSSystemCleanUp = new ECSSystemCleanUp;
	int propertyIndex = 0;
	ECSSystemCleanUp::_InitFromPropertiesSubset(pECSSystemCleanUp, properties, propertyIndex);
	return pECSSystemCleanUp;
}

void* ECSSystemCleanUp::_CreateEmptyObject()
{
	return new ECSSystemCleanUp;
}

// ECS
void ECS::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ECS* pECS = static_cast<ECS*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pECS), properties, propertyIndex);
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pECS->systems.push_back(static_cast<ECSSystemBase*>(static_cast<EditorTypePropertyClass*>(instancedProperty.get())->GetValue()));
		}
	}
	pECS->pSkillSystem = static_cast<RPGSkillSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* ECS::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ECS* pECS = new ECS;
	int propertyIndex = 0;
	ECS::_InitFromPropertiesSubset(pECS, properties, propertyIndex);
	return pECS;
}

void* ECS::_CreateEmptyObject()
{
	return new ECS;
}

// HUDObjectBase
void HUDObjectBase::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
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

void* HUDObjectBase::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	HUDObjectBase* pHUDObjectBase = new HUDObjectBase;
	int propertyIndex = 0;
	HUDObjectBase::_InitFromPropertiesSubset(pHUDObjectBase, properties, propertyIndex);
	return pHUDObjectBase;
}

void* HUDObjectBase::_CreateEmptyObject()
{
	return new HUDObjectBase;
}

// HUD
void HUD::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	HUD* pHUD = static_cast<HUD*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pHUD), properties, propertyIndex);
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

void* HUD::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	HUD* pHUD = new HUD;
	int propertyIndex = 0;
	HUD::_InitFromPropertiesSubset(pHUD, properties, propertyIndex);
	return pHUD;
}

void* HUD::_CreateEmptyObject()
{
	return new HUD;
}

// RPGSkillSystem
void RPGSkillSystem::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGSkillSystem* pRPGSkillSystem = static_cast<RPGSkillSystem*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pRPGSkillSystem), properties, propertyIndex);
	pRPGSkillSystem->pDirectoryData = static_cast<DirectoryData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pRPGSkillSystem->pEcs = static_cast<ECS*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pRPGSkillSystem->pEcsEntityMap = static_cast<ECSSystemEntityMap*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pRPGSkillSystem->pCameraSystem = static_cast<CameraSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pRPGSkillSystem->skillHighlightVisuals.push_back(*static_cast<RPGSkillHighlightEntry*>(static_cast<EditorTypePropertyStruct*>(instancedProperty.get())->GetValue()));
		}
	}
}

void* RPGSkillSystem::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGSkillSystem* pRPGSkillSystem = new RPGSkillSystem;
	int propertyIndex = 0;
	RPGSkillSystem::_InitFromPropertiesSubset(pRPGSkillSystem, properties, propertyIndex);
	return pRPGSkillSystem;
}

void* RPGSkillSystem::_CreateEmptyObject()
{
	return new RPGSkillSystem;
}

// RPGSystem
void RPGSystem::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGSystem* pRPGSystem = static_cast<RPGSystem*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pRPGSystem), properties, propertyIndex);
	pRPGSystem->pEcs = static_cast<ECS*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pRPGSystem->pDirectoryData = static_cast<DirectoryData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pRPGSystem->attributes.push_back(static_cast<RPGAttributeBase*>(static_cast<EditorTypePropertyClass*>(instancedProperty.get())->GetValue()));
		}
	}
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pRPGSystem->levels.push_back(*static_cast<RPGLevelProgressionData*>(static_cast<EditorTypePropertyStruct*>(instancedProperty.get())->GetValue()));
		}
	}
}

void* RPGSystem::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGSystem* pRPGSystem = new RPGSystem;
	int propertyIndex = 0;
	RPGSystem::_InitFromPropertiesSubset(pRPGSystem, properties, propertyIndex);
	return pRPGSystem;
}

void* RPGSystem::_CreateEmptyObject()
{
	return new RPGSystem;
}

// RPGAttributeMaxHealth
void RPGAttributeMaxHealth::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGAttributeMaxHealth* pRPGAttributeMaxHealth = static_cast<RPGAttributeMaxHealth*>(pObject);
	RPGAttributeBase::_InitFromPropertiesSubset(static_cast<RPGAttributeBase*>(pRPGAttributeMaxHealth), properties, propertyIndex);
}

void* RPGAttributeMaxHealth::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGAttributeMaxHealth* pRPGAttributeMaxHealth = new RPGAttributeMaxHealth;
	int propertyIndex = 0;
	RPGAttributeMaxHealth::_InitFromPropertiesSubset(pRPGAttributeMaxHealth, properties, propertyIndex);
	return pRPGAttributeMaxHealth;
}

void* RPGAttributeMaxHealth::_CreateEmptyObject()
{
	return new RPGAttributeMaxHealth;
}

// HUDObjectHealth
void HUDObjectHealth::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	HUDObjectHealth* pHUDObjectHealth = static_cast<HUDObjectHealth*>(pObject);
	HUDObjectBase::_InitFromPropertiesSubset(static_cast<HUDObjectBase*>(pHUDObjectHealth), properties, propertyIndex);
}

void* HUDObjectHealth::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	HUDObjectHealth* pHUDObjectHealth = new HUDObjectHealth;
	int propertyIndex = 0;
	HUDObjectHealth::_InitFromPropertiesSubset(pHUDObjectHealth, properties, propertyIndex);
	return pHUDObjectHealth;
}

void* HUDObjectHealth::_CreateEmptyObject()
{
	return new HUDObjectHealth;
}

namespace __Generated
{
	std::unordered_map<std::string, void* (*)(const std::vector<EditorTypePropertyBase*>&)> stringToCreateObjectFunction
	{
		{"MenuScreenClassSelectEntry", &MenuScreenClassSelectEntry::_InitFromProperties},
		{"MenuScreenBase", &MenuScreenBase::_InitFromProperties},
		{"HUDObjectSharedInitParams", &HUDObjectSharedInitParams::_InitFromProperties},
		{"TextRenderCharacterData", &TextRenderCharacterData::_InitFromProperties},
		{"TextboxParams", &TextboxParams::_InitFromProperties},
		{"RPGSkillData", &RPGSkillData::_InitFromProperties},
		{"RPGSkillEffectModuleBase", &RPGSkillEffectModuleBase::_InitFromProperties},
		{"RPGSkillAimModuleBase", &RPGSkillAimModuleBase::_InitFromProperties},
		{"RPGLevelProgressionData", &RPGLevelProgressionData::_InitFromProperties},
		{"RPGRacePerLevelAttributeValues", &RPGRacePerLevelAttributeValues::_InitFromProperties},
		{"RPGRaceInitialAttributeValues", &RPGRaceInitialAttributeValues::_InitFromProperties},
		{"RPGClassSpecialisationData", &RPGClassSpecialisationData::_InitFromProperties},
		{"RPGClassData", &RPGClassData::_InitFromProperties},
		{"RPGAttributeModifierBase", &RPGAttributeModifierBase::_InitFromProperties},
		{"InputActionBase", &InputActionBase::_InitFromProperties},
		{"ECSSystemBase", &ECSSystemBase::_InitFromProperties},
		{"ECSSystemAction", &ECSSystemAction::_InitFromProperties},
		{"DirectoryData", &DirectoryData::_InitFromProperties},
		{"GameSystem", &GameSystem::_InitFromProperties},
		{"GameInstance", &GameInstance::_InitFromProperties},
		{"CameraSystem", &CameraSystem::_InitFromProperties},
		{"WorldGenerator", &WorldGenerator::_InitFromProperties},
		{"MenuScreenMain", &MenuScreenMain::_InitFromProperties},
		{"MenuScreenClassSelect", &MenuScreenClassSelect::_InitFromProperties},
		{"MenuSystem", &MenuSystem::_InitFromProperties},
		{"HUDAnchorPoint", &HUDAnchorPoint::_InitFromProperties},
		{"TextRenderSystem", &TextRenderSystem::_InitFromProperties},
		{"RPGSkillHighlightEntry", &RPGSkillHighlightEntry::_InitFromProperties},
		{"RPGSkillEffectModuleTemp", &RPGSkillEffectModuleTemp::_InitFromProperties},
		{"RPGSkillAimModuleLine", &RPGSkillAimModuleLine::_InitFromProperties},
		{"RPGRaceData", &RPGRaceData::_InitFromProperties},
		{"RPGAttributeBase", &RPGAttributeBase::_InitFromProperties},
		{"InputSystem", &InputSystem::_InitFromProperties},
		{"InputContext", &InputContext::_InitFromProperties},
		{"InputActionPress", &InputActionPress::_InitFromProperties},
		{"ImGuiEditor", &ImGuiEditor::_InitFromProperties},
		{"ECSSystemRender", &ECSSystemRender::_InitFromProperties},
		{"ECSSystemPhysics", &ECSSystemPhysics::_InitFromProperties},
		{"ECSSystemNameslate", &ECSSystemNameslate::_InitFromProperties},
		{"ECSSystemEntityMap", &ECSSystemEntityMap::_InitFromProperties},
		{"ECSSystemDamage", &ECSSystemDamage::_InitFromProperties},
		{"ECSSystemCleanUp", &ECSSystemCleanUp::_InitFromProperties},
		{"ECS", &ECS::_InitFromProperties},
		{"HUDObjectBase", &HUDObjectBase::_InitFromProperties},
		{"HUD", &HUD::_InitFromProperties},
		{"RPGSkillSystem", &RPGSkillSystem::_InitFromProperties},
		{"RPGSystem", &RPGSystem::_InitFromProperties},
		{"RPGAttributeMaxHealth", &RPGAttributeMaxHealth::_InitFromProperties},
		{"HUDObjectHealth", &HUDObjectHealth::_InitFromProperties},
	};

	std::unordered_map<std::string, void* (*)()> stringToCreateEmptyObjectFunction
	{
		{"MenuScreenClassSelectEntry", &MenuScreenClassSelectEntry::_CreateEmptyObject},
		{"MenuScreenBase", &MenuScreenBase::_CreateEmptyObject},
		{"HUDObjectSharedInitParams", &HUDObjectSharedInitParams::_CreateEmptyObject},
		{"TextRenderCharacterData", &TextRenderCharacterData::_CreateEmptyObject},
		{"TextboxParams", &TextboxParams::_CreateEmptyObject},
		{"RPGSkillData", &RPGSkillData::_CreateEmptyObject},
		{"RPGSkillEffectModuleBase", &RPGSkillEffectModuleBase::_CreateEmptyObject},
		{"RPGSkillAimModuleBase", &RPGSkillAimModuleBase::_CreateEmptyObject},
		{"RPGLevelProgressionData", &RPGLevelProgressionData::_CreateEmptyObject},
		{"RPGRacePerLevelAttributeValues", &RPGRacePerLevelAttributeValues::_CreateEmptyObject},
		{"RPGRaceInitialAttributeValues", &RPGRaceInitialAttributeValues::_CreateEmptyObject},
		{"RPGClassSpecialisationData", &RPGClassSpecialisationData::_CreateEmptyObject},
		{"RPGClassData", &RPGClassData::_CreateEmptyObject},
		{"RPGAttributeModifierBase", &RPGAttributeModifierBase::_CreateEmptyObject},
		{"InputActionBase", &InputActionBase::_CreateEmptyObject},
		{"ECSSystemBase", &ECSSystemBase::_CreateEmptyObject},
		{"ECSSystemAction", &ECSSystemAction::_CreateEmptyObject},
		{"DirectoryData", &DirectoryData::_CreateEmptyObject},
		{"GameSystem", &GameSystem::_CreateEmptyObject},
		{"GameInstance", &GameInstance::_CreateEmptyObject},
		{"CameraSystem", &CameraSystem::_CreateEmptyObject},
		{"WorldGenerator", &WorldGenerator::_CreateEmptyObject},
		{"MenuScreenMain", &MenuScreenMain::_CreateEmptyObject},
		{"MenuScreenClassSelect", &MenuScreenClassSelect::_CreateEmptyObject},
		{"MenuSystem", &MenuSystem::_CreateEmptyObject},
		{"HUDAnchorPoint", &HUDAnchorPoint::_CreateEmptyObject},
		{"TextRenderSystem", &TextRenderSystem::_CreateEmptyObject},
		{"RPGSkillHighlightEntry", &RPGSkillHighlightEntry::_CreateEmptyObject},
		{"RPGSkillEffectModuleTemp", &RPGSkillEffectModuleTemp::_CreateEmptyObject},
		{"RPGSkillAimModuleLine", &RPGSkillAimModuleLine::_CreateEmptyObject},
		{"RPGRaceData", &RPGRaceData::_CreateEmptyObject},
		{"RPGAttributeBase", &RPGAttributeBase::_CreateEmptyObject},
		{"InputSystem", &InputSystem::_CreateEmptyObject},
		{"InputContext", &InputContext::_CreateEmptyObject},
		{"InputActionPress", &InputActionPress::_CreateEmptyObject},
		{"ImGuiEditor", &ImGuiEditor::_CreateEmptyObject},
		{"ECSSystemRender", &ECSSystemRender::_CreateEmptyObject},
		{"ECSSystemPhysics", &ECSSystemPhysics::_CreateEmptyObject},
		{"ECSSystemNameslate", &ECSSystemNameslate::_CreateEmptyObject},
		{"ECSSystemEntityMap", &ECSSystemEntityMap::_CreateEmptyObject},
		{"ECSSystemDamage", &ECSSystemDamage::_CreateEmptyObject},
		{"ECSSystemCleanUp", &ECSSystemCleanUp::_CreateEmptyObject},
		{"ECS", &ECS::_CreateEmptyObject},
		{"HUDObjectBase", &HUDObjectBase::_CreateEmptyObject},
		{"HUD", &HUD::_CreateEmptyObject},
		{"RPGSkillSystem", &RPGSkillSystem::_CreateEmptyObject},
		{"RPGSystem", &RPGSystem::_CreateEmptyObject},
		{"RPGAttributeMaxHealth", &RPGAttributeMaxHealth::_CreateEmptyObject},
		{"HUDObjectHealth", &HUDObjectHealth::_CreateEmptyObject},
	};

	std::unordered_map<std::string, void (*)(void*, const std::vector<EditorTypePropertyBase*>&, int&)> stringToInitialiseExistingObjectFunction
	{
		{"MenuScreenClassSelectEntry", &MenuScreenClassSelectEntry::_InitFromPropertiesSubset},
		{"MenuScreenBase", &MenuScreenBase::_InitFromPropertiesSubset},
		{"HUDObjectSharedInitParams", &HUDObjectSharedInitParams::_InitFromPropertiesSubset},
		{"TextRenderCharacterData", &TextRenderCharacterData::_InitFromPropertiesSubset},
		{"TextboxParams", &TextboxParams::_InitFromPropertiesSubset},
		{"RPGSkillData", &RPGSkillData::_InitFromPropertiesSubset},
		{"RPGSkillEffectModuleBase", &RPGSkillEffectModuleBase::_InitFromPropertiesSubset},
		{"RPGSkillAimModuleBase", &RPGSkillAimModuleBase::_InitFromPropertiesSubset},
		{"RPGLevelProgressionData", &RPGLevelProgressionData::_InitFromPropertiesSubset},
		{"RPGRacePerLevelAttributeValues", &RPGRacePerLevelAttributeValues::_InitFromPropertiesSubset},
		{"RPGRaceInitialAttributeValues", &RPGRaceInitialAttributeValues::_InitFromPropertiesSubset},
		{"RPGClassSpecialisationData", &RPGClassSpecialisationData::_InitFromPropertiesSubset},
		{"RPGClassData", &RPGClassData::_InitFromPropertiesSubset},
		{"RPGAttributeModifierBase", &RPGAttributeModifierBase::_InitFromPropertiesSubset},
		{"InputActionBase", &InputActionBase::_InitFromPropertiesSubset},
		{"ECSSystemBase", &ECSSystemBase::_InitFromPropertiesSubset},
		{"ECSSystemAction", &ECSSystemAction::_InitFromPropertiesSubset},
		{"DirectoryData", &DirectoryData::_InitFromPropertiesSubset},
		{"GameSystem", &GameSystem::_InitFromPropertiesSubset},
		{"GameInstance", &GameInstance::_InitFromPropertiesSubset},
		{"CameraSystem", &CameraSystem::_InitFromPropertiesSubset},
		{"WorldGenerator", &WorldGenerator::_InitFromPropertiesSubset},
		{"MenuScreenMain", &MenuScreenMain::_InitFromPropertiesSubset},
		{"MenuScreenClassSelect", &MenuScreenClassSelect::_InitFromPropertiesSubset},
		{"MenuSystem", &MenuSystem::_InitFromPropertiesSubset},
		{"HUDAnchorPoint", &HUDAnchorPoint::_InitFromPropertiesSubset},
		{"TextRenderSystem", &TextRenderSystem::_InitFromPropertiesSubset},
		{"RPGSkillHighlightEntry", &RPGSkillHighlightEntry::_InitFromPropertiesSubset},
		{"RPGSkillEffectModuleTemp", &RPGSkillEffectModuleTemp::_InitFromPropertiesSubset},
		{"RPGSkillAimModuleLine", &RPGSkillAimModuleLine::_InitFromPropertiesSubset},
		{"RPGRaceData", &RPGRaceData::_InitFromPropertiesSubset},
		{"RPGAttributeBase", &RPGAttributeBase::_InitFromPropertiesSubset},
		{"InputSystem", &InputSystem::_InitFromPropertiesSubset},
		{"InputContext", &InputContext::_InitFromPropertiesSubset},
		{"InputActionPress", &InputActionPress::_InitFromPropertiesSubset},
		{"ImGuiEditor", &ImGuiEditor::_InitFromPropertiesSubset},
		{"ECSSystemRender", &ECSSystemRender::_InitFromPropertiesSubset},
		{"ECSSystemPhysics", &ECSSystemPhysics::_InitFromPropertiesSubset},
		{"ECSSystemNameslate", &ECSSystemNameslate::_InitFromPropertiesSubset},
		{"ECSSystemEntityMap", &ECSSystemEntityMap::_InitFromPropertiesSubset},
		{"ECSSystemDamage", &ECSSystemDamage::_InitFromPropertiesSubset},
		{"ECSSystemCleanUp", &ECSSystemCleanUp::_InitFromPropertiesSubset},
		{"ECS", &ECS::_InitFromPropertiesSubset},
		{"HUDObjectBase", &HUDObjectBase::_InitFromPropertiesSubset},
		{"HUD", &HUD::_InitFromPropertiesSubset},
		{"RPGSkillSystem", &RPGSkillSystem::_InitFromPropertiesSubset},
		{"RPGSystem", &RPGSystem::_InitFromPropertiesSubset},
		{"RPGAttributeMaxHealth", &RPGAttributeMaxHealth::_InitFromPropertiesSubset},
		{"HUDObjectHealth", &HUDObjectHealth::_InitFromPropertiesSubset},
	};
}
