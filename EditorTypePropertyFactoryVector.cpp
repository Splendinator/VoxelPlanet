#include "pch.h"

#include "EditorTypePropertyFactoryVector.h"

#include "EditorTypeClass.h"
#include "EditorTypePropertyBase.h"
#include "EditorTypePropertyClass.h"
#include "EditorTypePropertyStruct.h"
#include "EditorTypePropertyVector.h"
#include "EditorTypeStruct.h"
#include "EditorTypeUtils.h"
#include "Game.h"
#include "ImGuiEditor.h"

EditorTypePropertyBase* EditorTypePropertyFactoryVector::CreateType(std::ifstream& stream)
{
	// "vector floatArray float"
	std::string unused;
	std::string propertyName;
	std::string dataType;
	stream >> unused >> propertyName >> dataType;
	
	// First we need to get the template property type from a given dataType string. This is the type that will be copied for every entry in the array
	EditorTypePropertyBase* pPropertyType = nullptr;
	
	// First case: it's a regular type and we can just generate it from the property factories (i.e floats can just be made with 0.0f)
	for (EditorTypePropertyFactoryBase* pPropertyFactory : EditorTypeUtils::propertyFactories)
	{
		if (pPropertyFactory->GetLabel() == dataType)
		{
			pPropertyType = pPropertyFactory->CreateTemplateType();
			break;
		}
	}

	// Second case: it's a user defined type and we need to find it in the template types array then wrap a relevant property around it
	if (pPropertyType == nullptr)
	{
		if (EditorTypeClass* pClass = Game::Editor().FindClassTemplateType(dataType))
		{
			// Class -- Make a property with a sensible default name and nullptr
			const std::string& classPropertyName = "p" + dataType;
			const std::string& className = dataType;
			const std::string& assetName = "nullptr";
			pPropertyType = new EditorTypePropertyClass(classPropertyName, className, assetName);
		}
		else if (EditorTypeStruct* pStruct = Game::Editor().FindStructTemplateType(dataType))
		{
			// Struct -- Make a property with sensible name and template values  

			std::string structPropertyName = dataType;
			structPropertyName[0] = (char)std::tolower(structPropertyName[0]);
			
			const std::string& structName = dataType;
			
			pPropertyType = new EditorTypePropertyStruct(structPropertyName, structName, static_cast<EditorTypeStruct*>(pStruct->DeepCopy()));
		}
		// #TODO: Enums
	}

	DOMLOG_ERROR_IF(pPropertyType == nullptr, "Can't have a vector of", dataType);
	
	return new EditorTypePropertyVector(pPropertyType, propertyName);
}
