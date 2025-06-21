#include "pch.h"
#include "__Generated.h"
#include "Editor/Types/Properties/EditorTypePropertyClass.h"
#include "Editor/Types/Properties/EditorTypePropertyFloat.h"
#include "Editor/Types/Properties/EditorTypePropertyInt.h"
#include "Editor/Types/Properties/EditorTypePropertyBool.h"
#include "Editor/Types/Properties/EditorTypePropertyString.h"
#include "Editor/Types/Properties/EditorTypePropertyStruct.h"
#include "Editor/Types/Properties/EditorTypePropertyVector.h"
#include "Editor/Types/Properties/EditorTypePropertyEnum.h"
#include "Editor/Types/Properties/EditorTypePropertyInstancedAssetPtr.h"
#include "Editor/Types/Properties/EditorTypePropertyDataCompositeProperty.h"
#include "..\Roguelike\AI\Actions\ActionDeciders\ActionDeciderAI.h"
#include "..\Roguelike\AI\Actions\ActionDeciders\ActionDeciderBase.h"
#include "..\Roguelike\AI\Actions\ActionDeciders\ActionDeciderPlayer.h"
#include "..\Roguelike\AI\Actions\ActionDeciders\ActionDeciderProjectile.h"
#include "..\Roguelike\AI\Actions\ActionHandlers\ActionHandlerAttack.h"
#include "..\Roguelike\AI\Actions\ActionHandlers\ActionHandlerBase.h"
#include "..\Roguelike\AI\Actions\ActionHandlers\ActionHandlerMove.h"
#include "..\Roguelike\AI\Actions\ActionHandlers\ActionHandlerProjectile.h"
#include "..\Roguelike\AI\Actions\ActionHandlers\ActionHandlerSkill.h"
#include "..\Roguelike\AI\Actions\ActionHandlers\ActionHandlerWait.h"
#include "..\Roguelike\Camera\CameraSystem.h"
#include "..\Roguelike\Core\DirectoryData.h"
#include "..\Roguelike\Core\GameInstance.h"
#include "..\Roguelike\Core\GameSystem.h"
#include "..\Roguelike\ECS\ECS.h"
#include "..\Roguelike\ECS\Systems\ECSSystemAction.h"
#include "..\Roguelike\ECS\Systems\ECSSystemBase.h"
#include "..\Roguelike\ECS\Systems\ECSSystemCleanUp.h"
#include "..\Roguelike\ECS\Systems\ECSSystemDamage.h"
#include "..\Roguelike\ECS\Systems\ECSSystemEntityMap.h"
#include "..\Roguelike\ECS\Systems\ECSSystemNameslate.h"
#include "..\Roguelike\ECS\Systems\ECSSystemPhysics.h"
#include "..\Roguelike\ECS\Systems\ECSSystemRender.h"
#include "..\Roguelike\Editor\ImGuiEditor.h"
#include "..\Roguelike\HotbarManager\HotbarManager.h"
#include "..\Roguelike\HotbarManager\IHotbarItem.h"
#include "..\Roguelike\HotbarManager\StatefulHotbarActions.h"
#include "..\Roguelike\Input\InputAction.h"
#include "..\Roguelike\Input\InputContext.h"
#include "..\Roguelike\Input\InputKey.h"
#include "..\Roguelike\Input\InputSystem.h"
#include "..\Roguelike\Input\MouseSystem.h"
#include "..\Roguelike\Performance\PerformanceMetricsManager.h"
#include "..\Roguelike\RPGSystems\Attributes\RPGAttributeModifiers.h"
#include "..\Roguelike\RPGSystems\Attributes\RPGAttributes.h"
#include "..\Roguelike\RPGSystems\Classes\RPGClassData.h"
#include "..\Roguelike\RPGSystems\Classes\RPGClassSpecialisationData.h"
#include "..\Roguelike\RPGSystems\Races\RPGRaceData.h"
#include "..\Roguelike\RPGSystems\RPGLevelScalingNumber.h"
#include "..\Roguelike\RPGSystems\RPGSystem.h"
#include "..\Roguelike\RPGSystems\Skills\AimModules\RPGSkillAimModuleBase.h"
#include "..\Roguelike\RPGSystems\Skills\AimModules\RPGSkillAimModuleLine.h"
#include "..\Roguelike\RPGSystems\Skills\EffectModules\RPGSkillEffectModuleBase.h"
#include "..\Roguelike\RPGSystems\Skills\EffectModules\RPGSkillEffectModuleDamage.h"
#include "..\Roguelike\RPGSystems\Skills\EffectModules\RPGSkillEffectModuleProjectile.h"
#include "..\Roguelike\RPGSystems\Skills\RPGSkillData.h"
#include "..\Roguelike\RPGSystems\Skills\RPGSkillsShared.h"
#include "..\Roguelike\RPGSystems\Skills\RPGSkillSystem.h"
#include "..\Roguelike\TestSuiteGameSystem.h"
#include "..\Roguelike\UI\DragAndDrop\DragAndDropManager.h"
#include "..\Roguelike\UI\DragAndDrop\IDragAndDroppable.h"
#include "..\Roguelike\UI\HUD\HUD.h"
#include "..\Roguelike\UI\HUD\HUDAnchorPoint.h"
#include "..\Roguelike\UI\HUD\HUDObjects\HUDObjectBase.h"
#include "..\Roguelike\UI\HUD\HUDObjects\HUDObjectHealth.h"
#include "..\Roguelike\UI\HUD\HUDObjects\HUDObjectHotbar.h"
#include "..\Roguelike\UI\HUD\HUDObjects\HUDObjectXP.h"
#include "..\Roguelike\UI\Menu\MenuSystem.h"
#include "..\Roguelike\UI\Menu\Screens\MenuScreenBase.h"
#include "..\Roguelike\UI\Menu\Screens\MenuScreenClassSelect.h"
#include "..\Roguelike\UI\Menu\Screens\MenuScreenMain.h"
#include "..\Roguelike\UI\Menu\Screens\MenuScreenSkillTree.h"
#include "..\Roguelike\UI\TextRenderSystem\TextRenderSystem.h"
#include "..\Roguelike\WorldGeneration\WorldGenerationContinent.h"
#include "..\Roguelike\WorldGeneration\WorldGenerationLogic.h"
#include "..\Roguelike\WorldGeneration\WorldGenerationTileDefinition.h"
#include "..\Roguelike\WorldGeneration\WorldGenerationUtils.h"
#include "..\Roguelike\WorldGeneration\WorldGenerator.h"

#pragma warning( disable : 4189 )

// BasicNoiseParams
void BasicNoiseParams::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	BasicNoiseParams* pBasicNoiseParams = static_cast<BasicNoiseParams*>(pObject);
	pBasicNoiseParams->gridSize = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pBasicNoiseParams->magnitude = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pBasicNoiseParams->gridModulus = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
}

void* BasicNoiseParams::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	BasicNoiseParams* pBasicNoiseParams = new BasicNoiseParams;
	int propertyIndex = 0;
	BasicNoiseParams::_InitFromPropertiesSubset(pBasicNoiseParams, properties, propertyIndex);
	return pBasicNoiseParams;
}

void* BasicNoiseParams::_CreateEmptyObject()
{
	return new BasicNoiseParams;
}

void BasicNoiseParams::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<BasicNoiseParams*>(pObject);
}

// WorldGenerationTileDefinition
void WorldGenerationTileDefinition::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	WorldGenerationTileDefinition* pWorldGenerationTileDefinition = static_cast<WorldGenerationTileDefinition*>(pObject);
	pWorldGenerationTileDefinition->fileName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pWorldGenerationTileDefinition->bRigidBody = static_cast<EditorTypePropertyBool*>(properties[propertyIndex++])->GetValue();
}

void* WorldGenerationTileDefinition::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	WorldGenerationTileDefinition* pWorldGenerationTileDefinition = new WorldGenerationTileDefinition;
	int propertyIndex = 0;
	WorldGenerationTileDefinition::_InitFromPropertiesSubset(pWorldGenerationTileDefinition, properties, propertyIndex);
	return pWorldGenerationTileDefinition;
}

void* WorldGenerationTileDefinition::_CreateEmptyObject()
{
	return new WorldGenerationTileDefinition;
}

void WorldGenerationTileDefinition::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<WorldGenerationTileDefinition*>(pObject);
}

// WorldGenerationLogicBase
void WorldGenerationLogicBase::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	WorldGenerationLogicBase* pWorldGenerationLogicBase = static_cast<WorldGenerationLogicBase*>(pObject);
	pWorldGenerationLogicBase->bMutateSeedAfterLogic = static_cast<EditorTypePropertyBool*>(properties[propertyIndex++])->GetValue();
}

void* WorldGenerationLogicBase::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	WorldGenerationLogicBase* pWorldGenerationLogicBase = new WorldGenerationLogicBase;
	int propertyIndex = 0;
	WorldGenerationLogicBase::_InitFromPropertiesSubset(pWorldGenerationLogicBase, properties, propertyIndex);
	return pWorldGenerationLogicBase;
}

void* WorldGenerationLogicBase::_CreateEmptyObject()
{
	return new WorldGenerationLogicBase;
}

void WorldGenerationLogicBase::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<WorldGenerationLogicBase*>(pObject);
}

// WorldGenerationEnemyParams
void WorldGenerationEnemyParams::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	WorldGenerationEnemyParams* pWorldGenerationEnemyParams = static_cast<WorldGenerationEnemyParams*>(pObject);
	pWorldGenerationEnemyParams->minDistanceFromPlayerSpawn = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pWorldGenerationEnemyParams->enemySpawnChanceAlpha = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pWorldGenerationEnemyParams->crabSpawnDistance = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pWorldGenerationEnemyParams->maxLevel = static_cast<EditorTypePropertyInt*>(properties[propertyIndex++])->GetValue();
	pWorldGenerationEnemyParams->maxLevelDistanceFromSpawn = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pWorldGenerationEnemyParams->pBanditRaceData = static_cast<RPGRaceData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pWorldGenerationEnemyParams->pCrabRaceData = static_cast<RPGRaceData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pWorldGenerationEnemyParams->pEnemyActionDecider = static_cast<ActionDeciderAI*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* WorldGenerationEnemyParams::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	WorldGenerationEnemyParams* pWorldGenerationEnemyParams = new WorldGenerationEnemyParams;
	int propertyIndex = 0;
	WorldGenerationEnemyParams::_InitFromPropertiesSubset(pWorldGenerationEnemyParams, properties, propertyIndex);
	return pWorldGenerationEnemyParams;
}

void* WorldGenerationEnemyParams::_CreateEmptyObject()
{
	return new WorldGenerationEnemyParams;
}

void WorldGenerationEnemyParams::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<WorldGenerationEnemyParams*>(pObject);
}

// WorldGenerationTreeParams
void WorldGenerationTreeParams::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	WorldGenerationTreeParams* pWorldGenerationTreeParams = static_cast<WorldGenerationTreeParams*>(pObject);
	pWorldGenerationTreeParams->distanceFromOceanTreesBegin = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pWorldGenerationTreeParams->distanceFromOceanTreesEnd = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pWorldGenerationTreeParams->treePercentageAtMinimumDistance = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pWorldGenerationTreeParams->treePercentageAtMaximumDistance = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
}

void* WorldGenerationTreeParams::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	WorldGenerationTreeParams* pWorldGenerationTreeParams = new WorldGenerationTreeParams;
	int propertyIndex = 0;
	WorldGenerationTreeParams::_InitFromPropertiesSubset(pWorldGenerationTreeParams, properties, propertyIndex);
	return pWorldGenerationTreeParams;
}

