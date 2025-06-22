#pragma once

#include "pch.h"

#include "Shared.h"

#include "../../../Graphics/VulkanUtils.h"
#include "DomImport/DomImport.h"
#include <string>

using namespace dmgf::Internal;

VkShaderModule Shared::CreateShader(VkDevice handleDevice, const char* pShaderName)
{
	std::string fullFilePath = COMPILED_SHADER_PATH;
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
