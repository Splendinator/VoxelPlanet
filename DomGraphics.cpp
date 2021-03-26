#define WIN32_LEAN_AND_MEAN

#include "DomGraphics.h"

#include "DomImport/DomImport.h"
#include "DomLog/DomLog.h"
#include "DomMath/Mat4.h"
#include "DomMath/Math.h"
#include "DomMath/Vec2.h"
#include "DomMath/Vec3.h"
#include "DomUtils/DomUtils.h"
#include "DomWindow/DomWindow.h"

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

#include <Windows.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_win32.h>

#ifdef DOMIMGUI
#include "imgui_impl_vulkan.h"
#endif //~ #ifdef DOMIMGUI

// C:/VulkanSDK/1.2.131.2/Bin/glslc.exe Triangle.vert -o vert.spv

struct MVP
{
	Mat4f model = Mat4f::Identity();
	Mat4f view = Mat4f::Identity();
	Mat4f proj = dmma::generateProjection(90.0f, 16.f / 9.f, 0.0001f, 1000.0f);
} mvp;

dmut::HeapAlloc<char> GetShaderFilepath(const char* shaderName);

Camera camera;
const int MAX_QUADS = 8000000;
u32 currentBuffer = 0;
const char* destPath = "../Graphics/Shaders/CompiledShaders"; // Destination shader files

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
VulkanModuleDeviceMemoryImage depthBufferMemory(&vulkanPhysicalDevice, &vulkanDevice, &vulkanDepthBufferImage); //We need to explicitly give the depth buffer memory, but not the swap chain images as they already have memory from the swap chain

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

VulkanModuleArraySized<VulkanModuleRenderSubpassDescription, 2> allSubpasses({ &renderSubpass, &imGuiSubpass});

// Subpass Dependancies
VulkanModuleOptional<VulkanModuleRenderSubpassDescription> noSubpass;
VulkanModuleOptional<VulkanModuleRenderSubpassDescription> firstSubpass(&renderSubpass);
VulkanModuleOptional<VulkanModuleRenderSubpassDescription> secondSubpass(&imGuiSubpass);
VulkanModuleArraySized<VulkanModuleOptional<VulkanModuleRenderSubpassDescription>, 2> imGuiSubpassParams({ &firstSubpass, &secondSubpass });
VulkanModuleRenderSubpassDependancy imGuiSubpassDependancy(
	VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
	VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_READ_BIT & VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
	VkPipelineStageFlagBits::VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
	VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_READ_BIT & VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
	&imGuiSubpassParams,
	&allSubpasses);

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
VulkanModuleBuffer<VulkanVertexData<VulkanVertexAttributePos, VulkanVertexAttributeNormal, VulkanVertexAttributeColour>> vulkanModuleVertexBuffer(VkBufferUsageFlagBits::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, &vulkanDevice, 6 * MAX_QUADS);
// Vertex Buffer Memory
VulkanModuleDeviceMemoryBuffer vulkanModuleVertexMemory(&vulkanPhysicalDevice, &vulkanDevice, &vulkanModuleVertexBuffer);
// Shader Array
VulkanModuleArraySized<VulkanModuleShader, 2> vulkanModuleShaders({ &vulkanShaderVertex, &vulkanShaderFragment });
// Descriptor Pool
VulkanModuleDescriptorPool vulkanModuleDescriptorPool(&vulkanDevice);
// Descriptor Buffer
VulkanModuleBuffer<VulkanVertexData<VulkanVertexAttributeMatrix1, VulkanVertexAttributeMatrix2, VulkanVertexAttributeMatrix3>> bufferMVPMatrix(VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, &vulkanDevice);
VulkanModuleDeviceMemoryBuffer bufferMemoryMVPMatrix(&vulkanPhysicalDevice, &vulkanDevice, &bufferMVPMatrix);
VulkanModuleDescriptorBufferInfo descriptorInfoBufferMVPMatrix(&bufferMVPMatrix);
// Descriptor Binding
VulkanModuleDescriptorSetBinding vulkanModuleDescriptorSetBinding(VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, &descriptorInfoBufferMVPMatrix);
// Descriptor Set Layout
VulkanModuleArraySized<VulkanModuleDescriptorSetBinding, 1> bindings({ &vulkanModuleDescriptorSetBinding });
VulkanModuleDescriptorSetLayout vulkanModuleDescriptorSetLayout(&vulkanDevice, &bindings);
// Descriptor Set
VulkanModuleDescriptorSet vulkanModuleDescriptorSet(&vulkanDevice, &vulkanModuleDescriptorPool, &vulkanModuleDescriptorSetLayout);
// Write operation
VulkanModuleDescriptorSetWrite vulkanModuleDescriptorSetWrite(&vulkanDevice, &vulkanModuleDescriptorSet, &vulkanModuleDescriptorSetBinding);
// Pipeline Layout
VulkanModuleArraySized<VulkanModuleDescriptorSetLayout, 1> pipelineLayoutDescriptorSets({ &vulkanModuleDescriptorSetLayout });
VulkanModulePipelineLayout vulkanModulePipelineLayout(&vulkanDevice, &pipelineLayoutDescriptorSets);
// Vertex Data
VulkanVertexData<VulkanVertexAttributePos> vertexData; /// #TODO: Instead of passing this in, make a VulkanModuleVertedBuffer that derrives from VulkanModuleBuffer and has a GetVertexData() thing"

