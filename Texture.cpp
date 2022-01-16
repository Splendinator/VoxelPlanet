#include "pch.h"

#include "Texture.h"

#include "stb_image.h"
#include "Renderer.h"
#include "../Graphics/VulkanUtils.h"


std::unordered_map<std::string, Texture*> textureMap;

Texture* Texture::CreateOrGetTexture(const char* fileName)
{
	std::string fullFilePath = std::string("..\\Content\\Textures\\") + std::string(fileName);

	// Check for duplicates
	auto foundTexture = textureMap.find(fullFilePath);
	if (foundTexture != textureMap.end())
	{
		return foundTexture->second;
	}

	Texture* texture = new Texture;
	int width, height, channels;
	unsigned char* pRawImageData = nullptr;

	// Read in image file
	{
		pRawImageData = stbi_load(fullFilePath.c_str(), &width, &height, &channels, 0);

		if (pRawImageData)
		{
			const int numBytes = width * height * channels;
			void* pRawTextureData;

			vkMapMemory(dmgf::GetDeviceHandle(), dmgf::GetStagingBufferMemoryHandle(), 0, numBytes, 0, &pRawTextureData);
			memcpy(pRawTextureData, pRawImageData, numBytes);
			vkUnmapMemory(dmgf::GetDeviceHandle(), dmgf::GetStagingBufferMemoryHandle());
		}
		else
		{
			DOMLOG_WARN("No image found!");
		}
	}

	// VkImage
	{
		DOMLOG_ERROR_IF(channels != 4, "Haven't got support for less than 4 channels yet");

		VkImageCreateInfo createInfo;

		u32 queueIndex = dmgf::GetGraphicsQueueFamily();

		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = (VkImageCreateFlagBits)0;
		createInfo.imageType = VkImageType::VK_IMAGE_TYPE_2D;
		createInfo.format = VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
		createInfo.extent = { (u32)width, (u32)height, 1 };
		createInfo.mipLevels = 1;
		createInfo.arrayLayers = 1;
		createInfo.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
		createInfo.tiling = VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
		createInfo.usage = (VkImageUsageFlagBits)(VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT);
		createInfo.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 1;
		createInfo.pQueueFamilyIndices = &queueIndex;
		createInfo.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;

		VulkanUtils::ErrorCheck(vkCreateImage(dmgf::GetDeviceHandle(), &createInfo, nullptr, &texture->imageHandle), "TextureImage");
	}

	// VkDeviceMemory
	{
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(dmgf::GetDeviceHandle(), texture->imageHandle, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VulkanUtils::FindMemoryType(memRequirements.memoryTypeBits, dmgf::GetPhysicalDeviceHandle(), VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VulkanUtils::ErrorCheck(vkAllocateMemory(dmgf::GetDeviceHandle(), &allocInfo, nullptr, &texture->imageMemory), "TextureMemory");
		VulkanUtils::ErrorCheck(vkBindImageMemory(dmgf::GetDeviceHandle(), texture->imageHandle, texture->imageMemory, 0), "BindTextureMemory");
	}

	// VkImageView
	{
		VkImageViewCreateInfo createInfo = {};

		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = texture->imageHandle;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		VulkanUtils::ErrorCheck(vkCreateImageView(dmgf::GetDeviceHandle(), &createInfo, nullptr, &texture->imageViewHandle), "Texture Image View");
	}

	// Move the texture data to device local memory and change format
	VkCommandBuffer commandBufferHandle;
	{
		VkCommandBufferAllocateInfo allocateInfo = {};

		allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocateInfo.commandPool = dmgf::GetCommandPool();
		allocateInfo.commandBufferCount = 1;
		allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

		VulkanUtils::ErrorCheck(vkAllocateCommandBuffers(dmgf::GetDeviceHandle(), &allocateInfo, &commandBufferHandle), "Texture command Buffer");
	}

	// Record and submit command buffer
	{
		VkCommandBufferBeginInfo commandBufferBegin = {};
		commandBufferBegin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer(commandBufferHandle, &commandBufferBegin);

		// 1. Translate the image from UYNDEFINED to TRANSFER_OPTIMAL to prepare it for the 
		{
			VkImageMemoryBarrier imageMemoryBarrier;
			imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imageMemoryBarrier.pNext = nullptr;
			imageMemoryBarrier.srcAccessMask = 0; // No access to the image has been done yet
			imageMemoryBarrier.dstAccessMask = VkAccessFlagBits::VK_ACCESS_TRANSFER_WRITE_BIT; // We are going to write to it.
			imageMemoryBarrier.oldLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED; // The image was UNDEFINED as we have only just created it.
			imageMemoryBarrier.newLayout = VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL; // We want it to be TRANSFER_DST since we're copying things into it from the staging buffer
			imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageMemoryBarrier.image = texture->imageHandle;
			imageMemoryBarrier.subresourceRange.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
imageMemoryBarrier.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
imageMemoryBarrier.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;

vkCmdPipelineBarrier(
	commandBufferHandle,
	VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, // Before the transform we need to finish the transfer (vkCmdCopyBufferToImage())
	VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, // No stages after the transform
	0,
	0,
	nullptr,
	0,
	nullptr,
	1,
	&imageMemoryBarrier
);
		}

		// 2. Copy from the raw bytes memcpy'd into stagingBuffer into the BrickWallImage texture
		{
		VkBufferImageCopy imageCopyParams;
		imageCopyParams.bufferOffset = 0;
		imageCopyParams.bufferImageHeight = 0;
		imageCopyParams.bufferRowLength = 0;

		imageCopyParams.imageSubresource.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopyParams.imageSubresource.mipLevel = 0;
		imageCopyParams.imageSubresource.baseArrayLayer = 0;
		imageCopyParams.imageSubresource.layerCount = 1;

		imageCopyParams.imageOffset.x = 0;
		imageCopyParams.imageOffset.y = 0;
		imageCopyParams.imageOffset.z = 0;

		imageCopyParams.imageExtent.width = width;
		imageCopyParams.imageExtent.height = height;
		imageCopyParams.imageExtent.depth = 1;

		vkCmdCopyBufferToImage(
			commandBufferHandle,
			dmgf::GetStagingBufferHandle(),
			texture->imageHandle,
			VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&imageCopyParams);
		}

		// 3. After the transfer is complete, change layout from TRANSFER_OPTIMAL to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL so it can be sampled from in the shader as a Sampler2D
		{
			VkImageMemoryBarrier imageMemoryBarrier;
			imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imageMemoryBarrier.pNext = nullptr;
			imageMemoryBarrier.srcAccessMask = 0; // VkAccessFlagBits::VK_ACCESS_HOST_WRITE_BIT; // We have done a write transfer so far
			imageMemoryBarrier.dstAccessMask = 0; // No access needed after transaction
			imageMemoryBarrier.oldLayout = VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			imageMemoryBarrier.newLayout = VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imageMemoryBarrier.image = texture->imageHandle;

			imageMemoryBarrier.subresourceRange.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
			imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
			imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
			imageMemoryBarrier.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
			imageMemoryBarrier.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;

			vkCmdPipelineBarrier(
				commandBufferHandle,
				VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT, // Before the transform we need to finish the transfer (vkCmdCopyBufferToImage())
				VkPipelineStageFlagBits::VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, // No stages after the transform since this is the last VkCommand
				0, // We have already written to the image
				0, // We are not doing anything else to the image afterwards so we don't need to access anything
				nullptr,
				0,
				nullptr,
				1,
				&imageMemoryBarrier
			);
		}

		vkEndCommandBuffer(commandBufferHandle);

		VkSubmitInfo submitInfo = {};
		submitInfo.pNext = nullptr;
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 0;
		submitInfo.pWaitSemaphores = nullptr;
		submitInfo.pWaitDstStageMask = nullptr;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBufferHandle;
		submitInfo.signalSemaphoreCount = 0;
		submitInfo.pSignalSemaphores = nullptr;

		vkQueueSubmit(dmgf::GetGraphicsQueueHandle(), 1, &submitInfo, nullptr);

		vkQueueWaitIdle(dmgf::GetGraphicsQueueHandle());
	}


	// Create the descriptor thingy?
	/// #TODO: I think these structs are created by the renderer when it swaps to another descriptor set.
	{

	}

	/// #TEMP: STBI_FREE raw data?
	/// #TEMP: Free all the vulkan resources??

	textureMap.emplace(fullFilePath, texture);
	return texture;
}