void* WorldGenerationTreeParams::_CreateEmptyObject()
{
	return new WorldGenerationTreeParams;
}

void WorldGenerationTreeParams::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<WorldGenerationTreeParams*>(pObject);
}

// WorldGenerationSandParams
void WorldGenerationSandParams::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	WorldGenerationSandParams* pWorldGenerationSandParams = static_cast<WorldGenerationSandParams*>(pObject);
	pWorldGenerationSandParams->baseDistanceFromOcean = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pWorldGenerationSandParams->pSandDistanceFromShoreDeltaLogic = static_cast<WorldGenerationLogicBase*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* WorldGenerationSandParams::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	WorldGenerationSandParams* pWorldGenerationSandParams = new WorldGenerationSandParams;
	int propertyIndex = 0;
	WorldGenerationSandParams::_InitFromPropertiesSubset(pWorldGenerationSandParams, properties, propertyIndex);
	return pWorldGenerationSandParams;
}

void* WorldGenerationSandParams::_CreateEmptyObject()
{
	return new WorldGenerationSandParams;
}

void WorldGenerationSandParams::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<WorldGenerationSandParams*>(pObject);
}

// WorldGenerationShorelineParams
void WorldGenerationShorelineParams::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	WorldGenerationShorelineParams* pWorldGenerationShorelineParams = static_cast<WorldGenerationShorelineParams*>(pObject);
	pWorldGenerationShorelineParams->baseDistanceFromEdge = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pWorldGenerationShorelineParams->maxShorelineAngle = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pWorldGenerationShorelineParams->pShorelineDistanceDeltaLogic = static_cast<WorldGenerationLogicBase*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* WorldGenerationShorelineParams::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	WorldGenerationShorelineParams* pWorldGenerationShorelineParams = new WorldGenerationShorelineParams;
	int propertyIndex = 0;
	WorldGenerationShorelineParams::_InitFromPropertiesSubset(pWorldGenerationShorelineParams, properties, propertyIndex);
	return pWorldGenerationShorelineParams;
}

void* WorldGenerationShorelineParams::_CreateEmptyObject()
{
	return new WorldGenerationShorelineParams;
}

void WorldGenerationShorelineParams::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<WorldGenerationShorelineParams*>(pObject);
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

void TextRenderCharacterData::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<TextRenderCharacterData*>(pObject);
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

void TextboxParams::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<TextboxParams*>(pObject);
}

// SkillTreeMenuSkillSlotData
void SkillTreeMenuSkillSlotData::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	SkillTreeMenuSkillSlotData* pSkillTreeMenuSkillSlotData = static_cast<SkillTreeMenuSkillSlotData*>(pObject);
	pSkillTreeMenuSkillSlotData->buttonLayerName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
}

void* SkillTreeMenuSkillSlotData::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	SkillTreeMenuSkillSlotData* pSkillTreeMenuSkillSlotData = new SkillTreeMenuSkillSlotData;
	int propertyIndex = 0;
	SkillTreeMenuSkillSlotData::_InitFromPropertiesSubset(pSkillTreeMenuSkillSlotData, properties, propertyIndex);
	return pSkillTreeMenuSkillSlotData;
}

void* SkillTreeMenuSkillSlotData::_CreateEmptyObject()
{
	return new SkillTreeMenuSkillSlotData;
}

void SkillTreeMenuSkillSlotData::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<SkillTreeMenuSkillSlotData*>(pObject);
}

// MainMenuButtonData
void MainMenuButtonData::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	MainMenuButtonData* pMainMenuButtonData = static_cast<MainMenuButtonData*>(pObject);
	pMainMenuButtonData->buttonLayerName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pMainMenuButtonData->pMenuScreenToOpen.SetAsset(static_cast<EditorTypePropertyInstancedAssetPtr*>(properties[propertyIndex++])->GetValue());
}

void* MainMenuButtonData::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	MainMenuButtonData* pMainMenuButtonData = new MainMenuButtonData;
	int propertyIndex = 0;
	MainMenuButtonData::_InitFromPropertiesSubset(pMainMenuButtonData, properties, propertyIndex);
	return pMainMenuButtonData;
}

void* MainMenuButtonData::_CreateEmptyObject()
{
	return new MainMenuButtonData;
}

void MainMenuButtonData::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<MainMenuButtonData*>(pObject);
}

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

void MenuScreenClassSelectEntry::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<MenuScreenClassSelectEntry*>(pObject);
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

void MenuScreenBase::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<MenuScreenBase*>(pObject);
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

void HUDObjectSharedInitParams::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<HUDObjectSharedInitParams*>(pObject);
}

// IDragAndDroppable
void IDragAndDroppable::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	IDragAndDroppable* pIDragAndDroppable = static_cast<IDragAndDroppable*>(pObject);
}

void* IDragAndDroppable::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	IDragAndDroppable* pIDragAndDroppable = new IDragAndDroppable;
	int propertyIndex = 0;
	IDragAndDroppable::_InitFromPropertiesSubset(pIDragAndDroppable, properties, propertyIndex);
	return pIDragAndDroppable;
}

void* IDragAndDroppable::_CreateEmptyObject()
{
	return new IDragAndDroppable;
}

void IDragAndDroppable::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<IDragAndDroppable*>(pObject);
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

void RPGSkillEffectModuleBase::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGSkillEffectModuleBase*>(pObject);
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

void RPGSkillAimModuleBase::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGSkillAimModuleBase*>(pObject);
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

void RPGLevelProgressionData::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGLevelProgressionData*>(pObject);
}

// RPGRacePerLevelAttributeValues
void RPGRacePerLevelAttributeValues::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGRacePerLevelAttributeValues* pRPGRacePerLevelAttributeValues = static_cast<RPGRacePerLevelAttributeValues*>(pObject);
	pRPGRacePerLevelAttributeValues->maxHealthMultiplierPerLevel = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pRPGRacePerLevelAttributeValues->spellDamageMultiplierPerLevel = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
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

void RPGRacePerLevelAttributeValues::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGRacePerLevelAttributeValues*>(pObject);
}

// RPGRaceInitialAttributeValues
void RPGRaceInitialAttributeValues::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGRaceInitialAttributeValues* pRPGRaceInitialAttributeValues = static_cast<RPGRaceInitialAttributeValues*>(pObject);
	pRPGRaceInitialAttributeValues->maxHealth = static_cast<EditorTypePropertyInt*>(properties[propertyIndex++])->GetValue();
	pRPGRaceInitialAttributeValues->spellDamage = static_cast<EditorTypePropertyInt*>(properties[propertyIndex++])->GetValue();
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

void RPGRaceInitialAttributeValues::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGRaceInitialAttributeValues*>(pObject);
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

void RPGClassSpecialisationData::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGClassSpecialisationData*>(pObject);
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

void RPGAttributeModifierBase::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGAttributeModifierBase*>(pObject);
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

void InputActionBase::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<InputActionBase*>(pObject);
}

// StatefulHotbarActionBase
void StatefulHotbarActionBase::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	StatefulHotbarActionBase* pStatefulHotbarActionBase = static_cast<StatefulHotbarActionBase*>(pObject);
}

void* StatefulHotbarActionBase::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	StatefulHotbarActionBase* pStatefulHotbarActionBase = new StatefulHotbarActionBase;
	int propertyIndex = 0;
	StatefulHotbarActionBase::_InitFromPropertiesSubset(pStatefulHotbarActionBase, properties, propertyIndex);
	return pStatefulHotbarActionBase;
}

void* StatefulHotbarActionBase::_CreateEmptyObject()
{
	return new StatefulHotbarActionBase;
}

void StatefulHotbarActionBase::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<StatefulHotbarActionBase*>(pObject);
}

// IHotbarItem
void IHotbarItem::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	IHotbarItem* pIHotbarItem = static_cast<IHotbarItem*>(pObject);
}

void* IHotbarItem::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	IHotbarItem* pIHotbarItem = new IHotbarItem;
	int propertyIndex = 0;
	IHotbarItem::_InitFromPropertiesSubset(pIHotbarItem, properties, propertyIndex);
	return pIHotbarItem;
}

void* IHotbarItem::_CreateEmptyObject()
{
	return new IHotbarItem;
}

void IHotbarItem::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<IHotbarItem*>(pObject);
}

// HotbarSlot
void HotbarSlot::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	HotbarSlot* pHotbarSlot = static_cast<HotbarSlot*>(pObject);
	pHotbarSlot->pInputAction = static_cast<InputActionBase*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* HotbarSlot::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	HotbarSlot* pHotbarSlot = new HotbarSlot;
	int propertyIndex = 0;
	HotbarSlot::_InitFromPropertiesSubset(pHotbarSlot, properties, propertyIndex);
	return pHotbarSlot;
}

void* HotbarSlot::_CreateEmptyObject()
{
	return new HotbarSlot;
}

void HotbarSlot::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<HotbarSlot*>(pObject);
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

void ECSSystemBase::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ECSSystemBase*>(pObject);
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

void ECSSystemAction::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ECSSystemAction*>(pObject);
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

void GameSystem::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<GameSystem*>(pObject);
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

void GameInstance::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<GameInstance*>(pObject);
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
	pDirectoryData->worldGenerationTiles = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
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

void DirectoryData::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<DirectoryData*>(pObject);
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

void CameraSystem::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<CameraSystem*>(pObject);
}

// ActionHandlerBase
void ActionHandlerBase::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ActionHandlerBase* pActionHandlerBase = static_cast<ActionHandlerBase*>(pObject);
	pActionHandlerBase->bCanThisHappenOnTheSameFrameAsOtherActions = static_cast<EditorTypePropertyBool*>(properties[propertyIndex++])->GetValue();
}

void* ActionHandlerBase::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ActionHandlerBase* pActionHandlerBase = new ActionHandlerBase;
	int propertyIndex = 0;
	ActionHandlerBase::_InitFromPropertiesSubset(pActionHandlerBase, properties, propertyIndex);
	return pActionHandlerBase;
}

void* ActionHandlerBase::_CreateEmptyObject()
{
	return new ActionHandlerBase;
}

void ActionHandlerBase::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ActionHandlerBase*>(pObject);
}

// ActionHandlerAttack
void ActionHandlerAttack::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ActionHandlerAttack* pActionHandlerAttack = static_cast<ActionHandlerAttack*>(pObject);
	ActionHandlerBase::_InitFromPropertiesSubset(static_cast<ActionHandlerBase*>(pActionHandlerAttack), properties, propertyIndex);
	pActionHandlerAttack->pRpgSystem = static_cast<RPGSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* ActionHandlerAttack::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ActionHandlerAttack* pActionHandlerAttack = new ActionHandlerAttack;
	int propertyIndex = 0;
	ActionHandlerAttack::_InitFromPropertiesSubset(pActionHandlerAttack, properties, propertyIndex);
	return pActionHandlerAttack;
}

void* ActionHandlerAttack::_CreateEmptyObject()
{
	return new ActionHandlerAttack;
}

void ActionHandlerAttack::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ActionHandlerAttack*>(pObject);
}

