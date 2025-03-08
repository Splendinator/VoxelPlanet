#pragma once

#include "ActionHandlerBase.h"

class RPGSkillSystem;
class RPGSkillData;

// Action to cast a specific skill
EDITORCLASS()
class ActionHandlerSkill : public ActionHandlerBase
{
	EDITORBODY()
public:
	
	//~ Begin ActionHandlerBase Interface
	void DoAction(ECS& ecs, EntityId e) override;
	void Reset() override;
	//~ End ActionHandlerBase Interface

	EDITORPROPERTY()
	RPGSkillSystem* pSkillSystem = nullptr;
	
	const RPGSkillData* pSkill = nullptr;
};
