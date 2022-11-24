#include "pch.h"

#include "ChunkGeneratorGrass.h"

#include "ActionDeciderAI.h" /// #TEMP: Not needed probably after we generate enemies in post
#include "FilePaths.h"
#include "RenderPriorities.h"
#include "Renderer.h"
#include "RendererObject.h"

EntityId ChunkGeneratorGrass::GetForeground(ECS* pEcs, int x, int y)
{
	if (!(rand() % 10))
	{
		// Generate tree
		EntityId entity = pEcs->GetNextFreeEntity();

		TransientPtr<RendererObject> pTreeRenderObject = dmgf::AddObjectFromSVG(FilePath::VectorArt::tree);

		ComponentMesh& mesh = pEcs->AddComponent<ComponentMesh>(entity);
		ComponentTransform& transform = pEcs->AddComponent<ComponentTransform>(entity);

		pEcs->AddComponent<ComponentRigid>(entity);

		mesh.pRendererObject = pTreeRenderObject;
		transform.x = x;
		transform.y = y;

		pTreeRenderObject->SetRenderPriority(RenderPriority::foreground);

		return entity;
	}
	else if (!(rand()%20))
	{
		const bool bEnemy = (rand() % 2);
		
		// Generate enemy
		EntityId entity = pEcs->GetNextFreeEntity();
		
		TransientPtr<RendererObject> pEnemyRenderObject = dmgf::AddObjectFromSVG(bEnemy ? FilePath::VectorArt::enemy : FilePath::VectorArt::player);
		
		ComponentMesh& mesh = pEcs->AddComponent<ComponentMesh>(entity);
		ComponentTransform& transform = pEcs->AddComponent<ComponentTransform>(entity);
		ComponentAction& action = pEcs->AddComponent<ComponentAction>(entity);
		ComponentHealth& health = pEcs->AddComponent<ComponentHealth>(entity);
		ComponentFaction& faction = pEcs->AddComponent<ComponentFaction>(entity);
		pEcs->AddComponent<ComponentRigid>(entity);
		
		mesh.pRendererObject = pEnemyRenderObject;
		transform.x = x;
		transform.y = y;
		action.maxEnergy = 100;
		action.energy = 100;
		action.pActionDecider = new ActionDeciderAI;
		health.health = 100;
		health.maxHealth = 100;
		faction.factionFlags = bEnemy ? ComponentFaction::EFactionFlags::Enemy : ComponentFaction::EFactionFlags::Player;
		
		pEnemyRenderObject->SetRenderPriority(RenderPriority::unit);
		
		return INVALID_ENTITY_ID; /// #TEMP: We don't want the chunk to own this entity
	}
	else
	{
		return INVALID_ENTITY_ID;
	}
}

EntityId ChunkGeneratorGrass::GetBackground(ECS* pEcs, int x, int y)
{
	EntityId entity = pEcs->GetNextFreeEntity();

	TransientPtr<RendererObject> pGrassRendererObject = dmgf::AddObjectFromSVG(FilePath::VectorArt::grass);

	pGrassRendererObject->SetRenderPriority(RenderPriority::background);

	// #TODO: Add mesh and transform creation to a util func. render prio should be based on layer
	ComponentMesh& mesh = pEcs->AddComponent<ComponentMesh>(entity);
	ComponentTransform& transform = pEcs->AddComponent<ComponentTransform>(entity);

	mesh.pRendererObject = pGrassRendererObject;
	transform.x = x;
	transform.y = y;

	return entity;
}

EntityId ChunkGeneratorDebug::GetBackground(ECS* pEcs, int x, int y)
{
	EntityId entity = pEcs->GetNextFreeEntity();

	TransientPtr<RendererObject> pGrassRendererObject = dmgf::AddObjectFromSVG(FilePath::VectorArt::dirt);

	pGrassRendererObject->SetRenderPriority(RenderPriority::background);

	// #TODO: Add mesh and transform creation to a util func. render prio should be based on layer
	ComponentMesh& mesh = pEcs->AddComponent<ComponentMesh>(entity);
	ComponentTransform& transform = pEcs->AddComponent<ComponentTransform>(entity);

	mesh.pRendererObject = pGrassRendererObject;
	transform.x = x;
	transform.y = y;

	return entity;
}
