#include "pch.h"

#define WIN32_LEAN_AND_MEAN

#ifdef RENDERER_OLD /// We aren't using this render any more, just here for reference

#include "Renderer.h"

#include "DomMath/Vec3.h"
#include "DomMath/Vec2.h"
#include "DomMath/Mat4.h"
#include "DomMath/Math.h"
#include "DomLog/DomLog.h"
#include "DomUtils/DomUtils.h"
#include "DomImport/DomImport.h"
#include "DomWindow/DomWindow.h"
#include "stb_image.h"
#include "tinyobj_loader.h"

#include "../Graphics/Camera.h"
#include "../Graphics/VulkanModuleBuffer.h"
#include "../Graphics/VulkanModuleCommandBuffer.h"
#include "../Graphics/VulkanModuleCommandPool.h"
#include "../Graphics/VulkanModuleDebugMessenger.h"
#include "../Graphics/VulkanModuleDescriptorBufferInfo.h"
#include "../Graphics/VulkanModuleDescriptorImageInfo.h"
#include "../Graphics/VulkanModuleDescriptorPool.h"
#include "../Graphics/VulkanModuleDescriptorSet.h"
#include "../Graphics/VulkanModuleDescriptorSetBinding.h"
#include "../Graphics/VulkanModuleDescriptorSetLayout.h"
#include "../Graphics/VulkanModuleDescriptorSetWrite.h"
#include "../Graphics/VulkanModuleDevice.h"
#include "../Graphics/VulkanModuleDeviceMemoryBuffer.h"
#include "../Graphics/VulkanModuleDeviceMemoryImage.h"
#include "../Graphics/VulkanModuleFramebuffer.h"
#include "../Graphics/VulkanModuleImage.h"
#include "../Graphics/VulkanModuleImageSwapChain.h"
#include "../Graphics/VulkanModuleImageView.h"
#include "../Graphics/VulkanModuleInstance.h"
#include "../Graphics/VulkanModulePhysicalDevice.h"
#include "../Graphics/VulkanModulePipeline.h"
#include "../Graphics/VulkanModulePipelineLayout.h"
#include "../Graphics/VulkanModuleQueue.h"
#include "../Graphics/VulkanModuleRenderPass.h"
#include "../Graphics/VulkanModuleSampler.h"
#include "../Graphics/VulkanModuleShader.h"
#include "../Graphics/VulkanModuleSurface.h"
#include "../Graphics/VulkanModuleSwapChain.h"
#include "../Graphics/VulkanUtils.h"
#include "../Graphics/VulkanVertexData.h"
#include "Texture.h"
#include "Mesh.h"

#include <Windows.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#ifdef DOMIMGUI
#include "imgui_impl_vulkan.h"
#include "../Graphics/VulkanModuleAttachmentReference.h"
#endif //~ #ifdef DOMIMGUI

// C:/VulkanSDK/1.2.131.2/Bin/glslc.exe Triangle.vert -o vert.spv

struct MVP
{
	Mat4f models[256] = {};
	Mat4f view = Mat4f::Identity();
	Mat4f proj = dmma::generateProjection(90.0f, 16.f / 9.f, 0.0001f, 1000.0f);
} mvp;

dmut::HeapAlloc<char> GetShaderFilepath(const char* shaderName);

Camera camera;
u32 currentBuffer = 0;
const char* destPath = "../Graphics/Shaders/CompiledShaders"; // Destination shader files

std::vector<float> rawVertexData;

// The instance. This is the alpha and the omega.
VulkanModuleInstance vulkanInstance;
// Debug messenger.
VulkanModuleDebugMessenger vulkanDebugMessenger(&vulkanInstance);
// Surface (window)
VulkanModuleSurface vulkanSurface(&vulkanInstance);
// Physical device
VulkanModulePhysicalDevice vulkanPhysicalDevice(&vulkanInstance);
// Logical device
VulkanModuleDevice vulkanDevice(&vulkanPhysicalDevice, &vulkanSurface);
// Graphics Queue
VulkanModuleQueue vulkanGraphicsQueue(VK_QUEUE_GRAPHICS_BIT, &vulkanDevice); 
// Command Pool
VulkanModuleCommandPool vulkanCommandPool(&vulkanDevice);
// Command Buffer
VulkanModuleCommandBuffer vulkanCommandBuffer(&vulkanDevice, &vulkanCommandPool);
VulkanModuleCommandBuffer vulkanCommandBuffer2(&vulkanDevice, &vulkanCommandPool);
VulkanModuleCommandBuffer commandBufferStaging(&vulkanDevice, &vulkanCommandPool);
// Swap Chain
VulkanModuleSwapChain vulkanSwapChain(VkFormat::VK_FORMAT_B8G8R8A8_UNORM, &vulkanPhysicalDevice, &vulkanDevice, &vulkanSurface);
// Images
VulkanModuleImageSwapChain vulkanSwapChainImage1(0, &vulkanSwapChain); // Colour images
VulkanModuleImageSwapChain vulkanSwapChainImage2(1, &vulkanSwapChain);
VulkanModuleImage vulkanDepthBufferImage // Depth image
(
	VkImageType::VK_IMAGE_TYPE_2D,
	VkFormat::VK_FORMAT_D24_UNORM_S8_UINT,
	&vulkanSwapChain.GetImageWidth(),
	&vulkanSwapChain.GetImageHeight(),
	nullptr,
	1,
	VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
	VkImageTiling::VK_IMAGE_TILING_OPTIMAL,
	VkImageUsageFlagBits::VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
	VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,
	VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
	&vulkanDevice
);
VulkanModuleDeviceMemoryImage depthBufferMemory(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &vulkanPhysicalDevice, &vulkanDevice, &vulkanDepthBufferImage); //We need to explicitly give the depth buffer memory, but not the swap chain images as they already have memory from the swap chain

