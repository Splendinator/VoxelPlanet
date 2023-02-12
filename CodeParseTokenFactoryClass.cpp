#include "pch.h"

#include "CodeParseTokenFactoryClass.h"

#include "CodeParseTokenClass.h"

bool CodeParseTokenFactoryClass::IsKeyword(const std::string& keyword)
{
	constexpr int STRING_LENGTH = 13;
	return keyword.size() >= STRING_LENGTH && keyword.substr(0, STRING_LENGTH) == "EDITORCLASS()";
}

CodeParseTokenBase* CodeParseTokenFactoryClass::CreateToken(const std::string& keyword, std::stringstream& stream)
{
	std::string nextString;
	
	stream >> nextString;
	if (nextString != "class")
	{
		return nullptr;
	}

	stream >> nextString;
	
	return new CodeParseTokenClass(nextString);

	// #TODO: Handle base class if one exists
}

std::string CodeParseTokenFactoryClass::GetEndString() const
{
	return "}";
}
