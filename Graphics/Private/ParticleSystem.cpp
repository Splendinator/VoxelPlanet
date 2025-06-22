#include "pch.h"

#include "ParticleSystem.h"

#include "../../../Graphics/VulkanUtils.h"
#include "Shared.h"
#include "DomMath/Mat4.h"
#include "DomMath/Math.h"

using namespace dmgf::Internal;
namespace fs = std::filesystem;

constexpr char PARTICLE_SYSTEM_SHADER_PATH[] = "../Graphics/Shaders/ParticleSystem";
constexpr char PARTICLE_SYSTEM_COMPILED_VERTEX_SHADER_FILE_NAME[] = "ParticleVertex.spv";
constexpr char PARTICLE_SYSTEM_VERTEX_SHADER_NAME[] = "ParticleVertex";
constexpr int MAX_NUM_PARTICLES = 8192;
// Max size push constants can handle, if we need more then use a uniform buffer instead but this is slower.
// #JANK: Some GPUs only support 128 bytes, so if we ever make this an actual game (lol) then lower this
constexpr int MAX_USER_DEFINED_PARAMETER_SIZE = 256;

// Represents one particle system type (one per shader)
struct ParticleSystemInfo
{
	VkPipeline handlePipeline = VK_NULL_HANDLE;
	VkShaderModule handleShader = VK_NULL_HANDLE;
};

struct Vertex
{
	float pos[3];
	float uv[2];
};

// The reason for the arrays is that we used instanced rendering. 
struct PerParticleSystemUniformBufferLayout
{
	Mat4f modelMatrix[MAX_NUM_PARTICLES];
	u64 particleSeed[MAX_NUM_PARTICLES];
	float currentTime[MAX_NUM_PARTICLES];
};

ParticleSystem::ParticleSystemInitInfo initInfo;

// These handles are shared across all particle systems
VkShaderModule handleShaderVertex = VK_NULL_HANDLE;
VkPipelineLayout handlePipelineLayout = VK_NULL_HANDLE;
VkDescriptorPool handleDescriptorPool = VK_NULL_HANDLE;
VkBuffer handleVertexBuffer = VK_NULL_HANDLE;
VkDeviceMemory handleDeviceMemoryVertexBuffer = VK_NULL_HANDLE;

// Projection matrix updated once at initialisation
VkDescriptorSetLayout handleDescriptorSetLayoutProjection = VK_NULL_HANDLE; 
VkDescriptorSet handleDescriptorSetProjection = VK_NULL_HANDLE;
VkDeviceMemory handleDeviceMemoryProjectionBuffer = VK_NULL_HANDLE;
VkBuffer handleBufferProjection = VK_NULL_HANDLE;

// View matrix updated once per frame
VkDescriptorSetLayout handleDescriptorSetLayoutViewMatrix = VK_NULL_HANDLE; 
VkDescriptorSet handleDescriptorSetViewMatrix = VK_NULL_HANDLE;
VkDeviceMemory handleDeviceMemoryViewMatrixBuffer = VK_NULL_HANDLE;
VkBuffer handleBufferViewMatrix = VK_NULL_HANDLE;

// Updated once per particle system
VkDescriptorSetLayout handleDescriptorSetLayoutPerParticleSystem = VK_NULL_HANDLE; 
VkDescriptorSet handleDescriptorSetPerParticleSystem = VK_NULL_HANDLE;
VkDeviceMemory handleDeviceMemoryPerParticleSystemBuffer = VK_NULL_HANDLE;
VkBuffer handleBufferPerParticleSystem = VK_NULL_HANDLE;

// Map of names of particle system shaders to their info.
std::unordered_map<std::string, ParticleSystemInfo> particleSystems;

void GatherParticleSystems()
{
	// This creates one particle system for all *.frag files in the PARTICLE_SYSTEM_SHADER_PATH directory. 
	
	auto fileIterator = fs::directory_iterator(PARTICLE_SYSTEM_SHADER_PATH);
	for (const auto& entry : fileIterator)
	{
		if (entry.is_regular_file())
		{
			const std::string fileName = fileIterator->path().filename().string();
            const std::string fileNameNoExtension = fileName.substr(0, fileName.find_last_of ('.'));

			if (fileNameNoExtension == PARTICLE_SYSTEM_VERTEX_SHADER_NAME)
			{
				continue;
			}

			particleSystems.emplace(fileNameNoExtension, ParticleSystemInfo());
		}
	}
}

