#include "pch.h"

#include "RPGSkillSystem.h"

#include "DirectoryData.h"
#include "ECS/ECS.h"
#include "Renderer.h"
#include "RendererObject.h"

void RPGSkillSystem::Init()
{
	DOMLOG_ERROR_IF(pDirectoryData == nullptr)
	DOMLOG_ERROR_IF(pEcs == nullptr)
	DOMLOG_ERROR_IF(pEcsEntityMap == nullptr)
}

void RPGSkillSystem::Tick(float deltaTime)
{
	if (pPlayerCurrentlyAimedSkill)
	{
		if (!pEcs || !pEcsEntityMap)
		{
			return;
		}

		RPGSkillParams params = {*pEcs, *pEcsEntityMap};
		params.caster = pEcs->GetPlayerEntityId();
		ComponentTransform& transformComponent = pEcs->GetComponent<ComponentTransform>(params.caster);
		params.startPos = { transformComponent.x, transformComponent.y };
		
	}
}

void RPGSkillSystem::UnInit()
{
	
}

void RPGSkillSystem::PlayerStartAimingSkill(const RPGSkillData* pSkill)
{
	pPlayerCurrentlyAimedSkill = pSkill;
}

void RPGSkillSystem::StopAimingSkill()
{
	pPlayerCurrentlyAimedSkill = nullptr;
}
