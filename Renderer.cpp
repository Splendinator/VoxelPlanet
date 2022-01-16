#include "pch.h"

#include "Renderer.h"

#include "../Graphics/Camera.h"
#include "../Graphics/VulkanUtils.h"
#include "DomImport/DomImport.h"
#include "DomWindow/DomWindow.h"

#include <vulkan/vulkan_win32.h>

// Struct describing a single vertex
struct Vertex
{
	float pos[3];
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

VkRenderPass CreateRenderPass();

VkDescriptorPool CreateDescriptorPool();

VkDescriptorSetLayout CreateDescriptorSetLayout();

VkDescriptorSet CreateDescriptorSet();

VkPipelineLayout CreatePipelineLayout();

VkPipeline CreatePipeline();

VkCommandPool CreateCommandPool();

VkCommandBuffer CreateCommandBuffer();

VkDeviceMemory CreateDeviceMemory();

VkImage CreateImage();

VkImageView CreateImageView(VkImage inHandleImage);

VkFramebuffer CreateFramebuffer(VkImageView inHandleImageView);

void SubmitDrawCommand();

Camera mainCamera;
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
VkDescriptorSetLayout handleDescriptorSetLayout = VK_NULL_HANDLE;
VkDescriptorSet handleDescriptorSet = VK_NULL_HANDLE;
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

// Unused
VkBuffer handleStagingBuffer = VK_NULL_HANDLE;
VkDeviceMemory handleDeviceMemory = VK_NULL_HANDLE;

uint32_t deviceQueueFamilyIndex = 0;
const char* pShaderPath = "../Graphics/Shaders/CompiledShaders"; // compiled SPIRV files

namespace dmgf
{
/// #TEMP: PRAGMA
#pragma optimize( "", off )
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
		handleImageView1 = CreateImageView(handleSwapChainImages[0]);
		handleImageView2 = CreateImageView(handleSwapChainImages[1]);
		handleFrameBuffer1 = CreateFramebuffer(handleImageView1);
		handleFrameBuffer2 = CreateFramebuffer(handleImageView2);
		handleDescriptorPool = CreateDescriptorPool();
		handleDescriptorSetLayout = CreateDescriptorSetLayout();
		handleDescriptorSet = CreateDescriptorSet();
		handlePipelineLayout = CreatePipelineLayout();
		handlePipeline = CreatePipeline();
		handleCommandPool = CreateCommandPool();
		handleCommandBuffer1 = CreateCommandBuffer();
		handleCommandBuffer2 = CreateCommandBuffer();
		handleDeviceMemory = CreateDeviceMemory();
		VulkanUtils::ErrorCheck(vkBindBufferMemory(handleDevice, handleVertextBuffer, handleDeviceMemory, 0), "BindVertexBuffer");
	}