// ActionDeciderBase
void ActionDeciderBase::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ActionDeciderBase* pActionDeciderBase = static_cast<ActionDeciderBase*>(pObject);
	pActionDeciderBase->pAttackAction = static_cast<ActionHandlerAttack*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pActionDeciderBase->pMoveAction = static_cast<ActionHandlerMove*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* ActionDeciderBase::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ActionDeciderBase* pActionDeciderBase = new ActionDeciderBase;
	int propertyIndex = 0;
	ActionDeciderBase::_InitFromPropertiesSubset(pActionDeciderBase, properties, propertyIndex);
	return pActionDeciderBase;
}

void* ActionDeciderBase::_CreateEmptyObject()
{
	return new ActionDeciderBase;
}

void ActionDeciderBase::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ActionDeciderBase*>(pObject);
}

// ActionDeciderAI
void ActionDeciderAI::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ActionDeciderAI* pActionDeciderAI = static_cast<ActionDeciderAI*>(pObject);
	ActionDeciderBase::_InitFromPropertiesSubset(static_cast<ActionDeciderBase*>(pActionDeciderAI), properties, propertyIndex);
	pActionDeciderAI->pWaitAction = static_cast<ActionHandlerWait*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pActionDeciderAI->aggroRange = static_cast<EditorTypePropertyInt*>(properties[propertyIndex++])->GetValue();
}

void* ActionDeciderAI::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ActionDeciderAI* pActionDeciderAI = new ActionDeciderAI;
	int propertyIndex = 0;
	ActionDeciderAI::_InitFromPropertiesSubset(pActionDeciderAI, properties, propertyIndex);
	return pActionDeciderAI;
}

void* ActionDeciderAI::_CreateEmptyObject()
{
	return new ActionDeciderAI;
}

void ActionDeciderAI::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ActionDeciderAI*>(pObject);
}

// WorldGenerator
void WorldGenerator::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	WorldGenerator* pWorldGenerator = static_cast<WorldGenerator*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pWorldGenerator), properties, propertyIndex);
	pWorldGenerator->pEcs = static_cast<ECS*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pWorldGenerator->pContinent = static_cast<WorldGenerationContinent*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
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

void WorldGenerator::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<WorldGenerator*>(pObject);
}

// WorldGenerationLogicBasicNoise1D
void WorldGenerationLogicBasicNoise1D::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	WorldGenerationLogicBasicNoise1D* pWorldGenerationLogicBasicNoise1D = static_cast<WorldGenerationLogicBasicNoise1D*>(pObject);
	WorldGenerationLogicBase::_InitFromPropertiesSubset(static_cast<WorldGenerationLogicBase*>(pWorldGenerationLogicBasicNoise1D), properties, propertyIndex);
	{
		BasicNoiseParams* temp = static_cast<BasicNoiseParams*>(static_cast<EditorTypePropertyStruct*>(properties[propertyIndex++])->GetValue());
		pWorldGenerationLogicBasicNoise1D->params = *temp;
		delete temp;
	}
}

void* WorldGenerationLogicBasicNoise1D::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	WorldGenerationLogicBasicNoise1D* pWorldGenerationLogicBasicNoise1D = new WorldGenerationLogicBasicNoise1D;
	int propertyIndex = 0;
	WorldGenerationLogicBasicNoise1D::_InitFromPropertiesSubset(pWorldGenerationLogicBasicNoise1D, properties, propertyIndex);
	return pWorldGenerationLogicBasicNoise1D;
}

void* WorldGenerationLogicBasicNoise1D::_CreateEmptyObject()
{
	return new WorldGenerationLogicBasicNoise1D;
}

void WorldGenerationLogicBasicNoise1D::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<WorldGenerationLogicBasicNoise1D*>(pObject);
}

// WorldGenerationLogicAdd
void WorldGenerationLogicAdd::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	WorldGenerationLogicAdd* pWorldGenerationLogicAdd = static_cast<WorldGenerationLogicAdd*>(pObject);
	WorldGenerationLogicBase::_InitFromPropertiesSubset(static_cast<WorldGenerationLogicBase*>(pWorldGenerationLogicAdd), properties, propertyIndex);
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pWorldGenerationLogicAdd->pLogicToAdd.push_back(static_cast<WorldGenerationLogicBase*>(static_cast<EditorTypePropertyClass*>(instancedProperty.get())->GetValue()));
		}
	}
}

void* WorldGenerationLogicAdd::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	WorldGenerationLogicAdd* pWorldGenerationLogicAdd = new WorldGenerationLogicAdd;
	int propertyIndex = 0;
	WorldGenerationLogicAdd::_InitFromPropertiesSubset(pWorldGenerationLogicAdd, properties, propertyIndex);
	return pWorldGenerationLogicAdd;
}

void* WorldGenerationLogicAdd::_CreateEmptyObject()
{
	return new WorldGenerationLogicAdd;
}

void WorldGenerationLogicAdd::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<WorldGenerationLogicAdd*>(pObject);
}

// WorldGenerationTileEntry
void WorldGenerationTileEntry::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	WorldGenerationTileEntry* pWorldGenerationTileEntry = static_cast<WorldGenerationTileEntry*>(pObject);
	pWorldGenerationTileEntry->type = static_cast<EWorldGenerationTile>(static_cast<EditorTypePropertyEnum*>(properties[propertyIndex++])->GetValue());
	pWorldGenerationTileEntry->pDefinition = static_cast<WorldGenerationTileDefinition*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* WorldGenerationTileEntry::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	WorldGenerationTileEntry* pWorldGenerationTileEntry = new WorldGenerationTileEntry;
	int propertyIndex = 0;
	WorldGenerationTileEntry::_InitFromPropertiesSubset(pWorldGenerationTileEntry, properties, propertyIndex);
	return pWorldGenerationTileEntry;
}

void* WorldGenerationTileEntry::_CreateEmptyObject()
{
	return new WorldGenerationTileEntry;
}

void WorldGenerationTileEntry::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<WorldGenerationTileEntry*>(pObject);
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
			TextRenderCharacterData* temp = static_cast<TextRenderCharacterData*>(static_cast<EditorTypePropertyStruct*>(instancedProperty.get())->GetValue());
			pTextRenderSystem->characterDatas.push_back(*temp);
			delete temp;
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

void TextRenderSystem::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<TextRenderSystem*>(pObject);
}

// MenuScreenSkillTree
void MenuScreenSkillTree::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	MenuScreenSkillTree* pMenuScreenSkillTree = static_cast<MenuScreenSkillTree*>(pObject);
	MenuScreenBase::_InitFromPropertiesSubset(static_cast<MenuScreenBase*>(pMenuScreenSkillTree), properties, propertyIndex);
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			SkillTreeMenuSkillSlotData* temp = static_cast<SkillTreeMenuSkillSlotData*>(static_cast<EditorTypePropertyStruct*>(instancedProperty.get())->GetValue());
			pMenuScreenSkillTree->skillSlotDatas.push_back(*temp);
			delete temp;
		}
	}
	pMenuScreenSkillTree->pDragAndDropManager = static_cast<DragAndDropManager*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pMenuScreenSkillTree->pEcs = static_cast<ECS*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* MenuScreenSkillTree::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	MenuScreenSkillTree* pMenuScreenSkillTree = new MenuScreenSkillTree;
	int propertyIndex = 0;
	MenuScreenSkillTree::_InitFromPropertiesSubset(pMenuScreenSkillTree, properties, propertyIndex);
	return pMenuScreenSkillTree;
}

void* MenuScreenSkillTree::_CreateEmptyObject()
{
	return new MenuScreenSkillTree;
}

void MenuScreenSkillTree::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<MenuScreenSkillTree*>(pObject);
}

// MenuScreenMain
void MenuScreenMain::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	MenuScreenMain* pMenuScreenMain = static_cast<MenuScreenMain*>(pObject);
	MenuScreenBase::_InitFromPropertiesSubset(static_cast<MenuScreenBase*>(pMenuScreenMain), properties, propertyIndex);
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			MainMenuButtonData* temp = static_cast<MainMenuButtonData*>(static_cast<EditorTypePropertyStruct*>(instancedProperty.get())->GetValue());
			pMenuScreenMain->buttonDatas.push_back(*temp);
			delete temp;
		}
	}
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

void MenuScreenMain::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<MenuScreenMain*>(pObject);
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
			MenuScreenClassSelectEntry* temp = static_cast<MenuScreenClassSelectEntry*>(static_cast<EditorTypePropertyStruct*>(instancedProperty.get())->GetValue());
			pMenuScreenClassSelect->classEntries.push_back(*temp);
			delete temp;
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

void MenuScreenClassSelect::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<MenuScreenClassSelect*>(pObject);
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
	pMenuSystem->menuSizeFraction = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
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

void MenuSystem::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<MenuSystem*>(pObject);
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

void HUDAnchorPoint::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<HUDAnchorPoint*>(pObject);
}

// DragAndDropManager
void DragAndDropManager::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	DragAndDropManager* pDragAndDropManager = static_cast<DragAndDropManager*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pDragAndDropManager), properties, propertyIndex);
	pDragAndDropManager->floatingIconSize = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pDragAndDropManager->floatingIconFileName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pDragAndDropManager->iconLoaderLayerName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pDragAndDropManager->floatingIconXOffset = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pDragAndDropManager->floatingIconYOffset = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pDragAndDropManager->pDirectoryData = static_cast<DirectoryData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* DragAndDropManager::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	DragAndDropManager* pDragAndDropManager = new DragAndDropManager;
	int propertyIndex = 0;
	DragAndDropManager::_InitFromPropertiesSubset(pDragAndDropManager, properties, propertyIndex);
	return pDragAndDropManager;
}

void* DragAndDropManager::_CreateEmptyObject()
{
	return new DragAndDropManager;
}

void DragAndDropManager::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<DragAndDropManager*>(pObject);
}

// TestSuiteGameSystem
void TestSuiteGameSystem::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	TestSuiteGameSystem* pTestSuiteGameSystem = static_cast<TestSuiteGameSystem*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pTestSuiteGameSystem), properties, propertyIndex);
}

void* TestSuiteGameSystem::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	TestSuiteGameSystem* pTestSuiteGameSystem = new TestSuiteGameSystem;
	int propertyIndex = 0;
	TestSuiteGameSystem::_InitFromPropertiesSubset(pTestSuiteGameSystem, properties, propertyIndex);
	return pTestSuiteGameSystem;
}

void* TestSuiteGameSystem::_CreateEmptyObject()
{
	return new TestSuiteGameSystem;
}

void TestSuiteGameSystem::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<TestSuiteGameSystem*>(pObject);
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

void RPGSkillHighlightEntry::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGSkillHighlightEntry*>(pObject);
}

// RPGSkillData
void RPGSkillData::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGSkillData* pRPGSkillData = static_cast<RPGSkillData*>(pObject);
	IDragAndDroppable::_InitFromPropertiesSubset(static_cast<IDragAndDroppable*>(pRPGSkillData), properties, propertyIndex);
	IHotbarItem::_InitFromPropertiesSubset(static_cast<IHotbarItem*>(pRPGSkillData), properties, propertyIndex);
	pRPGSkillData->iconLayerName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
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

void RPGSkillData::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGSkillData*>(pObject);
}

