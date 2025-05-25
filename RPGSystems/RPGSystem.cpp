#include "pch.h"

#include "RPGSystem.h"

#include "Attributes/RPGAttributes.h"
#include "Classes/RPGClassData.h"
#include "DirectoryData.h"
#include "Ecs/Components.h"
#include "Ecs/ECS.h"
#include "RenderPriorities.h"
#include "Renderer.h"
#include "RendererObject.h"

void RPGSystem::Init()
{
	DOMLOG_ERROR_IF(pEcs == nullptr, "Needs ECS")
	DOMLOG_ERROR_IF(pDirectoryData == nullptr, "Needs Directory Data")

	attributeSharedData.pEcs = pEcs;
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

// #TEMP: Optimisation
#pragma optimize("", off)
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
			
			actionComponent.maxEnergy = 100; // #TEMP: Setup energy attribute
			actionComponent.energy = actionComponent.maxEnergy;
			healthComponent.health = healthComponent.maxHealth;
		}
	}
}
#pragma optimize("", on)

void RPGSystem::DealDamage(RPGDamageParams params)
{
	if (pEcs)
	{
		ComponentHealth& targetHealthComponent = pEcs->GetComponent<ComponentHealth>(params.targetEntity);
		targetHealthComponent.health -= params.damage;
		
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
							if (pAttribute->CanApplyAttribute(entity, attributeSharedData))
							{
								ImGui::Text("%s: %d", pAttribute->displayName.c_str(), pAttribute->GetAttributeValue(entity, attributeSharedData));
							}
						}
					}
				}

				ImGui::PopID();
			}
		}
	}
}
#endif