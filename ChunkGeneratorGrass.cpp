#include "pch.h"

#include "ChunkGeneratorGrass.h"
#include "FilePaths.h"
#include "RenderPriorities.h"
#include "Renderer.h"
#include "RendererObject.h"

EntityId ChunkGeneratorGrass::GetForeground(ECS* pEcs, int x, int y)
{
	if (!(rand() % 4))
	{
		EntityId entity = pEcs->GetNextFreeEntity();

		RendererObject* pTreeRenderObject = dmgf::AddObjectFromSVG(FilePath::VectorArt::tree);

		ComponentMesh& mesh = pEcs->AddComponent<ComponentMesh>(entity);
		ComponentTransform& transform = pEcs->AddComponent<ComponentTransform>(entity);
		pEcs->AddComponent<ComponentRigid>(entity);

		mesh.pRendererObject = pTreeRenderObject;
		transform.x = x;
		transform.y = y;

		pTreeRenderObject->SetRenderPriority(RenderPriority::foreground);

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

	RendererObject* pGrassRendererObject = dmgf::AddObjectFromSVG(FilePath::VectorArt::grass);

	pGrassRendererObject->SetRenderPriority(RenderPriority::background);

	// #TODO: Add mesh and transform creation to a util func. render prio should be based on layer
	ComponentMesh& mesh = pEcs->AddComponent<ComponentMesh>(entity);
	ComponentTransform& transform = pEcs->AddComponent<ComponentTransform>(entity);

	mesh.pRendererObject = pGrassRendererObject;
	transform.x = x;
	transform.y = y;

	return entity;
}
