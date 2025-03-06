#pragma once

#include "Core/GameSystem.h"

#include "RPGSkillsShared.h"

class RPGSkillData;
class RendererObject;
class DirectoryData;

// Map of responses to their player-facing graphics
EDITORSTRUCT()
struct RPGSkillAimResponseVisualEntry
{
	EDITORBODY()

	// Response
	EDITORPROPERTY()
	ERPGSkillAimResponse response = ERPGSkillAimResponse::Valid;

	// Graphics to show when receiving this response 
	EDITORPROPERTY()
	std::string fileName;
	
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

protected:

	EDITORPROPERTY()
	const DirectoryData* pDirectoryData = nullptr;

	EDITORPROPERTY()
	ECS* pEcs = nullptr;

	EDITORPROPERTY()
	ECSSystemEntityMap* pEcsEntityMap = nullptr;

	// Map of skill aim responses to their 
	EDITORPROPERTY()
	std::vector<RPGSkillAimResponseVisualEntry> skillAimResponseVisuals;

	std::vector<TransientPtr<RendererObject*>> shownAimResponseVisuals;

	// This is set to the skill the player is aiming while they are aiming it
	const RPGSkillData* pPlayerCurrentlyAimedSkill = nullptr;
};
