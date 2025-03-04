#include "pch.h"

#include "RPGSystem.h"

#include "Attributes/RPGAttributes.h"
#include "Classes/RPGClassData.h"
#include "Components.h"
#include "DirectoryData.h"
#include "ECS.h"
#include "RenderPriorities.h"
#include "Renderer.h"
#include "RendererObject.h"

void RPGSystem::Init()
{
	DOMLOG_ERROR_IF(pEcs == nullptr, "Needs ECS")
	DOMLOG_ERROR_IF(pDirectoryData == nullptr, "Needs Directory Data")

	attributeSharedData.pEcs = pEcs;
}

void RPGSystem::SetupEntity(EntityId entity, RPGEntitySetupParams params)
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

			healthComponent.health = healthComponent.maxHealth;
		}
	}
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