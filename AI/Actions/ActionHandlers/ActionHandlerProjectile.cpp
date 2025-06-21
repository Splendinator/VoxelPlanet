#include "pch.h"

#include "ActionHandlerProjectile.h"

#include "ECS/ECS.h"
#include "RPGSystems/Skills/EffectModules/RPGSkillEffectModuleBase.h"

void ActionHandlerProjectile::Setup(const RPGSkillEffectModuleBase* pInSkillEffect, EntityId inCaster)
{
	pSkillEffect = pInSkillEffect;
	caster = inCaster;
}

void ActionHandlerProjectile::DoAction(ECS& ecs, EntityId e)
{
	if (!pRpgSystem || !pEntityMapSystem || !pDirectoryData)
	{
		DOMLOG_ERROR("Null systems");
		return;
	}

	if (!pSkillEffect)
	{
		return;
	}
	
	RPGSkillParams params = { ecs, *pEntityMapSystem, *pRpgSystem, *pDirectoryData };
	params.caster = caster;

	ComponentTransform& casterTransform = ecs.GetComponent<ComponentTransform>(caster);
	params.startPos = {casterTransform.x, casterTransform.y};
	
	ComponentTransform& projectileTransform = ecs.GetComponent<ComponentTransform>(e);
	params.targetPos = {projectileTransform.x, projectileTransform.y};

	pSkillEffect->ApplyEffect(params);

	ecs.DeleteEntity(e);
}

void ActionHandlerProjectile::Reset()
{
	
}
