#include "pch.h"

#include "Renderer.h"

#include "../Graphics/Camera.h"
#include "../Graphics/VulkanUtils.h"
#include "DomImport/DomImport.h"
#include "DomMath/Mat4.h"
#include "DomMath/Math.h"
#include "DomWindow/DomWindow.h"
#include "RendererObject.h"
#include "VectorArt.h"

#include <vulkan/vulkan_win32.h>

#ifdef DOMIMGUI
#include "imgui_impl_vulkan.h"
#endif //~ #ifdef DOMIMGUI

namespace
{
	// Struct describing a single vertex
	struct Vertex
	{
		float pos[3];
		float uv[2];
	};

	// Struct descriving a single rendered object
	struct RenderedObjectEntry
	{
		std::string fileName;
		std::shared_ptr<VectorArt> pVectorArt;
		VkDescriptorSet handleDescriptorSet = VK_NULL_HANDLE; // Descriptor set to bind
		VkBuffer handleBuffer = VK_NULL_HANDLE; // Buffer big enough to hold serilized vector art
		VkDeviceMemory handleDeviceMemory = VK_NULL_HANDLE; // Memory to hold buffer
		std::vector<std::unique_ptr<RendererObject>> rendererObjects; // #TODO: This memory will be fragmented to FUCK and accessed every frame, we need an allocator later on
	};

	// #TODO: These needs to be grabbed from the surface
	const int EXTENT_WIDTH = 1776; // Width of renderable portion of screen
	const int EXTENT_HEIGHT = 969; // Height of renderable portion of screen

	const int VERTEX_BUFFER_SIZE = 6; // Num vertices in our vertex buffer

	// #TODO: Functions and variables that are together should probably be moved to a class. i.e VulkanDevice will contain QueueFamily + NumQueues + GetNextFreeQueue()
	void PrintInstanceExtensions();
	void PrintInstanceLayers();
	VkInstance CreateInstance();

	void PrintDeviceMemoryInfo(VkPhysicalDevice inHandlePhysicalDevice);
	uint32_t FindMemoryIndex(VkMemoryPropertyFlags memoryFlags, uint32_t memoryTypeBits = (uint32_t)-1);
	VkPhysicalDevice PickPhysicalDevice(const dmut::HeapAllocSize<VkPhysicalDevice>& physicalDevices);
	VkPhysicalDevice CreatePhysicalDevice();

	VkSurfaceKHR CreateSurface();

	void PrintDeviceLayers();
	void PrintDeviceExtensions();
	uint32_t FindDeviceQueueFamilyIndex(uint32_t requestedFlags, uint32_t numQueues);
	VkDevice CreateDevice();

	VkQueue CreateQueue(uint32_t queueFamily, uint32_t queueIndex);

	VkShaderModule CreateShader(const char* pShaderName);

	VkSwapchainKHR CreateSwapchain();

	VkBuffer CreateVertexBuffer();
	VkBuffer CreateUniformBuffer(size_t size);

	VkRenderPass CreateRenderPass();

	VkDescriptorPool CreateDescriptorPool();

	VkDescriptorSetLayout CreateDescriptorSetLayout(VkShaderStageFlagBits shaderStage, uint32_t bindingNumber);

	VkDescriptorSet CreateDescriptorSet(VkDescriptorSetLayout layout);

	void UpdateVectorBuffer(RenderedObjectEntry& entry); // Update the buffer with the vector art data
	void UpdateViewBuffer(float deltaTime); // Update the view buffer with the view matrix 
	void UpdateProjectionBuffer(); // Update the projection buffer with the projection matrix
	void UpdateViewBufferHUD(); // Update the HUD view buffer with the view matrix for HUD objects
	void UpdateProjectionBufferHUD(); // Update the HUD projection buffer with the projection matrix for HUD objects

	VkPipelineLayout CreatePipelineLayout();

	VkPipeline CreatePipeline();

	VkCommandPool CreateCommandPool();

	VkCommandBuffer CreateCommandBuffer();

	// memoryTypeBits are optional -- It is a bitfield that says which memory index we can use i.e 4th bit being 1 means we can use memory 4
	VkDeviceMemory CreateDeviceMemory(VkDeviceSize memorySize, VkMemoryPropertyFlags memoryFlags, uint32_t memoryTypeBits = (uint32_t)-1);
	VkDeviceMemory CreateDeviceMemoryFromImage(VkImage image);
	void BindImageMemory(VkImage image, VkDeviceMemory memory);

	VkImage CreateImage(VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlagBits usage);

	VkImageView CreateImageView(VkImage inHandleImage, VkImageAspectFlags aspectMask, VkFormat format);

	VkFramebuffer CreateFramebuffer(dmut::HeapAllocSize<VkImageView> inHandleImageViews);

	void SubmitDrawCommand();

	// Clean up entry memory from vulkan
	void CleanUpEntry(RenderedObjectEntry& entry);

	VkInstance handleInstance = VK_NULL_HANDLE;
	VkPhysicalDevice handlePhysicalDevice = VK_NULL_HANDLE;
	VkSurfaceKHR handleSurface = VK_NULL_HANDLE;
	VkDevice handleDevice = VK_NULL_HANDLE;
	VkQueue handleGraphicsQueue = VK_NULL_HANDLE;
	VkSwapchainKHR handleSwapChain = VK_NULL_HANDLE;
	VkShaderModule handleShaderVertex = VK_NULL_HANDLE;
	VkShaderModule handleShaderFragment = VK_NULL_HANDLE;
	VkBuffer handleVertextBuffer = VK_NULL_HANDLE;
	VkRenderPass handleRenderPass = VK_NULL_HANDLE;
	VkDescriptorPool handleDescriptorPool = VK_NULL_HANDLE;
	VkDescriptorSetLayout handleDescriptorSetLayoutProjection = VK_NULL_HANDLE; // Updated once at start
	VkDescriptorSetLayout handleDescriptorSetLayoutView = VK_NULL_HANDLE; // Updated once per frame 
	VkDescriptorSetLayout handleDescriptorSetLayoutVector = VK_NULL_HANDLE; // Updated once per vector art
	
	VkDescriptorSet handleDescriptorSetProjection = VK_NULL_HANDLE; // Projection used for in-game objects
	VkDeviceMemory handleDeviceMemoryProjectionBuffer = VK_NULL_HANDLE;
	VkBuffer handleBufferProjection = VK_NULL_HANDLE;
	
	VkDescriptorSet handleDescriptorSetView = VK_NULL_HANDLE; // View used for in-game objects
	VkDeviceMemory handleDeviceMemoryViewBuffer = VK_NULL_HANDLE;
	VkBuffer handleBufferView = VK_NULL_HANDLE;
	
	VkDescriptorSet handleDescriptorSetProjectionHUD = VK_NULL_HANDLE; // Projection used for HUD objects
	VkDeviceMemory handleDeviceMemoryProjectionBufferHUD = VK_NULL_HANDLE;
	VkBuffer handleBufferProjectionHUD = VK_NULL_HANDLE;

	VkDescriptorSet handleDescriptorSetViewHUD = VK_NULL_HANDLE; // View used for HUD objects
	VkDeviceMemory handleDeviceMemoryViewBufferHUD = VK_NULL_HANDLE;
	VkBuffer handleBufferViewHUD = VK_NULL_HANDLE;
	
	VkPipelineLayout handlePipelineLayout = VK_NULL_HANDLE;
	VkPipeline handlePipeline = VK_NULL_HANDLE;
	VkCommandPool handleCommandPool = VK_NULL_HANDLE;
	VkImageView handleImageView1 = VK_NULL_HANDLE;
	VkImageView handleImageView2 = VK_NULL_HANDLE;
	VkFramebuffer handleFrameBuffer1 = VK_NULL_HANDLE;
	VkFramebuffer handleFrameBuffer2 = VK_NULL_HANDLE;
	VkCommandBuffer handleCommandBuffer1 = VK_NULL_HANDLE;
	VkCommandBuffer handleCommandBuffer2 = VK_NULL_HANDLE;
	VkDeviceMemory handleDeviceMemoryVertex = VK_NULL_HANDLE;
	dmut::HeapAllocSize<VkImage> handleSwapChainImages;
	VkDeviceMemory handleDepthMemory = VK_NULL_HANDLE;
	VkBuffer handleDepthBuffer = VK_NULL_HANDLE;
	VkImage handleDepthImage = VK_NULL_HANDLE;
	VkImageView handleDepthImageView = VK_NULL_HANDLE;

