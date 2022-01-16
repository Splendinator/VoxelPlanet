#pragma once

#include "DomUtils/DomUtils.h"

#include <vector>

class Camera;
class Chunk;
class VulkanModuleCommandBuffer;

namespace dmgf
{
	// Position - Normal - Colour
	struct Quad
	{
		float verts[54];
	};

	// Initialise graphics -- Must be called after the window has been initialised.
	void Init();

	// Uninitialise graphics
	void UnInit();

	// Once per frame
	void Tick(float deltaTime);

	Camera& GetCamera();

	void AddChunk(Chunk* chunk, std::vector<dmgf::Quad>& quads);
	void RemoveChunk(Chunk* chunk);
	void DrawChunks(VulkanModuleCommandBuffer& commandBuffer);
}