VkDescriptorPool CreateDescriptorPool()
{
	const u32 numParticleSystems = (u32)particleSystems.size();

	VkDescriptorPoolSize poolSizes[] =
	{
		{ VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, numParticleSystems * 3 }, 
	};

	VkDescriptorPoolCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = VkDescriptorPoolCreateFlagBits::VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	createInfo.maxSets = numParticleSystems * 3;
	createInfo.poolSizeCount = DMUT_ARRAY_SIZE(poolSizes);
	createInfo.pPoolSizes = poolSizes;

	VkDescriptorPool returnedHandle = VK_NULL_HANDLE;
	VulkanUtils::ErrorCheck(vkCreateDescriptorPool(initInfo.handleDevice, &createInfo, nullptr, &returnedHandle), "DescriptorPool");
	return returnedHandle;
}

VkDescriptorSetLayout CreateDescriptorSetLayout(VkShaderStageFlagBits shaderStage, uint32_t bindingNumber, VkDescriptorType type)
{
	VkDescriptorSetLayoutBinding binding;
	binding.binding = bindingNumber;
	binding.descriptorCount = 1;
	binding.stageFlags = shaderStage;
	binding.pImmutableSamplers = (VkSampler*)nullptr;
	binding.descriptorType = type;

	VkDescriptorSetLayoutCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.bindingCount = 1;
	createInfo.pBindings = &binding;

	VkDescriptorSetLayout returnedLayout = VK_NULL_HANDLE;
	VulkanUtils::ErrorCheck(vkCreateDescriptorSetLayout(initInfo.handleDevice, &createInfo, nullptr, &returnedLayout), "DescriptorSetLayout");
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
	VulkanUtils::ErrorCheck(vkAllocateDescriptorSets(initInfo.handleDevice, &allocInfo, &returnedLayout), "DescriptorSet");
	return returnedLayout;
}

VkPipelineLayout CreatePipelineLayout()
{
	VkDescriptorSetLayout layouts[] = {handleDescriptorSetLayoutProjection, handleDescriptorSetLayoutViewMatrix, handleDescriptorSetLayoutPerParticleSystem};

	// Push constants are for user defined data to be passed in to the particle shader. i.e a smoke shader might want to pass in the wind direction
	VkPushConstantRange pushConstantRange = {};
	pushConstantRange.offset = 0;
	pushConstantRange.size = MAX_USER_DEFINED_PARAMETER_SIZE;
	pushConstantRange.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
	
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.pSetLayouts = layouts;
	pipelineLayoutInfo.setLayoutCount = DMUT_ARRAY_SIZE(layouts);
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	

	VkPipelineLayout createdPipelineLayout = VK_NULL_HANDLE;
	VulkanUtils::ErrorCheck(vkCreatePipelineLayout(initInfo.handleDevice, &pipelineLayoutInfo, nullptr, &createdPipelineLayout), "CreatePipelineLayout");
	return createdPipelineLayout;
}

