#include "pch.h"
#include "__Generated.h"
#include "EditorTypePropertyClass.h"
#include "EditorTypePropertyFloat.h"
#include "EditorTypePropertyStruct.h"
#include "EditorTypePropertyVector.h"
#include "..\Roguelike\Game.h"

// SingleFloat
void SingleFloat::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	SingleFloat* pSingleFloat = static_cast<SingleFloat*>(pObject);
	pSingleFloat->x = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pSingleFloat->values.push_back(static_cast<EditorTypePropertyFloat*>(instancedProperty.get())->GetValue());
		}
	}
}

void* SingleFloat::InitFromProperties(const std::vector<EditorTypePropertyBase*>& properties)
{
	SingleFloat* pSingleFloat = new SingleFloat;
	int propertyIndex = 0;
	SingleFloat::InitFromPropertiesSubset(pSingleFloat, properties, propertyIndex);
	return pSingleFloat;
}

namespace __Generated
{
	std::unordered_map<std::string, void* (*)(const std::vector<EditorTypePropertyBase*>&)> stringToCreateObjectFunction
	{
		{"SingleFloat", &SingleFloat::InitFromProperties},
	};
}
