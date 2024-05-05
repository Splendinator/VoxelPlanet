#include "pch.h"

#include "EditorTypePropertyFactoryBool.h"

#include "EditorTypePropertyBool.h"

EditorTypePropertyBase* EditorTypePropertyFactoryBool::CreateType(std::ifstream& stream)
{
	EditorTypePropertyBool* pPropertyBool = new EditorTypePropertyBool();
	pPropertyBool->ReadFromFile(stream);

	return pPropertyBool;
}

EditorTypePropertyBase* EditorTypePropertyFactoryBool::CreateTemplateType()
{
	return new EditorTypePropertyBool("bool", false);
}

std::string EditorTypePropertyFactoryBool::GetLabel()
{
	return "bool";
}