// RPGSkillEffectModuleProjectile
void RPGSkillEffectModuleProjectile::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGSkillEffectModuleProjectile* pRPGSkillEffectModuleProjectile = static_cast<RPGSkillEffectModuleProjectile*>(pObject);
	RPGSkillEffectModuleBase::_InitFromPropertiesSubset(static_cast<RPGSkillEffectModuleBase*>(pRPGSkillEffectModuleProjectile), properties, propertyIndex);
	pRPGSkillEffectModuleProjectile->projectileFileName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pRPGSkillEffectModuleProjectile->pActionDecider = static_cast<ActionDeciderProjectile*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pRPGSkillEffectModuleProjectile->speed = static_cast<EditorTypePropertyDataCompositeProperty*>(properties[propertyIndex++])->GetValue();
	pRPGSkillEffectModuleProjectile->pProjectileEffect = static_cast<RPGSkillEffectModuleBase*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* RPGSkillEffectModuleProjectile::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGSkillEffectModuleProjectile* pRPGSkillEffectModuleProjectile = new RPGSkillEffectModuleProjectile;
	int propertyIndex = 0;
	RPGSkillEffectModuleProjectile::_InitFromPropertiesSubset(pRPGSkillEffectModuleProjectile, properties, propertyIndex);
	return pRPGSkillEffectModuleProjectile;
}

void* RPGSkillEffectModuleProjectile::_CreateEmptyObject()
{
	return new RPGSkillEffectModuleProjectile;
}

void RPGSkillEffectModuleProjectile::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGSkillEffectModuleProjectile*>(pObject);
}

// RPGSkillEffectModuleDamage
void RPGSkillEffectModuleDamage::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGSkillEffectModuleDamage* pRPGSkillEffectModuleDamage = static_cast<RPGSkillEffectModuleDamage*>(pObject);
	RPGSkillEffectModuleBase::_InitFromPropertiesSubset(static_cast<RPGSkillEffectModuleBase*>(pRPGSkillEffectModuleDamage), properties, propertyIndex);
	pRPGSkillEffectModuleDamage->damage = static_cast<EditorTypePropertyDataCompositeProperty*>(properties[propertyIndex++])->GetValue();
}

void* RPGSkillEffectModuleDamage::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGSkillEffectModuleDamage* pRPGSkillEffectModuleDamage = new RPGSkillEffectModuleDamage;
	int propertyIndex = 0;
	RPGSkillEffectModuleDamage::_InitFromPropertiesSubset(pRPGSkillEffectModuleDamage, properties, propertyIndex);
	return pRPGSkillEffectModuleDamage;
}

void* RPGSkillEffectModuleDamage::_CreateEmptyObject()
{
	return new RPGSkillEffectModuleDamage;
}

void RPGSkillEffectModuleDamage::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGSkillEffectModuleDamage*>(pObject);
}

// RPGSkillAimModuleLine
void RPGSkillAimModuleLine::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGSkillAimModuleLine* pRPGSkillAimModuleLine = static_cast<RPGSkillAimModuleLine*>(pObject);
	RPGSkillAimModuleBase::_InitFromPropertiesSubset(static_cast<RPGSkillAimModuleBase*>(pRPGSkillAimModuleLine), properties, propertyIndex);
	pRPGSkillAimModuleLine->maxRange = static_cast<EditorTypePropertyDataCompositeProperty*>(properties[propertyIndex++])->GetValue();
	pRPGSkillAimModuleLine->areaRadius = static_cast<EditorTypePropertyDataCompositeProperty*>(properties[propertyIndex++])->GetValue();
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

void RPGSkillAimModuleLine::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGSkillAimModuleLine*>(pObject);
}

// RPGDamageMagnitude
void RPGDamageMagnitude::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGDamageMagnitude* pRPGDamageMagnitude = static_cast<RPGDamageMagnitude*>(pObject);
	pRPGDamageMagnitude->damageType = static_cast<EDamageType>(static_cast<EditorTypePropertyEnum*>(properties[propertyIndex++])->GetValue());
	pRPGDamageMagnitude->baseDamage = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pRPGDamageMagnitude->scaling = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
}

void* RPGDamageMagnitude::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGDamageMagnitude* pRPGDamageMagnitude = new RPGDamageMagnitude;
	int propertyIndex = 0;
	RPGDamageMagnitude::_InitFromPropertiesSubset(pRPGDamageMagnitude, properties, propertyIndex);
	return pRPGDamageMagnitude;
}

void* RPGDamageMagnitude::_CreateEmptyObject()
{
	return new RPGDamageMagnitude;
}

void RPGDamageMagnitude::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGDamageMagnitude*>(pObject);
}

// RPGLevelScalingNumber
void RPGLevelScalingNumber::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGLevelScalingNumber* pRPGLevelScalingNumber = static_cast<RPGLevelScalingNumber*>(pObject);
	pRPGLevelScalingNumber->scalingAlgorithm = static_cast<ELevelScalingAlgorithm>(static_cast<EditorTypePropertyEnum*>(properties[propertyIndex++])->GetValue());
	pRPGLevelScalingNumber->baseValue = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pRPGLevelScalingNumber->perLevelScaleFactor = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
}

void* RPGLevelScalingNumber::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGLevelScalingNumber* pRPGLevelScalingNumber = new RPGLevelScalingNumber;
	int propertyIndex = 0;
	RPGLevelScalingNumber::_InitFromPropertiesSubset(pRPGLevelScalingNumber, properties, propertyIndex);
	return pRPGLevelScalingNumber;
}

void* RPGLevelScalingNumber::_CreateEmptyObject()
{
	return new RPGLevelScalingNumber;
}

void RPGLevelScalingNumber::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGLevelScalingNumber*>(pObject);
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

void RPGRaceData::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGRaceData*>(pObject);
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
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pRPGClassData->classSkills.push_back(static_cast<RPGSkillData*>(static_cast<EditorTypePropertyClass*>(instancedProperty.get())->GetValue()));
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

void RPGClassData::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGClassData*>(pObject);
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

void RPGAttributeBase::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGAttributeBase*>(pObject);
}

// PerformanceMetricsManager
void PerformanceMetricsManager::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	PerformanceMetricsManager* pPerformanceMetricsManager = static_cast<PerformanceMetricsManager*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pPerformanceMetricsManager), properties, propertyIndex);
}

void* PerformanceMetricsManager::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	PerformanceMetricsManager* pPerformanceMetricsManager = new PerformanceMetricsManager;
	int propertyIndex = 0;
	PerformanceMetricsManager::_InitFromPropertiesSubset(pPerformanceMetricsManager, properties, propertyIndex);
	return pPerformanceMetricsManager;
}

void* PerformanceMetricsManager::_CreateEmptyObject()
{
	return new PerformanceMetricsManager;
}

void PerformanceMetricsManager::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<PerformanceMetricsManager*>(pObject);
}

// MouseSystem
void MouseSystem::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	MouseSystem* pMouseSystem = static_cast<MouseSystem*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pMouseSystem), properties, propertyIndex);
}

void* MouseSystem::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	MouseSystem* pMouseSystem = new MouseSystem;
	int propertyIndex = 0;
	MouseSystem::_InitFromPropertiesSubset(pMouseSystem, properties, propertyIndex);
	return pMouseSystem;
}

void* MouseSystem::_CreateEmptyObject()
{
	return new MouseSystem;
}

void MouseSystem::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<MouseSystem*>(pObject);
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

void InputSystem::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<InputSystem*>(pObject);
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

void InputContext::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<InputContext*>(pObject);
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

void InputActionPress::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<InputActionPress*>(pObject);
}

// StatefulHotbarActionCastSkill
void StatefulHotbarActionCastSkill::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	StatefulHotbarActionCastSkill* pStatefulHotbarActionCastSkill = static_cast<StatefulHotbarActionCastSkill*>(pObject);
	StatefulHotbarActionBase::_InitFromPropertiesSubset(static_cast<StatefulHotbarActionBase*>(pStatefulHotbarActionCastSkill), properties, propertyIndex);
	pStatefulHotbarActionCastSkill->pSkillSystem = static_cast<RPGSkillSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pStatefulHotbarActionCastSkill->pInputSystem = static_cast<InputSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pStatefulHotbarActionCastSkill->pAimingSkillInputContext = static_cast<InputContext*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pStatefulHotbarActionCastSkill->pConfirmSkillAction = static_cast<InputActionBase*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pStatefulHotbarActionCastSkill->pCancelSkillAction = static_cast<InputActionBase*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pStatefulHotbarActionCastSkill->pActionHandlerSkill = static_cast<ActionHandlerSkill*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* StatefulHotbarActionCastSkill::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	StatefulHotbarActionCastSkill* pStatefulHotbarActionCastSkill = new StatefulHotbarActionCastSkill;
	int propertyIndex = 0;
	StatefulHotbarActionCastSkill::_InitFromPropertiesSubset(pStatefulHotbarActionCastSkill, properties, propertyIndex);
	return pStatefulHotbarActionCastSkill;
}

void* StatefulHotbarActionCastSkill::_CreateEmptyObject()
{
	return new StatefulHotbarActionCastSkill;
}

void StatefulHotbarActionCastSkill::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<StatefulHotbarActionCastSkill*>(pObject);
}

// HotbarManager
void HotbarManager::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	HotbarManager* pHotbarManager = static_cast<HotbarManager*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pHotbarManager), properties, propertyIndex);
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			HotbarSlot* temp = static_cast<HotbarSlot*>(static_cast<EditorTypePropertyStruct*>(instancedProperty.get())->GetValue());
			pHotbarManager->hotbarSlots.push_back(*temp);
			delete temp;
		}
	}
}

void* HotbarManager::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	HotbarManager* pHotbarManager = new HotbarManager;
	int propertyIndex = 0;
	HotbarManager::_InitFromPropertiesSubset(pHotbarManager, properties, propertyIndex);
	return pHotbarManager;
}

void* HotbarManager::_CreateEmptyObject()
{
	return new HotbarManager;
}

void HotbarManager::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<HotbarManager*>(pObject);
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

void ImGuiEditor::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ImGuiEditor*>(pObject);
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

void ECSSystemRender::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ECSSystemRender*>(pObject);
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

void ECSSystemPhysics::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ECSSystemPhysics*>(pObject);
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

void ECSSystemNameslate::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ECSSystemNameslate*>(pObject);
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

void ECSSystemEntityMap::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ECSSystemEntityMap*>(pObject);
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

void ECSSystemDamage::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ECSSystemDamage*>(pObject);
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

void ECSSystemCleanUp::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ECSSystemCleanUp*>(pObject);
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
			pECS->pSystems.push_back(static_cast<ECSSystemBase*>(static_cast<EditorTypePropertyClass*>(instancedProperty.get())->GetValue()));
		}
	}
	pECS->pPlayerActionDecider = static_cast<ActionDeciderPlayer*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
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

void ECS::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ECS*>(pObject);
}

// ActionHandlerWait
void ActionHandlerWait::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ActionHandlerWait* pActionHandlerWait = static_cast<ActionHandlerWait*>(pObject);
	ActionHandlerBase::_InitFromPropertiesSubset(static_cast<ActionHandlerBase*>(pActionHandlerWait), properties, propertyIndex);
}

void* ActionHandlerWait::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ActionHandlerWait* pActionHandlerWait = new ActionHandlerWait;
	int propertyIndex = 0;
	ActionHandlerWait::_InitFromPropertiesSubset(pActionHandlerWait, properties, propertyIndex);
	return pActionHandlerWait;
}

void* ActionHandlerWait::_CreateEmptyObject()
{
	return new ActionHandlerWait;
}

void ActionHandlerWait::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ActionHandlerWait*>(pObject);
}

