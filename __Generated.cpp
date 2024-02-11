#include "pch.h"
#include "__Generated.h"
#include "EditorTypePropertyClass.h"
#include "EditorTypePropertyFloat.h"
#include "EditorTypePropertyStruct.h"
#include "..\Roguelike\Game.h"

void* FloatStruct::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
FloatStruct* pReturn = new FloatStruct;
pReturn->a = static_cast<EditorTypePropertyFloat*>(properties[0])->GetValue();
pReturn->b = static_cast<EditorTypePropertyFloat*>(properties[1])->GetValue();
return pReturn;
}

void* SingleFloat::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
SingleFloat* pReturn = new SingleFloat;
pReturn->value = static_cast<EditorTypePropertyFloat*>(properties[0])->GetValue();
pReturn->pNext = static_cast<SingleFloat*>(static_cast<EditorTypePropertyClass*>(properties[1])->GetValue());
{
FloatStruct* temp = static_cast<FloatStruct*>(static_cast<EditorTypePropertyStruct*>(properties[2])->GetValue());
pReturn->floatStruct = *temp;
delete temp;
}
return pReturn;
}

namespace __Generated
{
	std::unordered_map<std::string, void* (*)(const std::vector<EditorTypePropertyBase*>&)> stringToCreateObjectFunction
	{
		{"FloatStruct", &FloatStruct::InitFromProperties},
		{"SingleFloat", &SingleFloat::InitFromProperties},
	};
}
