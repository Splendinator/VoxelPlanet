#pragma once

#include "DomMath/Mat4.h"

// Don't use this class outside of the graphics module, this isn't part of the API. This file is just to keep the particle system code in one place.
namespace dmgf
{
	namespace Internal
	{
		namespace ParticleSystem
		{
			struct ParticleSystemInitInfo
			{
				VkInstance handleInstance = VK_NULL_HANDLE;
				VkPhysicalDevice handlePhysicalDevice = VK_NULL_HANDLE;
				VkDevice handleDevice = VK_NULL_HANDLE;
				VkRenderPass handleRenderPass = VK_NULL_HANDLE; // The render pass we will draw too. #TODO: this should be a sub-pass
				Mat4f* pViewMatrix = nullptr;
			};
			
			void Init(const ParticleSystemInitInfo& info);

			void Draw(VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer);
		}
	}
}