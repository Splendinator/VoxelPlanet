#include "pch.h"

#include "EditorTypePropertyFactoryFloat.h"

#include "EditorTypePropertyFloat.h"

EditorTypePropertyBase* EditorTypePropertyFactoryFloat::CreateType(std::ifstream& stream)
{
	EditorTypePropertyFloat* pPropertyFloat = new EditorTypePropertyFloat();
	pPropertyFloat->ReadFromFile(stream);

	return pPropertyFloat;
}

EditorTypePropertyBase* EditorTypePropertyFactoryFloat::CreateTemplateType()
{
	return new EditorTypePropertyFloat("float", 0.0f);
}

std::string EditorTypePropertyFactoryFloat::GetLabel()
{
	return "float";
}
