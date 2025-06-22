namespace dmgf
{
	namespace Internal
	{
		namespace Shared
		{
			// #TODO: These need to be grabbed from the surface
			constexpr int EXTENT_WIDTH = 1776; // Width of renderable portion of screen
			constexpr int EXTENT_HEIGHT = 969; // Height of renderable portion of screen
		
			constexpr char SHADER_COMPILER_PATH[] = "C:/VulkanSDK/1.2.198.1/Bin/glslc.exe";
			constexpr char COMPILED_SHADER_PATH[] = "../Graphics/Shaders/CompiledShaders";	// All compiled shaders will be placed here

			VkShaderModule CreateShader(VkDevice handleDevice, const char* pShaderName);
		}
	}
}