// Image views
VulkanModuleImageView vulkanSwapChainImageView1(vulkanSwapChainImage1.GetImageFormat(), VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, &vulkanDevice, &vulkanSwapChainImage1);
VulkanModuleImageView vulkanSwapChainImageView2(vulkanSwapChainImage2.GetImageFormat(), VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, &vulkanDevice, &vulkanSwapChainImage2);
VulkanModuleImageView vulkanDepthBufferImageView(vulkanDepthBufferImage.GetImageFormat(), VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT, &vulkanDevice, &vulkanDepthBufferImage);

//~ Begin render pass
// Attachments
VulkanModuleAttachmentDescription colourAttachment(
	VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
	VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR,
	VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE,
	VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
	VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
	vulkanSwapChain.GetFormat(),
	&vulkanDevice);
VulkanModuleAttachmentDescription depthAttachment(
	VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
	VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR,
	VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE,
	VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
	VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
	vulkanDepthBufferImageView.GetFormat(),
	&vulkanDevice);
VulkanModuleArraySized<VulkanModuleAttachmentDescription, 2> allAttachments({ &colourAttachment, &depthAttachment });

// Subpass -- Colour render
VulkanModuleAttachmentReference colourAttachmentReference(EAttachmentType::Colour, VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, &colourAttachment, &allAttachments);
VulkanModuleAttachmentReference depthAttachmentReference(EAttachmentType::Depth, VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, &depthAttachment, &allAttachments);
VulkanModuleArraySized<VulkanModuleAttachmentReference, 2> renderSubpassAttachments({ &colourAttachmentReference, &depthAttachmentReference });
VulkanModuleRenderSubpassDescription renderSubpass(VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, &renderSubpassAttachments);
VulkanModuleArraySized<VulkanModuleAttachmentReference, 1> imguiSubpassAttachments({ &colourAttachmentReference });
VulkanModuleRenderSubpassDescription imGuiSubpass(VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, &imguiSubpassAttachments);

VulkanModuleArraySized<VulkanModuleRenderSubpassDescription, 2> allSubpasses({ &renderSubpass, &imGuiSubpass });

// Subpass Dependancies
VulkanModuleOptional<VulkanModuleRenderSubpassDescription> noSubpass;
VulkanModuleOptional<VulkanModuleRenderSubpassDescription> firstSubpass(&renderSubpass);
VulkanModuleOptional<VulkanModuleRenderSubpassDescription> secondSubpass(&imGuiSubpass);
VulkanModuleArraySized<VulkanModuleOptional<VulkanModuleRenderSubpassDescription>, 2> imGuiSubpassParams({ &firstSubpass, &secondSubpass });
VulkanModuleRenderSubpassDependancy imGuiSubpassDependancy(
	VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
	VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_READ_BIT& VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
	VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
	VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_READ_BIT& VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
	&imGuiSubpassParams,
	&allSubpasses);

///VulkanModuleArraySized<VulkanModuleOptional<VulkanModuleRenderSubpassDescription>, 2> imGuiSubpass2Params({ &noSubpass, &firstSubpass });
///VulkanModuleRenderSubpassDependancy imGuiSubpassDependancy2(
///	VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
///	0,
///	VkPipelineStageFlagBits::VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
///	VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
///	&imGuiSubpass2Params,
///	&allSubpasses);

VulkanModuleArraySized<VulkanModuleRenderSubpassDependancy, 1> allSubpassDependancies({ &imGuiSubpassDependancy });

// Render pass
VulkanModuleRenderPass vulkanRenderPass(&vulkanDevice, &allAttachments, &allSubpasses, &allSubpassDependancies);
//~ End render pass

