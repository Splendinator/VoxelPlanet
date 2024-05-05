#pragma once

#include <fstream>

// Write property to file utils. Shared between header tool (code parse tokens) and Editor (editor property types).
// As it's shared between 2 modules keep everything in the header
namespace PropertyFileUtils
{
	inline void WriteFloatToFile(std::ofstream& file, const std::string& propertyName, float value = 0.0f)
	{
		file << "float " << propertyName << " " << value << std::endl;
	};

	inline void WriteStringToFile(std::ofstream& file, const std::string& propertyName, const std::string& value = "")
	{
		file << "string " << propertyName << " " << value << std::endl;
	};

	inline void WriteIntToFile(std::ofstream& file, const std::string& propertyName, int value = 0)
	{
		file << "int " << propertyName << " " << value << std::endl;
	};

	inline void WriteBoolToFile(std::ofstream& file, const std::string& propertyName, bool value = false)
	{
		file << "bool " << propertyName << " " << std::boolalpha << value << std::endl;
	};

	
	inline void WriteClassToFile(std::ofstream& file, const std::string& propertyName, const std::string& className, const std::string& assetName = "")
	{
		file << "class " << className << " " << propertyName << " " << (assetName == "" ? "nullptr" : assetName) << std::endl;
	};

	inline void WriteStructToFile(std::ofstream& file, const std::string& propertyName, const std::string& structName)
	{
		file << "struct " << structName << " " << propertyName << std::endl;
	}; 
}
