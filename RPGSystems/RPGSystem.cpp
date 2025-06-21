#include "pch.h"

#include "RPGSystem.h"

#include "Attributes/RPGAttributes.h"
#include "Classes/RPGClassData.h"
#include "Core/DirectoryData.h"
#include "Ecs/Components.h"
#include "Ecs/ECS.h"
#include "Graphics/RenderPriorities.h"
#include "Graphics/Renderer.h"
#include "Graphics/RendererObject.h"

void RPGSystem::Init()
{
	DOMLOG_ERROR_IF(pEcs == nullptr, "Needs ECS")
	DOMLOG_ERROR_IF(pDirectoryData == nullptr, "Needs Directory Data")

	attributeSharedData.pEcs = pEcs;
	pSkillDamage = GetAttributeByType<RPGAttributeSkillDamage>();

	DOMLOG_WARN_IF(pSkillDamage == nullptr, "RPGAttributeSkillDamage needed to calculate skill damage");
}

void RPGSystem::RecalculateAttributesForEntity(EntityId entity)
{
	if (!attributeSharedData.pEcs)
	{
		DOMLOG_WARN("No ECS, can't recalculate attributes")
		return;
	}
	
	for (const RPGAttributeBase* pAttribute : attributes)
	{
		if (pAttribute)
		{
			if (pAttribute->CanApplyAttribute(entity, attributeSharedData))
			{
				pAttribute->ApplyAttribute(entity, attributeSharedData);
			}
		}
	}
}

void RPGSystem::SetupRPGEntity(EntityId entity, RPGEntitySetupParams params)
{
	if (pEcs && pDirectoryData)
	{
		if (params.pRaceData == nullptr)
		{
			DOMLOG_ERROR("All RPG entities need a race")
			return;
		}
		
		ComponentMesh& meshComponent = pEcs->AddComponent<ComponentMesh>(entity);
		ComponentProgression& progressionComponent = pEcs->AddComponent<ComponentProgression>(entity);
		pEcs->AddComponent<ComponentRigid>(entity);
		
		// Level setup
		progressionComponent.level = params.startLevel;
		
		// Class setup
		bool bAppliedClassMesh = false;
		if (params.pClassData)
		{
			ComponentClass& classComponent = pEcs->AddComponent<ComponentClass>(entity);
			classComponent.pClassData = params.pClassData;
			classComponent.pSpecialisationData = params.pSpecialisationData;

			if (params.bUseClassMeshOverRaceMesh)
			{
				meshComponent.pRendererObject = dmgf::AddObjectFromSVG(DirectoryData::ConcatenateSVGFilePathChecked(pDirectoryData->rpgClassVisuals, params.pClassData->vectorArtFileName).c_str());
				meshComponent.pRendererObject->SetRenderPriority(RenderPriority::unit);
				bAppliedClassMesh = true;
			}
		}

		// Race setup
		{
			ComponentRace& raceComponent = pEcs->AddComponent<ComponentRace>(entity);
			raceComponent.pRaceData = params.pRaceData;

			if (!bAppliedClassMesh)
			{
				meshComponent.pRendererObject = dmgf::AddObjectFromSVG(DirectoryData::ConcatenateSVGFilePathChecked(pDirectoryData->rpgRaceVisuals, params.pRaceData->vectorArtFileName).c_str());
				meshComponent.pRendererObject->SetRenderPriority(RenderPriority::unit);
			}
		}

		// Attribute setup (this must be done last as it relies on prior data)
		{
			ComponentHealth& healthComponent = pEcs->AddComponent<ComponentHealth>(entity);
			ComponentAction& actionComponent = pEcs->AddComponent<ComponentAction>(entity);
			
			RecalculateAttributesForEntity(entity);
			
			actionComponent.maxEnergy = 100; // #TODO: Setup energy attribute
			actionComponent.energy = actionComponent.maxEnergy;
			healthComponent.health = healthComponent.maxHealth;
		}
	}
}

void RPGSystem::DealDamage(RPGDamageParams params)
{
	if (pEcs)
	{
		float calculatedDamage = 0;
		
		switch (params.damageMagnitude.damageType)
		{
		case EDamageType::Skill:
		{
			int skillDamage = pSkillDamage->GetAttributeValue(params.attackerEntity, attributeSharedData);
			calculatedDamage = params.damageMagnitude.baseDamage + skillDamage * params.damageMagnitude.scaling;
			break;
		}

		case EDamageType::Physical:
		{
			// #TODO: This should be calculated based on the weapon.
			calculatedDamage = params.damageMagnitude.scaling * (20.f + pEcs->GetComponent<ComponentProgression>(params.attackerEntity).level * 10.f); 
			break;
		}

		default:
		{
			DOMLOG_ERROR("Unknown damage type", ENUMSTRING(EDamageType, params.damageMagnitude.damageType))
		}
		}
		
		ComponentHealth& targetHealthComponent = pEcs->GetComponent<ComponentHealth>(params.targetEntity);
		targetHealthComponent.health -= (int)calculatedDamage;
		
		if (targetHealthComponent.health <= 0)
		{
			// Killing blow

			// Award XP
			if (pEcs->EntityHasComponents<ComponentProgression>(params.attackerEntity))
			{
				ComponentProgression& progressionComponent = pEcs->GetComponent<ComponentProgression>(params.attackerEntity);
				if (const RPGLevelProgressionData* pLevelProgressionData = GetLevelProgressionDataForLevel(progressionComponent.level))
				{
					progressionComponent.currentXp += 20; // #TODO: Grab this from the race + level
					if (progressionComponent.currentXp >= pLevelProgressionData->requiredXp)
					{
						progressionComponent.currentXp = 0;
						++progressionComponent.level;
					
						RecalculateAttributesForEntity(params.attackerEntity);

						ComponentHealth& attackerHealthComponent = pEcs->GetComponent<ComponentHealth>(params.attackerEntity);
						attackerHealthComponent.health = attackerHealthComponent.maxHealth;
					}
				}
			}
		}
	}
}

const RPGLevelProgressionData* RPGSystem::GetLevelProgressionDataForLevel(u32 level) const
{
	if (level < levelProgressionData.size())
	{
		return &levelProgressionData[level];
	}
	
	return nullptr;
}

#ifdef DOMIMGUI
void RPGSystem::DrawImGui(float deltaTime)
{
	if (pEcs)
	{
		for (EntityId entity = 0; entity < NUM_ENTITIES; ++entity)
		{
			if (pEcs->EntityHasComponents<ComponentRace>(entity)) // We're just checking if the entity is a valid RPG one here, maybe there's a more robust way 
			{
				ImGui::PushID(entity);
				
				std::string treeString = "Entity " + std::to_string(entity);
				if (ImGui::TreeNodeEx(treeString.c_str()))
				{
					// Attributes
					for (const RPGAttributeBase* pAttribute : attributes)
					{
						if (pAttribute)
						{
							ImGui::Text("%s: %d", pAttribute->displayName.c_str(), pAttribute->GetAttributeValue(entity, attributeSharedData));
						}
					}
					ImGui::TreePop();
				}
				
				ImGui::PopID();
			}
		}
	}
}
#endif