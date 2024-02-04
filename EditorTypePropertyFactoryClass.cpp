#include "pch.h"

#include "EditorTypePropertyFactoryClass.h"

#include "EditorTypePropertyClass.h"

EditorTypePropertyBase* EditorTypePropertyFactoryClass::CreateType(std::ifstream& stream)
{
	EditorTypePropertyClass* pProperty = new EditorTypePropertyClass();
	pProperty->ReadFromFile(stream);

	return pProperty;
}

std::string EditorTypePropertyFactoryClass::GetLabel()
{
	return "object";	
}
