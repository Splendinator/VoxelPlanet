#include "pch.h"

#include "Chunk.h"
#include "Renderer.h"

// #TODO: remove when we generate layers in data
#include "ChunkGeneratorGrass.h" 

Chunk::Chunk(ECS* pEcs, int inX, int inY) : gridX(inX), gridY(inY)
{
	// #TODO: This should read in chunk generator layers from data
	pChunkGenerator = new ChunkGeneratorGrass();

	for (int x = 0; x < CHUNK_SIZE; ++x)
	{
		for (int y = 0; y < CHUNK_SIZE; ++y)
		{
			for (int layer = 0; layer < EWorldGenerationLayer::COUNT; ++layer)
			{
				GetEntity((EWorldGenerationLayer)layer, x, y) = pChunkGenerator->CreateEntity(pEcs, (EWorldGenerationLayer)layer, x + gridX * CHUNK_SIZE, y + gridY * CHUNK_SIZE);
			}
		}
	}
}

Chunk::~Chunk()
{
	delete pChunkGenerator;
}

void Chunk::DeleteChunk(ECS* pEcs)
{
	for (EntityId entity : entities)
	{
		// Remove the mesh from the renderer
		// #TODO: Do we want a generic destructor for components to allow for this to happen automatically? Without adding a virtual function with 8 byte overhead ideally
		ComponentMesh mesh = pEcs->GetComponent<ComponentMesh>(entity);
		if (mesh.pRendererObject)
		{
			dmgf::RemoveObject(mesh.pRendererObject);
			mesh.pRendererObject = nullptr;
		}

		pEcs->DeleteEntity(entity);
	}
}