// ActionHandlerSkill
void ActionHandlerSkill::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ActionHandlerSkill* pActionHandlerSkill = static_cast<ActionHandlerSkill*>(pObject);
	ActionHandlerBase::_InitFromPropertiesSubset(static_cast<ActionHandlerBase*>(pActionHandlerSkill), properties, propertyIndex);
	pActionHandlerSkill->pSkillSystem = static_cast<RPGSkillSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* ActionHandlerSkill::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ActionHandlerSkill* pActionHandlerSkill = new ActionHandlerSkill;
	int propertyIndex = 0;
	ActionHandlerSkill::_InitFromPropertiesSubset(pActionHandlerSkill, properties, propertyIndex);
	return pActionHandlerSkill;
}

void* ActionHandlerSkill::_CreateEmptyObject()
{
	return new ActionHandlerSkill;
}

void ActionHandlerSkill::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ActionHandlerSkill*>(pObject);
}

// ActionHandlerProjectile
void ActionHandlerProjectile::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ActionHandlerProjectile* pActionHandlerProjectile = static_cast<ActionHandlerProjectile*>(pObject);
	ActionHandlerBase::_InitFromPropertiesSubset(static_cast<ActionHandlerBase*>(pActionHandlerProjectile), properties, propertyIndex);
	pActionHandlerProjectile->pEntityMapSystem = static_cast<ECSSystemEntityMap*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pActionHandlerProjectile->pRpgSystem = static_cast<RPGSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pActionHandlerProjectile->pDirectoryData = static_cast<DirectoryData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* ActionHandlerProjectile::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ActionHandlerProjectile* pActionHandlerProjectile = new ActionHandlerProjectile;
	int propertyIndex = 0;
	ActionHandlerProjectile::_InitFromPropertiesSubset(pActionHandlerProjectile, properties, propertyIndex);
	return pActionHandlerProjectile;
}

void* ActionHandlerProjectile::_CreateEmptyObject()
{
	return new ActionHandlerProjectile;
}

void ActionHandlerProjectile::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ActionHandlerProjectile*>(pObject);
}

// ActionHandlerMove
void ActionHandlerMove::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ActionHandlerMove* pActionHandlerMove = static_cast<ActionHandlerMove*>(pObject);
	ActionHandlerBase::_InitFromPropertiesSubset(static_cast<ActionHandlerBase*>(pActionHandlerMove), properties, propertyIndex);
}

void* ActionHandlerMove::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ActionHandlerMove* pActionHandlerMove = new ActionHandlerMove;
	int propertyIndex = 0;
	ActionHandlerMove::_InitFromPropertiesSubset(pActionHandlerMove, properties, propertyIndex);
	return pActionHandlerMove;
}

void* ActionHandlerMove::_CreateEmptyObject()
{
	return new ActionHandlerMove;
}

void ActionHandlerMove::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ActionHandlerMove*>(pObject);
}

// ActionDeciderProjectile
void ActionDeciderProjectile::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ActionDeciderProjectile* pActionDeciderProjectile = static_cast<ActionDeciderProjectile*>(pObject);
	ActionDeciderBase::_InitFromPropertiesSubset(static_cast<ActionDeciderBase*>(pActionDeciderProjectile), properties, propertyIndex);
	pActionDeciderProjectile->pActionSystem = static_cast<ECSSystemAction*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pActionDeciderProjectile->pEntityMap = static_cast<ECSSystemEntityMap*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pActionDeciderProjectile->skipSquareThresholdSq = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pActionDeciderProjectile->pProjectileAction = static_cast<ActionHandlerProjectile*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* ActionDeciderProjectile::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ActionDeciderProjectile* pActionDeciderProjectile = new ActionDeciderProjectile;
	int propertyIndex = 0;
	ActionDeciderProjectile::_InitFromPropertiesSubset(pActionDeciderProjectile, properties, propertyIndex);
	return pActionDeciderProjectile;
}

void* ActionDeciderProjectile::_CreateEmptyObject()
{
	return new ActionDeciderProjectile;
}

void ActionDeciderProjectile::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ActionDeciderProjectile*>(pObject);
}

// ActionDeciderPlayer
void ActionDeciderPlayer::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	ActionDeciderPlayer* pActionDeciderPlayer = static_cast<ActionDeciderPlayer*>(pObject);
	ActionDeciderBase::_InitFromPropertiesSubset(static_cast<ActionDeciderBase*>(pActionDeciderPlayer), properties, propertyIndex);
	pActionDeciderPlayer->pHotbarManager = static_cast<HotbarManager*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pActionDeciderPlayer->pWaitAction = static_cast<ActionHandlerWait*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* ActionDeciderPlayer::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	ActionDeciderPlayer* pActionDeciderPlayer = new ActionDeciderPlayer;
	int propertyIndex = 0;
	ActionDeciderPlayer::_InitFromPropertiesSubset(pActionDeciderPlayer, properties, propertyIndex);
	return pActionDeciderPlayer;
}

void* ActionDeciderPlayer::_CreateEmptyObject()
{
	return new ActionDeciderPlayer;
}

void ActionDeciderPlayer::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<ActionDeciderPlayer*>(pObject);
}

// WorldGenerationContinent
void WorldGenerationContinent::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	WorldGenerationContinent* pWorldGenerationContinent = static_cast<WorldGenerationContinent*>(pObject);
	pWorldGenerationContinent->pEcs = static_cast<ECS*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pWorldGenerationContinent->pDirectoryData = static_cast<DirectoryData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pWorldGenerationContinent->pRPGSystem = static_cast<RPGSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pWorldGenerationContinent->continentSize = static_cast<EditorTypePropertyInt*>(properties[propertyIndex++])->GetValue();
	pWorldGenerationContinent->bRandomSeed = static_cast<EditorTypePropertyBool*>(properties[propertyIndex++])->GetValue();
	pWorldGenerationContinent->debugSeed = static_cast<EditorTypePropertyInt*>(properties[propertyIndex++])->GetValue();
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			WorldGenerationTileEntry* temp = static_cast<WorldGenerationTileEntry*>(static_cast<EditorTypePropertyStruct*>(instancedProperty.get())->GetValue());
			pWorldGenerationContinent->tileData.push_back(*temp);
			delete temp;
		}
	}
	{
		WorldGenerationShorelineParams* temp = static_cast<WorldGenerationShorelineParams*>(static_cast<EditorTypePropertyStruct*>(properties[propertyIndex++])->GetValue());
		pWorldGenerationContinent->shorelineParams = *temp;
		delete temp;
	}
	{
		WorldGenerationSandParams* temp = static_cast<WorldGenerationSandParams*>(static_cast<EditorTypePropertyStruct*>(properties[propertyIndex++])->GetValue());
		pWorldGenerationContinent->sandParams = *temp;
		delete temp;
	}
	{
		WorldGenerationTreeParams* temp = static_cast<WorldGenerationTreeParams*>(static_cast<EditorTypePropertyStruct*>(properties[propertyIndex++])->GetValue());
		pWorldGenerationContinent->treeParams = *temp;
		delete temp;
	}
	{
		WorldGenerationEnemyParams* temp = static_cast<WorldGenerationEnemyParams*>(static_cast<EditorTypePropertyStruct*>(properties[propertyIndex++])->GetValue());
		pWorldGenerationContinent->enemyParams = *temp;
		delete temp;
	}
}

void* WorldGenerationContinent::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	WorldGenerationContinent* pWorldGenerationContinent = new WorldGenerationContinent;
	int propertyIndex = 0;
	WorldGenerationContinent::_InitFromPropertiesSubset(pWorldGenerationContinent, properties, propertyIndex);
	return pWorldGenerationContinent;
}

void* WorldGenerationContinent::_CreateEmptyObject()
{
	return new WorldGenerationContinent;
}

void WorldGenerationContinent::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<WorldGenerationContinent*>(pObject);
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

void HUDObjectBase::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<HUDObjectBase*>(pObject);
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

void HUD::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<HUD*>(pObject);
}

// RPGSkillSystem
void RPGSkillSystem::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGSkillSystem* pRPGSkillSystem = static_cast<RPGSkillSystem*>(pObject);
	GameSystem::_InitFromPropertiesSubset(static_cast<GameSystem*>(pRPGSkillSystem), properties, propertyIndex);
	pRPGSkillSystem->pDirectoryData = static_cast<DirectoryData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pRPGSkillSystem->pEcs = static_cast<ECS*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pRPGSkillSystem->pEcsEntityMap = static_cast<ECSSystemEntityMap*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pRPGSkillSystem->pRpgSystem = static_cast<RPGSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pRPGSkillSystem->pCameraSystem = static_cast<CameraSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pRPGSkillSystem->skillIconFileName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pRPGSkillSystem->pCastSkillHotbarAction.SetAsset(static_cast<EditorTypePropertyInstancedAssetPtr*>(properties[propertyIndex++])->GetValue());
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			RPGSkillHighlightEntry* temp = static_cast<RPGSkillHighlightEntry*>(static_cast<EditorTypePropertyStruct*>(instancedProperty.get())->GetValue());
			pRPGSkillSystem->skillHighlightVisuals.push_back(*temp);
			delete temp;
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

void RPGSkillSystem::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGSkillSystem*>(pObject);
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
			RPGLevelProgressionData* temp = static_cast<RPGLevelProgressionData*>(static_cast<EditorTypePropertyStruct*>(instancedProperty.get())->GetValue());
			pRPGSystem->levelProgressionData.push_back(*temp);
			delete temp;
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

void RPGSystem::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGSystem*>(pObject);
}

// RPGAttributeSkillDamage
void RPGAttributeSkillDamage::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	RPGAttributeSkillDamage* pRPGAttributeSkillDamage = static_cast<RPGAttributeSkillDamage*>(pObject);
	RPGAttributeBase::_InitFromPropertiesSubset(static_cast<RPGAttributeBase*>(pRPGAttributeSkillDamage), properties, propertyIndex);
}

void* RPGAttributeSkillDamage::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	RPGAttributeSkillDamage* pRPGAttributeSkillDamage = new RPGAttributeSkillDamage;
	int propertyIndex = 0;
	RPGAttributeSkillDamage::_InitFromPropertiesSubset(pRPGAttributeSkillDamage, properties, propertyIndex);
	return pRPGAttributeSkillDamage;
}

void* RPGAttributeSkillDamage::_CreateEmptyObject()
{
	return new RPGAttributeSkillDamage;
}

void RPGAttributeSkillDamage::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGAttributeSkillDamage*>(pObject);
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

void RPGAttributeMaxHealth::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<RPGAttributeMaxHealth*>(pObject);
}

// HUDObjectXP
void HUDObjectXP::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	HUDObjectXP* pHUDObjectXP = static_cast<HUDObjectXP*>(pObject);
	HUDObjectBase::_InitFromPropertiesSubset(static_cast<HUDObjectBase*>(pHUDObjectXP), properties, propertyIndex);
	pHUDObjectXP->pRpgSystem = static_cast<RPGSystem*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* HUDObjectXP::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	HUDObjectXP* pHUDObjectXP = new HUDObjectXP;
	int propertyIndex = 0;
	HUDObjectXP::_InitFromPropertiesSubset(pHUDObjectXP, properties, propertyIndex);
	return pHUDObjectXP;
}

void* HUDObjectXP::_CreateEmptyObject()
{
	return new HUDObjectXP;
}

void HUDObjectXP::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<HUDObjectXP*>(pObject);
}

