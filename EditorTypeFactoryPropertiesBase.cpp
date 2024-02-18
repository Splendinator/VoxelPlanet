#include "pch.h"

#include "EditorTypeFactoryPropertiesBase.h"

#include "EditorTypeClass.h"
#include "EditorTypePropertiesBase.h"
#include "EditorTypePropertyBase.h"
#include "EditorTypeUtils.h"
#include "Game.h"
#include "ImGuiEditor.h"
#include "ImGuiEditorGlobals.h"

void EditorTypeFactoryPropertiesBase::PopulateProperties(std::ifstream& stream, EditorTypePropertiesBase* pEditorTypeProperties)
{
	while (true)
	{
		std::string keyword;

		std::streampos preKeywordStreamPosition = stream.tellg();

		stream >> keyword;
		
		if(keyword == ImGuiEditorGlobals::delimiterString)
		{
			// End of properties
			break;
		}

		bool bFound = false;
		
		if (keyword == "Child")
		{
			// Handle child classes by deep copying properties from their template object
			std::string childClassName;
			stream >> childClassName;

			EditorTypeClass* baseClassTemplate = Game::Editor().FindClassTemplateType(childClassName);
			DOMLOG_ERROR_IF(!baseClassTemplate, "Cannot find child class", childClassName);
			
			if (baseClassTemplate)
			{
				bFound = true;
				
				for(EditorTypePropertyBase* pChildProperty : baseClassTemplate->pProperties)
				{
					pEditorTypeProperties->pProperties.push_back(pChildProperty->DeepCopy());
				}
			}
		}
		else
		{
			// Handle properties per-property
			for (EditorTypePropertyFactoryBase* pFactory : EditorTypeUtils::propertyFactories)
			{
				if (pFactory->GetLabel() == keyword)
				{
					stream.seekg(preKeywordStreamPosition); // Get the keyword in too to unify how EditorTypePropertyBase::ReadFromFile() does things
					EditorTypePropertyBase* pProperty = pFactory->CreateType(stream);
					pEditorTypeProperties->pProperties.push_back(pProperty);
					bFound = true;
					break;
				}
			}
		}

		if (!bFound)
		{
			DOMLOG_ERROR("Cannot find property with keyword", keyword);
		}
	}

	pEditorTypeProperties->OnPropertiesPopulated();
}