	// Unused
	VkBuffer handleStagingBuffer = VK_NULL_HANDLE;
	VkDeviceMemory handleDeviceMemoryVertexBuffer = VK_NULL_HANDLE;

	uint32_t deviceQueueFamilyIndex = 0;
	const char* pShaderPath = "../Graphics/Shaders/CompiledShaders"; // compiled SPIRV files

	// List of all rendered objects in the game, one per ERenderObjectType
	std::vector<RenderedObjectEntry> renderedObjectsInGame;	// In game objects
	std::vector<RenderedObjectEntry> renderedObjectsHUD;	// HUD Objects

	Mat4f viewMatrix = Mat4f::Identity();
}

namespace dmgf
{
	template<typename TPredicate>
	void ForEachRenderedObjectList(TPredicate Predicate)
	{
		Predicate(renderedObjectsInGame);
		Predicate(renderedObjectsHUD);
	}

	std::vector<RenderedObjectEntry>& GetRenderedObjectsList(ERenderObjectType renderObjectType)
	{
		switch (renderObjectType)
		{
		case ERenderObjectType::InGame:
			return renderedObjectsInGame;
		case ERenderObjectType::HUD:
			return renderedObjectsHUD;
		default:
			DOMLOG_ERROR("You fucked it, add a new vector");
			return renderedObjectsInGame;
		}
	}

