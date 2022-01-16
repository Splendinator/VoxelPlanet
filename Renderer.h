#pragma once

#include "DomUtils/DomUtils.h"
#include "DomMath/Mat4.h"

class Camera;
class Mesh;
class Texture;
struct ObjectEntry;

namespace dmgf
{
	// Initialise graphics -- Must be called after the window has been initialised.
	void Init();

	// Uninitialise graphics
	void UnInit();

	// Once per frame
	void Tick(float deltaTime);

	Camera& GetCamera();

	// Getters for various parts of the vulkan pipeline
	VkDevice& GetDeviceHandle();
	VkPhysicalDevice& GetPhysicalDeviceHandle();
	VkDeviceMemory& GetStagingBufferMemoryHandle();
	VkBuffer& GetStagingBufferHandle();
	VkCommandPool& GetCommandPool();
	
	VkQueue& GetGraphicsQueueHandle();
	u32 GetGraphicsQueueFamily(); // Get the index of the queue family that we are using as the graphics queue

	// Add a new object to render. This returns a pointer handler that is used to update this objects position etc at runtime
	void* AddNewObject(Texture* texture, Mesh* mesh);
	void SetObjectPos(void* pHandle, const Mat4f& modelMatrix);
}
