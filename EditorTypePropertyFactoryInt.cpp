#include "pch.h"

#include "EditorTypePropertyFactoryInt.h"

#include "EditorTypePropertyInt.h"

EditorTypePropertyBase* EditorTypePropertyFactoryInt::CreateType(std::ifstream& stream)
{
	EditorTypePropertyInt* pPropertyInt = new EditorTypePropertyInt();
	pPropertyInt->ReadFromFile(stream);

	return pPropertyInt;
}

EditorTypePropertyBase* EditorTypePropertyFactoryInt::CreateTemplateType()
{
	return new EditorTypePropertyInt("int", 0);
}

std::string EditorTypePropertyFactoryInt::GetLabel()
{
	return "int";
}