// Shaders
VulkanModuleShader vulkanShaderVertex(EShaderStage::Vertex, GetShaderFilepath("TriangleVert"), &vulkanDevice);
VulkanModuleShader vulkanShaderFragment(EShaderStage::Fragment, GetShaderFilepath("TriangleFrag"), &vulkanDevice);
// Framebuffers
VulkanModuleArraySized<VulkanModuleImageView, 2> frameBuffer1ImageViews({ &vulkanSwapChainImageView1, &vulkanDepthBufferImageView });
VulkanModuleArraySized<VulkanModuleImageView, 2> frameBuffer2ImageViews({ &vulkanSwapChainImageView2, &vulkanDepthBufferImageView });
VulkanModuleFramebuffer vulkanModuleFrameBuffer1(&vulkanDevice, &vulkanRenderPass, &vulkanSwapChain, &frameBuffer1ImageViews);
VulkanModuleFramebuffer vulkanModuleFrameBuffer2(&vulkanDevice, &vulkanRenderPass, &vulkanSwapChain, &frameBuffer2ImageViews);
// Vertex Buffer Creation Data
VulkanModuleBuffer<VulkanVertexData<VulkanVertexAttributePos, VulkanVertexAttributeNormal, VulkanVertexAttributeUVCoords>> vulkanModuleVertexBuffer(VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, &vulkanDevice, 10000);
// Vertex Buffer Memory
VulkanModuleDeviceMemoryBuffer vulkanModuleVertexMemory(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &vulkanPhysicalDevice, &vulkanDevice, &vulkanModuleVertexBuffer);
// Shader Array
VulkanModuleArraySized<VulkanModuleShader, 2> vulkanModuleShaders({ &vulkanShaderVertex, &vulkanShaderFragment });

// Brick wall texture
const u32 brickWallImageSize = 1024;
const u32 brickWallImageDepth = 1;
VulkanModuleImage moduleBrickWallImage(
	VkImageType::VK_IMAGE_TYPE_2D,
	VkFormat::VK_FORMAT_R8G8B8A8_UNORM,
	&brickWallImageSize,
	&brickWallImageSize,
	&brickWallImageDepth,
	1,
	VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
	VkImageTiling::VK_IMAGE_TILING_OPTIMAL,
	(VkImageUsageFlagBits)(VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT),
	VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,
	VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
	&vulkanDevice
);
/// #TEMP: Make this into a staging buffer?
VulkanModuleDeviceMemoryImage moduleBrickWallMemory(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &vulkanPhysicalDevice, &vulkanDevice, &moduleBrickWallImage);
VulkanModuleImageView moduleBrickWallImageView(VkFormat::VK_FORMAT_R8G8B8A8_UNORM, VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, &vulkanDevice, &moduleBrickWallImage);

// Descriptor Pool
VulkanModuleDescriptorPool vulkanModuleDescriptorPool(&vulkanDevice);
// Descriptor Buffer
VulkanModuleBuffer<VulkanVertexData<VulkanVertexAttributeMatrix1, VulkanVertexAttributeMatrix2, VulkanVertexAttributeMatrix3>> bufferMVPMatrix(VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, &vulkanDevice);
VulkanModuleDeviceMemoryBuffer bufferMemoryMVPMatrix(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &vulkanPhysicalDevice, &vulkanDevice, &bufferMVPMatrix);
VulkanModuleDescriptorBufferInfo descriptorInfoBufferMVPMatrix(&bufferMVPMatrix);
// Descriptor Staging Buffer
VulkanModuleBuffer<VulkanVertexData<VulkanVertexAttributeSingleByte>> bufferStaging(VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT, &vulkanDevice, 1024*1024*16);
VulkanModuleDeviceMemoryBuffer bufferMemoryStaging(VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &vulkanPhysicalDevice, &vulkanDevice, &bufferStaging);
// Descriptor Sampler2D
VulkanModuleSampler moduleSampler(&vulkanDevice);
VulkanModuleDescriptorImageInfo moduleDescriptorImageInfo(&moduleSampler, &moduleBrickWallImageView);
// Descriptor Binding
VulkanModuleDescriptorSetBinding vulkanModuleDescriptorSetBinding(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, /*arraySize*/1, &descriptorInfoBufferMVPMatrix);
VulkanModuleDescriptorSetBinding moduleDescriptorSetBindingSampler(VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, &moduleDescriptorImageInfo);
// Descriptor Set Layout
VulkanModuleArraySized<VulkanModuleDescriptorSetBinding, 2> bindings({ &vulkanModuleDescriptorSetBinding, &moduleDescriptorSetBindingSampler });
VulkanModuleDescriptorSetLayout vulkanModuleDescriptorSetLayout(&vulkanDevice, &bindings);
// Descriptor Set
VulkanModuleDescriptorSet vulkanModuleDescriptorSet(&vulkanDevice, &vulkanModuleDescriptorPool, &vulkanModuleDescriptorSetLayout);
// Write operation
VulkanModuleDescriptorSetWrite vulkanModuleDescriptorSetWrite(VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &vulkanDevice, &vulkanModuleDescriptorSet, &vulkanModuleDescriptorSetBinding);
VulkanModuleDescriptorSetWrite moduleDescriptorSetWriteSampler(VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, &vulkanDevice, &vulkanModuleDescriptorSet, &moduleDescriptorSetBindingSampler);
// Pipeline Layout
VulkanModuleArraySized<VulkanModuleDescriptorSetLayout, 1> pipelineLayoutDescriptorSets({ &vulkanModuleDescriptorSetLayout });
VulkanModulePipelineLayout vulkanModulePipelineLayout(&vulkanDevice, &pipelineLayoutDescriptorSets);
// Vertex Data
VulkanVertexData<VulkanVertexAttributePos> vertexData; /// #TODO: Instead of passing this in, make a VulkanModuleVertedBuffer that derrives from VulkanModuleBuffer and has a GetVertexData() thing"

