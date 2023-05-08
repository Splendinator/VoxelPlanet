#include "pch.h"

#include "EditorTypePropertyFactoryFloat.h"

#include "EditorTypePropertyFloat.h"

EditorTypePropertyBase* EditorTypePropertyFactoryFloat::CreateType(std::ifstream& stream)
{
	std::string propertyName;
	float value;

	stream >> propertyName >> value;

	return new EditorTypePropertyFloat(propertyName, value);
}

std::string EditorTypePropertyFactoryFloat::GetLabel()
{
	return "float";
}