// IMGUI
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



void dmgf::UnInit()
{
#ifdef DOMIMGUI
	ImGui_ImplVulkan_DestroyFontUploadObjects();
	ImGui_ImplVulkan_Shutdown();
#endif //~ #ifdef DOMIMGUI

	vulkanInstance.TearDown();
}

void dmgf::Tick(float deltaTime)
{	
	camera.Tick(deltaTime);
	mvp.view = camera.GetViewMatrix();

	void* pData;
	vkMapMemory(vulkanDevice.GetHandle(), bufferMemoryMVPMatrix.GetHandle(), 0, sizeof(mvp), 0, &pData);
	memcpy(pData, &mvp, sizeof(mvp));
	vkUnmapMemory(vulkanDevice.GetHandle(), bufferMemoryMVPMatrix.GetHandle());
	/// End MVP

#ifdef DOMIMGUI

	const Vec3f cameraForward = camera.Forward();

	ImGui::SetWindowSize({ 400,400 }, ImGuiCond_FirstUseEver);
	ImGui::SetWindowPos({ 0, 200 }, ImGuiCond_FirstUseEver);
	ImGui::Begin("Graphics");
	///ImGui::Text("NumQuads: %d", currentQuad);
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
	renderPassBegin.pNext = NULL;
	renderPassBegin.renderPass = vulkanRenderPass.GetHandle();
	renderPassBegin.framebuffer = (currentBuffer % 2) ? vulkanModuleFrameBuffer1.GetHandle() : vulkanModuleFrameBuffer2.GetHandle();
	renderPassBegin.renderArea.offset.x = 0;
	renderPassBegin.renderArea.offset.y = 0;
	renderPassBegin.renderArea.extent.width = vulkanSwapChain.GetImageWidth();
	renderPassBegin.renderArea.extent.height = vulkanSwapChain.GetImageHeight();
	renderPassBegin.clearValueCount = 2;
	renderPassBegin.pClearValues = clearValue;

	vkCmdBeginRenderPass(commandBuffer.GetHandle(), &renderPassBegin, VK_SUBPASS_CONTENTS_INLINE);

	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(commandBuffer.GetHandle(), 0, 1, &vulkanModuleVertexBuffer.GetHandle(), &offset);

	vkCmdBindPipeline(commandBuffer.GetHandle(), VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline.GetHandle());

	vkCmdBindDescriptorSets(commandBuffer.GetHandle(), VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanModulePipelineLayout.GetHandle(), 0, 1, &vulkanModuleDescriptorSet.GetHandle(), 0, nullptr);

	DrawChunks(commandBuffer);

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

	// Swap back buffer and pressent buffer
	VkPresentInfoKHR present;
	present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present.pNext = NULL;
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

#include <unordered_map>


struct BufferRegion
{
	u32 start;
	u32 size;
};

std::unordered_map<Chunk*, BufferRegion> chunkMap;
u32 lastHostMemoryIndex = 0;
u32 lastDeviceMemoryIndex = 0;

void dmgf::AddChunk(Chunk* chunk, std::vector<dmgf::Quad>& quads)
{
	constexpr int MAX_VERTS = MAX_QUADS * 6;

	const u32 numVerts = quads.size() * 6;
	const u32 memorySize = sizeof(Quad) * quads.size();
	
	chunkMap.insert({ chunk, { lastHostMemoryIndex, numVerts } });

	/// #TODO: Actually write a memory manager instead of this shit
	// Circle buffer wrap around
	if (lastHostMemoryIndex + numVerts > MAX_VERTS)
	{
		lastDeviceMemoryIndex = 0;
		lastHostMemoryIndex = 0;
	}

	void* pData;
	vkMapMemory(vulkanDevice.GetHandle(), vulkanModuleVertexMemory.GetHandle(), lastDeviceMemoryIndex, memorySize, 0, &pData);
	memcpy(pData, quads.data(), memorySize);
	vkUnmapMemory(vulkanDevice.GetHandle(), vulkanModuleVertexMemory.GetHandle());
	
	lastHostMemoryIndex += numVerts;
	lastDeviceMemoryIndex += memorySize;
}

void dmgf::RemoveChunk(Chunk* chunk)
{
	chunkMap.erase(chunk);
}

void dmgf::DrawChunks(VulkanModuleCommandBuffer& commandBuffer)
{
	for (auto& it : chunkMap)
	{
		vkCmdDraw(commandBuffer.GetHandle(), it.second.size, 1, it.second.start, 0);
	}
}

dmut::HeapAlloc<char> GetShaderFilepath(const char* shaderName)
{
	const int BUFFER_SIZE = 256;
	dmut::HeapAlloc<char> shaderFullPath(BUFFER_SIZE);
	sprintf_s(shaderFullPath.RawPtr(), BUFFER_SIZE, "%s/%s.spv\0", destPath, shaderName);
	return shaderFullPath;
}