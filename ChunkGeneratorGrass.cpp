#include "pch.h"

#include "ChunkGeneratorGrass.h"
#include "Renderer.h"
#include "RendererObject.h"

EntityId ChunkGeneratorGrass::GetForeground(ECS* pEcs, int x, int y)
{
	if (!(rand() % 4))
	{
		EntityId entity = pEcs->GetNextFreeEntity();

		RendererObject* pTreeRenderObject = dmgf::AddObjectFromSVG("C:/Users/Dominic/Desktop/Tree.svg");

		ComponentMesh& mesh = pEcs->AddComponent<ComponentMesh>(entity);
		ComponentTransform& transform = pEcs->AddComponent<ComponentTransform>(entity);
		pEcs->AddComponent<ComponentRigid>(entity);
		ComponentHealth& health = pEcs->AddComponent<ComponentHealth>(entity);
		health.health = 100;

		mesh.pRendererObject = pTreeRenderObject;
		transform.x = x;
		transform.y = y;

		pTreeRenderObject->SetRenderPriority(0.2f);

		return entity;
	}
	else
	{
		return INVALID_ENTITY_ID;
	}
}

EntityId ChunkGeneratorGrass::GetBackground(ECS* pEcs, int x, int y)
{
	EntityId entity = pEcs->GetNextFreeEntity();

	RendererObject* pGrassRendererObject = dmgf::AddObjectFromSVG("C:/Users/Dominic/Desktop/Grass.svg");

	pGrassRendererObject->SetRenderPriority(0.3f);

	// #TODO: Add mesh and transform creation to a util func. render prio should be based on layer
	ComponentMesh& mesh = pEcs->AddComponent<ComponentMesh>(entity);
	ComponentTransform& transform = pEcs->AddComponent<ComponentTransform>(entity);

	mesh.pRendererObject = pGrassRendererObject;
	transform.x = x;
	transform.y = y;

	return entity;
}