	void Init()
	{
#ifndef DOMRELEASE
		const char* pCompilerPath = "C:/VulkanSDK/1.2.198.1/Bin/glslc.exe"; // Compiler
		const char* pSourcePath = "../Graphics/Shaders"; // Source text files
		VulkanUtils::CompileShaders(pCompilerPath, pSourcePath, pShaderPath);
#endif //~ #ifndef DOMRELEASE

		handleInstance = CreateInstance();
		handlePhysicalDevice = CreatePhysicalDevice();
		handleSurface = CreateSurface();
		handleDevice = CreateDevice();
		handleGraphicsQueue = CreateQueue(deviceQueueFamilyIndex, 0);
		handleSwapChain = CreateSwapchain();
		handleShaderVertex = CreateShader("VectorVert.spv");
		handleShaderFragment = CreateShader("VectorFrag.spv");
		handleVertextBuffer = CreateVertexBuffer();
		handleRenderPass = CreateRenderPass();
		DOMASSERT(handleSwapChainImages.GetSize() == 2, "We're fucked if this isn't 2, I can't be arsed to program a for loop");
		
		handleDepthImage = CreateImage(VkFormat::VK_FORMAT_D24_UNORM_S8_UINT, EXTENT_WIDTH, EXTENT_HEIGHT, VkImageUsageFlagBits::VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
		handleDepthMemory = CreateDeviceMemoryFromImage(handleDepthImage);
		BindImageMemory(handleDepthImage, handleDepthMemory);
		handleDepthImageView = CreateImageView(handleDepthImage, VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT | VkImageAspectFlagBits::VK_IMAGE_ASPECT_STENCIL_BIT, VkFormat::VK_FORMAT_D24_UNORM_S8_UINT);
		

		handleImageView1 = CreateImageView(handleSwapChainImages[0], VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, VkFormat::VK_FORMAT_B8G8R8A8_UNORM);
		handleImageView2 = CreateImageView(handleSwapChainImages[1], VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT, VkFormat::VK_FORMAT_B8G8R8A8_UNORM);
		
		dmut::HeapAllocSize<VkImageView> frameBufferOneParams(2);
		frameBufferOneParams[0] = handleImageView1;
		frameBufferOneParams[1] = handleDepthImageView;
		handleFrameBuffer1 = CreateFramebuffer(std::move(frameBufferOneParams));
		
		dmut::HeapAllocSize<VkImageView> frameBufferTwoParams(2);
		frameBufferTwoParams[0] = handleImageView2;
		frameBufferTwoParams[1] = handleDepthImageView;
		handleFrameBuffer2 = CreateFramebuffer(std::move(frameBufferTwoParams));

		handleDescriptorPool = CreateDescriptorPool();
		handleDescriptorSetLayoutProjection = CreateDescriptorSetLayout(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, 0); 
		handleDescriptorSetLayoutView = CreateDescriptorSetLayout(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, 1);       
		handleDescriptorSetLayoutVector = CreateDescriptorSetLayout(VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, 0); 
		handleDescriptorSetView = CreateDescriptorSet(handleDescriptorSetLayoutView);
		handleDescriptorSetProjection = CreateDescriptorSet(handleDescriptorSetLayoutProjection);
		handleDescriptorSetViewHUD = CreateDescriptorSet(handleDescriptorSetLayoutView);
		handleDescriptorSetProjectionHUD = CreateDescriptorSet(handleDescriptorSetLayoutProjection);
		handleDescriptorSetViewHUD = CreateDescriptorSet(handleDescriptorSetLayoutView);
		handleDescriptorSetProjectionHUD = CreateDescriptorSet(handleDescriptorSetLayoutProjection);
		handlePipelineLayout = CreatePipelineLayout();
		handlePipeline = CreatePipeline();
		handleCommandPool = CreateCommandPool();
		handleCommandBuffer1 = CreateCommandBuffer();
		handleCommandBuffer2 = CreateCommandBuffer();
		handleDeviceMemoryVertexBuffer = CreateDeviceMemory(0x100, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT); // #TODO: Query memory requirements with vkGetBufferMemoryRequirements() instead of hard coding 0x100
		VulkanUtils::ErrorCheck(vkBindBufferMemory(handleDevice, handleVertextBuffer, handleDeviceMemoryVertexBuffer, 0), "BindVertexBuffer");

		// View matrix buffer (in-game)
		const size_t viewBufferSize = sizeof(Mat4f);
		handleDeviceMemoryViewBuffer = CreateDeviceMemory(0x100, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		handleBufferView = CreateUniformBuffer(viewBufferSize);
		VulkanUtils::ErrorCheck(vkBindBufferMemory(handleDevice, handleBufferView, handleDeviceMemoryViewBuffer, 0), "BindUniformBuffer");

		// Projection matrix buffer (in-game)
		const size_t projectionBufferSize = sizeof(Mat4f);
		handleDeviceMemoryProjectionBuffer = CreateDeviceMemory(0x100, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		handleBufferProjection = CreateUniformBuffer(projectionBufferSize);
		VulkanUtils::ErrorCheck(vkBindBufferMemory(handleDevice, handleBufferProjection, handleDeviceMemoryProjectionBuffer, 0), "BindUniformBuffer");
		UpdateProjectionBuffer();

		// View matrix buffer (HUD)
		const size_t viewBufferSizeHUD = sizeof(Mat4f);
		handleDeviceMemoryViewBufferHUD = CreateDeviceMemory(0x100, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		handleBufferViewHUD = CreateUniformBuffer(viewBufferSizeHUD);
		VulkanUtils::ErrorCheck(vkBindBufferMemory(handleDevice, handleBufferViewHUD, handleDeviceMemoryViewBufferHUD, 0), "BindUniformBuffer");
		UpdateViewBufferHUD();
		
		// Projection matrix buffer (HUD)
		const size_t projectionBufferSizeHUD = sizeof(Mat4f);
		handleDeviceMemoryProjectionBufferHUD = CreateDeviceMemory(0x100, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		handleBufferProjectionHUD = CreateUniformBuffer(projectionBufferSizeHUD);
		VulkanUtils::ErrorCheck(vkBindBufferMemory(handleDevice, handleBufferProjectionHUD, handleDeviceMemoryProjectionBufferHUD, 0), "BindUniformBuffer");
		UpdateProjectionBufferHUD();
		

#ifdef DOMIMGUI
		
		// Initialise
		{
			ImGui_ImplVulkan_InitInfo imGuiInitParams = {};
			imGuiInitParams.Instance = handleInstance;
			imGuiInitParams.PhysicalDevice = handlePhysicalDevice;
			imGuiInitParams.Device = handleDevice;
			imGuiInitParams.QueueFamily = deviceQueueFamilyIndex;
			imGuiInitParams.Queue = handleGraphicsQueue;
			imGuiInitParams.DescriptorPool = handleDescriptorPool;
			imGuiInitParams.Subpass = 1;
			imGuiInitParams.MinImageCount = 2;
			imGuiInitParams.ImageCount = 2;
			imGuiInitParams.MSAASamples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;

			ImGui_ImplVulkan_Init(&imGuiInitParams, handleRenderPass);
		}
		
		// Load font texture into memory with command buffer
		{
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VkCommandBufferUsageFlagBits::VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			vkBeginCommandBuffer(handleCommandBuffer1, &beginInfo);
			ImGui_ImplVulkan_CreateFontsTexture(handleCommandBuffer1);
			vkEndCommandBuffer(handleCommandBuffer1);

			VkSubmitInfo submitInfo = {};
			submitInfo.pNext = nullptr;
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &handleCommandBuffer1;
			submitInfo.waitSemaphoreCount = 0;
			submitInfo.pWaitSemaphores = nullptr;
			submitInfo.pWaitDstStageMask = nullptr;
			submitInfo.signalSemaphoreCount = 0;
			submitInfo.pSignalSemaphores = nullptr;

			VkFence handleFence;
			VkFenceCreateInfo fenceInfo;
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.pNext = nullptr;
			fenceInfo.flags = 0;
			VulkanUtils::ErrorCheck(vkCreateFence(handleDevice, &fenceInfo, nullptr, &handleFence));
			VulkanUtils::ErrorCheck(vkQueueSubmit(handleGraphicsQueue, 1, &submitInfo, handleFence));
			vkWaitForFences(handleDevice, 1, &handleFence, true, UINT64_MAX);

			ImGui_ImplVulkan_DestroyFontUploadObjects();
		}
#endif //~ #ifdef DOMIMGUI
	}

	void UnInit()
	{
#ifdef DOMIMGUI
		ImGui_ImplVulkan_Shutdown();
#endif //~ #ifdef DOMIMGUI

		auto CleanUpRenderedObjects = [](std::vector<RenderedObjectEntry>& renderedObjects)
		{
			for (RenderedObjectEntry& entry : renderedObjects)
			{
				CleanUpEntry(entry);
			}
			renderedObjects.clear();
		};
		ForEachRenderedObjectList(CleanUpRenderedObjects);
	
		vkDestroyBuffer(handleDevice, handleDepthBuffer, nullptr);
		vkFreeMemory(handleDevice, handleDepthMemory, nullptr);
		vkDestroyImageView(handleDevice, handleDepthImageView, nullptr);
		vkDestroyImage(handleDevice, handleDepthImage, nullptr);
		vkDestroyBuffer(handleDevice, handleBufferProjection, nullptr);
		vkDestroyBuffer(handleDevice, handleBufferView, nullptr);
		vkDestroyBuffer(handleDevice, handleBufferProjectionHUD, nullptr);
		vkDestroyBuffer(handleDevice, handleBufferViewHUD, nullptr);
		vkFreeMemory(handleDevice, handleDeviceMemoryProjectionBuffer, nullptr);
		vkFreeMemory(handleDevice, handleDeviceMemoryViewBuffer, nullptr);
		vkFreeMemory(handleDevice, handleDeviceMemoryProjectionBufferHUD, nullptr);
		vkFreeMemory(handleDevice, handleDeviceMemoryViewBufferHUD, nullptr);
		vkFreeMemory(handleDevice, handleDeviceMemoryVertexBuffer, nullptr);
		vkDestroyCommandPool(handleDevice, handleCommandPool, nullptr);
		vkDestroyPipeline(handleDevice, handlePipeline, nullptr);
		vkDestroyPipelineLayout(handleDevice, handlePipelineLayout, nullptr);
		vkDestroyDescriptorSetLayout(handleDevice, handleDescriptorSetLayoutVector, nullptr);
		vkDestroyDescriptorSetLayout(handleDevice, handleDescriptorSetLayoutView, nullptr);
		vkDestroyDescriptorSetLayout(handleDevice, handleDescriptorSetLayoutProjection, nullptr);
		vkDestroyDescriptorPool(handleDevice, handleDescriptorPool, nullptr);
		vkDestroyFramebuffer(handleDevice, handleFrameBuffer2, nullptr);
		vkDestroyFramebuffer(handleDevice, handleFrameBuffer1, nullptr);
		vkDestroyImageView(handleDevice, handleImageView2, nullptr);
		vkDestroyImageView(handleDevice, handleImageView1, nullptr);
		vkDestroyRenderPass(handleDevice, handleRenderPass, nullptr);
		vkDestroyBuffer(handleDevice, handleVertextBuffer, nullptr);
		vkDestroyShaderModule(handleDevice, handleShaderVertex, nullptr);
		vkDestroyShaderModule(handleDevice, handleShaderFragment, nullptr);
		vkDestroySwapchainKHR(handleDevice, handleSwapChain, nullptr);
		vkDestroyDevice(handleDevice, nullptr); // #JANK: When destroying the device we get a warning about a VKFence still being in use.
		vkDestroySurfaceKHR(handleInstance, handleSurface, nullptr);
		vkDestroyInstance(handleInstance, nullptr); 
	}

	void Tick(float deltaTime)
	{
		UpdateViewBuffer(deltaTime);
		SubmitDrawCommand();
#ifdef DOMIMGUI
		ImGui_ImplVulkan_NewFrame();
#endif //~ #ifdef DOMIMGUI
	}

	TransientPtr<RendererObject> AddObjectFromSVG(const char* pFileName, ERenderObjectType renderObjectType /*= ERenderObjectType::InGame*/)
	{
		std::vector<RenderedObjectEntry>& renderedObjects = GetRenderedObjectsList(renderObjectType);

		auto result = std::find_if(renderedObjects.begin(), renderedObjects.end(), [pFileName](RenderedObjectEntry& entry)
			{
				return entry.fileName == pFileName;
			});

		RenderedObjectEntry* pEntry = nullptr;

		if (result == renderedObjects.end())
		{
			renderedObjects.resize(renderedObjects.size() + 1);

			pEntry = &renderedObjects.back();
			pEntry->fileName = pFileName;
			pEntry->pVectorArt = std::make_shared<VectorArt>(pFileName);
			
			// Set up vulkan handles (descriptor set + memory)
			const size_t vectorBufferSize = sizeof(int) * 4 * 4096; // #TODO: Figure out how large the serialized vector art is
			pEntry->handleDescriptorSet = CreateDescriptorSet(handleDescriptorSetLayoutVector);
			pEntry->handleDeviceMemory = CreateDeviceMemory(vectorBufferSize, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			pEntry->handleBuffer = CreateUniformBuffer(vectorBufferSize);
			VulkanUtils::ErrorCheck(vkBindBufferMemory(handleDevice, pEntry->handleBuffer, pEntry->handleDeviceMemory, 0), "BindUniformBuffer");
		}
		else
		{
			pEntry = &(*result);
		}

		pEntry->rendererObjects.emplace_back(std::make_unique<RendererObject>());
		return pEntry->rendererObjects.back().get();
	}


	TransientPtr<RendererObject> AddObjectFromVectorArt(const std::shared_ptr<VectorArt>& vectorArt, ERenderObjectType renderObjectType /*= ERenderObjectType::InGame*/)
	{
		std::vector<RenderedObjectEntry>& renderedObjects = GetRenderedObjectsList(renderObjectType);

		renderedObjects.resize(renderedObjects.size() + 1);
		RenderedObjectEntry& pEntry = renderedObjects.back();
		
		pEntry.pVectorArt = vectorArt;
	
		const size_t vectorBufferSize = sizeof(int) * 4 * 4096; // #TODO: Figure out how large the serialized vector art is
		pEntry.handleDescriptorSet = CreateDescriptorSet(handleDescriptorSetLayoutVector);
		pEntry.handleDeviceMemory = CreateDeviceMemory(vectorBufferSize, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		pEntry.handleBuffer = CreateUniformBuffer(vectorBufferSize);
		VulkanUtils::ErrorCheck(vkBindBufferMemory(handleDevice, pEntry.handleBuffer, pEntry.handleDeviceMemory, 0), "BindUniformBuffer");
		
		pEntry.rendererObjects.emplace_back(std::make_unique<RendererObject>());
		return pEntry.rendererObjects.back().get();
	}

	void RemoveObject(TransientPtr<RendererObject> pRendererObject)
	{
		auto RemoveObjectFromList = [pRendererObject](std::vector<RenderedObjectEntry>& renderedObjects)
		{
			for (auto renderObjectEntryIterator = renderedObjects.begin(); renderObjectEntryIterator != renderedObjects.end(); ++renderObjectEntryIterator)
			{
				for (auto renderedObjectIterator = renderObjectEntryIterator->rendererObjects.begin(); renderedObjectIterator != renderObjectEntryIterator->rendererObjects.end(); ++renderedObjectIterator)
				{
					if ((*renderedObjectIterator).get() == pRendererObject.Get())
					{
						renderObjectEntryIterator->rendererObjects.erase(renderedObjectIterator);

						if (renderObjectEntryIterator->rendererObjects.size() == 0)
						{
							// If that was the last instance of this render entry, we remove the whole entry
							CleanUpEntry(*renderObjectEntryIterator);
							renderedObjects.erase(renderObjectEntryIterator);
						}

						return;
					}
				}
			}
		};

		ForEachRenderedObjectList(RemoveObjectFromList);
	}

	struct CameraData
	{
		float x = 0.0f, y = 0.0f;
		float zoom = 1.0f;
	} cameraData;

	void RefreshViewMatrix()
	{
		const float inverseZoom = 1.0f / cameraData.zoom;
		const float cameraX = EXTENT_WIDTH * inverseZoom * 0.5f - cameraData.x;
		const float cameraY = EXTENT_HEIGHT * inverseZoom * 0.5f - cameraData.y;
		viewMatrix = dmma::generateTranslation({ cameraX, cameraY, 0.0f}) * dmma::generateScale({cameraData.zoom, cameraData.zoom, 1.0f});
		viewMatrix.transpose();
	}

	void SetCameraCenter(float x, float y)
	{
		cameraData.x = x;
		cameraData.y = y;

		RefreshViewMatrix();
	}

	void SetCameraZoom(float zoom)
	{
		cameraData.zoom = zoom;

		RefreshViewMatrix();
	}

	float GetScreenWidth()
	{
		return EXTENT_WIDTH;
	}

	float GetScreenHeight()
	{
		return EXTENT_HEIGHT;
	}

}

namespace
{
	void PrintInstanceExtensions()
	{
		uint32_t numExtensions = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &numExtensions, nullptr);
		dmut::HeapAllocSize<VkExtensionProperties> allExtensions(numExtensions);
		vkEnumerateInstanceExtensionProperties(nullptr, &numExtensions, allExtensions.RawPtr());

		for (VkExtensionProperties& extension : allExtensions)
		{
			DOMLOG_PRINT("Instance Extension:", extension.extensionName);
		}
	}

	void PrintInstanceLayers()
	{
		uint32_t numLayers = 0;
		vkEnumerateInstanceLayerProperties(&numLayers, nullptr);
		dmut::HeapAllocSize<VkLayerProperties> allLayers(numLayers);
		vkEnumerateInstanceLayerProperties(&numLayers, allLayers.RawPtr());

		for (VkLayerProperties& layer : allLayers)
		{
			DOMLOG_PRINT("Instance Layers:", layer.layerName, "-", layer.description);
		}
	}

	VkInstance CreateInstance()
	{
		///PrintInstanceLayers();
		///PrintInstanceExtensions();

#ifndef DOMRELEASE
		const char* extensionNames[] = { "VK_KHR_surface", "VK_KHR_win32_surface", VK_EXT_DEBUG_UTILS_EXTENSION_NAME }; // Extensions are integral to the vulkan API and are quick. Things like swap chain or WIN32_SURFACE are integral. *** These can only be created by lunarG ***
		const char* layerNames[] = { /*"VK_LAYER_NV_nsight",*/ "VK_LAYER_KHRONOS_validation" }; // Layers are slower, and built on top of vulkan. They intercept all function calls and do pre-processing on the inputs for validation (RenderDoc uses these?). *** These can be created by anyone ***
#else //~ #ifndef DOMRELEASE
		const char* extensionNames[] = { "VK_KHR_surface", "VK_KHR_win32_surface" };
		const char* layerNames[] = { };
#endif //~ #ifndef DOMRELEASE

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = DMUT_ARRAY_SIZE(extensionNames);
		createInfo.ppEnabledExtensionNames = extensionNames;
		createInfo.enabledLayerCount = DMUT_ARRAY_SIZE(layerNames);
		createInfo.ppEnabledLayerNames = layerNames;

		VkInstance returnedHandle = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkCreateInstance(&createInfo, nullptr, &returnedHandle));
		return returnedHandle;
	}

	void PrintDeviceMemoryInfo(VkPhysicalDevice inHandlePhysicalDevice)
	{
		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(inHandlePhysicalDevice, &memoryProperties);

		// See VkMemoryPropertyFlags for what the flags mean
		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
		{
			DOMLOG_PRINT("Memory Type", i, "flags:", memoryProperties.memoryTypes[i].propertyFlags);
		}
	}

	uint32_t FindMemoryIndex(VkMemoryPropertyFlags memoryFlags, uint32_t memoryTypeBits /*= (uint32_t)-1*/)
	{
		VkPhysicalDeviceMemoryProperties memoryProperties;
		vkGetPhysicalDeviceMemoryProperties(handlePhysicalDevice, &memoryProperties);

		// See VkMemoryPropertyFlags for what the flags mean
		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
		{
			if (!((1 << i) & memoryTypeBits))
			{
				// Not valid memory index
				continue;
			}
			if ((memoryProperties.memoryTypes[i].propertyFlags & memoryFlags) == memoryFlags)
			{
				return i;
			}
		}

		DOMLOG_WARN("Unable to find memory with flags", memoryFlags);
		return VK_MAX_MEMORY_TYPES;
	}

	VkPhysicalDevice PickPhysicalDevice(const dmut::HeapAllocSize<VkPhysicalDevice>& physicalDevices)
	{
		DOMASSERT(physicalDevices.GetSize() > 0, "This isn't a text adventure game, you need a graphics card");

		// #TODO: Actually pick the best one
		return physicalDevices[0];
	}

	VkPhysicalDevice CreatePhysicalDevice()
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(handleInstance, &deviceCount, nullptr);
		dmut::HeapAllocSize<VkPhysicalDevice> physicalDevices = dmut::HeapAllocSize<VkPhysicalDevice>((size_t)deviceCount);
		vkEnumeratePhysicalDevices(handleInstance, &deviceCount, physicalDevices.RawPtr());
		VkPhysicalDevice returnedPhysicalDevice = PickPhysicalDevice(physicalDevices);

		///PrintDeviceMemoryInfo(returnedPhysicalDevice);
		return returnedPhysicalDevice;
	}

	VkSurfaceKHR CreateSurface()
	{
		VkWin32SurfaceCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd = dmwi::getWindowHandle();
		createInfo.hinstance = dmwi::getWindowInstance();

		VkSurfaceKHR returnedSurface = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkCreateWin32SurfaceKHR(handleInstance, &createInfo, nullptr, &returnedSurface), "Surface");
		return returnedSurface;
	}

	void PrintDeviceLayers()
	{
		uint32_t numLayers = 0;
		vkEnumerateDeviceLayerProperties(handlePhysicalDevice, &numLayers, nullptr);
		dmut::HeapAllocSize<VkLayerProperties> allLayers(numLayers);
		vkEnumerateDeviceLayerProperties(handlePhysicalDevice, &numLayers, allLayers.RawPtr());

		for (VkLayerProperties& layer : allLayers)
		{
			DOMLOG_PRINT("Device Layers:", layer.layerName, "-", layer.description);
		}
	}

	void PrintDeviceExtensions()
	{
		uint32_t numExtensions = 0;
		vkEnumerateDeviceExtensionProperties(handlePhysicalDevice, nullptr, &numExtensions, nullptr);
		dmut::HeapAllocSize<VkExtensionProperties> allExtensions(numExtensions);
		vkEnumerateDeviceExtensionProperties(handlePhysicalDevice, nullptr, &numExtensions, allExtensions.RawPtr());

		for (VkExtensionProperties& extension : allExtensions)
		{
			DOMLOG_PRINT("Device Extension:", extension.extensionName);
		}
	}

	uint32_t FindDeviceQueueFamilyIndex(uint32_t requestedFlags, uint32_t numQueues)
	{
		int queueFamily = -1; // Queue family index to return

		u32 queueCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(handlePhysicalDevice, &queueCount, nullptr);
		dmut::HeapAllocSize<VkQueueFamilyProperties> properties(queueCount);
		vkGetPhysicalDeviceQueueFamilyProperties(handlePhysicalDevice, &queueCount, properties.RawPtr());

		for (u32 i = 0; i < queueCount; ++i)
		{
			if (properties[(size_t)i].queueCount > numQueues && (properties[(size_t)i].queueFlags & requestedFlags) == requestedFlags)
			{
				// If there are enough queues and they all support the requested flags then we're good to go!

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(handlePhysicalDevice, (u32)i, handleSurface, &presentSupport);

				if (presentSupport)
				{
					// If the queue can present to the screen then we're good to go too.
					queueFamily = i;
					break;
				}
			}

		}

		DOMLOG_ERROR_IF(queueFamily == -1, "Cannot find queue family that supports flags:", requestedFlags, "and has", numQueues, "queues");

		return queueFamily;
	}

	VkDevice CreateDevice()
	{
		///PrintDeviceLayers();
		///PrintDeviceExtensions();

#ifndef DOMRELEASE
		const char* layerNames[] = { "VK_LAYER_KHRONOS_validation" };
#else //~ #ifndef DOMRELEASE
		const char* layerNames[] = { };
#endif //~ #ifndef DOMRELEASE
		const char* extensionNames[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		uint32_t numQueues = 2; // Num queues we want (2 because we are using swapchain with 2 render targets and want to render to both at the same time)
		deviceQueueFamilyIndex = FindDeviceQueueFamilyIndex(VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT, numQueues); // Family of queues to pick from
		float queuePriorities[] = { 1.0f, 1.0f };

		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = deviceQueueFamilyIndex;
		queueCreateInfo.pQueuePriorities = queuePriorities;
		queueCreateInfo.queueCount = numQueues;

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.enabledExtensionCount = DMUT_ARRAY_SIZE(extensionNames);
		createInfo.ppEnabledExtensionNames = extensionNames;
		createInfo.enabledLayerCount = DMUT_ARRAY_SIZE(layerNames);
		createInfo.ppEnabledLayerNames = layerNames;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1; // If we want multiple families (graphics one and compute one maybe?) then this might need to be 2

		VkDevice returnedDevice = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkCreateDevice(handlePhysicalDevice, &createInfo, nullptr, &returnedDevice), "Device");
		return returnedDevice;

	}

	VkQueue CreateQueue(uint32_t queueFamily, uint32_t queueIndex)
	{
		VkQueue returnedQueue = VK_NULL_HANDLE;
		vkGetDeviceQueue(handleDevice, queueFamily, queueIndex, &returnedQueue);
		return returnedQueue;
	}

	VkSwapchainKHR CreateSwapchain()
	{
		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = handleSurface;
		createInfo.imageFormat = VK_FORMAT_B8G8R8A8_UNORM; // Query device (surface?) to see if this is supported
		createInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		createInfo.imageExtent = { EXTENT_WIDTH, EXTENT_HEIGHT };
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.minImageCount = 2;
		createInfo.pQueueFamilyIndices = &deviceQueueFamilyIndex;
		createInfo.queueFamilyIndexCount = 1;
		createInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR; // Check this against surface capabilities?
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR; // Check this against surface capabilities?
		createInfo.presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		createInfo.clipped = VK_TRUE; // This discards things offscreen -- might want to turn it off for post processing?
		createInfo.oldSwapchain = VK_NULL_HANDLE;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkSwapchainKHR returnedSwapChain = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkCreateSwapchainKHR(handleDevice, &createInfo, nullptr, &returnedSwapChain), "Swapchain");

		uint32_t numSwapchainImages = 0;
		vkGetSwapchainImagesKHR(handleDevice, returnedSwapChain, &numSwapchainImages, nullptr);
		handleSwapChainImages.Alloc((size_t)numSwapchainImages);
		vkGetSwapchainImagesKHR(handleDevice, returnedSwapChain, &numSwapchainImages, handleSwapChainImages.RawPtr());

		return returnedSwapChain;
	}

	VkShaderModule CreateShader(const char* pShaderName)
	{
		std::string fullFilePath = pShaderPath;
		fullFilePath.append("/");
		fullFilePath.append(pShaderName);
		dmut::HeapAllocSize<char> spirvCode = std::move(dmim::importText(fullFilePath.c_str()));

		VkShaderModuleCreateInfo createInfo = {};

		const size_t codeSize = spirvCode.GetSize() - 1;
		DOMLOG_ERROR_IF(codeSize % sizeof(uint32_t), "spirv code should be a multiple of 4 bytes");

		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = codeSize; // Ignore null terminator
		createInfo.pCode = (const uint32_t*)(spirvCode.RawPtr());

		VkShaderModule returnedShader = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkCreateShaderModule(handleDevice, &createInfo, nullptr, &returnedShader), fullFilePath.c_str());
		return returnedShader;
	}

	VkBuffer CreateVertexBuffer()
	{
		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.usage = VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		createInfo.size = sizeof(Vertex) * VERTEX_BUFFER_SIZE;
		createInfo.queueFamilyIndexCount = 0; // Don't need this?
		createInfo.pQueueFamilyIndices = nullptr;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkBuffer returnedHandle = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkCreateBuffer(handleDevice, &createInfo, nullptr, &returnedHandle), "VertexBuffer");
		return returnedHandle;
	}

