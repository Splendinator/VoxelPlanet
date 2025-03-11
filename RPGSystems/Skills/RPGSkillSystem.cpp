#include "pch.h"

#include "RPGSkillSystem.h"

#include "AimModule/RPGSkillAimModules.h"
#include "Camera/CameraSystem.h"
#include "DirectoryData.h"
#include "ECS/ECS.h"
#include "ECS/Systems/ECSSystemRender.h"
#include "EffectModule/RPGSkillEffectModules.h"
#include "RPGSkillData.h"
#include "RenderPriorities.h"
#include "Renderer.h"
#include "RendererObject.h"

// RPGSkillHighlightEntry
void RPGSkillHighlightEntry::AddVisual(Vec2i visualsGridPosition, const DirectoryData& directoryData)
{
	++pooledHighlightVisualsStackIndex;
	if (pooledHighlightVisuals.size() <= pooledHighlightVisualsStackIndex)
	{
		// Add new render object
		pooledHighlightVisuals.push_back(dmgf::AddObjectFromSVG(DirectoryData::ConcatenateSVGFilePathChecked(directoryData.rpgSkillVisuals, fileName).c_str()));
		ECSSystemRender::SetupRenderObjectOnGrid(pooledHighlightVisuals[pooledHighlightVisualsStackIndex]);
		pooledHighlightVisuals[pooledHighlightVisualsStackIndex]->SetRenderPriority(RenderPriority::spellcastUI);
	}
	else
	{
		// Reuse pooled render object
		pooledHighlightVisuals[pooledHighlightVisualsStackIndex]->SetVisible(true);
	}

	pooledHighlightVisuals[pooledHighlightVisualsStackIndex]->SetPosition((float)visualsGridPosition.x, (float)visualsGridPosition.y);
}

void RPGSkillHighlightEntry::ClearVisualsStack()
{
	while (pooledHighlightVisualsStackIndex >= 0)
	{
		pooledHighlightVisuals[pooledHighlightVisualsStackIndex]->SetVisible(false);
		--pooledHighlightVisualsStackIndex;
	}
}

// RPGSkillSystem
void RPGSkillSystem::Init()
{
	DOMLOG_ERROR_IF(pDirectoryData == nullptr)
	DOMLOG_ERROR_IF(pEcs == nullptr)
	DOMLOG_ERROR_IF(pEcsEntityMap == nullptr)
}

void RPGSkillSystem::UnInit()
{
	for (RPGSkillHighlightEntry& skillVisualEntry : skillHighlightVisuals)
	{
		for (TransientPtr<RendererObject> pooledVisual : skillVisualEntry.pooledHighlightVisuals)
		{
			dmgf::RemoveObject(pooledVisual);
		}
	}
}


void RPGSkillSystem::Tick(float deltaTime)
{
	if (pPlayerCurrentlyAimedSkill && pPlayerCurrentlyAimedSkill->IsValid())
	{
		if (!pEcs || !pEcsEntityMap || !pCameraSystem)
		{
			return;
		}

		// Clear previous frame's graphics
		ClearAllAimingVisuals();

		
		RPGSkillParams params = {*pEcs, *pEcsEntityMap, *pRpgSystem};
		params.caster = pEcs->GetPlayerEntityId();
		params.targetPos = pCameraSystem->GetWorldGridCoordinateUnderMouse();

		ComponentTransform& transformComponent = pEcs->GetComponent<ComponentTransform>(params.caster);
		params.startPos = { transformComponent.x, transformComponent.y };
		
		constexpr int MAX_VISUALS_DISTANCE_AROUND_PLAYER = 20;
		const int startX = params.startPos.x - MAX_VISUALS_DISTANCE_AROUND_PLAYER;
		const int endX = params.startPos.x + MAX_VISUALS_DISTANCE_AROUND_PLAYER;
		const int startY = params.startPos.y - MAX_VISUALS_DISTANCE_AROUND_PLAYER;
		const int endY = params.startPos.y + MAX_VISUALS_DISTANCE_AROUND_PLAYER;
		
		for (int x = startX; x < endX; ++x)
		{
			for (int y = startY; y < endY; ++y)
			{
				const Vec2i visualsGridPosition = { x, y };
				ERPGSkillHighlightType highlightType = pPlayerCurrentlyAimedSkill->pAimModule->GetSkillHighlightTypeForLocation(params, visualsGridPosition);

				if (highlightType != ERPGSkillHighlightType::None)
				{
					// #OPTIMISE: This should be a hashmap using the enum as an index into it. If this pattern is used elsewhere we maybe want a class + EDITORPROPERTY() handling
					for (RPGSkillHighlightEntry& highlightVisualsEntry : skillHighlightVisuals)
					{
						if (highlightVisualsEntry.highlight == highlightType)
						{
							highlightVisualsEntry.AddVisual(visualsGridPosition, *pDirectoryData);
						}
					}
				}
			}
		}
	}
}

void RPGSkillSystem::PlayerStartAimingSkill(const RPGSkillData* pSkill)
{
	pPlayerCurrentlyAimedSkill = pSkill;
}

void RPGSkillSystem::StopAimingSkill()
{
	pPlayerCurrentlyAimedSkill = nullptr;
	ClearAllAimingVisuals();
}

Vec2i RPGSkillSystem::GetPlayerAimLocation() const
{
	DOMLOG_ERROR_IF(pPlayerCurrentlyAimedSkill == nullptr, "Not aiming")

	return pCameraSystem->GetWorldGridCoordinateUnderMouse();
}

bool RPGSkillSystem::TryFireSkill(const RPGSkillData* pSkill, EntityId caster, const Vec2i& targetLocation)
{
	if (!pSkill || !pEcs || !pEcsEntityMap || !pRpgSystem || !pSkill->IsValid())
	{
		return false;
	}

	RPGSkillParams params = {*pEcs, *pEcsEntityMap, *pRpgSystem};
	params.caster = pEcs->GetPlayerEntityId();
	params.targetPos = pCameraSystem->GetWorldGridCoordinateUnderMouse();

	ComponentTransform& transformComponent = pEcs->GetComponent<ComponentTransform>(params.caster);
	params.startPos = { transformComponent.x, transformComponent.y };
	
	if (pSkill->pAimModule->IsAimValid(params))
	{
		pSkill->pEffectModule->ApplyEffect(params);
		return true;
	}

	return false;
}

void RPGSkillSystem::ClearAllAimingVisuals()
{
	for (RPGSkillHighlightEntry& skillHighlightVisual : skillHighlightVisuals)
	{
		skillHighlightVisual.ClearVisualsStack();
	}
}

std::unique_ptr<StatefulHotbarActionCastSkill> RPGSkillSystem::CreateStatefulHotbarActionCastSkillInstanceForSkill(const RPGSkillData* pSkill)
{
	std::unique_ptr<StatefulHotbarActionCastSkill> pHotbarAction = pCastSkillHotbarAction.MakeUnique();
	pHotbarAction->Setup(pSkill);
	return pHotbarAction;
}