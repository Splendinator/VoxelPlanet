#pragma once

#include "Core/GameSystem.h"

#include "RPGSkillsShared.h"

class CameraSystem;
class RPGSkillData;
class RendererObject;
class DirectoryData;

// Map of responses to their player-facing graphics
EDITORSTRUCT()
struct RPGSkillHighlightEntry
{
	EDITORBODY()

	// Highlight enum
	EDITORPROPERTY()
	ERPGSkillHighlightType highlight = ERPGSkillHighlightType::Valid;

	// Graphics to show this highlight type
	EDITORPROPERTY()
	std::string fileName;

	// Pooled renderer objects of this type, this will not shrink once populated and the visuals will just be made invisible.
	std::vector<TransientPtr<RendererObject>> pooledHighlightVisuals;

	// This will point to the head of the stack of pooledHighlightVisuals.
	// Anything less than this will be visible, anything more than this will be invisible  
	int pooledHighlightVisualsStackIndex = -1;

	void AddVisual(Vec2i visualsGridPosition, const DirectoryData& directoryData);
	void ClearVisualsStack();
	
};


// System responsible for spell casting
EDITORCLASS()
class RPGSkillSystem : public GameSystem
{
	EDITORBODY()
public:

	//~ Begin GameSystem Interface
	void Init() override;
	void Tick(float deltaTime) override;
	void UnInit() override;
	//~ End GameSystem Interface

	void PlayerStartAimingSkill(const RPGSkillData* pSkill);
	void StopAimingSkill();

	// Try and fire the skill at location, will return false if the skill can't be fired at the location.
	bool TryFireSkill(const RPGSkillData* pSkill, EntityId caster, const Vec2i& targetLocation);
	bool TryFirePlayerAimedSkill(); // This will automatically cancel aiming if successful.

	const std::string& GetSkillIconFileName() const { return skillIconFileName; }

protected:

	void ClearAllAimingVisuals();
	
	EDITORPROPERTY()
	const DirectoryData* pDirectoryData = nullptr;

	EDITORPROPERTY()
	ECS* pEcs = nullptr;

	EDITORPROPERTY()
	ECSSystemEntityMap* pEcsEntityMap = nullptr;

	EDITORPROPERTY()
	RPGSystem* pRpgSystem = nullptr;

	EDITORPROPERTY()
	const CameraSystem* pCameraSystem = nullptr;

	// File name of the .svg with all skill icons in. This will be in the UI Shared directory 
	EDITORPROPERTY()
	std::string skillIconFileName;
	
	// Map of skill aim responses to their 
	EDITORPROPERTY()
	std::vector<RPGSkillHighlightEntry> skillHighlightVisuals;

	// This is set to the skill the player is aiming while they are aiming it
	const RPGSkillData* pPlayerCurrentlyAimedSkill = nullptr;
};