	VkBuffer CreateUniformBuffer(size_t size)
	{

		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.usage = VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		createInfo.size = size;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
		createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkBuffer returnedHandle = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkCreateBuffer(handleDevice, &createInfo, nullptr, &returnedHandle), "UniformBuffer");
		return returnedHandle;
	}

	VkRenderPass CreateRenderPass()
	{
		VkAttachmentDescription attachments[2] = {};
		
		// Colour Attachment
		attachments[0].flags = 0;
		attachments[0].format = VkFormat::VK_FORMAT_B8G8R8A8_UNORM;
		attachments[0].samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		
		// Depth Attachment
		attachments[1].flags = 0;
		attachments[1].format = VkFormat::VK_FORMAT_D24_UNORM_S8_UINT;
		attachments[1].samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[1].stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colourAttachment = {};
		colourAttachment.attachment = 0;
		colourAttachment.layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachment = {};
		depthAttachment.attachment = 1;
		depthAttachment.layout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.flags = 0;
		subpass.pipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.inputAttachmentCount = 0;
		subpass.pInputAttachments = nullptr;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colourAttachment;
		subpass.pResolveAttachments = nullptr;
		subpass.pDepthStencilAttachment = &depthAttachment;
		subpass.preserveAttachmentCount = 0;
		subpass.pPreserveAttachments = nullptr;

#ifdef DOMIMGUI
		// Second subpass for rendering ImGUI

		VkSubpassDescription subpassImGui = {};
		subpassImGui.flags = 0;
		subpassImGui.pipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassImGui.inputAttachmentCount = 0;
		subpassImGui.pInputAttachments = nullptr;
		subpassImGui.colorAttachmentCount = 1;
		subpassImGui.pColorAttachments = &colourAttachment;
		subpassImGui.pResolveAttachments = nullptr;
		subpassImGui.pDepthStencilAttachment = nullptr;
		subpassImGui.preserveAttachmentCount = 0;
		subpassImGui.pPreserveAttachments = nullptr;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = 0;
		dependency.dstSubpass = 1;
		dependency.srcStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		dependency.dstStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstAccessMask = VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependency.dependencyFlags = 0;
#endif //~ #ifdef DOMIMGUI

		VkSubpassDescription subpasses[] = 
		{ 
			subpass
#ifdef DOMIMGUI
			, subpassImGui
#endif //~ #ifdef DOMIMGUI
		};

		VkSubpassDependency dependencies[] =
		{
#ifdef DOMIMGUI
			dependency
#endif //~ #ifdef DOMIMGUI
		};

		VkRenderPassCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.attachmentCount = 2;
		createInfo.pAttachments = attachments;
		createInfo.subpassCount = DMUT_ARRAY_SIZE(subpasses);
		createInfo.pSubpasses = subpasses;
		createInfo.dependencyCount = DMUT_ARRAY_SIZE(dependencies);
		createInfo.pDependencies = dependencies;

		VkRenderPass returnedHandle = VK_NULL_HANDLE;
 		VulkanUtils::ErrorCheck(vkCreateRenderPass(handleDevice, &createInfo, nullptr, &returnedHandle), "RenderPass");
		return returnedHandle;
	}

	VkDescriptorPool CreateDescriptorPool()
	{
		// Increase this if we need more things rendered
		const int maxDescriptorSets = 1024; // #TODO: This can be lowered a lot once we cull shit off-screen. Right now it's showing every health bar for every enemy in the game.

		VkDescriptorPoolSize poolSizes[] =
		{
			{ VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, maxDescriptorSets }

#ifdef DOMIMGUI
			,
			{ VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1}
#endif //~ #ifdef DOMIMGUI
		};

		VkDescriptorPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = VkDescriptorPoolCreateFlagBits::VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		createInfo.maxSets = maxDescriptorSets;
		createInfo.poolSizeCount = DMUT_ARRAY_SIZE(poolSizes);
		createInfo.pPoolSizes = poolSizes;

		VkDescriptorPool returnedHandle = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkCreateDescriptorPool(handleDevice, &createInfo, nullptr, &returnedHandle), "DescriptorPool");
		return returnedHandle;

	}

	VkDescriptorSetLayout CreateDescriptorSetLayout(VkShaderStageFlagBits shaderStage, uint32_t bindingNumber)
{
		VkDescriptorSetLayoutBinding binding;
		binding.binding = bindingNumber;
		binding.descriptorCount = 1;
		binding.stageFlags = shaderStage; // This uniform buffer will hold the array of vector art primitives
		binding.pImmutableSamplers = (VkSampler*)nullptr;
		binding.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		VkDescriptorSetLayoutCreateInfo createInfo;
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.bindingCount = 1;
		createInfo.pBindings = &binding;

		VkDescriptorSetLayout returnedLayout = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkCreateDescriptorSetLayout(handleDevice, &createInfo, nullptr, &returnedLayout), "DescriptorSetLayout");
		return returnedLayout;
	}

	VkDescriptorSet CreateDescriptorSet(VkDescriptorSetLayout layout)
	{
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.descriptorPool = handleDescriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;

		VkDescriptorSet returnedLayout = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkAllocateDescriptorSets(handleDevice, &allocInfo, &returnedLayout), "DescriptorSet");
		return returnedLayout;
	}

	void UpdateVectorBuffer(RenderedObjectEntry& entry)
	{
		u32 data[4096] = {};

		entry.pVectorArt->Serialize(data);

		void* pDeviceData = nullptr;
		vkMapMemory(handleDevice, entry.handleDeviceMemory, 0, sizeof(data), 0, &pDeviceData);
		int* pDeviceInts = (int*)pDeviceData;
		for (int i = 0; i < 1024; ++i)
		{
			// #TODO: Try and understand why there's a fucking 16 byte stride or whatever in the GPU memory
			pDeviceInts[i * 4] = data[i];
		}
		vkUnmapMemory(handleDevice, entry.handleDeviceMemory);

		VkDescriptorBufferInfo descriptorBufferInfo = {};
		descriptorBufferInfo.buffer = entry.handleBuffer;
		descriptorBufferInfo.offset = 0;
		descriptorBufferInfo.range = VK_WHOLE_SIZE;

		VkWriteDescriptorSet descriptorSetWrite = {};
		descriptorSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorSetWrite.pNext = nullptr;
		descriptorSetWrite.dstSet = entry.handleDescriptorSet;
		descriptorSetWrite.dstBinding = 0;
		descriptorSetWrite.dstArrayElement = 0;
		descriptorSetWrite.descriptorCount = 1;
		descriptorSetWrite.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorSetWrite.pImageInfo = nullptr;
		descriptorSetWrite.pBufferInfo = &descriptorBufferInfo;
		descriptorSetWrite.pTexelBufferView = nullptr;
		vkUpdateDescriptorSets(handleDevice, 1, &descriptorSetWrite, 0, nullptr);
	}

	void UpdateViewBuffer(float deltaTime)
	{
		void* pDeviceData = nullptr;
		vkMapMemory(handleDevice, handleDeviceMemoryViewBuffer, 0, sizeof(Mat4f), 0, &pDeviceData);
		memcpy(pDeviceData, &viewMatrix, sizeof(Mat4f));
		vkUnmapMemory(handleDevice, handleDeviceMemoryViewBuffer);

		VkDescriptorBufferInfo descriptorBufferInfo = {};
		descriptorBufferInfo.buffer = handleBufferView;
		descriptorBufferInfo.offset = 0;
		descriptorBufferInfo.range = VK_WHOLE_SIZE;

		VkWriteDescriptorSet descriptorSetWrite = {};
		descriptorSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorSetWrite.pNext = nullptr;
		descriptorSetWrite.dstSet = handleDescriptorSetView;
		descriptorSetWrite.dstBinding = 1;
		descriptorSetWrite.dstArrayElement = 0;
		descriptorSetWrite.descriptorCount = 1;
		descriptorSetWrite.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorSetWrite.pImageInfo = nullptr;
		descriptorSetWrite.pBufferInfo = &descriptorBufferInfo;
		descriptorSetWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(handleDevice, 1, &descriptorSetWrite, 0, nullptr);
	}

	void UpdateProjectionBuffer()
	{
		Mat4f projectionMatrix = dmma::generateOrthoganol(0, EXTENT_WIDTH, 0, EXTENT_HEIGHT, 0.0f, 1.0f);
		projectionMatrix.transpose();
		
		void* pDeviceData = nullptr;
		vkMapMemory(handleDevice, handleDeviceMemoryProjectionBuffer, 0, sizeof(Mat4f), 0, &pDeviceData);
		*((Mat4f*)pDeviceData) = projectionMatrix;
		vkUnmapMemory(handleDevice, handleDeviceMemoryProjectionBuffer);

		VkDescriptorBufferInfo descriptorBufferInfo = {};
		descriptorBufferInfo.buffer = handleBufferProjection;
		descriptorBufferInfo.offset = 0;
		descriptorBufferInfo.range = VK_WHOLE_SIZE;

		VkWriteDescriptorSet descriptorSetWrite = {};
		descriptorSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorSetWrite.pNext = nullptr;
		descriptorSetWrite.dstSet = handleDescriptorSetProjection;
		descriptorSetWrite.dstBinding = 0;
		descriptorSetWrite.dstArrayElement = 0;
		descriptorSetWrite.descriptorCount = 1;
		descriptorSetWrite.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorSetWrite.pImageInfo = nullptr;
		descriptorSetWrite.pBufferInfo = &descriptorBufferInfo;
		descriptorSetWrite.pTexelBufferView = nullptr;
		
		vkUpdateDescriptorSets(handleDevice, 1, &descriptorSetWrite, 0, nullptr);
	}

	void UpdateViewBufferHUD()
	{
		Mat4f viewMatrixHUD = Mat4f::Identity(); // Identity view matrix because the HUD shouldn't ever move

		void* pDeviceData = nullptr;
		vkMapMemory(handleDevice, handleDeviceMemoryViewBufferHUD, 0, sizeof(Mat4f), 0, &pDeviceData);
		memcpy(pDeviceData, &viewMatrixHUD, sizeof(Mat4f));
		vkUnmapMemory(handleDevice, handleDeviceMemoryViewBufferHUD);

		VkDescriptorBufferInfo descriptorBufferInfo = {};
		descriptorBufferInfo.buffer = handleBufferViewHUD;
		descriptorBufferInfo.offset = 0;
		descriptorBufferInfo.range = VK_WHOLE_SIZE;

		VkWriteDescriptorSet descriptorSetWrite = {};
		descriptorSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorSetWrite.pNext = nullptr;
		descriptorSetWrite.dstSet = handleDescriptorSetViewHUD;
		descriptorSetWrite.dstBinding = 1;
		descriptorSetWrite.dstArrayElement = 0;
		descriptorSetWrite.descriptorCount = 1;
		descriptorSetWrite.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorSetWrite.pImageInfo = nullptr;
		descriptorSetWrite.pBufferInfo = &descriptorBufferInfo;
		descriptorSetWrite.pTexelBufferView = nullptr;
		
		vkUpdateDescriptorSets(handleDevice, 1, &descriptorSetWrite, 0, nullptr);
	}

	void UpdateProjectionBufferHUD()
	{
		// #JANK: This is the same as the in-game projection matrix because it's a 2D game but in-case we ever go 3D I guess I'll have this function

		Mat4f projectionMatrix = dmma::generateOrthoganol(0, EXTENT_WIDTH, 0, EXTENT_HEIGHT, 0.0f, 1.0f);
		projectionMatrix.transpose();

		void* pDeviceData = nullptr;
		vkMapMemory(handleDevice, handleDeviceMemoryProjectionBufferHUD, 0, sizeof(Mat4f), 0, &pDeviceData);
		*((Mat4f*)pDeviceData) = projectionMatrix;
		vkUnmapMemory(handleDevice, handleDeviceMemoryProjectionBufferHUD);

		VkDescriptorBufferInfo descriptorBufferInfo = {};
		descriptorBufferInfo.buffer = handleBufferProjectionHUD;
		descriptorBufferInfo.offset = 0;
		descriptorBufferInfo.range = VK_WHOLE_SIZE;

		VkWriteDescriptorSet descriptorSetWrite = {};
		descriptorSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorSetWrite.pNext = nullptr;
		descriptorSetWrite.dstSet = handleDescriptorSetProjectionHUD;
		descriptorSetWrite.dstBinding = 0;
		descriptorSetWrite.dstArrayElement = 0;
		descriptorSetWrite.descriptorCount = 1;
		descriptorSetWrite.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorSetWrite.pImageInfo = nullptr;
		descriptorSetWrite.pBufferInfo = &descriptorBufferInfo;
		descriptorSetWrite.pTexelBufferView = nullptr;
		vkUpdateDescriptorSets(handleDevice, 1, &descriptorSetWrite, 0, nullptr);
	}

	VkPipelineLayout CreatePipelineLayout()
	{
		// Model matrix
		VkPushConstantRange pushConstantRange = {};
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(Mat4f); 
		pushConstantRange.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;

		VkDescriptorSetLayout layouts[] = { handleDescriptorSetLayoutProjection, handleDescriptorSetLayoutView, handleDescriptorSetLayoutVector };

		VkPipelineLayoutCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.setLayoutCount = DMUT_ARRAY_SIZE(layouts);
		createInfo.pSetLayouts = layouts;
		createInfo.pushConstantRangeCount = 1;
		createInfo.pPushConstantRanges = &pushConstantRange;

		VkPipelineLayout returnedLayout = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkCreatePipelineLayout(handleDevice, &createInfo, nullptr, &returnedLayout), "PipelineLayout");
		return returnedLayout;
	}

	VkPipeline CreatePipeline()
	{
		VkPipelineShaderStageCreateInfo shaderStageInfos[2] = {};
		shaderStageInfos[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfos[0].module = handleShaderVertex;
		shaderStageInfos[0].pName = "main";
		shaderStageInfos[0].stage = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
		shaderStageInfos[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfos[1].module = handleShaderFragment;
		shaderStageInfos[1].pName = "main";
		shaderStageInfos[1].stage = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;

		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = EXTENT_WIDTH;
		viewport.height = EXTENT_HEIGHT;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = { EXTENT_WIDTH, EXTENT_HEIGHT };

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer = {};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_NONE; // Culling
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f;
		rasterizer.depthBiasClamp = 0.0f;
		rasterizer.depthBiasSlopeFactor = 0.0f;

		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

		// Alpha Blending -- Entire Pipeline Options
		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		// Depth buffer state
		VkPipelineDepthStencilStateCreateInfo depthBufferStateInfo = {};
		depthBufferStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthBufferStateInfo.pNext = nullptr;
		depthBufferStateInfo.flags = (VkPipelineDepthStencilStateCreateFlags)0;
		depthBufferStateInfo.depthTestEnable = VK_TRUE;
		depthBufferStateInfo.depthWriteEnable = VK_TRUE;
		depthBufferStateInfo.depthCompareOp = VkCompareOp::VK_COMPARE_OP_GREATER;
		depthBufferStateInfo.depthBoundsTestEnable = VK_FALSE;
		depthBufferStateInfo.stencilTestEnable = VK_FALSE;

		VkVertexInputBindingDescription vertexDescription = {};
		vertexDescription.binding = 0;
		vertexDescription.stride = sizeof(Vertex);
		vertexDescription.inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;

		VkVertexInputAttributeDescription vertexAttributeDescriptions[2] = {};
		
		// Position - binding 0 location 0
		vertexAttributeDescriptions[0].binding = 0;
		vertexAttributeDescriptions[0].location = 0;
		vertexAttributeDescriptions[0].offset = offsetof(Vertex, pos);
		vertexAttributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		
		// UV - binding 0 location 1
		vertexAttributeDescriptions[1].binding = 0;
		vertexAttributeDescriptions[1].location = 1;
		vertexAttributeDescriptions[1].offset = offsetof(Vertex, uv);
		vertexAttributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &vertexDescription; // Optional
		vertexInputInfo.vertexAttributeDescriptionCount = 2;
		vertexInputInfo.pVertexAttributeDescriptions = vertexAttributeDescriptions;

		// The actual create structure
		VkGraphicsPipelineCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.stageCount = 2;
		createInfo.pStages = shaderStageInfos;
		createInfo.pInputAssemblyState = &inputAssembly;
		createInfo.pViewportState = &viewportState;
		createInfo.pRasterizationState = &rasterizer;
		createInfo.pMultisampleState = &multisampling;
		createInfo.pDepthStencilState = &depthBufferStateInfo; // Optional
		createInfo.pColorBlendState = &colorBlending;
		createInfo.layout = handlePipelineLayout;
		createInfo.renderPass = handleRenderPass;
		createInfo.subpass = 0;
		createInfo.pVertexInputState = &vertexInputInfo;
		createInfo.pDynamicState = nullptr; // Optional
		createInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		createInfo.basePipelineIndex = -1; // Optional

		VkPipeline returnedHandle = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkCreateGraphicsPipelines(handleDevice, VK_NULL_HANDLE, 1, &createInfo, nullptr, &returnedHandle), "Pipeline");
		return returnedHandle;
	}

	VkCommandPool CreateCommandPool()
	{
		VkCommandPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		createInfo.queueFamilyIndex = deviceQueueFamilyIndex;

		VkCommandPool returnedHandle = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkCreateCommandPool(handleDevice, &createInfo, nullptr, &returnedHandle), "CommandPool");
		return returnedHandle;
	}

	VkCommandBuffer CreateCommandBuffer()
	{
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.commandPool = handleCommandPool;
		allocInfo.level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer returnedHandle = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkAllocateCommandBuffers(handleDevice, &allocInfo, &returnedHandle), "CommandBuffer");
		return returnedHandle;
	}

	VkDeviceMemory CreateDeviceMemory(VkDeviceSize memorySize, VkMemoryPropertyFlags memoryFlags, uint32_t memoryTypeBits /*= (uint32_t)-1*/)
	{
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.allocationSize = memorySize;
		allocInfo.memoryTypeIndex = FindMemoryIndex(memoryFlags);

		VkDeviceMemory returnedHandle = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkAllocateMemory(handleDevice, &allocInfo, nullptr, &returnedHandle), "DeviceMemory");
		return returnedHandle;
	}

	VkDeviceMemory CreateDeviceMemoryFromImage(VkImage image)
	{
		VkMemoryRequirements requirements = {};
		vkGetImageMemoryRequirements(handleDevice, image, &requirements);

		return CreateDeviceMemory(requirements.size, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, requirements.memoryTypeBits);
	}

	void BindImageMemory(VkImage image, VkDeviceMemory memory)
	{
		VulkanUtils::ErrorCheck(vkBindImageMemory(handleDevice, image, memory, 0));
	}

	VkImage CreateImage(VkFormat format, uint32_t width, uint32_t height, VkImageUsageFlagBits usage)
	{
		VkImageCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.imageType = VkImageType::VK_IMAGE_TYPE_2D;
		createInfo.format = format;
		createInfo.extent = {width, height, 1};
		createInfo.mipLevels = 1;
		createInfo.arrayLayers = 1;
		createInfo.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
		createInfo.tiling = VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
		createInfo.usage = usage;
		createInfo.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 1;
		createInfo.pQueueFamilyIndices = &deviceQueueFamilyIndex;
		createInfo.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;

		VkImage returnedHandle = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkCreateImage(handleDevice, &createInfo, nullptr, &returnedHandle), "Image");
		return returnedHandle;
	}

	VkImageView CreateImageView(VkImage inHandleImage, VkImageAspectFlags aspectMask, VkFormat format)
	{
		VkImageViewCreateInfo createInfo = {};

		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = inHandleImage;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = format;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = aspectMask;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		VkImageView returnedHandle = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkCreateImageView(handleDevice, &createInfo, nullptr, &returnedHandle), "ImageView");
		return returnedHandle;
	}

	VkFramebuffer CreateFramebuffer(dmut::HeapAllocSize<VkImageView> inHandleImageViews)
	{
		VkFramebufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.pNext = NULL;
		createInfo.renderPass = handleRenderPass;
		createInfo.attachmentCount = (uint32_t)inHandleImageViews.GetSize();
		createInfo.pAttachments = inHandleImageViews.RawPtr();
		createInfo.width = EXTENT_WIDTH;
		createInfo.height = EXTENT_HEIGHT;
		createInfo.layers = 1;

		VkFramebuffer returnedHandle = VK_NULL_HANDLE;
		VulkanUtils::ErrorCheck(vkCreateFramebuffer(handleDevice, &createInfo, nullptr, &returnedHandle), "Framebuffer");
		return returnedHandle;
	}

	void SubmitDrawCommand()
	{
		static uint32_t currentBuffer = 0; // Which framebuffer to use (swapchain swaps between 2 framebuffers)

		VkSemaphore handleSemaphore;
		VkSemaphoreCreateInfo semaphoreCreateInfo;
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreCreateInfo.pNext = nullptr;
		semaphoreCreateInfo.flags = 0;
		VulkanUtils::ErrorCheck(vkCreateSemaphore(handleDevice, &semaphoreCreateInfo, nullptr, &handleSemaphore));

		VulkanUtils::ErrorCheck(vkAcquireNextImageKHR(handleDevice, handleSwapChain, UINT64_MAX, handleSemaphore, VK_NULL_HANDLE, &currentBuffer));

		VkCommandBuffer handleCommandBuffer = currentBuffer == 0 ? handleCommandBuffer1 : handleCommandBuffer2;
		VkCommandBufferBeginInfo commandBufferBegin = {};
		commandBufferBegin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		VulkanUtils::ErrorCheck(vkBeginCommandBuffer(handleCommandBuffer, &commandBufferBegin));

		VkClearValue clearValues[2] =
		{
			{ 0.1f, 0.1f, 0.1f, 1.0f }, // Colour
			{ 0.0f, 0 } // Depth
		};

		VkRenderPassBeginInfo renderPassBegin = {};
		renderPassBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBegin.pNext = nullptr;
		renderPassBegin.renderPass = handleRenderPass;
		renderPassBegin.framebuffer = (currentBuffer % 2) ? handleFrameBuffer1 : handleFrameBuffer2;
		renderPassBegin.renderArea.offset.x = 0;
		renderPassBegin.renderArea.offset.y = 0;
		renderPassBegin.renderArea.extent.width = EXTENT_WIDTH;
		renderPassBegin.renderArea.extent.height = EXTENT_HEIGHT;
		renderPassBegin.clearValueCount = 2;
		renderPassBegin.pClearValues = clearValues;
		vkCmdBeginRenderPass(handleCommandBuffer, &renderPassBegin, VK_SUBPASS_CONTENTS_INLINE);

		VkDeviceSize vertexBufferOffset = 0;

		vkCmdBindVertexBuffers(handleCommandBuffer, 0, 1, &handleVertextBuffer, &vertexBufferOffset);
		vkCmdBindPipeline(handleCommandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, handlePipeline);

		const float sNum = 0.0f;
		const float bNum = 1.0f;

		Vertex quad[6] =
		{
			{ sNum,  sNum, sNum,       sNum, sNum},
			{ bNum,  sNum, sNum,       bNum, sNum},
			{ sNum,  bNum, sNum,       sNum, bNum},
			{ bNum,  sNum, sNum,       bNum, sNum},
			{ bNum,  bNum, sNum,       bNum, bNum},
			{ sNum,  bNum, sNum,       sNum, bNum}
		};

		void* pDeviceData = nullptr;
		vkMapMemory(handleDevice, handleDeviceMemoryVertexBuffer, 0, sizeof(quad), 0, &pDeviceData);
		memcpy(pDeviceData, quad, sizeof(quad));
		vkUnmapMemory(handleDevice, handleDeviceMemoryVertexBuffer);

		auto RenderObjects = [handleCommandBuffer](std::vector<RenderedObjectEntry>& inRenderObjects)
		{
			for (auto& renderObject : inRenderObjects)
			{
				UpdateVectorBuffer(renderObject);
				vkCmdBindDescriptorSets(handleCommandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, handlePipelineLayout, 2, 1, &renderObject.handleDescriptorSet, 0, nullptr);

				for (auto& rendererObject : renderObject.rendererObjects)
				{
					Mat4f modelMatrix = rendererObject->GetModelMatrix();
					vkCmdPushConstants(handleCommandBuffer, handlePipelineLayout, VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(Mat4f), &modelMatrix);

					vkCmdDraw(handleCommandBuffer, DMUT_ARRAY_SIZE(quad), 1, 0, 0);
				}
			}
		};

		// Render all in-game objects
		{
			vkCmdBindDescriptorSets(handleCommandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, handlePipelineLayout, 0, 1, &handleDescriptorSetProjection, 0, nullptr);
			vkCmdBindDescriptorSets(handleCommandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, handlePipelineLayout, 1, 1, &handleDescriptorSetView, 0, nullptr);

			RenderObjects(renderedObjectsInGame);
		}

		// Render all HUD objects
		{
			vkCmdBindDescriptorSets(handleCommandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, handlePipelineLayout, 0, 1, &handleDescriptorSetProjectionHUD, 0, nullptr);
			vkCmdBindDescriptorSets(handleCommandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, handlePipelineLayout, 1, 1, &handleDescriptorSetViewHUD, 0, nullptr);

			RenderObjects(renderedObjectsHUD);
		}

#ifdef DOMIMGUI
		vkCmdNextSubpass(handleCommandBuffer, VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE);

		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), handleCommandBuffer);