// HUDObjectHotbar
void HUDObjectHotbar::_InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	HUDObjectHotbar* pHUDObjectHotbar = static_cast<HUDObjectHotbar*>(pObject);
	HUDObjectBase::_InitFromPropertiesSubset(static_cast<HUDObjectBase*>(pHUDObjectHotbar), properties, propertyIndex);
	pHUDObjectHotbar->slotSpawnAreaLayerName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pHUDObjectHotbar->slotLayerName = static_cast<EditorTypePropertyString*>(properties[propertyIndex++])->GetValue();
	pHUDObjectHotbar->pDirectoryData = static_cast<DirectoryData*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pHUDObjectHotbar->pHotbarManager = static_cast<HotbarManager*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
	pHUDObjectHotbar->pDragAndDropManager = static_cast<DragAndDropManager*>(static_cast<EditorTypePropertyClass*>(properties[propertyIndex++])->GetValue());
}

void* HUDObjectHotbar::_InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	HUDObjectHotbar* pHUDObjectHotbar = new HUDObjectHotbar;
	int propertyIndex = 0;
	HUDObjectHotbar::_InitFromPropertiesSubset(pHUDObjectHotbar, properties, propertyIndex);
	return pHUDObjectHotbar;
}

void* HUDObjectHotbar::_CreateEmptyObject()
{
	return new HUDObjectHotbar;
}

void HUDObjectHotbar::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<HUDObjectHotbar*>(pObject);
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

void HUDObjectHealth::_DeleteObject(void* pObject)
{
	delete reinterpret_cast<HUDObjectHealth*>(pObject);
}

namespace __Generated
{
	std::unordered_map<std::string, void* (*)(const std::vector<EditorTypePropertyBase*>&)> stringToCreateObjectFunction
	{
		{"BasicNoiseParams", &BasicNoiseParams::_InitFromProperties},
		{"WorldGenerationTileDefinition", &WorldGenerationTileDefinition::_InitFromProperties},
		{"WorldGenerationLogicBase", &WorldGenerationLogicBase::_InitFromProperties},
		{"WorldGenerationEnemyParams", &WorldGenerationEnemyParams::_InitFromProperties},
		{"WorldGenerationTreeParams", &WorldGenerationTreeParams::_InitFromProperties},
		{"WorldGenerationSandParams", &WorldGenerationSandParams::_InitFromProperties},
		{"WorldGenerationShorelineParams", &WorldGenerationShorelineParams::_InitFromProperties},
		{"TextRenderCharacterData", &TextRenderCharacterData::_InitFromProperties},
		{"TextboxParams", &TextboxParams::_InitFromProperties},
		{"SkillTreeMenuSkillSlotData", &SkillTreeMenuSkillSlotData::_InitFromProperties},
		{"MainMenuButtonData", &MainMenuButtonData::_InitFromProperties},
		{"MenuScreenClassSelectEntry", &MenuScreenClassSelectEntry::_InitFromProperties},
		{"MenuScreenBase", &MenuScreenBase::_InitFromProperties},
		{"HUDObjectSharedInitParams", &HUDObjectSharedInitParams::_InitFromProperties},
		{"IDragAndDroppable", &IDragAndDroppable::_InitFromProperties},
		{"RPGSkillEffectModuleBase", &RPGSkillEffectModuleBase::_InitFromProperties},
		{"RPGSkillAimModuleBase", &RPGSkillAimModuleBase::_InitFromProperties},
		{"RPGLevelProgressionData", &RPGLevelProgressionData::_InitFromProperties},
		{"RPGRacePerLevelAttributeValues", &RPGRacePerLevelAttributeValues::_InitFromProperties},
		{"RPGRaceInitialAttributeValues", &RPGRaceInitialAttributeValues::_InitFromProperties},
		{"RPGClassSpecialisationData", &RPGClassSpecialisationData::_InitFromProperties},
		{"RPGAttributeModifierBase", &RPGAttributeModifierBase::_InitFromProperties},
		{"InputActionBase", &InputActionBase::_InitFromProperties},
		{"StatefulHotbarActionBase", &StatefulHotbarActionBase::_InitFromProperties},
		{"IHotbarItem", &IHotbarItem::_InitFromProperties},
		{"HotbarSlot", &HotbarSlot::_InitFromProperties},
		{"ECSSystemBase", &ECSSystemBase::_InitFromProperties},
		{"ECSSystemAction", &ECSSystemAction::_InitFromProperties},
		{"GameSystem", &GameSystem::_InitFromProperties},
		{"GameInstance", &GameInstance::_InitFromProperties},
		{"DirectoryData", &DirectoryData::_InitFromProperties},
		{"CameraSystem", &CameraSystem::_InitFromProperties},
		{"ActionHandlerBase", &ActionHandlerBase::_InitFromProperties},
		{"ActionHandlerAttack", &ActionHandlerAttack::_InitFromProperties},
		{"ActionDeciderBase", &ActionDeciderBase::_InitFromProperties},
		{"ActionDeciderAI", &ActionDeciderAI::_InitFromProperties},
		{"WorldGenerator", &WorldGenerator::_InitFromProperties},
		{"WorldGenerationLogicBasicNoise1D", &WorldGenerationLogicBasicNoise1D::_InitFromProperties},
		{"WorldGenerationLogicAdd", &WorldGenerationLogicAdd::_InitFromProperties},
		{"WorldGenerationTileEntry", &WorldGenerationTileEntry::_InitFromProperties},
		{"TextRenderSystem", &TextRenderSystem::_InitFromProperties},
		{"MenuScreenSkillTree", &MenuScreenSkillTree::_InitFromProperties},
		{"MenuScreenMain", &MenuScreenMain::_InitFromProperties},
		{"MenuScreenClassSelect", &MenuScreenClassSelect::_InitFromProperties},
		{"MenuSystem", &MenuSystem::_InitFromProperties},
		{"HUDAnchorPoint", &HUDAnchorPoint::_InitFromProperties},
		{"DragAndDropManager", &DragAndDropManager::_InitFromProperties},
		{"TestSuiteGameSystem", &TestSuiteGameSystem::_InitFromProperties},
		{"RPGSkillHighlightEntry", &RPGSkillHighlightEntry::_InitFromProperties},
		{"RPGSkillData", &RPGSkillData::_InitFromProperties},
		{"RPGSkillEffectModuleProjectile", &RPGSkillEffectModuleProjectile::_InitFromProperties},
		{"RPGSkillEffectModuleDamage", &RPGSkillEffectModuleDamage::_InitFromProperties},
		{"RPGSkillAimModuleLine", &RPGSkillAimModuleLine::_InitFromProperties},
		{"RPGDamageMagnitude", &RPGDamageMagnitude::_InitFromProperties},
		{"RPGLevelScalingNumber", &RPGLevelScalingNumber::_InitFromProperties},
		{"RPGRaceData", &RPGRaceData::_InitFromProperties},
		{"RPGClassData", &RPGClassData::_InitFromProperties},
		{"RPGAttributeBase", &RPGAttributeBase::_InitFromProperties},
		{"PerformanceMetricsManager", &PerformanceMetricsManager::_InitFromProperties},
		{"MouseSystem", &MouseSystem::_InitFromProperties},
		{"InputSystem", &InputSystem::_InitFromProperties},
		{"InputContext", &InputContext::_InitFromProperties},
		{"InputActionPress", &InputActionPress::_InitFromProperties},
		{"StatefulHotbarActionCastSkill", &StatefulHotbarActionCastSkill::_InitFromProperties},
		{"HotbarManager", &HotbarManager::_InitFromProperties},
		{"ImGuiEditor", &ImGuiEditor::_InitFromProperties},
		{"ECSSystemRender", &ECSSystemRender::_InitFromProperties},
		{"ECSSystemPhysics", &ECSSystemPhysics::_InitFromProperties},
		{"ECSSystemNameslate", &ECSSystemNameslate::_InitFromProperties},
		{"ECSSystemEntityMap", &ECSSystemEntityMap::_InitFromProperties},
		{"ECSSystemDamage", &ECSSystemDamage::_InitFromProperties},
		{"ECSSystemCleanUp", &ECSSystemCleanUp::_InitFromProperties},
		{"ECS", &ECS::_InitFromProperties},
		{"ActionHandlerWait", &ActionHandlerWait::_InitFromProperties},
		{"ActionHandlerSkill", &ActionHandlerSkill::_InitFromProperties},
		{"ActionHandlerProjectile", &ActionHandlerProjectile::_InitFromProperties},
		{"ActionHandlerMove", &ActionHandlerMove::_InitFromProperties},
		{"ActionDeciderProjectile", &ActionDeciderProjectile::_InitFromProperties},
		{"ActionDeciderPlayer", &ActionDeciderPlayer::_InitFromProperties},
		{"WorldGenerationContinent", &WorldGenerationContinent::_InitFromProperties},
		{"HUDObjectBase", &HUDObjectBase::_InitFromProperties},
		{"HUD", &HUD::_InitFromProperties},
		{"RPGSkillSystem", &RPGSkillSystem::_InitFromProperties},
		{"RPGSystem", &RPGSystem::_InitFromProperties},
		{"RPGAttributeSkillDamage", &RPGAttributeSkillDamage::_InitFromProperties},
		{"RPGAttributeMaxHealth", &RPGAttributeMaxHealth::_InitFromProperties},
		{"HUDObjectXP", &HUDObjectXP::_InitFromProperties},
		{"HUDObjectHotbar", &HUDObjectHotbar::_InitFromProperties},
		{"HUDObjectHealth", &HUDObjectHealth::_InitFromProperties},
	};

