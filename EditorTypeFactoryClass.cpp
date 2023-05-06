#include "pch.h"

#include "EditorTypeFactoryClass.h"

#include "EditorTypeClass.h"

EditorTypeBase* EditorTypeFactoryClass::CreateType(std::ifstream& stream)
{
	EditorTypeClass* pNewClass = new EditorTypeClass();

	std::string name;
	stream >> name;
	pNewClass->name = name;

	PopulateProperties(stream, pNewClass);

	return pNewClass;
}

std::string EditorTypeFactoryClass::GetKeyword() const
{
	return "Object";
}