	void UnInit()
	{
		/// #TEMP: Destroy shit
		vkDestroyCommandPool(handleDevice, handleCommandPool, nullptr);
		vkDestroyPipeline(handleDevice, handlePipeline, nullptr);
		vkDestroyPipelineLayout(handleDevice, handlePipelineLayout, nullptr);
		vkDestroyDescriptorSetLayout(handleDevice, handleDescriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(handleDevice, handleDescriptorPool, nullptr);
		vkDestroyRenderPass(handleDevice, handleRenderPass, nullptr);
		vkDestroyBuffer(handleDevice, handleVertextBuffer, nullptr);
		vkDestroyShaderModule(handleDevice, handleShaderVertex, nullptr);
		vkDestroyShaderModule(handleDevice, handleShaderFragment, nullptr);
		vkDestroySwapchainKHR(handleDevice, handleSwapChain, nullptr);
		vkDestroyDevice(handleDevice, nullptr);
		vkDestroySurfaceKHR(handleInstance, handleSurface, nullptr);
		vkDestroyInstance(handleInstance, nullptr);
	}
#pragma optimize( "", on )
	void Tick(float deltaTime)
	{
		SubmitDrawCommand();
	}

	Camera& GetCamera()
	{
		return mainCamera;
	}

	VkDevice& GetDeviceHandle()
	{
		return handleDevice;
	}

	VkPhysicalDevice& GetPhysicalDeviceHandle()
	{
		return handlePhysicalDevice;
	}

	VkDeviceMemory& GetStagingBufferMemoryHandle()
	{
		return handleDeviceMemory;
	}

	VkBuffer& GetStagingBufferHandle()
	{
		return handleStagingBuffer;
	}

	VkCommandPool& GetCommandPool()
	{
		return handleCommandPool;
	}

	VkQueue& GetGraphicsQueueHandle()
	{
		return handleGraphicsQueue;
	}

	u32 GetGraphicsQueueFamily()
	{
		/// #TEMP: DO THIS
		return 0;
	}

	void* AddNewObject(Texture* texture, Mesh* mesh)
	{
		/// #TEMP: DO THIS
		return nullptr;
	}

	void SetObjectPos(void* pHandle, const Mat4f& modelMatrix)
	{
		/// #TEMP: DO THIS
	}
}

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

VkPhysicalDevice PickPhysicalDevice(const dmut::HeapAllocSize<VkPhysicalDevice>& physicalDevices)
{
	DOMASSERT(physicalDevices.GetSize() > 0, "This isn't a text adventure game, you need a graphics card");

	/// #TODO: Actually pick the best one
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
	createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	createInfo.size = sizeof(Vertex) * VERTEX_BUFFER_SIZE;
	createInfo.queueFamilyIndexCount = 0; // Don't need this?
	createInfo.pQueueFamilyIndices = nullptr;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkBuffer returnedHandle = VK_NULL_HANDLE;
	VulkanUtils::ErrorCheck(vkCreateBuffer(handleDevice, &createInfo, nullptr, &returnedHandle), "VertexBuffer");
	return returnedHandle;
}

VkRenderPass CreateRenderPass()
{
	VkAttachmentDescription attachment = {};
	attachment.flags = 0;
	attachment.format = VkFormat::VK_FORMAT_B8G8R8A8_UNORM;
	attachment.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
	attachment.loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachment.storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
	attachment.stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachment.stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachment.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
	attachment.finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colourAttachment = {};
	colourAttachment.attachment = 0;
	colourAttachment.layout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.flags = 0;
	subpass.pipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.inputAttachmentCount = 0;
	subpass.pInputAttachments = nullptr;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colourAttachment;
	subpass.pResolveAttachments = nullptr;
	subpass.pDepthStencilAttachment = nullptr;
	subpass.preserveAttachmentCount = 0;
	subpass.pPreserveAttachments = nullptr;

	///VkSubpassDependency dependency = {};
	///dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	///dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
	///dependency.srcStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
	///dependency.dstStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT;
	///dependency.srcAccessMask = 0;
	///dependency.dstAccessMask = VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	///dependency.dependencyFlags = 0;

	VkRenderPassCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.attachmentCount = 1;
	createInfo.pAttachments = &attachment;
	createInfo.subpassCount = 1;
	createInfo.pSubpasses = &subpass;
	createInfo.dependencyCount = 0;
	createInfo.pDependencies = nullptr;
	

	VkRenderPass returnedHandle = VK_NULL_HANDLE;
	VulkanUtils::ErrorCheck(vkCreateRenderPass(handleDevice, &createInfo, nullptr, &returnedHandle), "RenderPass");
	return returnedHandle;
}

VkDescriptorPool CreateDescriptorPool()
{
	VkDescriptorPoolSize poolSizes[] =
	{
		{ VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1 }
	};

	VkDescriptorPoolCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.maxSets = 1;
	createInfo.poolSizeCount = DMUT_ARRAY_SIZE(poolSizes);
	createInfo.pPoolSizes = poolSizes;

	VkDescriptorPool returnedHandle = VK_NULL_HANDLE;
	VulkanUtils::ErrorCheck(vkCreateDescriptorPool(handleDevice, &createInfo, nullptr, &returnedHandle), "DescriptorPool");
	return returnedHandle;

}

VkDescriptorSetLayout CreateDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding binding;
	binding.binding = 0;
	binding.descriptorCount = 1;
	binding.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT; // This uniform buffer will hold the array of vector art primitives
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

VkDescriptorSet CreateDescriptorSet()
{
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.descriptorPool = handleDescriptorPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &handleDescriptorSetLayout;

	VkDescriptorSet returnedLayout = VK_NULL_HANDLE;
	VulkanUtils::ErrorCheck(vkAllocateDescriptorSets(handleDevice, &allocInfo, &returnedLayout), "DescriptorSet");
	return returnedLayout;
}

VkPipelineLayout CreatePipelineLayout()
{
	VkPipelineLayoutCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.setLayoutCount = 1;
	createInfo.pSetLayouts = &handleDescriptorSetLayout;

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
	
	VkVertexInputAttributeDescription positionAttributeDescription = {};
	positionAttributeDescription.binding = 0;
	positionAttributeDescription.location = 0;
	positionAttributeDescription.offset = offsetof(Vertex, pos);
	positionAttributeDescription.format = VK_FORMAT_R32G32B32_SFLOAT;

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &vertexDescription; // Optional
	vertexInputInfo.vertexAttributeDescriptionCount = 1;
	vertexInputInfo.pVertexAttributeDescriptions = &positionAttributeDescription;
	
	///// The actual create structure
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

VkDeviceMemory CreateDeviceMemory()
{
	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.allocationSize = 0x100; // #TODO: This number is what the error message said to put here, we need to query the VkBuffer's memory requirements
	allocInfo.memoryTypeIndex = 8; // #TODO: Util function to pick the physical device memory that has certain flags (in this case 8 == HOST_VISIBLE)

	VkDeviceMemory returnedHandle = VK_NULL_HANDLE;
	VulkanUtils::ErrorCheck(vkAllocateMemory(handleDevice, &allocInfo, nullptr, &returnedHandle), "DeviceMemory");
	return returnedHandle;
}

VkImage CreateImage()
{
	// #TODO: Do this if we want textures

	VkImage returnedHandle = VK_NULL_HANDLE;
	///VulkanUtils::ErrorCheck(vkCreateImage(handleDevice, &createInfo, nullptr, &returnedHandle), "Image");
	return returnedHandle;
}

VkImageView CreateImageView(VkImage inHandleImage)
{
	VkImageViewCreateInfo createInfo = {};

	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.image = inHandleImage;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = VkFormat::VK_FORMAT_B8G8R8A8_UNORM;
	createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
	createInfo.subresourceRange.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;

	VkImageView returnedHandle = VK_NULL_HANDLE;
	VulkanUtils::ErrorCheck(vkCreateImageView(handleDevice, &createInfo, nullptr, &returnedHandle), "ImageView");
	return returnedHandle;
}

VkFramebuffer CreateFramebuffer(VkImageView inHandleImageView)
{
	VkFramebufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.renderPass = handleRenderPass;
	createInfo.attachmentCount = 1;
	createInfo.pAttachments = &inHandleImageView;
	createInfo.width = EXTENT_WIDTH;
	createInfo.height = EXTENT_HEIGHT;
	createInfo.layers = 1;

	VkFramebuffer returnedHandle = VK_NULL_HANDLE;
	VulkanUtils::ErrorCheck(vkCreateFramebuffer(handleDevice, &createInfo, nullptr, &returnedHandle), "Framebuffer");
	return returnedHandle;
}

/// #TEMP: PRAGMA
#pragma optimize( "", off )
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

	VkClearValue clearValue = { 0.1f, 0.1f, 0.1f, 1.0f };

	VkRenderPassBeginInfo renderPassBegin = {};
	renderPassBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBegin.pNext = nullptr;
	renderPassBegin.renderPass = handleRenderPass;
	renderPassBegin.framebuffer = (currentBuffer % 2) ? handleFrameBuffer1: handleFrameBuffer2;
	renderPassBegin.renderArea.offset.x = 0;
	renderPassBegin.renderArea.offset.y = 0;
	renderPassBegin.renderArea.extent.width = EXTENT_WIDTH;
	renderPassBegin.renderArea.extent.height = EXTENT_HEIGHT;
	renderPassBegin.clearValueCount = 1;
	renderPassBegin.pClearValues = &clearValue;
	vkCmdBeginRenderPass(handleCommandBuffer, &renderPassBegin, VK_SUBPASS_CONTENTS_INLINE);

	VkDeviceSize vertexBufferOffset = 0;

	vkCmdBindVertexBuffers(handleCommandBuffer, 0, 1, &handleVertextBuffer, &vertexBufferOffset);
	vkCmdBindPipeline(handleCommandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, handlePipeline);

	Vertex quad[6] =
	{
		{-1.0f, -1.0f, 0.0f},
		{ 1.0f, -1.0f, 0.0f},
		{-1.0f,  1.0f, 0.0f},
		{ 1.0f, -1.0f, 0.0f},
		{ 1.0f,  1.0f, 0.0f},
		{-1.0f,  1.0f, 0.0f}
	};

	void* pDeviceData = nullptr;
	vkMapMemory(handleDevice, handleDeviceMemory, 0, sizeof(quad), 0, &pDeviceData);
	memcpy(pDeviceData, quad, sizeof(quad));
	vkUnmapMemory(handleDevice, handleDeviceMemory);

	vkCmdDraw(handleCommandBuffer, DMUT_ARRAY_SIZE(quad), 1, 0, 0);

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
#pragma optimize( "", on )
