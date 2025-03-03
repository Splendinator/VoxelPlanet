#include "pch.h"

#include "RPGClassSystem.h"

#include "DirectoryData.h"
#include "ECS.h"
#include "RPGClassData.h"
#include "RenderPriorities.h"
#include "Renderer.h"
#include "RendererObject.h"

void RPGClassSystem::Init()
{
	DOMLOG_ERROR_IF(pEcs == nullptr, "Requires ECS")
	DOMLOG_ERROR_IF(pDirectoryData == nullptr, "Requires DirectoryData")
}

void RPGClassSystem::SetEntityClass(EntityId entity, ERPGClass rpgClass, bool bSetMesh) const
{
	if (pEcs)
	{
		ComponentClass& classComponent = pEcs->AddComponent<ComponentClass>(entity);
		classComponent.rpgClass = rpgClass;

		if (bSetMesh && pDirectoryData)
		{
			if (const RPGClassData* pData = FindClassData(rpgClass))
			{
				ComponentMesh& meshComponent = pEcs->AddComponent<ComponentMesh>(entity);
				meshComponent.pRendererObject = dmgf::AddObjectFromSVG(DirectoryData::ConcatenateFilePathChecked(pDirectoryData->rpgClassVisuals, pData->vectorArtFileName, ".svg").c_str());
				meshComponent.pRendererObject->SetRenderPriority(RenderPriority::unit);
			}
			else
			{
				DOMLOG_ERROR("No class data for class", ENUMSTRING(ERPGClass, rpgClass));
			}
		}
	}
}

void RPGClassSystem::SetEntitySpecialisation(EntityId entity, ERPGClassSpecialisation rpgSpecialisation) const
{
	if (pEcs)
	{
		ComponentClass& classComponent = pEcs->AddComponent<ComponentClass>(entity);
		classComponent.specialisation = rpgSpecialisation;	
	}
}

const RPGClassData* RPGClassSystem::FindClassData(ERPGClass rpgClass) const
{
	for (const RPGClassEntry& entry : classes)
	{
		if (entry.rpgClass == rpgClass)
		{
			return entry.pClassData;
		}
	}
	
	return nullptr;
}
