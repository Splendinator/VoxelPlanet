#include "pch.h"
#include "__Generated.h"
#include "EditorTypePropertyClass.h"
#include "EditorTypePropertyFloat.h"
#include "EditorTypePropertyInt.h"
#include "EditorTypePropertyBool.h"
#include "EditorTypePropertyString.h"
#include "EditorTypePropertyStruct.h"
#include "EditorTypePropertyVector.h"
#include "..\Roguelike\Game.h"

// SingleFloat
void SingleFloat::InitFromPropertiesSubset(void* pObject, const std::vector<EditorTypePropertyBase*>& properties, int& propertyIndex)
{
	SingleFloat* pSingleFloat = static_cast<SingleFloat*>(pObject);
	pSingleFloat->floatOne = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	pSingleFloat->floatTwo = static_cast<EditorTypePropertyFloat*>(properties[propertyIndex++])->GetValue();
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pSingleFloat->strings.push_back(static_cast<EditorTypePropertyString*>(instancedProperty.get())->GetValue());
		}
	}
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pSingleFloat->integers.push_back(static_cast<EditorTypePropertyInt*>(instancedProperty.get())->GetValue());
		}
	}
	{
		EditorTypePropertyVector* pVectorProperty = static_cast<EditorTypePropertyVector*>(properties[propertyIndex++]);
		for (std::unique_ptr<EditorTypePropertyBase>& instancedProperty : pVectorProperty->instancedProperties)
		{
			pSingleFloat->bools.push_back(static_cast<EditorTypePropertyBool*>(instancedProperty.get())->GetValue());
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
