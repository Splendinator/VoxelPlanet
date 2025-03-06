#pragma once

#include "ActionHandlerBase.h"

class RPGSkillData;

// #TEMP: This class doesn't work at all, redo these with editor exposed
class ActionHandlerSkill : public ActionHandlerBase
{
public:

	static ActionHandlerSkill& GetSingleton() 
	{
		static ActionHandlerSkill staticActionHandlerSkill;
		return staticActionHandlerSkill;
	}
	
	//~ Begin ActionHandlerBase Interface
	void DoAction(ECS& ecs, EntityId e) override;
	void Reset() override;
	//~ End ActionHandlerBase Interface

	const RPGSkillData* pSkill = nullptr;
};
