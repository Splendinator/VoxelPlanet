#include "pch.h"

#include "EditorTypePropertyFactoryString.h"

#include "EditorTypePropertyString.h"

EditorTypePropertyBase* EditorTypePropertyFactoryString::CreateType(std::ifstream& stream)
{
	EditorTypePropertyString* pPropertyString = new EditorTypePropertyString();
	pPropertyString->ReadFromFile(stream);

	return pPropertyString;
}

EditorTypePropertyBase* EditorTypePropertyFactoryString::CreateTemplateType()
{
	return new EditorTypePropertyString("string", "");
}

std::string EditorTypePropertyFactoryString::GetLabel()
{
	return "string";
}