/// IMGUI
VulkanModuleShader imGuiShaderVertex(EShaderStage::Vertex, GetShaderFilepath("ImGuiVert"), &vulkanDevice);
VulkanModuleShader imGuiShaderFragment(EShaderStage::Fragment, GetShaderFilepath("ImGuiFrag"), &vulkanDevice);

VulkanModuleArraySized<VulkanModuleShader, 2> imGuiShaders({ &vulkanShaderVertex, &vulkanShaderFragment });

// Pipeline
VulkanModulePipeline vulkanPipeline(&vulkanDevice, &vulkanSwapChain, &vulkanRenderPass, &vulkanModulePipelineLayout, &imGuiShaders, &vulkanModuleVertexBuffer);

#ifdef DOMIMGUI
VulkanModuleDescriptorPool imGuiDescriptorPool(&vulkanDevice);

VulkanModuleSampler imGuiSampler(&vulkanDevice);
VulkanModuleDescriptorImageInfo imGuiImageInfo(&imGuiSampler, &vulkanSwapChainImageView1);
VulkanModuleDescriptorSetBinding imGuiDescriptorSetBinding(VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, &imGuiImageInfo);

VulkanModuleArraySized<VulkanModuleDescriptorSetBinding, 1> imGuiBindings({ &imGuiDescriptorSetBinding });
VulkanModuleDescriptorSetLayout imGuiDescriptorSetLayout(&vulkanDevice, &imGuiBindings);
VulkanModuleDescriptorSetReserve imGuiDescriptorSet(&vulkanDevice, &imGuiDescriptorPool, &imGuiDescriptorSetLayout);

VulkanModuleCommandBuffer imGuiFontCommandBuffer(&vulkanDevice, &vulkanCommandPool);
#endif //~ #ifdef DOMIMGUI

#ifdef DOMIMGUI
void ImGuiDebugCallback(VkResult result)
{
	VulkanUtils::ErrorCheck(result, "IMGUI");
}
#endif //~ #ifdef DOMIMGUI

