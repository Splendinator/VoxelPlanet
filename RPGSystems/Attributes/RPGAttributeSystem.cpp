#include "pch.h"

#include "RPGAttributeSystem.h"

#include "ECSTypes.h"
#include "RPGAttributes.h"

void RPGAttributeSystem::Init()
{
	DOMLOG_ERROR_IF(pEcs == nullptr, "Need ECS to calcualte anything")
	sharedData.pEcs = pEcs;
}

#ifdef DOMIMGUI
void RPGAttributeSystem::DrawImGui(float deltaTime)
{
	if (pEcs)
	{
		for (EntityId entity = 0; entity < NUM_ENTITIES; ++entity)
		{
			ImGui::PushID(entity);
			bool bDisplayingThisEntity = false;
			for (const RPGAttributeBase* pAttribute : attributes)
			{
				if (pAttribute->CanApplyAttribute(entity, sharedData))
				{
					if (!bDisplayingThisEntity)
					{
						ImGui::Text("Entity %d", entity);
						bDisplayingThisEntity = true;
					}

					ImGui::Text("%s: %d", pAttribute->displayName.c_str(), pAttribute->GetAttributeValue(entity, sharedData));
				}
			}
			if (bDisplayingThisEntity)
			{
				ImGui::Separator();
			}
			ImGui::PopID();
		}
	}
}
#endif