#endif //~ #ifdef DOMIMGUI

		vkCmdEndRenderPass(handleCommandBuffer);
		vkEndCommandBuffer(handleCommandBuffer);

		VkFence handleFence;
		VkFenceCreateInfo fenceInfo;
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.pNext = nullptr;
		fenceInfo.flags = 0;
		VulkanUtils::ErrorCheck(vkCreateFence(handleDevice, &fenceInfo, nullptr, &handleFence));

		VkPipelineStageFlags pipeStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		VkSubmitInfo submitInfo[1] = {};
		submitInfo[0].pNext = nullptr;
		submitInfo[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo[0].waitSemaphoreCount = 1;
		submitInfo[0].pWaitSemaphores = &handleSemaphore;
		submitInfo[0].pWaitDstStageMask = &pipeStageFlags;
		submitInfo[0].commandBufferCount = 1;
		submitInfo[0].pCommandBuffers = &handleCommandBuffer;
		submitInfo[0].signalSemaphoreCount = 0;
		submitInfo[0].pSignalSemaphores = nullptr;

		// Submit queue
		VulkanUtils::ErrorCheck(vkQueueSubmit(handleGraphicsQueue, 1, submitInfo, handleFence));

		// Wait for queue to finish rendering on to the ***BACK Buffer***
		vkWaitForFences(handleDevice, 1, &handleFence, VK_TRUE, UINT64_MAX);

		vkDestroyFence(handleDevice, handleFence, nullptr);

		// Swap back buffer and present buffer
		VkPresentInfoKHR present;
		present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present.pNext = nullptr;
		present.swapchainCount = 1;
		present.pSwapchains = &handleSwapChain;
		present.pImageIndices = &currentBuffer;
		present.pWaitSemaphores = nullptr;
		present.waitSemaphoreCount = 0;
		present.pResults = nullptr;

		VulkanUtils::ErrorCheck(vkQueuePresentKHR(handleGraphicsQueue, &present));

		vkDestroySemaphore(handleDevice, handleSemaphore, nullptr);

		currentBuffer ^= 1;
	}

	void CleanUpEntry(RenderedObjectEntry& entry)
	{
		vkDestroyBuffer(handleDevice, entry.handleBuffer, nullptr);
		vkFreeMemory(handleDevice, entry.handleDeviceMemory, nullptr);
		vkFreeDescriptorSets(handleDevice, handleDescriptorPool, 1, &entry.handleDescriptorSet);
	}
}