void dmgf::Init_OLD()
{
	vulkanModuleDescriptorPool.ReserveBufferType(VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
	vulkanModuleDescriptorPool.ReserveBufferType(VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
	vulkanModuleDescriptorPool.ReserveBufferType(VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
	vulkanModuleDescriptorPool.ReserveBufferType(VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

#ifndef DOMRELEASE
	const char* compilerPath = "C:/VulkanSDK/1.2.131.2/Bin/glslc.exe"; // Compiler
	const char* sourcePath = "../Graphics/Shaders"; // Source text files
	VulkanUtils::CompileShaders(compilerPath, sourcePath, destPath);
#endif //~ #ifndef DOMRELEASE

	vulkanInstance.PreInitialise();
	vulkanInstance.Initialise();

	// Load a 3d model from file
	{
		tinyobj::attrib_t attributes;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string error;
		std::string warning;

		bool bSuccess = tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, "..\\Content\\Meshes\\Monkey.obj");

		if (bSuccess)
		{
			for (const tinyobj::shape_t& shape : shapes)
			{
				for (tinyobj::index_t index : shape.mesh.indices)
				{
					// Position
					rawVertexData.push_back(attributes.vertices[(int)index.vertex_index * 3 + 0]);
					rawVertexData.push_back(attributes.vertices[(int)index.vertex_index * 3 + 1]);
					rawVertexData.push_back(attributes.vertices[(int)index.vertex_index * 3 + 2]);

					// Normals
					rawVertexData.push_back(attributes.normals[(int)index.normal_index * 3 + 0]);
					rawVertexData.push_back(attributes.normals[(int)index.normal_index * 3 + 1]);
					rawVertexData.push_back(attributes.normals[(int)index.normal_index * 3 + 2]);

					// TexCoords
					rawVertexData.push_back(attributes.texcoords[(int)index.texcoord_index * 2 + 0]);
					rawVertexData.push_back(attributes.texcoords[(int)index.texcoord_index * 2 + 1]);
				}
			}
		}
		else
		{
			DOMLOG_WARN("No 3d model!")
		}

		void* pData;
		const size_t numBytes = rawVertexData.size() * sizeof(*rawVertexData.data());

		vkMapMemory(vulkanDevice.GetHandle(), vulkanModuleVertexMemory.GetHandle(), 0, rawVertexData.size(), 0, &pData);
		memcpy(pData, rawVertexData.data(), numBytes);
		vkUnmapMemory(vulkanDevice.GetHandle(), vulkanModuleVertexMemory.GetHandle());
	}

	// Load the image data from the jpg file
	{
		int width, height, channels;
		unsigned char* rawImageData = nullptr;
		rawImageData = stbi_load("..\\Content\\Textures\\Green.png", &width, &height, &channels, 0);

		if (rawImageData)
		{
			const int numBytes = width * height * channels;
			void* pRawTextureData;

			vkMapMemory(vulkanDevice.GetHandle(), bufferMemoryStaging.GetHandle(), 0, numBytes, 0, &pRawTextureData);
			memcpy(pRawTextureData, rawImageData, numBytes);
			vkUnmapMemory(vulkanDevice.GetHandle(), bufferMemoryStaging.GetHandle());
		}
		else
		{
			DOMLOG_WARN("No image found!");
		}
	}

	// Staging command buffer
	{
		VkCommandBufferBeginInfo commandBufferBegin = {};
		commandBufferBegin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkBeginCommandBuffer(commandBufferStaging.GetHandle(), &commandBufferBegin);

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
			imageMemoryBarrier.image = moduleBrickWallImage.GetHandle();

			imageMemoryBarrier.subresourceRange.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
			imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
			imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
			imageMemoryBarrier.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
			imageMemoryBarrier.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;

			vkCmdPipelineBarrier(
				commandBufferStaging.GetHandle(),
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

			imageCopyParams.imageExtent.width = brickWallImageSize;
			imageCopyParams.imageExtent.height = brickWallImageSize;
			imageCopyParams.imageExtent.depth = 1;

			vkCmdCopyBufferToImage(
				commandBufferStaging.GetHandle(),
				bufferStaging.GetHandle(),
				moduleBrickWallImage.GetHandle(),
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
			imageMemoryBarrier.image = moduleBrickWallImage.GetHandle();

			imageMemoryBarrier.subresourceRange.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
			imageMemoryBarrier.subresourceRange.baseMipLevel = 0;
			imageMemoryBarrier.subresourceRange.baseArrayLayer = 0;
			imageMemoryBarrier.subresourceRange.layerCount = VK_REMAINING_ARRAY_LAYERS;
			imageMemoryBarrier.subresourceRange.levelCount = VK_REMAINING_MIP_LEVELS;

			vkCmdPipelineBarrier(
				commandBufferStaging.GetHandle(),
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

		vkEndCommandBuffer(commandBufferStaging.GetHandle());

		VkSubmitInfo submitInfo = {};
		submitInfo.pNext = NULL;
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 0;
		submitInfo.pWaitSemaphores = nullptr;
		submitInfo.pWaitDstStageMask = nullptr;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBufferStaging.GetHandle();
		submitInfo.signalSemaphoreCount = 0;
		submitInfo.pSignalSemaphores = NULL;

		vkQueueSubmit(vulkanGraphicsQueue.GetHandle(), 1, &submitInfo, nullptr);
	}


#ifdef DOMIMGUI
	// ImGui
	ImGui_ImplVulkan_InitInfo initParams;

	initParams.CheckVkResultFn = &ImGuiDebugCallback;
	initParams.Instance = vulkanInstance.GetHandle();
	initParams.PhysicalDevice = vulkanPhysicalDevice.GetHandle();
	initParams.Device = vulkanDevice.GetHandle();
	initParams.QueueFamily = vulkanDevice.GetQueueFamilyIndex();
	initParams.Queue = vulkanGraphicsQueue.GetHandle();
	initParams.PipelineCache = VK_NULL_HANDLE;
	initParams.DescriptorPool = imGuiDescriptorPool.GetHandle();
	initParams.Subpass = allSubpasses.GetIndexOfModule(imGuiSubpass);
	initParams.MinImageCount = 2;
	initParams.ImageCount = (u32)vulkanSwapChain.GetNumImages();
	initParams.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	initParams.Allocator = nullptr;

	ImGui_ImplVulkan_Init(&initParams, vulkanRenderPass.GetHandle());

	VkCommandBufferBeginInfo commandBufferBegin = {};
	commandBufferBegin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	vkBeginCommandBuffer(imGuiFontCommandBuffer.GetHandle(), &commandBufferBegin);

	ImGui_ImplVulkan_CreateFontsTexture(imGuiFontCommandBuffer.GetHandle());

	vkEndCommandBuffer(imGuiFontCommandBuffer.GetHandle());

	VkSubmitInfo submitInfo = {};
	submitInfo.pNext = NULL;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 0;
	submitInfo.pWaitSemaphores = nullptr;
	submitInfo.pWaitDstStageMask = nullptr;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &imGuiFontCommandBuffer.GetHandle();
	submitInfo.signalSemaphoreCount = 0;
	submitInfo.pSignalSemaphores = NULL;

	vkQueueSubmit(vulkanGraphicsQueue.GetHandle(), 1, &submitInfo, nullptr); /// FENCE HERE?
#endif //~ #ifdef DOMIMGUI

	/// #TODO: This should use semaphores and that
	vkQueueWaitIdle(vulkanGraphicsQueue.GetHandle());
 }

void dmgf::UnInit()
{
#ifdef DOMIMGUI
	ImGui_ImplVulkan_DestroyFontUploadObjects();
	ImGui_ImplVulkan_Shutdown();
#endif //~ #ifdef DOMIMGUI

	vulkanInstance.TearDown();
}

//~ Region ObjectHandler
#pragma region ObjectHandler

struct ObjectEntry
{
	Mat4f modelMatrix;
	Texture* texture = nullptr;
	Mesh* mesh = nullptr;
};

// Vector of all the objects 
/// #TODO: sort this by meshes and then textures and use proper instanced rendering and that
/// #TEMP: need to delete the objects I s'pose
std::vector<ObjectEntry*> objects;

void* dmgf::AddNewObject(Texture* texture, Mesh* mesh)
{
	ObjectEntry* pNewObject = new ObjectEntry();
	pNewObject->texture = texture;
	pNewObject->mesh = mesh;

	objects.push_back(pNewObject);
	return pNewObject;
}

void dmgf::SetObjectPos(void* pHandle, const Mat4f& modelMatrix)
{
	for (ObjectEntry* pObject : objects)
	{
		if (pObject == pHandle)
		{
			pObject->modelMatrix = modelMatrix;
			return;
		}
	}

	DOMLOG_ERROR("Invalid handle, cannot find object", pHandle)
}

#pragma endregion Handles a Texture+Mesh combo
//~ EndRegion ObjectHandler

void dmgf::Tick(float deltaTime)
{
	camera.Tick(deltaTime);
	mvp.view = camera.GetViewMatrix();

#ifdef DOMIMGUI

	const Vec3f cameraForward = camera.Forward();

	ImGui::SetWindowSize({ 400,400 }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos({ 0, 200 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Graphics");
	ImGui::Separator();
	ImGui::InputFloat3("Camera Pos", (float*)&camera.GetPos());
	ImGui::InputFloat3("Camera Forward", (float*)&cameraForward);
	ImGui::End();
	ImGui_ImplVulkan_NewFrame();

#endif //~ #ifdef DOMIMGUI

	// Sempahore
	VkSemaphore aquiredImageSemaphore;
	VkSemaphoreCreateInfo semaphoreCreateInfo;
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = nullptr;
	semaphoreCreateInfo.flags = 0;
	VulkanUtils::ErrorCheck(vkCreateSemaphore(vulkanDevice.GetHandle(), &semaphoreCreateInfo, nullptr, &aquiredImageSemaphore));

	VulkanUtils::ErrorCheck(vkAcquireNextImageKHR(vulkanDevice.GetHandle(), vulkanSwapChain.GetHandle(), UINT64_MAX, aquiredImageSemaphore, VK_NULL_HANDLE, &currentBuffer));

	VkCommandBufferBeginInfo commandBufferBegin = {};
	commandBufferBegin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	VulkanModuleCommandBuffer& commandBuffer = currentBuffer == 0 ? vulkanCommandBuffer : vulkanCommandBuffer2;
	VulkanUtils::ErrorCheck(vkBeginCommandBuffer(commandBuffer.GetHandle(), &commandBufferBegin));

	VkClearValue clearValue[2];
	clearValue[0] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Colour attachment
	clearValue[1] = { 0.0f, 0 }; // Depth/Stencil attachment

	VkRenderPassBeginInfo renderPassBegin = {};
	renderPassBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBegin.pNext = nullptr;
	renderPassBegin.renderPass = vulkanRenderPass.GetHandle();
	renderPassBegin.framebuffer = (currentBuffer % 2) ? vulkanModuleFrameBuffer1.GetHandle() : vulkanModuleFrameBuffer2.GetHandle();
	renderPassBegin.renderArea.offset.x = 0;
	renderPassBegin.renderArea.offset.y = 0;
	renderPassBegin.renderArea.extent.width = vulkanSwapChain.GetImageWidth();
	renderPassBegin.renderArea.extent.height = vulkanSwapChain.GetImageHeight();
	renderPassBegin.clearValueCount = 2;
	renderPassBegin.pClearValues = clearValue;

	vkCmdBeginRenderPass(commandBuffer.GetHandle(), &renderPassBegin, VK_SUBPASS_CONTENTS_INLINE);

	/// #TEMP: This needs to get all thingies with the same vertex data + texture combo and split them into seperate vectors that can be instanced.

	///// Prepare model matrices
	///for (int i = 0; i < objects.size(); ++i)
	///{
	///	ObjectEntry* pObject = objects[i];
	///	mvp.models[i] = pObject->modelMatrix;
	///}
	///
	///// Copy model matrix data
	///{
	///	void* pData;
	///	vkMapMemory(vulkanDevice.GetHandle(), bufferMemoryMVPMatrix.GetHandle(), 0, sizeof(mvp), 0, &pData);
	///	memcpy(pData, &mvp, sizeof(mvp));
	///	vkUnmapMemory(vulkanDevice.GetHandle(), bufferMemoryMVPMatrix.GetHandle());
	///}

	VkDescriptorPool descriptorPoolHandle = VK_NULL_HANDLE;

	// For each object in our scene
	for (int i = 0; i < objects.size(); ++i)
	{
		ObjectEntry* pObject = objects[i];

		// Change mesh
		{
			void* pData;
			Mesh* mesh = pObject->mesh;
			const u32 numBytes = mesh->GetNumBytes();
			const u32 numVertices = mesh->GetNumVertices();

			vkMapMemory(vulkanDevice.GetHandle(), vulkanModuleVertexMemory.GetHandle(), 0, numVertices, 0, &pData);
			memcpy(pData, mesh->GetRawData(), numBytes);
			vkUnmapMemory(vulkanDevice.GetHandle(), vulkanModuleVertexMemory.GetHandle());

			VkDeviceSize offset = 0;
			vkCmdBindVertexBuffers(commandBuffer.GetHandle(), 0, 1, &vulkanModuleVertexBuffer.GetHandle(), &offset);
		}

		vkCmdBindPipeline(commandBuffer.GetHandle(), VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline.GetHandle());

		// Change descriptor set
		{
			// Descriptor Pool
		
			VkDescriptorPoolSize poolSizes[] =
			{
				{ VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1 },
				{ VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 }
			};
	
			VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
			descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			descriptorPoolCreateInfo.pNext = nullptr;
			descriptorPoolCreateInfo.maxSets = 1;
			descriptorPoolCreateInfo.poolSizeCount = sizeof(poolSizes) / sizeof(*poolSizes);
			descriptorPoolCreateInfo.pPoolSizes = poolSizes;
			VulkanUtils::ErrorCheck(vkCreateDescriptorPool(vulkanDevice.GetHandle(), &descriptorPoolCreateInfo, nullptr, &descriptorPoolHandle), "DescriptorPoolInternal");
		
			// Bindings
			VkDescriptorSetLayoutBinding localBindings[2];
			{
				// MVP
				localBindings[0].binding = 0;
				localBindings[0].descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				localBindings[0].descriptorCount = 1;
				localBindings[0].stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
				localBindings[0].pImmutableSamplers = nullptr;

				// Sampler
				localBindings[1].binding = 1;
				localBindings[1].descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				localBindings[1].descriptorCount = 1;
				localBindings[1].stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
				localBindings[1].pImmutableSamplers = nullptr;
			}

			// Set Layout
			VkDescriptorSetLayout descriptorSetLayoutHandle;
			VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
			descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			descriptorSetLayoutCreateInfo.pNext = nullptr;
			descriptorSetLayoutCreateInfo.flags = 0;
			descriptorSetLayoutCreateInfo.bindingCount = sizeof(localBindings) / sizeof(*localBindings);
			descriptorSetLayoutCreateInfo.pBindings = localBindings;
		
			VulkanUtils::ErrorCheck(vkCreateDescriptorSetLayout(vulkanDevice.GetHandle(), &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayoutHandle), "DescriptorSetInternal");
		
			// Descriptor Set
			VkDescriptorSet descriptorSetHandle;
			VkDescriptorSetAllocateInfo allocateInfo = {};
			allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocateInfo.pNext = nullptr;
			allocateInfo.descriptorPool = descriptorPoolHandle;
			allocateInfo.descriptorSetCount = 1;
			allocateInfo.pSetLayouts = &descriptorSetLayoutHandle;
			VulkanUtils::ErrorCheck(vkAllocateDescriptorSets(vulkanDevice.GetHandle(), &allocateInfo, &descriptorSetHandle), "DescriptorSetInternal");

			// Update Descriptor Sets
			// MVP
			{
				mvp.models[0] = pObject->modelMatrix;
				void* pData;
				vkMapMemory(vulkanDevice.GetHandle(), bufferMemoryMVPMatrix.GetHandle(), 0, sizeof(mvp), 0, &pData);
				memcpy(pData, &mvp, sizeof(mvp));
				vkUnmapMemory(vulkanDevice.GetHandle(), bufferMemoryMVPMatrix.GetHandle());

				VkDescriptorBufferInfo mvpBufferInfo;
				mvpBufferInfo.buffer = bufferMVPMatrix.GetHandle();
				mvpBufferInfo.offset = 0;
				mvpBufferInfo.range = VK_WHOLE_SIZE;

				VkWriteDescriptorSet descriptorSetWriteMVP;
				descriptorSetWriteMVP.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorSetWriteMVP.pNext = nullptr;
				descriptorSetWriteMVP.dstSet = descriptorSetHandle;
				descriptorSetWriteMVP.dstBinding = 0; // MVP
				descriptorSetWriteMVP.dstArrayElement = 0;
				descriptorSetWriteMVP.descriptorCount = 1;
				descriptorSetWriteMVP.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				descriptorSetWriteMVP.pBufferInfo = &mvpBufferInfo;
				descriptorSetWriteMVP.pImageInfo = nullptr;
				descriptorSetWriteMVP.pTexelBufferView = nullptr;

				vkUpdateDescriptorSets(vulkanDevice.GetHandle(), /*descriptorWriteCount =*/1, &descriptorSetWriteMVP, /*descriptorCopyCount =*/0, (VkCopyDescriptorSet*)nullptr);
			}

			// Texture
			{
				VkDescriptorImageInfo imageInfo;
				imageInfo.imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = pObject->texture->imageViewHandle;
				imageInfo.sampler = moduleSampler.GetHandle();

				VkWriteDescriptorSet descriptorSetWriteImage;
				descriptorSetWriteImage.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorSetWriteImage.pNext = nullptr;
				descriptorSetWriteImage.dstSet = descriptorSetHandle;
				descriptorSetWriteImage.dstBinding = 1; // Sampler
				descriptorSetWriteImage.dstArrayElement = 0;
				descriptorSetWriteImage.descriptorCount = 1;
				descriptorSetWriteImage.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				descriptorSetWriteImage.pBufferInfo = nullptr;
				descriptorSetWriteImage.pImageInfo = &imageInfo;
				descriptorSetWriteImage.pTexelBufferView = nullptr;

				vkUpdateDescriptorSets(vulkanDevice.GetHandle(), /*descriptorWriteCount =*/1, &descriptorSetWriteImage, /*descriptorCopyCount =*/0, (VkCopyDescriptorSet*)nullptr);
			}

			DOMLOG_INFO("Binding:", pObject->texture, pObject->mesh);

			vkCmdBindDescriptorSets(commandBuffer.GetHandle(), VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanModulePipelineLayout.GetHandle(), 0, 1, &descriptorSetHandle, 0, nullptr);
		}

		vkCmdDraw(commandBuffer.GetHandle(), (u32)(rawVertexData.size() / 8), 1, 0, 0);
	}

#ifdef DOMIMGUI
	vkCmdNextSubpass(commandBuffer.GetHandle(), VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

	ImGui::Render();
	ImDrawData* pDrawData = ImGui::GetDrawData();
	if (pDrawData)
	{
		pDrawData->DisplaySize.x = (float)dmwi::getWindowWidth();
		pDrawData->DisplaySize.y = (float)dmwi::getWindowHeight();
		ImGui_ImplVulkan_RenderDrawData(pDrawData, commandBuffer.GetHandle(), VK_NULL_HANDLE);
	}
#endif //~ #ifdef DOMIMGUI

	vkCmdEndRenderPass(commandBuffer.GetHandle());
	vkEndCommandBuffer(commandBuffer.GetHandle());

	VkFence drawFence;
	VkFenceCreateInfo fenceInfo;
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.pNext = nullptr;
	fenceInfo.flags = 0;
	VulkanUtils::ErrorCheck(vkCreateFence(vulkanDevice.GetHandle(), &fenceInfo, nullptr, &drawFence));

	VkPipelineStageFlags pipeStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo submitInfo[1] = {};
	submitInfo[0].pNext = NULL;
	submitInfo[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo[0].waitSemaphoreCount = 1;
	submitInfo[0].pWaitSemaphores = &aquiredImageSemaphore;
	submitInfo[0].pWaitDstStageMask = &pipeStageFlags;
	submitInfo[0].commandBufferCount = 1;
	submitInfo[0].pCommandBuffers = &commandBuffer.GetHandle();
	submitInfo[0].signalSemaphoreCount = 0;
	submitInfo[0].pSignalSemaphores = NULL;

	// Submit queue
	VulkanUtils::ErrorCheck(vkQueueSubmit(vulkanGraphicsQueue.GetHandle(), 1, submitInfo, drawFence));

	// Wait for queue to finish rendering on to the ***BACK Buffer***
	vkWaitForFences(vulkanDevice.GetHandle(), 1, &drawFence, VK_TRUE, UINT64_MAX);

	vkDestroyFence(vulkanDevice.GetHandle(), drawFence, nullptr);

	vkDestroyDescriptorPool(vulkanDevice.GetHandle(), descriptorPoolHandle, nullptr);

	// Swap back buffer and pressent buffer
	VkPresentInfoKHR present;
	present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present.pNext = nullptr;
	present.swapchainCount = 1;
	present.pSwapchains = &vulkanSwapChain.GetHandle();
	present.pImageIndices = &currentBuffer;
	present.pWaitSemaphores = nullptr;
	present.waitSemaphoreCount = 0;
	present.pResults = NULL;

	VulkanUtils::ErrorCheck(vkQueuePresentKHR(vulkanGraphicsQueue.GetHandle(), &present));

	vkDestroySemaphore(vulkanDevice.GetHandle(), aquiredImageSemaphore, nullptr);
}

Camera& dmgf::GetCamera()
{
	return camera;
}

VkDevice& dmgf::GetDeviceHandle()
{
	return vulkanDevice.GetHandle();
}

VkPhysicalDevice& dmgf::GetPhysicalDeviceHandle()
{
	return vulkanPhysicalDevice.GetHandle();
}

VkDeviceMemory& dmgf::GetStagingBufferMemoryHandle()
{
	return bufferMemoryStaging.GetHandle();
}

VkBuffer& dmgf::GetStagingBufferHandle()
{
	return bufferStaging.GetHandle();
}

VkCommandPool& dmgf::GetCommandPool()
{
	return vulkanCommandPool.GetHandle();
}

VkQueue& dmgf::GetGraphicsQueueHandle()
{
	return vulkanGraphicsQueue.GetHandle();
}

const u32& dmgf::GetGraphicsQueueFamily()
{
	return vulkanDevice.GetQueueFamilyIndex();
}

dmut::HeapAlloc<char> GetShaderFilepath(const char* shaderName)
{
	const int BUFFER_SIZE = 256;
	dmut::HeapAlloc<char> shaderFullPath(BUFFER_SIZE);
	sprintf_s(shaderFullPath.RawPtr(), BUFFER_SIZE, "%s/%s.spv\0", destPath, shaderName);
	return shaderFullPath;
}

#endif