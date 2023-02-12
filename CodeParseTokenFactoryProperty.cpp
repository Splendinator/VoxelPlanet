#include "pch.h"

#include "CodeParseTokenFactoryProperty.h"

#include "CodeParseTokenPropertyFloat.h"

// PropertyTypeBase
class PropertyTypeBase
{
public:
	// Return whether the input data type is this data type
	virtual bool IsPropertyType(std::string& dataType) const = 0;

	// Create a token for this data type
	CodeParseTokenBase* CreateParseTokenTypeBase(std::stringstream& stream)
	{
		stream >> propertyName;
		return CreateParseTokenType(stream);
	};

protected:
	
	virtual CodeParseTokenBase* CreateParseTokenType(std::stringstream& stream) = 0;

	std::string GetDefaultValueString(std::stringstream& stream) const
	{
		std::string nextString;
		stream >> nextString;
		
		if (nextString == "=")
		{
			stream >> nextString;
			
			if (nextString.substr(nextString.size() - 1, 1) == ";")
			{
				nextString = nextString.substr(0, nextString.size() - 1); // Trim semicolon at the end since we don't care
			}

			return nextString;
		}
		return "";
	};

	std::string propertyName;
};

// PropertyTypeFloat
class PropertyTypeFloat : public PropertyTypeBase
{
public:
	//~ Begin PropertyTypeBase Interface
	bool IsPropertyType(std::string& dataType) const override { return dataType == "float"; }
	//~ End PropertyTypeBase Interface

protected:
	//~ Begin PropertyTypeBase Interface
	CodeParseTokenBase* CreateParseTokenType(std::stringstream & stream) override
	{
		std::string defaultValue = GetDefaultValueString(stream);
		if (defaultValue == "")
		{
			defaultValue = "0.0f";
		}
		
		DOMLOG_ERROR_IF(defaultValue.substr(defaultValue.size() - 1, 1) != "f", "default float value not in correct format, must end in f");
		DOMLOG_ERROR_IF(defaultValue.substr(defaultValue.size() - 2, 1) == ".", "default float value not in correct format, must have a number after the .");

		return new CodeParseTokenPropertyFloat(propertyName, defaultValue);
	}
	//~ End PropertyTypeBase Interface
};

// CodeParseTokenFactoryProperty
bool CodeParseTokenFactoryProperty::IsKeyword(const std::string& keyword)
{
	constexpr int STRING_LENGTH = 16;
	return keyword.size() >= STRING_LENGTH && keyword.substr(0, STRING_LENGTH) == "EDITORPROPERTY()";
}

CodeParseTokenBase* CodeParseTokenFactoryProperty::CreateToken(const std::string& keyword, std::stringstream& stream)
{
	// Setup property types
	PropertyTypeFloat propertyTypeFloat;
	
	PropertyTypeBase* pPropertyTypes[] = 
	{ 
		&propertyTypeFloat
	};

	// Try all property types
	std::string propertyTypeString;
	stream >> propertyTypeString;
	for (PropertyTypeBase* pPropertyType : pPropertyTypes)
	{
		if (pPropertyType->IsPropertyType(propertyTypeString))
		{
			return pPropertyType->CreateParseTokenTypeBase(stream);
		}
	}
	
	DOMLOG_ERROR("Unknown property type", propertyTypeString);
	return nullptr;
}