	std::unordered_map<std::string, void* (*)()> stringToCreateEmptyObjectFunction
	{
		{"BasicNoiseParams", &BasicNoiseParams::_CreateEmptyObject},
		{"WorldGenerationTileDefinition", &WorldGenerationTileDefinition::_CreateEmptyObject},
		{"WorldGenerationLogicBase", &WorldGenerationLogicBase::_CreateEmptyObject},
		{"WorldGenerationEnemyParams", &WorldGenerationEnemyParams::_CreateEmptyObject},
		{"WorldGenerationTreeParams", &WorldGenerationTreeParams::_CreateEmptyObject},
		{"WorldGenerationSandParams", &WorldGenerationSandParams::_CreateEmptyObject},
		{"WorldGenerationShorelineParams", &WorldGenerationShorelineParams::_CreateEmptyObject},
		{"TextRenderCharacterData", &TextRenderCharacterData::_CreateEmptyObject},
		{"TextboxParams", &TextboxParams::_CreateEmptyObject},
		{"SkillTreeMenuSkillSlotData", &SkillTreeMenuSkillSlotData::_CreateEmptyObject},
		{"MainMenuButtonData", &MainMenuButtonData::_CreateEmptyObject},
		{"MenuScreenClassSelectEntry", &MenuScreenClassSelectEntry::_CreateEmptyObject},
		{"MenuScreenBase", &MenuScreenBase::_CreateEmptyObject},
		{"HUDObjectSharedInitParams", &HUDObjectSharedInitParams::_CreateEmptyObject},
		{"IDragAndDroppable", &IDragAndDroppable::_CreateEmptyObject},
		{"RPGSkillEffectModuleBase", &RPGSkillEffectModuleBase::_CreateEmptyObject},
		{"RPGSkillAimModuleBase", &RPGSkillAimModuleBase::_CreateEmptyObject},
		{"RPGLevelProgressionData", &RPGLevelProgressionData::_CreateEmptyObject},
		{"RPGRacePerLevelAttributeValues", &RPGRacePerLevelAttributeValues::_CreateEmptyObject},
		{"RPGRaceInitialAttributeValues", &RPGRaceInitialAttributeValues::_CreateEmptyObject},
		{"RPGClassSpecialisationData", &RPGClassSpecialisationData::_CreateEmptyObject},
		{"RPGAttributeModifierBase", &RPGAttributeModifierBase::_CreateEmptyObject},
		{"InputActionBase", &InputActionBase::_CreateEmptyObject},
		{"StatefulHotbarActionBase", &StatefulHotbarActionBase::_CreateEmptyObject},
		{"IHotbarItem", &IHotbarItem::_CreateEmptyObject},
		{"HotbarSlot", &HotbarSlot::_CreateEmptyObject},
		{"ECSSystemBase", &ECSSystemBase::_CreateEmptyObject},
		{"ECSSystemAction", &ECSSystemAction::_CreateEmptyObject},
		{"GameSystem", &GameSystem::_CreateEmptyObject},
		{"GameInstance", &GameInstance::_CreateEmptyObject},
		{"DirectoryData", &DirectoryData::_CreateEmptyObject},
		{"CameraSystem", &CameraSystem::_CreateEmptyObject},
		{"ActionHandlerBase", &ActionHandlerBase::_CreateEmptyObject},
		{"ActionHandlerAttack", &ActionHandlerAttack::_CreateEmptyObject},
		{"ActionDeciderBase", &ActionDeciderBase::_CreateEmptyObject},
		{"ActionDeciderAI", &ActionDeciderAI::_CreateEmptyObject},
		{"WorldGenerator", &WorldGenerator::_CreateEmptyObject},
		{"WorldGenerationLogicBasicNoise1D", &WorldGenerationLogicBasicNoise1D::_CreateEmptyObject},
		{"WorldGenerationLogicAdd", &WorldGenerationLogicAdd::_CreateEmptyObject},
		{"WorldGenerationTileEntry", &WorldGenerationTileEntry::_CreateEmptyObject},
		{"TextRenderSystem", &TextRenderSystem::_CreateEmptyObject},
		{"MenuScreenSkillTree", &MenuScreenSkillTree::_CreateEmptyObject},
		{"MenuScreenMain", &MenuScreenMain::_CreateEmptyObject},
		{"MenuScreenClassSelect", &MenuScreenClassSelect::_CreateEmptyObject},
		{"MenuSystem", &MenuSystem::_CreateEmptyObject},
		{"HUDAnchorPoint", &HUDAnchorPoint::_CreateEmptyObject},
		{"DragAndDropManager", &DragAndDropManager::_CreateEmptyObject},
		{"TestSuiteGameSystem", &TestSuiteGameSystem::_CreateEmptyObject},
		{"RPGSkillHighlightEntry", &RPGSkillHighlightEntry::_CreateEmptyObject},
		{"RPGSkillData", &RPGSkillData::_CreateEmptyObject},
		{"RPGSkillEffectModuleProjectile", &RPGSkillEffectModuleProjectile::_CreateEmptyObject},
		{"RPGSkillEffectModuleDamage", &RPGSkillEffectModuleDamage::_CreateEmptyObject},
		{"RPGSkillAimModuleLine", &RPGSkillAimModuleLine::_CreateEmptyObject},
		{"RPGDamageMagnitude", &RPGDamageMagnitude::_CreateEmptyObject},
		{"RPGLevelScalingNumber", &RPGLevelScalingNumber::_CreateEmptyObject},
		{"RPGRaceData", &RPGRaceData::_CreateEmptyObject},
		{"RPGClassData", &RPGClassData::_CreateEmptyObject},
		{"RPGAttributeBase", &RPGAttributeBase::_CreateEmptyObject},
		{"PerformanceMetricsManager", &PerformanceMetricsManager::_CreateEmptyObject},
		{"MouseSystem", &MouseSystem::_CreateEmptyObject},
		{"InputSystem", &InputSystem::_CreateEmptyObject},
		{"InputContext", &InputContext::_CreateEmptyObject},
		{"InputActionPress", &InputActionPress::_CreateEmptyObject},
		{"StatefulHotbarActionCastSkill", &StatefulHotbarActionCastSkill::_CreateEmptyObject},
		{"HotbarManager", &HotbarManager::_CreateEmptyObject},
		{"ImGuiEditor", &ImGuiEditor::_CreateEmptyObject},
		{"ECSSystemRender", &ECSSystemRender::_CreateEmptyObject},
		{"ECSSystemPhysics", &ECSSystemPhysics::_CreateEmptyObject},
		{"ECSSystemNameslate", &ECSSystemNameslate::_CreateEmptyObject},
		{"ECSSystemEntityMap", &ECSSystemEntityMap::_CreateEmptyObject},
		{"ECSSystemDamage", &ECSSystemDamage::_CreateEmptyObject},
		{"ECSSystemCleanUp", &ECSSystemCleanUp::_CreateEmptyObject},
		{"ECS", &ECS::_CreateEmptyObject},
		{"ActionHandlerWait", &ActionHandlerWait::_CreateEmptyObject},
		{"ActionHandlerSkill", &ActionHandlerSkill::_CreateEmptyObject},
		{"ActionHandlerProjectile", &ActionHandlerProjectile::_CreateEmptyObject},
		{"ActionHandlerMove", &ActionHandlerMove::_CreateEmptyObject},
		{"ActionDeciderProjectile", &ActionDeciderProjectile::_CreateEmptyObject},
		{"ActionDeciderPlayer", &ActionDeciderPlayer::_CreateEmptyObject},
		{"WorldGenerationContinent", &WorldGenerationContinent::_CreateEmptyObject},
		{"HUDObjectBase", &HUDObjectBase::_CreateEmptyObject},
		{"HUD", &HUD::_CreateEmptyObject},
		{"RPGSkillSystem", &RPGSkillSystem::_CreateEmptyObject},
		{"RPGSystem", &RPGSystem::_CreateEmptyObject},
		{"RPGAttributeSkillDamage", &RPGAttributeSkillDamage::_CreateEmptyObject},
		{"RPGAttributeMaxHealth", &RPGAttributeMaxHealth::_CreateEmptyObject},
		{"HUDObjectXP", &HUDObjectXP::_CreateEmptyObject},
		{"HUDObjectHotbar", &HUDObjectHotbar::_CreateEmptyObject},
		{"HUDObjectHealth", &HUDObjectHealth::_CreateEmptyObject},
	};

