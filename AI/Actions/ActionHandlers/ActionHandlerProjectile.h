#pragma once

#include "ActionHandlerBase.h"

#include "RPGSystems/Skills/RPGSkillsShared.h"

class RPGSkillEffectModuleBase;

// Used when a projectile gets to the end of its path or hits something along the way
EDITORCLASS()
class ActionHandlerProjectile : public ActionHandlerBase
{
	EDITORBODY()
public:

	void Setup(const RPGSkillEffectModuleBase* pInSkillEffect, EntityId inCaster);

protected:

	//~ Begin ActionHandlerBase Interface
	void DoAction(ECS& ecs, EntityId e) override;
	void Reset() override;
	//~ End ActionHandlerBase Interface
	
	const RPGSkillEffectModuleBase* pSkillEffect = nullptr;
	EntityId caster = INVALID_ENTITY_ID;
	
	EDITORPROPERTY()
	ECSSystemEntityMap* pEntityMapSystem = nullptr;
	
	EDITORPROPERTY()
	RPGSystem* pRpgSystem = nullptr;

	EDITORPROPERTY()
	DirectoryData* pDirectoryData = nullptr;
};
