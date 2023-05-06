#include "pch.h"

#include "EditorTypePropertyFactoryFloat.h"

#include "EditorTypePropertyFloat.h"

EditorTypePropertyBase* EditorTypePropertyFactoryFloat::CreateType(std::ifstream& stream)
{
	std::string propertyName;
	float value;
	std::string unused; // this is for the f at the end of "5.0f"

	stream >> propertyName >> value >> unused;

	return new EditorTypePropertyFloat(propertyName, value);
}

std::string EditorTypePropertyFactoryFloat::GetLabel()
{
	return "float";
}
