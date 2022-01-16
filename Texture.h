#pragma once

class Texture
{
public:
	// This should only be called after the renderer has initialised
	static Texture* CreateOrGetTexture(const char* fileName);

	VkImage imageHandle;
	VkImageView imageViewHandle;
	VkDeviceMemory imageMemory;
};

