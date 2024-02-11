#include "pch.h"

#include "EditorTypeFactoryStruct.h"

#include "EditorTypeStruct.h"

EditorTypeBase* EditorTypeFactoryStruct::CreateType(std::ifstream& stream)
{
	EditorTypeStruct* pNewStruct = new EditorTypeStruct();

	std::string name;
	stream >> name;
	pNewStruct->name = name;

	PopulateProperties(stream, pNewStruct);

	return pNewStruct;
}

std::string EditorTypeFactoryStruct::GetKeyword() const
{
	return "Struct";
}
