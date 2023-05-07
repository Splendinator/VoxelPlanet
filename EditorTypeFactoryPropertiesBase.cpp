#include "pch.h"

#include "EditorTypeFactoryPropertiesBase.h"

#include "EditorTypePropertiesBase.h"
#include "EditorTypePropertyFactoryFloat.h"
#include "ImGuiEditorGlobals.h"

void EditorTypeFactoryPropertiesBase::PopulateProperties(std::ifstream& stream, EditorTypePropertiesBase* pEditorTypeProperties)
{
	EditorTypePropertyFactoryFloat floatFactory;
	EditorTypePropertyFactoryBase* propertyFactories[] = 
	{
		&floatFactory
	};

	while (true)
	{
		std::string keyword;
		
		stream >> keyword;
		if(keyword == ImGuiEditorGlobals::delimiterString)
		{
			// End of properties
			break;
		}
		
		bool bFound = false;
		for (EditorTypePropertyFactoryBase* pFactory : propertyFactories)
		{
			if (pFactory->GetLabel() == keyword)
			{
				EditorTypePropertyBase* pProperty = pFactory->CreateType(stream);
				pEditorTypeProperties->pProperties.push_back(pProperty);
				bFound = true;
				break;
			}
		}

		if (!bFound)
		{
			DOMLOG_ERROR("Cannot find property with keyword", keyword);
		}
	}
}
