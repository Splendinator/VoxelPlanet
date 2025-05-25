#include "pch.h"

#include "EditorTypePropertyFactoryDataCompositeProperty.h"

#include "EditorTypePropertyDataCompositeProperty.h"

EditorTypePropertyBase* EditorTypePropertyFactoryDataCompositeProperty::CreateType(std::ifstream& stream)
{
	EditorTypePropertyDataCompositeProperty* pProperty = new EditorTypePropertyDataCompositeProperty();
	pProperty->ReadFromFile(stream);

	return pProperty;
}

std::string EditorTypePropertyFactoryDataCompositeProperty::GetLabel()
{
	return "datacompositeproperty";
}