VkPipeline InitPipeline(const ParticleSystemInfo& info)
{
	VkPipelineShaderStageCreateInfo shaderStageInfos[2] = {};
	shaderStageInfos[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfos[0].module = handleShaderVertex;
	shaderStageInfos[0].pName = "main";
	shaderStageInfos[0].stage = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
	shaderStageInfos[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfos[1].module = info.handleShader;
	shaderStageInfos[1].pName = "main";
	shaderStageInfos[1].stage = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = Shared::EXTENT_WIDTH;
	viewport.height = Shared::EXTENT_HEIGHT;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = {Shared::EXTENT_WIDTH, Shared::EXTENT_HEIGHT};

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
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; // Optional
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
	createInfo.renderPass = initInfo.handleRenderPass;
	createInfo.subpass = 0;
	createInfo.pVertexInputState = &vertexInputInfo;
	createInfo.pDynamicState = nullptr; // Optional
	createInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	createInfo.basePipelineIndex = -1; // Optional

	VkPipeline returnedHandle = VK_NULL_HANDLE;
	VulkanUtils::ErrorCheck(vkCreateGraphicsPipelines(initInfo.handleDevice, VK_NULL_HANDLE, 1, &createInfo, nullptr, &returnedHandle), "Pipeline");
	return returnedHandle;
}

VkBuffer CreateVertexBuffer()
{
	VkBufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.usage = VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	createInfo.size = sizeof(Vertex) * 6;
	createInfo.queueFamilyIndexCount = 0; // Don't need this?
	createInfo.pQueueFamilyIndices = nullptr;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkBuffer returnedHandle = VK_NULL_HANDLE;
	VulkanUtils::ErrorCheck(vkCreateBuffer(initInfo.handleDevice, &createInfo, nullptr, &returnedHandle), "VertexBuffer");
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
	VulkanUtils::ErrorCheck(vkCreateBuffer(initInfo.handleDevice, &createInfo, nullptr, &returnedHandle), "UniformBuffer");
	return returnedHandle;
}

VkDeviceMemory CreateDeviceMemory(VkDeviceSize memorySize, VkMemoryPropertyFlags memoryFlags, uint32_t memoryTypeBits = (uint32_t)-1)
{
	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.pNext = nullptr;
	allocInfo.allocationSize = memorySize;
	allocInfo.memoryTypeIndex = VulkanUtils::FindMemoryIndex(initInfo.handlePhysicalDevice, memoryFlags);

	VkDeviceMemory returnedHandle = VK_NULL_HANDLE;
	VulkanUtils::ErrorCheck(vkAllocateMemory(initInfo.handleDevice, &allocInfo, nullptr, &returnedHandle), "DeviceMemory");
	return returnedHandle;
}

ParticleSystemInfo InitParticleSystemInfo(std::string shaderName)
{
	ParticleSystemInfo info = {};

	info.handleShader = Shared::CreateShader(initInfo.handleDevice, (shaderName + ".spv").c_str());
	info.handlePipeline = InitPipeline(info);

	return info;
}

void UpdateVertexBuffer()
{
	const float sNum = 0.0f;
	const float bNum = 1.0f;

	Vertex quad[6] =
	{
		{sNum, sNum, sNum, sNum, sNum},
		{bNum, sNum, sNum, bNum, sNum},
		{sNum, bNum, sNum, sNum, bNum},
		{bNum, sNum, sNum, bNum, sNum},
		{bNum, bNum, sNum, bNum, bNum},
		{sNum, bNum, sNum, sNum, bNum}
	};

	void* pDeviceData = nullptr;
	vkMapMemory(initInfo.handleDevice, handleDeviceMemoryVertexBuffer, 0, sizeof(quad), 0, &pDeviceData);
	memcpy(pDeviceData, quad, sizeof(quad));
	vkUnmapMemory(initInfo.handleDevice, handleDeviceMemoryVertexBuffer);
}

void UpdateProjectionBuffer()
{
	Mat4f projectionMatrix = dmma::generateOrthoganol(0, Shared::EXTENT_WIDTH, 0, Shared::EXTENT_HEIGHT, 0.0f, 1.0f);
	projectionMatrix.transpose();
	
	void* pDeviceData = nullptr;
	vkMapMemory(initInfo.handleDevice, handleDeviceMemoryProjectionBuffer, 0, sizeof(Mat4f), 0, &pDeviceData);
	*((Mat4f*)pDeviceData) = projectionMatrix;
	vkUnmapMemory(initInfo.handleDevice, handleDeviceMemoryProjectionBuffer);

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
	
	vkUpdateDescriptorSets(initInfo.handleDevice, 1, &descriptorSetWrite, 0, nullptr);
}

void UpdateViewBuffer()
{
	DOMASSERT(initInfo.pViewMatrix);
	
	void* pDeviceData = nullptr;
	vkMapMemory(initInfo.handleDevice, handleDeviceMemoryViewMatrixBuffer, 0, sizeof(Mat4f), 0, &pDeviceData);
	memcpy(pDeviceData, initInfo.pViewMatrix, sizeof(Mat4f));
	vkUnmapMemory(initInfo.handleDevice, handleDeviceMemoryViewMatrixBuffer);
	
	VkDescriptorBufferInfo descriptorBufferInfo = {};
	descriptorBufferInfo.buffer = handleBufferViewMatrix;
	descriptorBufferInfo.offset = 0;
	descriptorBufferInfo.range = VK_WHOLE_SIZE;

	VkWriteDescriptorSet descriptorSetWrite = {};
	descriptorSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorSetWrite.pNext = nullptr;
	descriptorSetWrite.dstSet = handleDescriptorSetViewMatrix;
	descriptorSetWrite.dstBinding = 0;
	descriptorSetWrite.dstArrayElement = 0;
	descriptorSetWrite.descriptorCount = 1;
	descriptorSetWrite.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorSetWrite.pImageInfo = nullptr;
	descriptorSetWrite.pBufferInfo = &descriptorBufferInfo;
	descriptorSetWrite.pTexelBufferView = nullptr;
	
	vkUpdateDescriptorSets(initInfo.handleDevice, 1, &descriptorSetWrite, 0, nullptr);
}

void UpdatePerParticleSystemBuffer(const ParticleSystemInfo& particleSystem)
{
	PerParticleSystemUniformBufferLayout perParticleBufferLayout = {};

	Mat4f modelMatrix = Mat4f::getScale({100.f,100.f,1.0f}) * Mat4f::getTranslation({100.f, 250.f, -0.1f});
	modelMatrix.transpose();

	perParticleBufferLayout.modelMatrix[0] = modelMatrix; 
	perParticleBufferLayout.currentTime[0] = 0.0f;
	perParticleBufferLayout.particleSeed[0] = 69;
	
	void* pDeviceData = nullptr;
	vkMapMemory(initInfo.handleDevice, handleDeviceMemoryPerParticleSystemBuffer, 0, sizeof(PerParticleSystemUniformBufferLayout), 0, &pDeviceData);
	memcpy(pDeviceData, &perParticleBufferLayout, sizeof(PerParticleSystemUniformBufferLayout));
	vkUnmapMemory(initInfo.handleDevice, handleDeviceMemoryPerParticleSystemBuffer);
	
	VkDescriptorBufferInfo descriptorBufferInfo = {};
	descriptorBufferInfo.buffer = handleBufferPerParticleSystem;
	descriptorBufferInfo.offset = 0;
	descriptorBufferInfo.range = 65536; //VK_WHOLE_SIZE;

	VkWriteDescriptorSet descriptorSetWrite = {};
	descriptorSetWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorSetWrite.pNext = nullptr;
	descriptorSetWrite.dstSet = handleDescriptorSetPerParticleSystem;
	descriptorSetWrite.dstBinding = 0;
	descriptorSetWrite.dstArrayElement = 0;
	descriptorSetWrite.descriptorCount = 1;
	descriptorSetWrite.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	descriptorSetWrite.pImageInfo = nullptr;
	descriptorSetWrite.pBufferInfo = &descriptorBufferInfo;
	descriptorSetWrite.pTexelBufferView = nullptr;
	
	vkUpdateDescriptorSets(initInfo.handleDevice, 1, &descriptorSetWrite, 0, nullptr);
}

void ParticleSystem::Init(const ParticleSystemInitInfo& info)
{
	// We basically need to make one VkPipeline for each particle system shader we're using, and then compile a command buffer to render them. 

	initInfo = info;

#ifndef DOMRELEASE
	VulkanUtils::CompileShaders(Shared::SHADER_COMPILER_PATH, PARTICLE_SYSTEM_SHADER_PATH, Shared::COMPILED_SHADER_PATH);
#endif

	GatherParticleSystems();

	if (particleSystems.empty())
	{
		return;
	}
	
	handleShaderVertex = Shared::CreateShader(initInfo.handleDevice, PARTICLE_SYSTEM_COMPILED_VERTEX_SHADER_FILE_NAME);
	handleDescriptorPool = CreateDescriptorPool();

	handleDescriptorSetLayoutProjection = CreateDescriptorSetLayout(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER); 
	handleDescriptorSetLayoutViewMatrix = CreateDescriptorSetLayout(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	handleDescriptorSetLayoutPerParticleSystem = CreateDescriptorSetLayout(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, 0, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	
	handleDescriptorSetProjection = CreateDescriptorSet(handleDescriptorSetLayoutProjection);
	handleDescriptorSetViewMatrix = CreateDescriptorSet(handleDescriptorSetLayoutViewMatrix);
	handleDescriptorSetPerParticleSystem = CreateDescriptorSet(handleDescriptorSetLayoutPerParticleSystem);
	
	handlePipelineLayout = CreatePipelineLayout();

	// Vertex buffer
	handleVertexBuffer = CreateVertexBuffer();
	handleDeviceMemoryVertexBuffer = CreateDeviceMemory(0x100, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT); // 0x100 is the smallest size we can do
	VulkanUtils::ErrorCheck(vkBindBufferMemory(initInfo.handleDevice, handleVertexBuffer, handleDeviceMemoryVertexBuffer, 0), "BindUniformBuffer");
	UpdateVertexBuffer();

	// Projection matrix buffer
	handleDeviceMemoryProjectionBuffer = CreateDeviceMemory(0x100, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	handleBufferProjection = CreateUniformBuffer(sizeof(Mat4f));
	VulkanUtils::ErrorCheck(vkBindBufferMemory(initInfo.handleDevice, handleBufferProjection, handleDeviceMemoryProjectionBuffer, 0), "BindUniformBuffer");
	UpdateProjectionBuffer();
	
	// View matrix buffer
	handleDeviceMemoryViewMatrixBuffer = CreateDeviceMemory(0x100, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	handleBufferViewMatrix = CreateUniformBuffer(sizeof(Mat4f));
	VulkanUtils::ErrorCheck(vkBindBufferMemory(initInfo.handleDevice, handleBufferViewMatrix, handleDeviceMemoryViewMatrixBuffer, 0), "BindUniformBuffer");

	// Per particle system buffer
	handleDeviceMemoryPerParticleSystemBuffer = CreateDeviceMemory(sizeof(PerParticleSystemUniformBufferLayout), VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
	handleBufferPerParticleSystem = CreateUniformBuffer(sizeof(PerParticleSystemUniformBufferLayout));
	VulkanUtils::ErrorCheck(vkBindBufferMemory(initInfo.handleDevice, handleBufferPerParticleSystem, handleDeviceMemoryPerParticleSystemBuffer, 0), "BindUniformBuffer");
	
	for (auto& it : particleSystems)
	{
		it.second = InitParticleSystemInfo(it.first);
	}
}

void ParticleSystem::Draw(VkCommandBuffer handleCommandBuffer, VkFramebuffer handleFrameBuffer)
{
	VkDeviceSize vertexBufferOffset = 0;
	vkCmdBindVertexBuffers(handleCommandBuffer, 0, 1, &handleVertexBuffer, &vertexBufferOffset);

	UpdateViewBuffer();
	
	for (auto it : particleSystems)
	{
		vkCmdBindPipeline(handleCommandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, it.second.handlePipeline);
		
		UpdatePerParticleSystemBuffer(it.second);
		vkCmdBindDescriptorSets(handleCommandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, handlePipelineLayout, 0, 1, &handleDescriptorSetProjection, 0, nullptr);
		vkCmdBindDescriptorSets(handleCommandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, handlePipelineLayout, 1, 1, &handleDescriptorSetViewMatrix, 0, nullptr);
		vkCmdBindDescriptorSets(handleCommandBuffer, VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, handlePipelineLayout, 2, 1, &handleDescriptorSetPerParticleSystem, 0, nullptr);

		
		// Batch draw all particles
		const int numInstancesToRender = 1;
		vkCmdDraw(handleCommandBuffer, 6, numInstancesToRender, 0, 0);
	}
}