	std::unordered_map<std::string, void (*)(void*, const std::vector<EditorTypePropertyBase*>&, int&)> stringToInitialiseExistingObjectFunction
	{
		{"BasicNoiseParams", &BasicNoiseParams::_InitFromPropertiesSubset},
		{"WorldGenerationTileDefinition", &WorldGenerationTileDefinition::_InitFromPropertiesSubset},
		{"WorldGenerationLogicBase", &WorldGenerationLogicBase::_InitFromPropertiesSubset},
		{"WorldGenerationEnemyParams", &WorldGenerationEnemyParams::_InitFromPropertiesSubset},
		{"WorldGenerationTreeParams", &WorldGenerationTreeParams::_InitFromPropertiesSubset},
		{"WorldGenerationSandParams", &WorldGenerationSandParams::_InitFromPropertiesSubset},
		{"WorldGenerationShorelineParams", &WorldGenerationShorelineParams::_InitFromPropertiesSubset},
		{"TextRenderCharacterData", &TextRenderCharacterData::_InitFromPropertiesSubset},
		{"TextboxParams", &TextboxParams::_InitFromPropertiesSubset},
		{"SkillTreeMenuSkillSlotData", &SkillTreeMenuSkillSlotData::_InitFromPropertiesSubset},
		{"MainMenuButtonData", &MainMenuButtonData::_InitFromPropertiesSubset},
		{"MenuScreenClassSelectEntry", &MenuScreenClassSelectEntry::_InitFromPropertiesSubset},
		{"MenuScreenBase", &MenuScreenBase::_InitFromPropertiesSubset},
		{"HUDObjectSharedInitParams", &HUDObjectSharedInitParams::_InitFromPropertiesSubset},
		{"IDragAndDroppable", &IDragAndDroppable::_InitFromPropertiesSubset},
		{"RPGSkillEffectModuleBase", &RPGSkillEffectModuleBase::_InitFromPropertiesSubset},
		{"RPGSkillAimModuleBase", &RPGSkillAimModuleBase::_InitFromPropertiesSubset},
		{"RPGLevelProgressionData", &RPGLevelProgressionData::_InitFromPropertiesSubset},
		{"RPGRacePerLevelAttributeValues", &RPGRacePerLevelAttributeValues::_InitFromPropertiesSubset},
		{"RPGRaceInitialAttributeValues", &RPGRaceInitialAttributeValues::_InitFromPropertiesSubset},
		{"RPGClassSpecialisationData", &RPGClassSpecialisationData::_InitFromPropertiesSubset},
		{"RPGAttributeModifierBase", &RPGAttributeModifierBase::_InitFromPropertiesSubset},
		{"InputActionBase", &InputActionBase::_InitFromPropertiesSubset},
		{"StatefulHotbarActionBase", &StatefulHotbarActionBase::_InitFromPropertiesSubset},
		{"IHotbarItem", &IHotbarItem::_InitFromPropertiesSubset},
		{"HotbarSlot", &HotbarSlot::_InitFromPropertiesSubset},
		{"ECSSystemBase", &ECSSystemBase::_InitFromPropertiesSubset},
		{"ECSSystemAction", &ECSSystemAction::_InitFromPropertiesSubset},
		{"GameSystem", &GameSystem::_InitFromPropertiesSubset},
		{"GameInstance", &GameInstance::_InitFromPropertiesSubset},
		{"DirectoryData", &DirectoryData::_InitFromPropertiesSubset},
		{"CameraSystem", &CameraSystem::_InitFromPropertiesSubset},
		{"ActionHandlerBase", &ActionHandlerBase::_InitFromPropertiesSubset},
		{"ActionHandlerAttack", &ActionHandlerAttack::_InitFromPropertiesSubset},
		{"ActionDeciderBase", &ActionDeciderBase::_InitFromPropertiesSubset},
		{"ActionDeciderAI", &ActionDeciderAI::_InitFromPropertiesSubset},
		{"WorldGenerator", &WorldGenerator::_InitFromPropertiesSubset},
		{"WorldGenerationLogicBasicNoise1D", &WorldGenerationLogicBasicNoise1D::_InitFromPropertiesSubset},
		{"WorldGenerationLogicAdd", &WorldGenerationLogicAdd::_InitFromPropertiesSubset},
		{"WorldGenerationTileEntry", &WorldGenerationTileEntry::_InitFromPropertiesSubset},
		{"TextRenderSystem", &TextRenderSystem::_InitFromPropertiesSubset},
		{"MenuScreenSkillTree", &MenuScreenSkillTree::_InitFromPropertiesSubset},
		{"MenuScreenMain", &MenuScreenMain::_InitFromPropertiesSubset},
		{"MenuScreenClassSelect", &MenuScreenClassSelect::_InitFromPropertiesSubset},
		{"MenuSystem", &MenuSystem::_InitFromPropertiesSubset},
		{"HUDAnchorPoint", &HUDAnchorPoint::_InitFromPropertiesSubset},
		{"DragAndDropManager", &DragAndDropManager::_InitFromPropertiesSubset},
		{"TestSuiteGameSystem", &TestSuiteGameSystem::_InitFromPropertiesSubset},
		{"RPGSkillHighlightEntry", &RPGSkillHighlightEntry::_InitFromPropertiesSubset},
		{"RPGSkillData", &RPGSkillData::_InitFromPropertiesSubset},
		{"RPGSkillEffectModuleProjectile", &RPGSkillEffectModuleProjectile::_InitFromPropertiesSubset},
		{"RPGSkillEffectModuleDamage", &RPGSkillEffectModuleDamage::_InitFromPropertiesSubset},
		{"RPGSkillAimModuleLine", &RPGSkillAimModuleLine::_InitFromPropertiesSubset},
		{"RPGDamageMagnitude", &RPGDamageMagnitude::_InitFromPropertiesSubset},
		{"RPGLevelScalingNumber", &RPGLevelScalingNumber::_InitFromPropertiesSubset},
		{"RPGRaceData", &RPGRaceData::_InitFromPropertiesSubset},
		{"RPGClassData", &RPGClassData::_InitFromPropertiesSubset},
		{"RPGAttributeBase", &RPGAttributeBase::_InitFromPropertiesSubset},
		{"PerformanceMetricsManager", &PerformanceMetricsManager::_InitFromPropertiesSubset},
		{"MouseSystem", &MouseSystem::_InitFromPropertiesSubset},
		{"InputSystem", &InputSystem::_InitFromPropertiesSubset},
		{"InputContext", &InputContext::_InitFromPropertiesSubset},
		{"InputActionPress", &InputActionPress::_InitFromPropertiesSubset},
		{"StatefulHotbarActionCastSkill", &StatefulHotbarActionCastSkill::_InitFromPropertiesSubset},
		{"HotbarManager", &HotbarManager::_InitFromPropertiesSubset},
		{"ImGuiEditor", &ImGuiEditor::_InitFromPropertiesSubset},
		{"ECSSystemRender", &ECSSystemRender::_InitFromPropertiesSubset},
		{"ECSSystemPhysics", &ECSSystemPhysics::_InitFromPropertiesSubset},
		{"ECSSystemNameslate", &ECSSystemNameslate::_InitFromPropertiesSubset},
		{"ECSSystemEntityMap", &ECSSystemEntityMap::_InitFromPropertiesSubset},
		{"ECSSystemDamage", &ECSSystemDamage::_InitFromPropertiesSubset},
		{"ECSSystemCleanUp", &ECSSystemCleanUp::_InitFromPropertiesSubset},
		{"ECS", &ECS::_InitFromPropertiesSubset},
		{"ActionHandlerWait", &ActionHandlerWait::_InitFromPropertiesSubset},
		{"ActionHandlerSkill", &ActionHandlerSkill::_InitFromPropertiesSubset},
		{"ActionHandlerProjectile", &ActionHandlerProjectile::_InitFromPropertiesSubset},
		{"ActionHandlerMove", &ActionHandlerMove::_InitFromPropertiesSubset},
		{"ActionDeciderProjectile", &ActionDeciderProjectile::_InitFromPropertiesSubset},
		{"ActionDeciderPlayer", &ActionDeciderPlayer::_InitFromPropertiesSubset},
		{"WorldGenerationContinent", &WorldGenerationContinent::_InitFromPropertiesSubset},
		{"HUDObjectBase", &HUDObjectBase::_InitFromPropertiesSubset},
		{"HUD", &HUD::_InitFromPropertiesSubset},
		{"RPGSkillSystem", &RPGSkillSystem::_InitFromPropertiesSubset},
		{"RPGSystem", &RPGSystem::_InitFromPropertiesSubset},
		{"RPGAttributeSkillDamage", &RPGAttributeSkillDamage::_InitFromPropertiesSubset},
		{"RPGAttributeMaxHealth", &RPGAttributeMaxHealth::_InitFromPropertiesSubset},
		{"HUDObjectXP", &HUDObjectXP::_InitFromPropertiesSubset},
		{"HUDObjectHotbar", &HUDObjectHotbar::_InitFromPropertiesSubset},
		{"HUDObjectHealth", &HUDObjectHealth::_InitFromPropertiesSubset},
	};
	std::unordered_map<std::string, void (*)(void*)> stringToDeleteObjectFunction
	{
		{"BasicNoiseParams", &BasicNoiseParams::_DeleteObject},
		{"WorldGenerationTileDefinition", &WorldGenerationTileDefinition::_DeleteObject},
		{"WorldGenerationLogicBase", &WorldGenerationLogicBase::_DeleteObject},
		{"WorldGenerationEnemyParams", &WorldGenerationEnemyParams::_DeleteObject},
		{"WorldGenerationTreeParams", &WorldGenerationTreeParams::_DeleteObject},
		{"WorldGenerationSandParams", &WorldGenerationSandParams::_DeleteObject},
		{"WorldGenerationShorelineParams", &WorldGenerationShorelineParams::_DeleteObject},
		{"TextRenderCharacterData", &TextRenderCharacterData::_DeleteObject},
		{"TextboxParams", &TextboxParams::_DeleteObject},
		{"SkillTreeMenuSkillSlotData", &SkillTreeMenuSkillSlotData::_DeleteObject},
		{"MainMenuButtonData", &MainMenuButtonData::_DeleteObject},
		{"MenuScreenClassSelectEntry", &MenuScreenClassSelectEntry::_DeleteObject},
		{"MenuScreenBase", &MenuScreenBase::_DeleteObject},
		{"HUDObjectSharedInitParams", &HUDObjectSharedInitParams::_DeleteObject},
		{"IDragAndDroppable", &IDragAndDroppable::_DeleteObject},
		{"RPGSkillEffectModuleBase", &RPGSkillEffectModuleBase::_DeleteObject},
		{"RPGSkillAimModuleBase", &RPGSkillAimModuleBase::_DeleteObject},
		{"RPGLevelProgressionData", &RPGLevelProgressionData::_DeleteObject},
		{"RPGRacePerLevelAttributeValues", &RPGRacePerLevelAttributeValues::_DeleteObject},
		{"RPGRaceInitialAttributeValues", &RPGRaceInitialAttributeValues::_DeleteObject},
		{"RPGClassSpecialisationData", &RPGClassSpecialisationData::_DeleteObject},
		{"RPGAttributeModifierBase", &RPGAttributeModifierBase::_DeleteObject},
		{"InputActionBase", &InputActionBase::_DeleteObject},
		{"StatefulHotbarActionBase", &StatefulHotbarActionBase::_DeleteObject},
		{"IHotbarItem", &IHotbarItem::_DeleteObject},
		{"HotbarSlot", &HotbarSlot::_DeleteObject},
		{"ECSSystemBase", &ECSSystemBase::_DeleteObject},
		{"ECSSystemAction", &ECSSystemAction::_DeleteObject},
		{"GameSystem", &GameSystem::_DeleteObject},
		{"GameInstance", &GameInstance::_DeleteObject},
		{"DirectoryData", &DirectoryData::_DeleteObject},
		{"CameraSystem", &CameraSystem::_DeleteObject},
		{"ActionHandlerBase", &ActionHandlerBase::_DeleteObject},
		{"ActionHandlerAttack", &ActionHandlerAttack::_DeleteObject},
		{"ActionDeciderBase", &ActionDeciderBase::_DeleteObject},
		{"ActionDeciderAI", &ActionDeciderAI::_DeleteObject},
		{"WorldGenerator", &WorldGenerator::_DeleteObject},
		{"WorldGenerationLogicBasicNoise1D", &WorldGenerationLogicBasicNoise1D::_DeleteObject},
		{"WorldGenerationLogicAdd", &WorldGenerationLogicAdd::_DeleteObject},
		{"WorldGenerationTileEntry", &WorldGenerationTileEntry::_DeleteObject},
		{"TextRenderSystem", &TextRenderSystem::_DeleteObject},
		{"MenuScreenSkillTree", &MenuScreenSkillTree::_DeleteObject},
		{"MenuScreenMain", &MenuScreenMain::_DeleteObject},
		{"MenuScreenClassSelect", &MenuScreenClassSelect::_DeleteObject},
		{"MenuSystem", &MenuSystem::_DeleteObject},
		{"HUDAnchorPoint", &HUDAnchorPoint::_DeleteObject},
		{"DragAndDropManager", &DragAndDropManager::_DeleteObject},
		{"TestSuiteGameSystem", &TestSuiteGameSystem::_DeleteObject},
		{"RPGSkillHighlightEntry", &RPGSkillHighlightEntry::_DeleteObject},
		{"RPGSkillData", &RPGSkillData::_DeleteObject},
		{"RPGSkillEffectModuleProjectile", &RPGSkillEffectModuleProjectile::_DeleteObject},
		{"RPGSkillEffectModuleDamage", &RPGSkillEffectModuleDamage::_DeleteObject},
		{"RPGSkillAimModuleLine", &RPGSkillAimModuleLine::_DeleteObject},
		{"RPGDamageMagnitude", &RPGDamageMagnitude::_DeleteObject},
		{"RPGLevelScalingNumber", &RPGLevelScalingNumber::_DeleteObject},
		{"RPGRaceData", &RPGRaceData::_DeleteObject},
		{"RPGClassData", &RPGClassData::_DeleteObject},
		{"RPGAttributeBase", &RPGAttributeBase::_DeleteObject},
		{"PerformanceMetricsManager", &PerformanceMetricsManager::_DeleteObject},
		{"MouseSystem", &MouseSystem::_DeleteObject},
		{"InputSystem", &InputSystem::_DeleteObject},
		{"InputContext", &InputContext::_DeleteObject},
		{"InputActionPress", &InputActionPress::_DeleteObject},
		{"StatefulHotbarActionCastSkill", &StatefulHotbarActionCastSkill::_DeleteObject},
		{"HotbarManager", &HotbarManager::_DeleteObject},
		{"ImGuiEditor", &ImGuiEditor::_DeleteObject},
		{"ECSSystemRender", &ECSSystemRender::_DeleteObject},
		{"ECSSystemPhysics", &ECSSystemPhysics::_DeleteObject},
		{"ECSSystemNameslate", &ECSSystemNameslate::_DeleteObject},
		{"ECSSystemEntityMap", &ECSSystemEntityMap::_DeleteObject},
		{"ECSSystemDamage", &ECSSystemDamage::_DeleteObject},
		{"ECSSystemCleanUp", &ECSSystemCleanUp::_DeleteObject},
		{"ECS", &ECS::_DeleteObject},
		{"ActionHandlerWait", &ActionHandlerWait::_DeleteObject},
		{"ActionHandlerSkill", &ActionHandlerSkill::_DeleteObject},
		{"ActionHandlerProjectile", &ActionHandlerProjectile::_DeleteObject},
		{"ActionHandlerMove", &ActionHandlerMove::_DeleteObject},
		{"ActionDeciderProjectile", &ActionDeciderProjectile::_DeleteObject},
		{"ActionDeciderPlayer", &ActionDeciderPlayer::_DeleteObject},
		{"WorldGenerationContinent", &WorldGenerationContinent::_DeleteObject},
		{"HUDObjectBase", &HUDObjectBase::_DeleteObject},
		{"HUD", &HUD::_DeleteObject},
		{"RPGSkillSystem", &RPGSkillSystem::_DeleteObject},
		{"RPGSystem", &RPGSystem::_DeleteObject},
		{"RPGAttributeSkillDamage", &RPGAttributeSkillDamage::_DeleteObject},
		{"RPGAttributeMaxHealth", &RPGAttributeMaxHealth::_DeleteObject},
		{"HUDObjectXP", &HUDObjectXP::_DeleteObject},
		{"HUDObjectHotbar", &HUDObjectHotbar::_DeleteObject},
		{"HUDObjectHealth", &HUDObjectHealth::_DeleteObject},
	};
}
